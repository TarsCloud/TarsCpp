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

#ifndef _TARS_PROMISE_H_
#define _TARS_PROMISE_H_

#include <memory>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "promise/template_helper.h"
#include "promise/exception.h"
#include "promise/bind.h"

namespace tars {

// pre-declare future type.
template <typename T> class Future;

template <typename T> struct IsFutureType : std::false_type {};
// only Future<T> is a future type.
template <typename T> struct IsFutureType<Future<T> > : std::true_type {};

// pre-declare promise type.
template <typename T> class Promise;

namespace promise {

template <typename T> struct UnderlyType { using Type = T; };
template <typename T> struct UnderlyType<Future<T> > { using Type = T; };

template <typename T> 
struct FutureTypeTraits {
    using StorageType = std::shared_ptr<T>;
    using RValueType = const T&;
    using LValueType = const T&;
    using DestType = T&;

    static void init(StorageType& s, RValueType v) 
    {
        s.reset(new T(v));
    }

    static void assign(DestType d, const StorageType& s)
    {
        d = *s;
    }
};

// since value maybe set in the future, there is no need to check type [const T&]
template <typename T> 
struct FutureTypeTraits<T&> {
    using StorageType = T*; 
    using RValueType = T&;
    using LValueType = T&;
    using DestType = T*&;

    static void init(StorageType& s, RValueType v) 
    {
        s = &v;
    }

    static void assign(DestType d, const StorageType& s)
    {
        d = s;
    }
};

// void type.
template <> 
struct FutureTypeTraits<void> {
    using RValueType = void;
    using LValueType = void;
};

template <typename T>
class FutureInterface {
public:
    using RValueType = typename FutureTypeTraits<T>::RValueType;
    using LValueType = typename FutureTypeTraits<T>::LValueType;
    using DestType = typename FutureTypeTraits<T>::DestType;
    using FutureType = std::shared_ptr<FutureInterface<T> >;
    using Watcher = Callback<void(const FutureType&)>;

    virtual ~FutureInterface() {}

    virtual bool isDone() const = 0;
    virtual bool hasValue() const = 0;
    virtual bool hasException() const = 0;
    virtual LValueType get() const = 0;
    virtual bool tryGet(DestType d) const = 0;

    virtual void setValue(RValueType v) = 0;
    virtual void setException(ExceptionPtr exp) = 0;
    virtual void appendWacther(const Watcher& watcher) = 0;

protected:
    FutureInterface() {}
};

template <> class FutureInterface<void> {
public:
    using RValueType = typename FutureTypeTraits<void>::RValueType;
    using LValueType = typename FutureTypeTraits<void>::LValueType;
    using FutureType = std::shared_ptr<FutureInterface<void> >;
    using Watcher = Callback<void(const FutureType&)>;

    virtual ~FutureInterface() {}

    virtual bool isDone() const = 0;
    virtual bool hasValue() const = 0;
    virtual bool hasException() const = 0;
    virtual LValueType get() const  = 0;
    
    virtual void set() = 0;
    virtual void setException(ExceptionPtr exp) = 0;
    virtual void appendWacther(const Watcher& watcher) = 0;

protected:
    FutureInterface() {}
};

template <typename T> 
class PromptFutureImpl final : public FutureInterface<T>
                         , public std::enable_shared_from_this<PromptFutureImpl<T> > {
public:
    using RValueType = typename FutureInterface<T>::RValueType;
    using LValueType = typename FutureInterface<T>::LValueType;
    using DestType = typename FutureInterface<T>::DestType;
    using Watcher = typename FutureInterface<T>::Watcher; 

    PromptFutureImpl(RValueType v)
        : m_value()
        , m_exp()
    {
        FutureTypeTraits<T>::init(m_value, v);
    }

    PromptFutureImpl(ExceptionPtr exp)
        : m_value()
        , m_exp(exp)
    {
    }

    bool isDone() const override { return true; }

    bool hasValue() const override
    { 
        if (m_exp) 
            return false; 
        return true;
    }

    bool hasException() const override
    {
        return (!!m_exp);
    }

    LValueType get() const override
    {
        if (m_exp)
            m_exp->rethrow();
        return *m_value;
    }

    bool tryGet(DestType d) const override 
    {
        if (m_exp)
            m_exp->rethrow();

        FutureTypeTraits<T>::assign(d, m_value);
        return true;
    }

    void setValue(RValueType v) override
    {
        // do not touch this.
    }

    void setException(ExceptionPtr exp) override
    {
        // do not touch this.
    }

    void appendWacther(const Watcher& watcher) override
    {
        try {
            if (watcher)
                watcher.run(this->shared_from_this());
        } catch (...) {
        }
    }

private:
    typename FutureTypeTraits<T>::StorageType m_value;
    ExceptionPtr m_exp;
};

template <> class PromptFutureImpl<void> final : public FutureInterface<void>
                         , public std::enable_shared_from_this<PromptFutureImpl<void> > {
public:
    using RValueType = typename FutureInterface<void>::RValueType;
    using LValueType = typename FutureInterface<void>::LValueType;
    using Watcher = typename FutureInterface<void>::Watcher; 

    PromptFutureImpl()
        : m_exp()
    {
    }

    PromptFutureImpl(ExceptionPtr exp)
        : m_exp(exp)
    {
    }

    bool isDone() const override { return true; }

    bool hasValue() const override
    { 
        if (m_exp) 
            return false; 
        return true;
    }

    bool hasException() const override
    {
        return (!!m_exp);
    }

    LValueType get() const override
    {
        if (m_exp)
            m_exp->rethrow();
    }

    void set() override
    {
        // do not touch this.
    }

    void setException(ExceptionPtr exp) override
    {
        // do not touch this.
    }

    void appendWacther(const Watcher& watcher) override
    {
        try {
            if (watcher)
                watcher.run(this->shared_from_this());
        } catch (...) {
        }
    }

private:
    ExceptionPtr m_exp;
};

class FutureInternal {
public:
    virtual ~FutureInternal() {}

