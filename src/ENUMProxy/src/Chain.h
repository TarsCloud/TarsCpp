#ifndef __Chain_h
#define __Chain_h

#include <memory>
#include <vector>
#include "util/tc_autoptr.h"
#include "util/tc_thread_mutex.h"

class RingBase;
class ChainBase;
class Ring;
class Chain;

typedef tars::TC_AutoPtr<RingBase> RingBasePtr;
typedef tars::TC_AutoPtr<ChainBase> ChainBasePtr;
typedef tars::TC_AutoPtr<Ring> RingPtr;
typedef tars::TC_AutoPtr<Chain> ChainPtr;

class RingBase : public virtual tars::TC_HandleBase
{
public:
    RingBase(const ChainBasePtr& chain)
    {
        _chain = chain;
    }

    ChainBasePtr _chain;
    RingBasePtr _prev;
    RingBasePtr _next;
};

class ChainBase : public virtual tars::TC_HandleBase, public tars::TC_ThreadRecMutex
{
public:
    ChainBase(size_t n)
    {
        _first.resize(n + 1);
        _last.resize(n + 1);
        for(size_t i = 0; i < n+1; i++)
        {
            _first[i] = new RingBase(nullptr);
            _last[i] = new RingBase(nullptr);
            _first[i]->_next = _last[i];
            _last[i]->_prev = _first[i];
        }
    }

    std::vector<RingBasePtr> _first;
    std::vector<RingBasePtr> _last;
};

class Ring : public RingBase
{
public:
    Ring(const ChainPtr& chain): RingBase(chain)
    {
    }

    void linkTail(size_t n = 0)
    {
        tars::TC_LockT<tars::TC_ThreadRecMutex> lock(*_chain);
        _unlink();
        _prev = _chain->_last[n]->_prev;
        _prev->_next = this;
        _next = _chain->_last[n];
        _next->_prev = this;
    }

    void _unlink()
    {
        if(_prev)
        {
            _prev->_next = _next;
            _next->_prev = _prev;
            _prev = nullptr;
            _next = nullptr;
        }
    }

    void unlink()
    {
        tars::TC_LockT<tars::TC_ThreadRecMutex> lock(*_chain);
        _unlink();
    }
};

class Chain : public ChainBase
{
public:
    Chain(size_t n = 0) : ChainBase(n)
    {
    }

    template<typename T>
    T first( size_t n = 0)
    {
        tars::TC_LockT<tars::TC_ThreadRecMutex> lock(*this);

        if(_first[n]->_next != _last[n])
            return T::dynamicCast(_first[n]->_next);
        return 0;
    }

    template<typename T>
    T fetch(size_t n = 0)
    {
        tars::TC_LockT<tars::TC_ThreadRecMutex> lock(*this);
        if(_first[n]->_next != _last[n])
        {
            RingPtr r = RingPtr::dynamicCast(_first[n]->_next);

            r->_unlink();
            return T::dynamicCast(r);
        }

        return 0;
    }

    void store(const RingPtr& ring, size_t n = 0)
    {
        tars::TC_LockT<tars::TC_ThreadRecMutex> lock(*this);
        ring->_prev = _first[n];
        ring->_next = _first[n]->_next;
        ring->_prev->_next = ring;
        ring->_next->_prev = ring;
    }
};

#endif
