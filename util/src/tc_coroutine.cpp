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

#include "util/tc_coroutine.h"
#include "util/tc_platform.h"
#include "util/tc_logger.h"

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

namespace tars
{

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
TC_CoroutineInfo::TC_CoroutineInfo()
: _prev(NULL)
, _next(NULL)
, _scheduler(NULL)
, _uid(0)
, _eStatus(CORO_FREE)
{
}

TC_CoroutineInfo::TC_CoroutineInfo(TC_CoroutineScheduler* scheduler, uint32_t iUid, stack_context stack_ctx)
: _prev(NULL)
, _next(NULL)
, _scheduler(scheduler)
, _uid(iUid)
, _eStatus(CORO_FREE)
, _stack_ctx(stack_ctx)
{
}

TC_CoroutineInfo::~TC_CoroutineInfo()
{
}

void TC_CoroutineInfo::setStackContext(stack_context stack_ctx)
{
	_stack_ctx = stack_ctx;
}

void TC_CoroutineInfo::registerFunc(const std::function<void ()>& callback)
{
    _callback           = callback;

    _init_func.coroFunc = TC_CoroutineInfo::corotineProc;

    _init_func.args     = this;

	fcontext_t ctx      = make_fcontext(_stack_ctx.sp, _stack_ctx.size, TC_CoroutineInfo::corotineEntry);

	transfer_t tf       = jump_fcontext(ctx, this);

	//实际的ctx
	this->setCtx(tf.fctx);
}

void TC_CoroutineInfo::corotineEntry(transfer_t tf)
{
    TC_CoroutineInfo * coro = static_cast< TC_CoroutineInfo * >(tf.data);

    auto    func  = coro->_init_func.coroFunc;
    void*    args = coro->_init_func.args;

	transfer_t t = jump_fcontext(tf.fctx, NULL);

	//拿到自己的协程堆栈, 当前协程结束以后, 好跳转到main
	coro->_scheduler->setMainCtx(t.fctx);

    //再跳转到具体函数
    func(args, t);
}

void TC_CoroutineInfo::corotineProc(void * args, transfer_t t)
{
    TC_CoroutineInfo *coro = (TC_CoroutineInfo*)args;

    try
    {
    	//执行具体业务代码
	    coro->_callback();
    }
    catch(std::exception &ex)
    {
        cerr << "TC_CoroutineInfo::corotineProc exception:" << ex.what() << endl;
    }
    catch(...)
    {
        cerr << "TC_CoroutineInfo::corotineProc unknown exception." << endl;
    }

    TC_CoroutineScheduler* scheduler =  coro->getScheduler();
    scheduler->decUsedSize();
    scheduler->moveToFreeList(coro);

    //当前业务执行完, 会跳到main
	scheduler->switchCoro(&(scheduler->getMainCoroutine()));
}

///////////////////////////////////////////////////////////////////////////////////////////

thread_local shared_ptr<TC_CoroutineScheduler> TC_CoroutineScheduler::g_scheduler;

const shared_ptr<TC_CoroutineScheduler> &TC_CoroutineScheduler::create()
{
    if(!g_scheduler) 
    {
        g_scheduler = std::make_shared<TC_CoroutineScheduler>();
    }
    
    return g_scheduler;
}

const shared_ptr<TC_CoroutineScheduler> &TC_CoroutineScheduler::scheduler()
{
    return g_scheduler;
}

void TC_CoroutineScheduler::reset()
{
	g_scheduler.reset();
}

TC_CoroutineScheduler::TC_CoroutineScheduler()
: _currentSize(0)
, _usedSize(0)
, _uniqId(0)
, _currentCoro(NULL)
, _all_coro(NULL)
{
    // LOG_CONSOLE_DEBUG << endl;

    _epoller = new TC_Epoller();

    _epoller->create(10240);
}

TC_CoroutineScheduler::~TC_CoroutineScheduler()
{
    // LOG_CONSOLE_DEBUG << endl;
    if(_epoller)
	{
		delete _epoller;
		_epoller = NULL;
	}
}

void TC_CoroutineScheduler::createCoroutineInfo(size_t poolSize)
{
	if(_all_coro != NULL)
	{
		delete [] _all_coro;
	}

	_all_coro = new TC_CoroutineInfo*[_poolSize+1];
	for(size_t i = 0; i <= _poolSize; ++i)
	{
        //id=0不使用, 给mainCoro来使用!
		_all_coro[i] = NULL;
	}
}

void TC_CoroutineScheduler::setPoolStackSize(uint32_t iPoolSize, size_t iStackSize)
{
	_poolSize   = iPoolSize;
	_stackSize  = iStackSize;
}

void TC_CoroutineScheduler::init()
{
	_usedSize   = 0;
	_uniqId     = 0;

    if(_poolSize <= 100)
    {
        _currentSize = _poolSize;
    }
    else
    {
        _currentSize = 100;
    }

	createCoroutineInfo(_poolSize);

    TC_CoroutineInfo::CoroutineHeadInit(&_active);
    TC_CoroutineInfo::CoroutineHeadInit(&_avail);
    TC_CoroutineInfo::CoroutineHeadInit(&_inactive);
    TC_CoroutineInfo::CoroutineHeadInit(&_timeout);
    TC_CoroutineInfo::CoroutineHeadInit(&_free);

    int iSucc = 0;

    for(size_t i = 0; i < _currentSize; ++i)
    {
        //iId=0不使用, 给mainCoro使用!!!! 
	    uint32_t iId = generateId();

        assert(iId != 0);

        stack_context s_ctx = stack_traits::allocate(_stackSize);

	    TC_CoroutineInfo *coro = new TC_CoroutineInfo(this, iId, s_ctx);

        _all_coro[iId] = coro;

        TC_CoroutineInfo::CoroutineAddTail(coro, &_free);

        ++iSucc;
    }

    _currentSize = iSucc;

    _mainCoro.setUid(0);
    _mainCoro.setStatus(TC_CoroutineInfo::CORO_FREE);

    _currentCoro = &_mainCoro;
}

int TC_CoroutineScheduler::increaseCoroPoolSize()
{
    if(_poolSize <= _currentSize)
    	return -1;

    int iInc = ((_poolSize - _currentSize) > 100) ? 100 : (_poolSize - _currentSize);

    for(int i = 0; i < iInc; ++i)
    {
	    uint32_t iId        = generateId();
	    stack_context s_ctx = stack_traits::allocate(_stackSize);

	    TC_CoroutineInfo *coro = new TC_CoroutineInfo(this, iId, s_ctx);

        _all_coro[iId] = coro;

        TC_CoroutineInfo::CoroutineAddTail(coro, &_free);
    }

    _currentSize += iInc;

    return 0;
}

uint32_t TC_CoroutineScheduler::go(const std::function<void ()> &callback)
{
	if(!_all_coro)
	{
		init();
	}

    if(_usedSize >= _currentSize || TC_CoroutineInfo::CoroutineHeadEmpty(&_free))
    {
        int iRet = increaseCoroPoolSize();

        if(iRet != 0)
            return 0;
    }

    TC_CoroutineInfo *coro = _free._next;
    assert(coro != NULL);

    TC_CoroutineInfo::CoroutineDel(coro);

    _usedSize++;

    coro->setStatus(TC_CoroutineInfo::CORO_AVAIL);

    TC_CoroutineInfo::CoroutineAddTail(coro, &_avail);

    coro->registerFunc(callback);

    return coro->getUid();
}

bool TC_CoroutineScheduler::full()
{
	if(_usedSize >= _currentSize || TC_CoroutineInfo::CoroutineHeadEmpty(&_free))
	{
		if(_poolSize <= _currentSize)
			return true;
	}

	return false;
}

void TC_CoroutineScheduler::notify()
{
	assert(_epoller);

    _epoller->notify();
}

void TC_CoroutineScheduler::run()
{
	if(!_all_coro)
	{
		init();
	}

	_ready = true;

	while(!_epoller->isTerminate())
	{
		if(_activeCoroQueue.empty() && TC_CoroutineInfo::CoroutineHeadEmpty(&_avail) && TC_CoroutineInfo::CoroutineHeadEmpty(&_active))
		{
			_epoller->done(1000);
		}

		//唤醒需要激活的协程
		wakeup();

		//唤醒sleep的协程
		wakeupbytimeout();

		//唤醒yield的协程
		wakeupbyself();

		int iLoop = 100;

		//执行active协程, 每次执行100个, 避免占满cpu
		while(iLoop > 0 && !TC_CoroutineInfo::CoroutineHeadEmpty(&_active))
		{
			TC_CoroutineInfo *coro = _active._next;

			assert(coro != NULL);

			switchCoro(coro);

			--iLoop;
		}

		//检查yield的线程, 执行
		if(!TC_CoroutineInfo::CoroutineHeadEmpty(&_avail))
		{
			TC_CoroutineInfo *coro = _avail._next;

			assert(coro != NULL);

			switchCoro(coro);
		}

        //没有任何可执行的写成了, 直接退出!
        if(_usedSize == 0 && _noCoroutineCallback)
        {
            _noCoroutineCallback(this);
        }
	}

	destroy();

	_ready = false;
}

void TC_CoroutineScheduler::yield(bool bFlag)
{
    //主协程不允许yield
    if(_currentCoro->getUid() == 0)
    {
        return;
    }

    if(bFlag)
    {
	    _needActiveCoroId.push_back(_currentCoro->getUid());
    }

    moveToInactive(_currentCoro);
    switchCoro(&_mainCoro);
}

void TC_CoroutineScheduler::sleep(int iSleepTime)
{
    //主协程不允许sleep
    if(_currentCoro->getUid() == 0)
        return;

    int64_t iNow = TNOWMS;
    int64_t iTimeout = iNow + (iSleepTime >= 0 ? iSleepTime : -iSleepTime);

    _timeoutCoroId.insert(make_pair(iTimeout, _currentCoro->getUid()));

    moveToTimeout(_currentCoro);

    _epoller->postAtTime(iTimeout, [](){});

    switchCoro(&_mainCoro);
}

void TC_CoroutineScheduler::wakeupbyself()
{
    if(!_needActiveCoroId.empty() && !_epoller->isTerminate())
    {
        list<uint32_t>::iterator it = _needActiveCoroId.begin();
        while(it != _needActiveCoroId.end())
        {
            TC_CoroutineInfo *coro = _all_coro[*it];

            assert(coro != NULL);

            moveToAvail(coro);

            ++it;
        }
        _needActiveCoroId.clear();
    }
}

void TC_CoroutineScheduler::put(uint32_t iCoroId)
{
    if(!_epoller->isTerminate())
    {
        _activeCoroQueue.push_back(iCoroId);

	    _epoller->notify();
    }
}

void TC_CoroutineScheduler::wakeup()
{
    if(!_activeCoroQueue.empty() && !_epoller->isTerminate())
    {
        deque<uint32_t> coroIds;

        _activeCoroQueue.swap(coroIds);

        auto it = coroIds.begin();

        auto itEnd = coroIds.end();

        while(it != itEnd)
        {
            TC_CoroutineInfo *coro = _all_coro[*it];

            assert(coro != NULL);

            moveToActive(coro);

            ++it;
        }
    }
}

void TC_CoroutineScheduler::wakeupbytimeout()
{
    if(!_timeoutCoroId.empty() && !_epoller->isTerminate())
    {
        int64_t iNow = TNOWMS;
        while(true)
        {
            multimap<int64_t, uint32_t>::iterator it = _timeoutCoroId.begin();

            if(it == _timeoutCoroId.end() || it->first > iNow)
                break;

            TC_CoroutineInfo *coro = _all_coro[it->second];

            assert(coro != NULL);

            moveToActive(coro);

            _timeoutCoroId.erase(it);
        }

    }
}

void TC_CoroutineScheduler::terminate()
{
	assert(_epoller);

	_epoller->terminate();
}

uint32_t TC_CoroutineScheduler::generateId()
{
    uint32_t i = ++_uniqId;
    if(i == 0) {
        i = ++_uniqId;
    }

    assert(i <= _poolSize);

    return i;
}

void TC_CoroutineScheduler::switchCoro(TC_CoroutineInfo *to)
{
    //跳转到to协程
    _currentCoro = to;

	transfer_t t = jump_fcontext(to->getCtx(), NULL);

	//并保存协程堆栈
	to->setCtx(t.fctx);
}

void TC_CoroutineScheduler::moveToActive(TC_CoroutineInfo *coro)
{
    if(coro->getStatus() == TC_CoroutineInfo::CORO_INACTIVE || coro->getStatus() == TC_CoroutineInfo::CORO_TIMEOUT)
    {
        TC_CoroutineInfo::CoroutineDel(coro);
        coro->setStatus(TC_CoroutineInfo::CORO_ACTIVE);
        TC_CoroutineInfo::CoroutineAddTail(coro, &_active);
    }
    else
    {
    	assert(false);
    }
}

void TC_CoroutineScheduler::moveToAvail(TC_CoroutineInfo *coro)
{
    if(coro->getStatus() == TC_CoroutineInfo::CORO_INACTIVE)
    {
        TC_CoroutineInfo::CoroutineDel(coro);
        coro->setStatus(TC_CoroutineInfo::CORO_AVAIL);
        TC_CoroutineInfo::CoroutineAddTail(coro, &_avail);
    }
    else
    {
    	assert(false);
    }
}

void TC_CoroutineScheduler::moveToInactive(TC_CoroutineInfo *coro)
{
    if(coro->getStatus() == TC_CoroutineInfo::CORO_ACTIVE || coro->getStatus() == TC_CoroutineInfo::CORO_AVAIL)
    {
        TC_CoroutineInfo::CoroutineDel(coro);
        coro->setStatus(TC_CoroutineInfo::CORO_INACTIVE);
        TC_CoroutineInfo::CoroutineAddTail(coro, &_inactive);
    }
    else
    {
    	assert(false);
    }
}

void TC_CoroutineScheduler::moveToTimeout(TC_CoroutineInfo *coro)
{
    if(coro->getStatus() == TC_CoroutineInfo::CORO_ACTIVE || coro->getStatus() == TC_CoroutineInfo::CORO_AVAIL)
    {
        TC_CoroutineInfo::CoroutineDel(coro);
        coro->setStatus(TC_CoroutineInfo::CORO_TIMEOUT);
        TC_CoroutineInfo::CoroutineAddTail(coro, &_timeout);
    }
    else
    {
    	assert(false);
    }
}

void TC_CoroutineScheduler::moveToFreeList(TC_CoroutineInfo *coro)
{
    if(coro->getStatus() != TC_CoroutineInfo::CORO_FREE)
    {
        TC_CoroutineInfo::CoroutineDel(coro);
        coro->setStatus(TC_CoroutineInfo::CORO_FREE);
        TC_CoroutineInfo::CoroutineAddTail(coro, &_free);
    }
    else
    {
    	assert(false);
    }
}

void TC_CoroutineScheduler::destroy()
{
    if(_all_coro)
    {
        //id=0是保留不用的, 给mainCoro作为id用
        assert(_all_coro[0] == NULL);

        for (size_t i = 1; i <= _poolSize; i++)
        {
            if(_all_coro[i])
            {
                stack_traits::deallocate(_all_coro[i]->getStackContext());
                delete _all_coro[i];
                _all_coro[i] = NULL;
            }
        }
        delete [] _all_coro;
		_all_coro = NULL;
    }
}
/////////////////////////////////////////////////////////
TC_Coroutine::TC_Coroutine()
: _coroSched(NULL)
, _num(1)
, _maxNum(128)
, _stackSize(128*1024)
{
}

TC_Coroutine::~TC_Coroutine()
{
    if(isAlive())
    {
        terminate();

        getThreadControl().join();
    }
}

void TC_Coroutine::setCoroInfo(uint32_t iNum, uint32_t iMaxNum, size_t iStackSize)
{
    _maxNum     = (iMaxNum > 0 ? iMaxNum : 1);
    _num        = (iNum > 0 ? (iNum <= _maxNum ? iNum : _maxNum) : 1);
    _stackSize  = (iStackSize >= pagesize() ? iStackSize : pagesize());
}

void TC_Coroutine::run()
{
    _coroSched = TC_CoroutineScheduler::create();

    initialize();

    handleCoro();

    destroy();
}

void TC_Coroutine::terminate()
{
    if(_coroSched)
    {
        _coroSched->terminate();
    }
}

void TC_Coroutine::handleCoro()
{
    _coroSched->setPoolStackSize(_maxNum, _stackSize);

    _coroSched->setNoCoroutineCallback([&](TC_CoroutineScheduler *scheduler){scheduler->terminate();});

	//把协程创建出来
    for(uint32_t i = 0; i < _num; ++i)
    {
		_coroSched->go(std::bind(&TC_Coroutine::coroEntry, this));
    }


    _coroSched->run();
}

void TC_Coroutine::coroEntry(TC_Coroutine *pCoro)
{
    pCoro->handle();
}

uint32_t TC_Coroutine::go(const std::function<void ()> &coroFunc)
{
    return _coroSched->go(coroFunc);
}

void TC_Coroutine::yield()
{
    _coroSched->yield();
}

void TC_Coroutine::sleep(int millseconds)
{
    _coroSched->sleep(millseconds);
}

}