    bool isDone() const 
    { 
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_isDone;
    }

    bool hasValue() const 
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_isDone && !m_exp;
    }

    bool hasException() const 
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_isDone && m_exp;
    }

    void markFinishWithException(const ExceptionPtr& exp) 
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_isDone)
                throw Exception("Duplicated mark finish with exception.");

            m_isDone = true;
            m_exp = exp;
        }

        m_cv.notify_all();
    }

    void wait() const
    {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock, [&]{ return m_isDone; });
        }

        if (m_exp)
            m_exp->rethrow();
    }

protected:
    FutureInternal()
        : m_mutex()
        , m_cv()
        , m_isDone(false)
        , m_exp()
    {
    }

    mutable std::mutex m_mutex;
    mutable std::condition_variable m_cv;
    bool m_isDone;
    ExceptionPtr m_exp;
};

template <typename T>
class FutureImpl : public FutureInterface<T>
                   , public std::enable_shared_from_this<FutureImpl<T> >
                   , private FutureInternal {
public:
    using RValueType = typename FutureInterface<T>::RValueType;
    using LValueType = typename FutureInterface<T>::LValueType;
    using DestType = typename FutureInterface<T>::DestType;
    using Watcher = typename FutureInterface<T>::Watcher; 

    FutureImpl()
        : m_value()
        , m_watchers()
    {
    }

    bool isDone() const override { return FutureInternal::isDone(); }

    bool hasValue() const override { return FutureInternal::hasValue(); }

    bool hasException() const override { return FutureInternal::hasException(); }

    LValueType get() const override
    {
        wait();
        return *m_value;
    }

    bool tryGet(DestType d) const override 
    {
        if (!FutureInternal::isDone())
            return false;

        if (m_exp)
            m_exp->rethrow();

        FutureTypeTraits<T>::assign(d, m_value);
        return true;
    }

    void setValue(RValueType v) override
    {
        {
            // FixMe: move this to FutureInternal.
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_isDone)
                throw Exception("Duplicated set value.");

            m_isDone = true;
            FutureTypeTraits<T>::init(m_value, v);
        }

        m_cv.notify_all();
        invokeWatchers();
    }

    void setException(ExceptionPtr exp) override
    {
        markFinishWithException(exp);
        invokeWatchers();
    }

    void appendWacther(const Watcher& watcher) override
    {
        if (!watcher)
            return;

        if (FutureInternal::isDone()) {
            try {
                if (watcher)
                    watcher.run(this->shared_from_this());
            } catch (...) {
            }
        } else {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_watchers.push_back(watcher);
        }
    }

