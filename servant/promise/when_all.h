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

#ifndef _TARS_WHEN_ALL_H_
#define _TARS_WHEN_ALL_H_

#include <vector>
#include <atomic>
#include "promise/template_helper.h"
#include "promise/promise.h"

namespace tars {

namespace wa {

template <typename... Futures>
struct MakeFuturesStorageImpl {
    using Type = std::tuple<Futures...>;
};

template <typename... Futures>
using MakeFuturesStorage = typename MakeFuturesStorageImpl<
      typename std::decay<Futures>::type...>::Type;

template <typename... Futures>
struct FutureList {
    using StorgeType = MakeFuturesStorage<Futures...>;
    using FutureType = Future<StorgeType>;
    using PromiseType = Promise<StorgeType>;
};

template <typename T>
class ParallelCallbackBase {
public:
    virtual ~ParallelCallbackBase() {}

protected:
    using PromiseAll = Promise<T>;

    ParallelCallbackBase(const ParallelCallbackBase&) = delete;
    ParallelCallbackBase(ParallelCallbackBase&&) = delete;
    ParallelCallbackBase& operator = (const ParallelCallbackBase&) = delete;

    ParallelCallbackBase(const Promise<T>& p, const int totalCount)
        : m_promise(p)
        , m_futures()
        , m_waitCount(totalCount)
    {
    }

    
    PromiseAll m_promise;
    T m_futures;
    std::atomic<int> m_waitCount;
};

template <typename... Futures>
class ParallelCallback final 
                : public std::enable_shared_from_this<ParallelCallback<Futures...> >
                  , public ParallelCallbackBase<
                  typename FutureList<Futures...>::StorgeType> {
public:
    using StorgeType = typename FutureList<Futures...>::StorgeType;
    using PromiseAll = typename ParallelCallbackBase<StorgeType>::PromiseAll;

    ParallelCallback(const PromiseAll& p)
        : ParallelCallbackBase<StorgeType>(p, sizeof...(Futures))
    {
    }

    ~ParallelCallback() override {}

    void registerCallback()
    {
        // do nothing here.
    }

    template <size_t N, typename T>
    void registerCallback(T&& f)
    {
        f.then(Bind(&ParallelCallback::template onFuture<N>, 
                    this->shared_from_this()));
    }

    template <size_t N, typename T, typename... Types>
    void registerCallback(T&& f, Types&&... fs)
    {
        registerCallback<N>(std::forward<T>(f));
        registerCallback<N+1>(std::forward<Types>(fs)...);
    }

    template <typename T, typename... Types>
    void registerCallback(T&& f, Types&&... fs)
    {
        registerCallback<0>(std::forward<T>(f), std::forward<Types>(fs)...);
    }

    template <size_t N>
    void onFuture(const typename std::tuple_element<N, StorgeType>::type& f) 
    {
        std::get<N>(this->m_futures) = f; 
        int waitCount = this->m_waitCount.fetch_sub(1);
        if (waitCount > 1)
            return;

        try {
            this->m_promise.setValue(this->m_futures);
        } catch (...) {
            this->m_promise.setException(currentException());
        }
    }
};

template <typename Futures>
class ParallelCallback<std::vector<Futures> > final 
                : public std::enable_shared_from_this<ParallelCallback<std::vector<Futures> > >
                , public ParallelCallbackBase<std::vector<Futures> > {
public:
    using PromiseAll = typename ParallelCallbackBase<std::vector<Futures> >::PromiseAll;

    ParallelCallback(const PromiseAll& p, const size_t count)
        : ParallelCallbackBase<std::vector<Futures> >(p, count)
    {
        this->m_futures.resize(count);
    }

    ~ParallelCallback() override {}

    void onFuture(const size_t n, const Futures& f) 
    {
        this->m_futures[n] = f; 
        int waitCount = this->m_waitCount.fetch_sub(1);
        if (waitCount > 1)
            return;

        try {
            this->m_promise.setValue(this->m_futures);
        } catch (...) {
            this->m_promise.setException(currentException());
        }
    }
};

} // end namespace wa(when all)

template <typename... Futures>
typename wa::FutureList<Futures...>::FutureType WhenAll(Futures... f)
{
    static_assert((sizeof...(Futures) > 1), "TarsWhenAll need at least two features."); 

    using PromiseType = typename wa::FutureList<Futures...>::PromiseType;
    using WhenAllCallback = wa::ParallelCallback<Futures...>;

    PromiseType p;
    std::shared_ptr<WhenAllCallback> c = std::make_shared<WhenAllCallback>(p);
    c->registerCallback(f...);
    return p.getFuture();
}

template <typename T>
Future<std::vector<T> > WhenAll(std::vector<T>& f)
{
    static_assert(IsFutureType<T>::value, "T is not a Future type."); 

    using PromiseType = Promise<std::vector<T> >;
    using WhenAllCallback = wa::ParallelCallback<std::vector<T> >;

    PromiseType p;
    if (f.empty())
        p.setValue(std::vector<T>());
    else {
        std::shared_ptr<WhenAllCallback> c = std::make_shared<WhenAllCallback>(p, f.size());
        for (size_t i=0; i<f.size(); ++i)
            f[i].then(Bind(&WhenAllCallback::onFuture, c, i));
    }

    return p.getFuture();
}

} // end namespace tars

#endif
