/**
 * Tencent is pleased to support the open source community by making Tars available.
 *
 * Copyright (C) 2016THL A29 Limited, a Tencent company. All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use this file except 
 * in compliance with the License. You may obtain a copy of the License at
 *
 * https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software distributed 
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
 * CONDITIONS OF ANY KIND, either express or implied. See the License for the 
 * specific language governing permissions and limitations under the License.
 */
#include "servant/CoroutineScheduler.h"

#if TARGET_PLATFORM_LINUX || TARGET_PLATFORM_IOS
#include <fcntl.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <algorithm>
#include <cmath>
#include <cstring>
#include <stdexcept>
#include <assert.h>
#include "util/tc_timeprovider.h"

#include "servant/RemoteLogger.h"
#include "servant/ServantHandle.h"

namespace tars 
{

#define MAX_WAIT_TIME_MS 1000
#define MIN_WAIT_TIME_MS 1

#if TARGET_PLATFORM_WINDOWS

// x86_64
// test x86_64 before i386 because icc might
// define __i686__ for x86_64 too
#if defined(__x86_64__) || defined(__x86_64) \
    || defined(__amd64__) || defined(__amd64) \
    || defined(_M_X64) || defined(_M_AMD64)

// Windows seams not to provide a constant or function
// telling the minimal stacksize
# define MIN_STACKSIZE  8 * 1024
#else
# define MIN_STACKSIZE  4 * 1024
#endif

void system_info_( SYSTEM_INFO * si) {
    ::GetSystemInfo( si);
}

SYSTEM_INFO system_info() {
    static SYSTEM_INFO si;
    static std::once_flag flag;
    std::call_once( flag, static_cast< void(*)( SYSTEM_INFO *) >( system_info_), & si);
    return si;
}

std::size_t pagesize() {
    return static_cast< std::size_t >( system_info().dwPageSize);
}

// Windows seams not to provide a limit for the stacksize
// libcoco uses 32k+4k bytes as minimum
bool stack_traits::is_unbounded() {
    return true;
}

std::size_t stack_traits::page_size() {
    return pagesize();
}

std::size_t stack_traits::default_size() {
    return 128 * 1024;
}

// because Windows seams not to provide a limit for minimum stacksize
std::size_t stack_traits::minimum_size() {
    return MIN_STACKSIZE;
}

// because Windows seams not to provide a limit for maximum stacksize
// maximum_size() can never be called (pre-condition ! is_unbounded() )
std::size_t stack_traits::maximum_size() {
    assert( ! is_unbounded() );
    return  1 * 1024 * 1024 * 1024; // 1GB
}

stack_context stack_traits::allocate(size_t size_) {
	// calculate how many pages are required
	const std::size_t pages(static_cast< std::size_t >( std::ceil( static_cast< float >( size_) / stack_traits::page_size() ) ) );
	// add one page at bottom that will be used as guard-page
	const std::size_t size__ = ( pages + 1) * stack_traits::page_size();

	void * vp = ::VirtualAlloc( 0, size__, MEM_COMMIT, PAGE_READWRITE);
	if ( ! vp) throw std::bad_alloc();

	DWORD old_options;
	const BOOL result = ::VirtualProtect(
		vp, stack_traits::page_size(), PAGE_READWRITE | PAGE_GUARD /*PAGE_NOACCESS*/, & old_options);
	assert( FALSE != result);

	stack_context sctx;
	sctx.size = size__;
	sctx.sp = static_cast< char * >( vp) + sctx.size;
	return sctx;
}

void stack_traits::deallocate( stack_context & sctx)  {
	assert( sctx.sp);

	void * vp = static_cast< char * >( sctx.sp) - sctx.size;
	::VirtualFree( vp, 0, MEM_RELEASE);
}

#else

// 128kb recommended stack size
// # define MINSIGSTKSZ (131072) 

void pagesize_( std::size_t * size)  {
    // conform to POSIX.1-2001
    * size = ::sysconf( _SC_PAGESIZE);
}

void stacksize_limit_( rlimit * limit)  {
    // conforming to POSIX.1-2001
    ::getrlimit( RLIMIT_STACK, limit);
}

std::size_t pagesize()  {
    static std::size_t size = 0;
    static std::once_flag flag;
    std::call_once( flag, pagesize_, & size);
    return size;
}

rlimit stacksize_limit()  {
    static rlimit limit;
    static std::once_flag flag;
    std::call_once( flag, stacksize_limit_, & limit);
    return limit;
}

bool stack_traits::is_unbounded() {
    return RLIM_INFINITY == stacksize_limit().rlim_max;
}

std::size_t stack_traits::page_size() {
    return pagesize();
}

std::size_t stack_traits::default_size() {
	return 128 * 1024;    
}

std::size_t stack_traits::minimum_size() {
    return MINSIGSTKSZ;
}

std::size_t stack_traits::maximum_size() {
    assert( ! is_unbounded() );
    return static_cast< std::size_t >( stacksize_limit().rlim_max);
}

stack_context stack_traits::allocate(std::size_t size_) {
	// calculate how many pages are required
	const std::size_t pages(static_cast< std::size_t >( std::ceil( static_cast< float >( size_) / stack_traits::page_size() ) ) );
	// add one page at bottom that will be used as guard-page
	const std::size_t size__ = ( pages + 1) * stack_traits::page_size();

	// conform to POSIX.4 (POSIX.1b-1993, _POSIX_C_SOURCE=199309L)
#if defined(MAP_ANON)
	void * vp = ::mmap( 0, size__, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
#else
	void * vp = ::mmap( 0, size__, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif
	if ( MAP_FAILED == vp) throw std::bad_alloc();

	// conforming to POSIX.1-2001
	const int result( ::mprotect( vp, stack_traits::page_size(), PROT_NONE) );
	assert( 0 == result);

	stack_context sctx;
	sctx.size = size__;
	sctx.sp = static_cast< char * >( vp) + sctx.size;

	return sctx;
}

void stack_traits::deallocate(stack_context & sctx) {
	assert( sctx.sp);

	void * vp = static_cast< char * >( sctx.sp) - sctx.size;
	// conform to POSIX.4 (POSIX.1b-1993, _POSIX_C_SOURCE=199309L)
	::munmap( vp, sctx.size);
}

#endif

////////////////////////////////////////////////////////
CoroutineInfo::CoroutineInfo()
: _prev(NULL)
, _next(NULL)
// , _main(true)
, _scheduler(NULL)
, _uid(0)
, _eStatus(CORO_FREE)
// , _ctx_to(NULL)
{
}

CoroutineInfo::CoroutineInfo(CoroutineScheduler* scheduler)
: _prev(NULL)
, _next(NULL)
// , _main(false)
, _scheduler(scheduler)
, _uid(0)
, _eStatus(CORO_FREE)
// , _ctx_to(NULL)
{
}

CoroutineInfo::CoroutineInfo(CoroutineScheduler* scheduler, uint32_t iUid, stack_context stack_ctx)
: _prev(NULL)
, _next(NULL)
// , _main(false)
, _scheduler(scheduler)
, _uid(iUid)
, _eStatus(CORO_FREE)
, _stack_ctx(stack_ctx)
// , _ctx_to(NULL)
{
}

CoroutineInfo::~CoroutineInfo()
{
}

void CoroutineInfo::registerFunc(const std::function<void ()>& callback)
{
    _callback = callback;

    _init_func.coroFunc = CoroutineInfo::corotineProc;

    _init_func.args = this;

    // _ctx_to = make_fcontext(_stack_ctx.sp, _stack_ctx.size, CoroutineInfo::corotineEntry);

    // jump_fcontext(&_ctx_from, _ctx_to, (intptr_t)this, false);

	fcontext_t ctx = make_fcontext(_stack_ctx.sp, _stack_ctx.size, CoroutineInfo::corotineEntry);

	transfer_t tf = jump_fcontext(ctx, this);

	//实际的ctx
	this->setCtx(tf.fctx);
}

void CoroutineInfo::setStackContext(stack_context stack_ctx)
{
    _stack_ctx = stack_ctx;
}

void CoroutineInfo::corotineEntry(transfer_t tf)
{
    CoroutineInfo * coro = static_cast< CoroutineInfo * >(tf.data);

    Func    func = coro->_init_func.coroFunc;
    void*    args = coro->_init_func.args;

    // jump_fcontext(coro->_ctx_to, &(coro->_ctx_from), 0, false);
	transfer_t t = jump_fcontext(tf.fctx, NULL);

	coro->_scheduler->setMainCtx(t.fctx);
    try
    {
        func(args, t);
    }
    catch(std::exception &ex)
    {
        TLOGERROR("[CoroutineInfo::corotineEntry exception:" << ex.what() << endl);
    }
    catch(...)
    {
        TLOGERROR("[CoroutineInfo::corotineEntry unknown exception." << endl);
    }
}

void CoroutineInfo::corotineProc(void * args, transfer_t t)
{
    CoroutineInfo *coro = (CoroutineInfo*)args;

    try
    {
        std::function<void ()> cb = coro->_callback;

        cb();
    }
    catch(std::exception &ex)
    {
        TLOGERROR("[CoroutineInfo::corotineProc exception:" << ex.what() << endl);
    }
    catch(...)
    {
        TLOGERROR("[CoroutineInfo::corotineProc unknown exception." << endl);
    }

    CoroutineScheduler* scheduler =  coro->getScheduler();
    scheduler->decUsedSize();
    scheduler->moveToFreeList(coro);

	scheduler->switchCoro(&(scheduler->getMainCoroutine()));
    // scheduler->switchCoro(coro, &(scheduler->getMainCoroutine()));
    TLOGERROR("[CoroutineInfo::corotineProc no come." << endl);
}

//////////////////////////////////////////////////////////////
CoroutineScheduler::CoroutineScheduler()
: _terminal(false)
, _poolSize(1000)
, _stackSize(128*1024)
, _currentSize(0)
, _usedSize(0)
, _uniqId(0)
, _handle(NULL)
, _currentCoro(NULL)
, _all_coro(NULL)
{
    _all_coro = new CoroutineInfo*[_poolSize+1];
    for(size_t i = 0; i <= _poolSize; ++i)
    {
        _all_coro[i] = NULL;
    }

    CoroutineInfo::CoroutineHeadInit(&_active);
    CoroutineInfo::CoroutineHeadInit(&_avail);
    CoroutineInfo::CoroutineHeadInit(&_inactive);
    CoroutineInfo::CoroutineHeadInit(&_timeout);
    CoroutineInfo::CoroutineHeadInit(&_free);
}

CoroutineScheduler::~CoroutineScheduler()
{}

void CoroutineScheduler::init(uint32_t iPoolSize, size_t iStackSize)
{
    if(iPoolSize <= 0)
    {
        TLOGERROR("[CoroutineScheduler::init iPoolSize <= 0." << endl);
        return ;
    }

    _terminal = false;
    _poolSize = iPoolSize;
    _stackSize = iStackSize;

    if(_poolSize <= 100)
    {
        _currentSize = _poolSize;
    }
    else
    {
        _currentSize = 100;
    }

    if(_all_coro != NULL)
    {
        delete [] _all_coro;
        _all_coro = new CoroutineInfo*[_poolSize+1];
        for(size_t i = 0; i <= _poolSize; ++i)
        {
            _all_coro[i] = NULL;
        }
    }

    _usedSize = 0;
    _uniqId = 0;

    int iSucc = 0;
    for(size_t i = 0; i < _currentSize; ++i)
    {
        CoroutineInfo *coro = new CoroutineInfo(this);

        // stack_context s_ctx;
        // int ret = _alloc.allocate(s_ctx, iStackSize);
        // if(ret != 0)
        // {
        //     TLOGERROR("[CoroutineScheduler::init iPoolSize:" << iPoolSize << "|iStackSize:" << iStackSize << "|i:" << i << endl);

        //     delete coro;
        //     coro = NULL;
        //     break;
        // }

		stack_context s_ctx = stack_traits::allocate(iStackSize);

        coro->setStackContext(s_ctx);

        uint32_t iId = generateId();
        coro->setUid(iId);
        coro->setStatus(CORO_FREE);

        //_free.push_front(coro);

        //_all.insert(make_pair(coro->getUid(), coro));

        _all_coro[iId] = coro;

        CoroutineInfo::CoroutineAddTail(coro, &_free);

        ++iSucc;
    }

    _currentSize = iSucc;

    _mainCoro.setUid(0);
    _mainCoro.setStatus(CORO_FREE);

    _currentCoro = &_mainCoro;

    TLOGDEBUG("[CoroutineScheduler::init iPoolSize:" << _poolSize << "|iCurrentSize:" << _currentSize << "|iStackSize:" << _stackSize << endl);
}

int    CoroutineScheduler::increaseCoroPoolSize()
{
    int iInc = ((_poolSize - _currentSize) > 100) ? 100 : (_poolSize - _currentSize);
    if(iInc <= 0)
    {
        TLOGERROR("[CoroutineScheduler::increaseCoroPoolSize full iPoolSize:" << _poolSize << "|iCurrentSize:" << _currentSize << endl);
        return -1;
    }

    int iSucc = 0;
    for(int i = 0; i < iInc; ++i)
    {
        CoroutineInfo *coro = new CoroutineInfo(this);
        uint32_t iId = generateId();
        coro->setUid(iId);
        coro->setStatus(CORO_FREE);

        // stack_context s_ctx;
        // int ret = _alloc.allocate(s_ctx, _stackSize);
        // if(ret != 0)
        // {
        //     TLOGERROR("[CoroutineScheduler::increaseCoroPoolSize iPoolSize:" << _poolSize << "|iStackSize:" << _stackSize << "|i:" << i << endl);

        //     delete coro;
        //     coro = NULL;
        //     break;
        // }

		stack_context s_ctx = stack_traits::allocate(_stackSize);

        coro->setStackContext(s_ctx);

        _all_coro[iId] = coro;

        CoroutineInfo::CoroutineAddTail(coro, &_free);

        ++iSucc;
    }

    if(iSucc == 0)
    {
        TLOGERROR("[CoroutineScheduler::increaseCoroPoolSize cannot create iInc:" << iInc << "|iPoolSize:" << _poolSize << endl);
        return -1;
    }

    _currentSize += iSucc;

    return 0;
}

uint32_t CoroutineScheduler::createCoroutine(const std::function<void ()> &callback)
{
    if(_usedSize >= _currentSize || CoroutineInfo::CoroutineHeadEmpty(&_free))
    {
        int iRet = increaseCoroPoolSize();

        if(iRet != 0)
            return 0;
    }

    CoroutineInfo *coro = _free._next;
    assert(coro != NULL);

    CoroutineInfo::CoroutineDel(coro);

    _usedSize++;

    coro->setStatus(CORO_AVAIL);

    CoroutineInfo::CoroutineAddTail(coro, &_avail);

    coro->registerFunc(callback);

    return coro->getUid();
}

void CoroutineScheduler::run()
{
    while(!_terminal)
    {
        wakeupbyself();

        if(!CoroutineInfo::CoroutineHeadEmpty(&_avail))
        {
            CoroutineInfo *coro = _avail._next;

            assert(coro != NULL);

            // switchCoro(&_mainCoro, coro);
            switchCoro(coro);

        }

        //获取第一个即将timeout的剩余时长
        int waitTime = wakeupbytimeout();

        if(CoroutineInfo::CoroutineHeadEmpty(&_active) && (_activeCoroQueue.size() <= 0))
        {
            waitTime = min(max(waitTime, MIN_WAIT_TIME_MS), MAX_WAIT_TIME_MS);
            TC_ThreadLock::Lock lock(_monitor);
            //限定最多等待时长为waitTime,尽量保证sleep的协程及时被唤醒
            _monitor.timedWait(waitTime);
        }

        wakeup();

        if(!CoroutineInfo::CoroutineHeadEmpty(&_active))
        {
            int iLoop = 100;
            while(iLoop > 0 && !CoroutineInfo::CoroutineHeadEmpty(&_active))
            {
                CoroutineInfo *coro = _active._next;

                assert(coro != NULL);

                // switchCoro(&_mainCoro, coro);
                switchCoro(coro);

                --iLoop;
            }

        }

        if(_usedSize == 0)
            break;
    }

    destroy();
}

void CoroutineScheduler::tars_run()
{
    if(!_terminal)
    {
        wakeupbytimeout();

        wakeupbyself();

        wakeup();

        if(!CoroutineInfo::CoroutineHeadEmpty(&_active))
        {
            int iLoop = 100;
            while(iLoop > 0 && !CoroutineInfo::CoroutineHeadEmpty(&_active))
            {
                CoroutineInfo *coro = _active._next;

                assert(coro != NULL);

                // switchCoro(&_mainCoro, coro);
                switchCoro(coro);

                --iLoop;
            }

        }

        if(!CoroutineInfo::CoroutineHeadEmpty(&_avail))
        {
            int iLoop = 100;
            while(iLoop > 0 && !CoroutineInfo::CoroutineHeadEmpty(&_avail))
            {
                CoroutineInfo *coro = _avail._next;

                assert(coro != NULL);

                // switchCoro(&_mainCoro, coro);
                switchCoro(coro);

                --iLoop;
            }

        }
    }
}

void CoroutineScheduler::yield(bool bFlag)
{
    if(bFlag)
    {
        putbyself(_currentCoro->getUid());
    }

    moveToInactive(_currentCoro);
    // switchCoro(_currentCoro, &_mainCoro);
    switchCoro(&_mainCoro);
}

void CoroutineScheduler::sleep(int iSleepTime)
{
    int64_t iNow = TNOWMS;
    int64_t iTimeout = iNow + (iSleepTime >= 0 ? iSleepTime : -iSleepTime);

    _timeoutCoroId.insert(make_pair(iTimeout, _currentCoro->getUid()));

    moveToTimeout(_currentCoro);
    // switchCoro(_currentCoro, &_mainCoro);
    switchCoro(&_mainCoro);
}

void CoroutineScheduler::putbyself(uint32_t iCoroId)
{
    if(!_terminal)
    {
        _needActiveCoroId.push_back(iCoroId);
    }
}

void CoroutineScheduler::wakeupbyself()
{
    if(!_terminal)
    {
        if(_needActiveCoroId.size() > 0)
        {
            list<uint32_t>::iterator it = _needActiveCoroId.begin();
            while(it != _needActiveCoroId.end())
            {
                CoroutineInfo *coro = _all_coro[*it];

                assert(coro != NULL);

                moveToAvail(coro);

                ++it;
            }
            _needActiveCoroId.clear();
        }
    }
}

void CoroutineScheduler::put(uint32_t iCoroId)
{
    if(!_terminal)
    {
        _activeCoroQueue.push_back(iCoroId);

        if(_handle)
        {
            _handle->notifyFilter();
        }
        else
        {
            TC_ThreadLock::Lock lock(_monitor);

            _monitor.notifyAll();
        }
    }
}

void CoroutineScheduler::wakeup()
{
    if(!_terminal)
    {
        if(_activeCoroQueue.size() > 0)
        {

            TC_ThreadQueue<uint32_t, deque<uint32_t> >::queue_type coroIds;
            _activeCoroQueue.swap(coroIds);

            TC_ThreadQueue<uint32_t, deque<uint32_t> >::queue_type::iterator it = coroIds.begin();

            TC_ThreadQueue<uint32_t, deque<uint32_t> >::queue_type::iterator itEnd = coroIds.end();

            while(it != itEnd)
            {
                CoroutineInfo *coro = _all_coro[*it];

                assert(coro != NULL);

                moveToActive(coro);

                ++it;
            }

        }
    }
}

int CoroutineScheduler::wakeupbytimeout()
{
    int leftTime = MAX_WAIT_TIME_MS;
    if(!_terminal)
    {
        if(_timeoutCoroId.size() > 0)
        {
            int64_t iNow = TNOWMS;
            while(true)
            {
                multimap<int64_t, uint32_t>::iterator it = _timeoutCoroId.begin();

                if(it == _timeoutCoroId.end())
                    break;

                leftTime = it->first - iNow;
                if(leftTime > 0)
                    break;

                CoroutineInfo *coro = _all_coro[it->second];

                assert(coro != NULL);

                moveToActive(coro);

                _timeoutCoroId.erase(it);
            }

        }
    }
    return leftTime;
}

void CoroutineScheduler::terminate()
{
    _terminal = true;

    if(_handle)
    {
        _handle->notifyFilter();
    }
    else
    {
        TC_ThreadLock::Lock lock(_monitor);

        _monitor.notifyAll();
    }
}

uint32_t CoroutineScheduler::generateId()
{
    uint32_t i = ++_uniqId;
    if(i == 0) {
        i = ++_uniqId;
    }

    assert(i <= _poolSize);

    return i;
}

// void CoroutineScheduler::switchCoro(CoroutineInfo *from, CoroutineInfo *to)
void CoroutineScheduler::switchCoro(CoroutineInfo *to)
{
    _currentCoro = to;

    // jump_fcontext(from->getCtx(), to->getCtx(), 0, false);
	transfer_t t = jump_fcontext(to->getCtx(), NULL);
	to->setCtx(t.fctx);
}

void CoroutineScheduler::moveToActive(CoroutineInfo *coro, bool bFlag)
{
    if(coro->getStatus() == CORO_INACTIVE)
    {
        CoroutineInfo::CoroutineDel(coro);
        coro->setStatus(CORO_ACTIVE);
        CoroutineInfo::CoroutineAddTail(coro, &_active);
    }
    else if(coro->getStatus() == CORO_TIMEOUT)
    {
        CoroutineInfo::CoroutineDel(coro);
        coro->setStatus(CORO_ACTIVE);
        CoroutineInfo::CoroutineAddTail(coro, &_active);
    }
    else
    {
        TLOGERROR("[CoroutineScheduler::moveToActive ERROR|iCoroId:" << coro->getUid() << "|tyep:" << coro->getStatus() << endl);
    }
}

void CoroutineScheduler::moveToAvail(CoroutineInfo *coro)
{
    if(coro->getStatus() == CORO_INACTIVE)
    {
        CoroutineInfo::CoroutineDel(coro);
        coro->setStatus(CORO_AVAIL);
        CoroutineInfo::CoroutineAddTail(coro, &_avail);
    }
    else
    {
        TLOGERROR("[CoroutineScheduler::moveToAvail ERROR:|iCoroId:" << coro->getUid() << "|tyep:" << coro->getStatus() << endl);
    }
}

void CoroutineScheduler::moveToInactive(CoroutineInfo *coro)
{
    if(coro->getStatus() == CORO_ACTIVE)
    {
        CoroutineInfo::CoroutineDel(coro);
        coro->setStatus(CORO_INACTIVE);
        CoroutineInfo::CoroutineAddTail(coro, &_inactive);
    }
    else if(coro->getStatus() == CORO_AVAIL)
    {
        CoroutineInfo::CoroutineDel(coro);
        coro->setStatus(CORO_INACTIVE);
        CoroutineInfo::CoroutineAddTail(coro, &_inactive);
    }
    else
    {
        TLOGERROR("[CoroutineScheduler::moveToInactive ERROR|iCoroId:" << coro->getUid() << "|tyep:" << coro->getStatus() << endl);
    }
}

void CoroutineScheduler::moveToTimeout(CoroutineInfo *coro)
{
    if(coro->getStatus() == CORO_ACTIVE)
    {
        CoroutineInfo::CoroutineDel(coro);
        coro->setStatus(CORO_TIMEOUT);
        CoroutineInfo::CoroutineAddTail(coro, &_timeout);
    }
    else if(coro->getStatus() == CORO_AVAIL)
    {
        CoroutineInfo::CoroutineDel(coro);
        coro->setStatus(CORO_TIMEOUT);
        CoroutineInfo::CoroutineAddTail(coro, &_timeout);
    }
    else
    {
        TLOGERROR("[CoroutineScheduler::moveToTimeout ERROR|iCoroId:" << coro->getUid() << "|tyep:" << coro->getStatus() << endl);
    }
}

void CoroutineScheduler::moveToFreeList(CoroutineInfo *coro)
{
    if(coro->getStatus() == CORO_ACTIVE)
    {
        CoroutineInfo::CoroutineDel(coro);
        coro->setStatus(CORO_FREE);
        CoroutineInfo::CoroutineAddTail(coro, &_free);
    }
    else if(coro->getStatus() == CORO_AVAIL)
    {
        CoroutineInfo::CoroutineDel(coro);
        coro->setStatus(CORO_FREE);
        CoroutineInfo::CoroutineAddTail(coro, &_free);
    }
    else if(coro->getStatus() == CORO_INACTIVE)
    {
        CoroutineInfo::CoroutineDel(coro);
        coro->setStatus(CORO_FREE);
        CoroutineInfo::CoroutineAddTail(coro, &_free);
    }
    else if(coro->getStatus() == CORO_TIMEOUT)
    {
        CoroutineInfo::CoroutineDel(coro);
        coro->setStatus(CORO_FREE);
        CoroutineInfo::CoroutineAddTail(coro, &_free);
    }
    else
    {
        TLOGERROR("[CoroutineScheduler::moveToFreeList ERROR: already free|iCoroId:" << coro->getUid() << "|tyep:" << coro->getStatus() << endl);
    }
}

void CoroutineScheduler::destroy()
{
    if(_all_coro)
    {
        for(size_t i = 1; i <= _poolSize; i++)
        {
            if(_all_coro[i])
            {
                stack_traits::deallocate(_all_coro[i]->getStackContext());
            }
        }
        delete [] _all_coro;
    }
}
/////////////////////////////////////////////////////////
Coroutine::Coroutine()
: _coroSched(NULL)
, _num(1)
, _maxNum(128)
, _stackSize(128*1024)
{
}

Coroutine::~Coroutine()
{
    if(isAlive())
    {
        terminate();

        getThreadControl().join();
    }
}

void Coroutine::setCoroInfo(uint32_t iNum, uint32_t iMaxNum, size_t iStackSize)
{
    _maxNum = (iMaxNum > 0 ? iMaxNum : 1);
    _num = (iNum > 0 ? (iNum <= _maxNum ? iNum : _maxNum) : 1);
    _stackSize = (iStackSize >= pagesize() ? iStackSize : pagesize());
}

void Coroutine::run()
{
    initialize();

    handleCoro();

    destroy();
}

void Coroutine::terminate()
{
    if(_coroSched)
    {
        _coroSched->terminate();
    }
}

void Coroutine::handleCoro()
{
    _coroSched = new CoroutineScheduler();

    _coroSched->init(_maxNum, _stackSize);

    ServantProxyThreadData * pSptd = ServantProxyThreadData::getData();

    assert(pSptd != NULL);

    pSptd->_sched = _coroSched;

    for(uint32_t i = 0; i < _num; ++i)
    {
        _coroSched->createCoroutine(std::bind(&Coroutine::coroEntry, this));
    }

    _coroSched->run();

    delete _coroSched;
    _coroSched = NULL;
}

void Coroutine::coroEntry(Coroutine *pCoro)
{
    try
    {
        pCoro->handle();
    }
    catch(exception &ex)
    {
        TLOGERROR("[Coroutine::coroEntry exception:" << ex.what() << "]" << endl);
    }
    catch(...)
    {
        TLOGERROR("[Coroutine::coroEntry unknown exception]" << endl);
    }
}

uint32_t Coroutine::createCoroutine(const std::function<void ()> &coroFunc)
{
    if(_coroSched)
    {
        return _coroSched->createCoroutine(coroFunc);
    }
    else
    {
        TLOGERROR("[Coroutine::createCoroutine coro sched no init]" << endl);
    }
    return -1;
}

void Coroutine::yield()
{
    if(_coroSched)
    {
        _coroSched->yield();
    }
    else
    {
        throw CoroutineException("[Coroutine::yield coro sched no init]");
    }
}

void Coroutine::Sleep(int iSleepTime)
{
    if(_coroSched)
    {
        _coroSched->sleep(iSleepTime);
    }
    else
    {
        throw CoroutineException("[Coroutine::yield coro sched no init]");
    }
}

}