protected:
    void invokeWatchers() 
    {
        std::vector<Watcher> watchers;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            watchers.swap(m_watchers);
        }

        for (const Watcher& w : watchers) {
            try {
                w.run(this->shared_from_this());
            } catch (...) {
            }
        }
    }

    typename FutureTypeTraits<T>::StorageType m_value;
    std::vector<Watcher> m_watchers;
};

template <> class FutureImpl<void> : public FutureInterface<void>
                   , public std::enable_shared_from_this<FutureImpl<void> >
                   , private FutureInternal {
public:
    using RValueType = typename FutureInterface<void>::RValueType;
    using LValueType = typename FutureInterface<void>::LValueType;
    using Watcher = typename FutureInterface<void>::Watcher; 

    FutureImpl()
        : m_watchers()
    {
    }

    bool isDone() const override { return FutureInternal::isDone(); }

    bool hasValue() const override { return FutureInternal::hasValue(); }

    bool hasException() const override { return FutureInternal::hasException(); }

    LValueType get() const override
    {
        wait();
    }

    void set() override
    {
        {
            // FixMe: move this to FutureInternal.
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_isDone)
                throw Exception("Duplicated set finish.");

            m_isDone = true;
        }

        m_cv.notify_all();
        invokeWatchers();
    }

    void setException(ExceptionPtr exp) override
    {
        markFinishWithException(exp);
        invokeWatchers();
    }

    void appendWacther(const Watcher& watcher) override
    {
        if (!watcher)
            return;

        if (FutureInternal::isDone()) {
            try {
                if (watcher)
                    watcher.run(this->shared_from_this());
            } catch (...) {
            }
        } else {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_watchers.push_back(watcher);
        }
    }

protected:
    void invokeWatchers() 
    {
        std::vector<Watcher> watchers;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            watchers.swap(m_watchers);
        }

        for (const Watcher& w : watchers) {
            try {
                w.run(this->shared_from_this());
            } catch (...) {
            }
        }
    }

    std::vector<Watcher> m_watchers;
};

// support sequential watchers.
template <typename T> class ForwardWatcher;

template <typename R, typename T> class SequentialWatcher {
    using ValueType = typename UnderlyType<R>::Type;
    using FuturePtr = std::shared_ptr<FutureInterface<T> >;
    using Watcher = Callback<R(const Future<T>&)>;
public:
    SequentialWatcher(const Watcher& w, const Promise<ValueType>& p)
        : m_watcher(w)
        , m_promise(p)
    {
    }
    
    template <typename U>
    enable_if_t<std::is_void<U>::value> invoke(const FuturePtr& future)
    {
        try {
            m_watcher.run(future);
            m_promise.set();
        } catch (...) {
            m_promise.setException(currentException());
        }
    }

    template <typename U>
    enable_if_t<!std::is_void<U>::value && !IsFutureType<U>::value> invoke(const FuturePtr& future)
    {
        try {
            m_promise.setValue(m_watcher.run(future));
        } catch (...) {
            m_promise.setException(currentException());
        }
    }

    template <typename U>
    enable_if_t<IsFutureType<U>::value> invoke(const FuturePtr& future)
    {
        try {
            // sequential watcher.
            m_watcher.run(future).then(Bind(&ForwardWatcher<ValueType>::template invoke<ValueType>,
                        std::make_shared<ForwardWatcher<ValueType> >(m_promise)));
        } catch (...) {
            m_promise.setException(currentException());
        }
    }

protected:
    Watcher m_watcher;
    Promise<ValueType> m_promise;
};

template <typename R>
class ForwardWatcher {
public:
    ForwardWatcher(const Promise<R>& p)
        : m_promise(p)
    {
    }

    template <typename V>
    enable_if_t<std::is_void<V>::value> invoke(const Future<V>& future)
    {
        try {
            future.get();
            m_promise.set();
        } catch (...) {
            m_promise.setException(currentException());
        }
    }

    template <typename V>
    enable_if_t<!std::is_void<V>::value> invoke(const Future<V>& future)
    {
        try {
            m_promise.setValue(future.get());
        } catch (...) {
            m_promise.setException(currentException());
        }
    }

protected:
    Promise<R> m_promise;
};

template <typename T>
class FutureBase {
public:
    using LValueType = typename FutureTypeTraits<T>::LValueType;

    FutureBase()
        : m_future()
    {
    }

    FutureBase(ExceptionPtr exp)
        : m_future(new PromptFutureImpl<T>(exp))
    {
    }

    virtual ~FutureBase() {}

    LValueType get() const 
    {
        if (!m_future) 
            throw Exception("future uninitialized");
        return m_future->get();
    }

    bool isDone() const 
    {
        if (!m_future)
            return false;

        return m_future->isDone();
    }

    bool hasValue() const
    {
        if (!m_future)
            return false;

        return m_future->hasValue();
    }

    bool hasException() const
    {
        if (!m_future)
            return false;

        return m_future->hasException();
    }

    // safe bool idiom
    // refer: https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Safe_bool
    // c++11 standard
    explicit operator bool() const
    {
        return m_future ? true : false;
    }

protected:
    using FuturePtr = std::shared_ptr<FutureInterface<T> >;

    FutureBase(const FuturePtr& future)
        : m_future(future)
    {
    }

    FuturePtr m_future;
};

} // end namespace promise

template <typename T>
class Future : public promise::FutureBase<T> {
public:
    explicit Future(typename promise::FutureTypeTraits<T>::RValueType v)
        : promise::FutureBase<T>(new promise::PromptFutureImpl<T>(v))
    {
    }

    Future(ExceptionPtr exp)
        : promise::FutureBase<T>(exp)
    {
    }

    // can be initialize in tuple.
    Future() = default;

    ~Future() override {}

    template <typename R>
    Future<typename promise::UnderlyType<R>::Type> then(const Callback<R(const Future<T>&)>& watcher) 
    {
        using ValueType = typename promise::UnderlyType<R>::Type;
        if (!this->m_future)
            throw Exception("future uninitialized");

        Promise<ValueType> promise;
        this->m_future->appendWacther(Bind(&promise::SequentialWatcher<R, T>::template invoke<R>,
                    std::make_shared<promise::SequentialWatcher<R, T> >(watcher, promise)));

        return promise.getFuture();
    }

private:
    using FuturePtr = typename promise::FutureBase<T>::FuturePtr;
    Future(const FuturePtr& future)
        : promise::FutureBase<T>(future)
    {
    }

    template <typename U, typename V> friend class promise::SequentialWatcher;
    friend class Promise<T>;
};

template <> class Future<void> : public promise::FutureBase<void> {
public:
    // can be initialize in tuple.
    Future() = default;

    Future(ExceptionPtr exp)
        : promise::FutureBase<void>(exp)
    {
    }

    ~Future() override {}

    template <typename R>
    Future<typename promise::UnderlyType<R>::Type> then(const Callback<R(const Future<void>&)>& watcher) 
    {
        using ValueType = typename promise::UnderlyType<R>::Type;
        if (!this->m_future)
            throw Exception("future uninitialized");

        Promise<ValueType> promise;
        this->m_future->appendWacther(Bind(&promise::SequentialWatcher<R, void>::template invoke<R>,
                    std::make_shared<promise::SequentialWatcher<R, void> >(watcher, promise)));

        return promise.getFuture();
    }

private:
    using FuturePtr = typename promise::FutureBase<void>::FuturePtr;
    Future(const FuturePtr& future)
        : promise::FutureBase<void>(future)
    {
    }

    template <typename U, typename V> friend class promise::SequentialWatcher;
    friend class Promise<void>;
};

template <typename T>
class Promise final {
public:
    using ValueType = typename promise::UnderlyType<T>::Type;
    Promise()
        : m_future(new promise::FutureImpl<ValueType>())
    {
    }

    void setValue(typename promise::FutureTypeTraits<T>::RValueType v)
    {
        m_future->setValue(v);
    }

    void setException(const ExceptionPtr& e)
    {
        m_future->setException(e);
    }

    Future<ValueType> getFuture()
    {
        return m_future;
    }

private:
    std::shared_ptr<promise::FutureInterface<ValueType> > m_future;
};

template <> class Promise<void> final {
public:
    Promise()
        : m_future(new promise::FutureImpl<void>())
    {
    }

    void set()
    {
        m_future->set();
    }

    void setException(const ExceptionPtr& e)
    {
        m_future->setException(e);
    }

    Future<void> getFuture()
    {
        return m_future;
    }

private:
    std::shared_ptr<promise::FutureInterface<void> > m_future;
};

} // end namespace tars

#endif
