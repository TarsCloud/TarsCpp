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

#ifndef _TARS_BIND_INTERNAL_H_
#define _TARS_BIND_INTERNAL_H_

#include <memory>
#include <tuple>
#include <vector>
#include "promise/function.h"

namespace tars {

namespace bind {

template <size_t...>
struct IndexSequence {};

template <size_t... Ns>
struct MakeIndexSequenceImpl;

template <size_t... Ns>
struct MakeIndexSequenceImpl<0, Ns...> {
    using Type = IndexSequence<Ns...>;
};

template <size_t N, size_t... Ns>
struct MakeIndexSequenceImpl<N, Ns...> 
                : MakeIndexSequenceImpl<N-1, N-1, Ns...> {};

template <size_t N>
using MakeIndexSequence = typename MakeIndexSequenceImpl<N>::Type;

// Used to implement MakeArgsStorage.
template <bool IsRefCall, typename... BoundArgs>
struct MakeArgsStorageImpl {
    using Type = std::tuple<BoundArgs...>;
};

template <typename Obj, typename... BoundArgs>
struct MakeArgsStorageImpl<true, Obj*, BoundArgs...> {
    using Type = std::tuple<std::shared_ptr<Obj>, BoundArgs...>;
};

template <bool IsRefCall, typename... BoundArgs>
using MakeArgsStorage = typename MakeArgsStorageImpl<IsRefCall, 
      typename std::decay<BoundArgs>::type...>::Type;


class BindStateBase : public std::enable_shared_from_this<BindStateBase> {
public:
    virtual ~BindStateBase() {};

protected:
    BindStateBase() {}
};

template <typename Functor>
class RunnableAdapter;

template <typename R, typename... Args>
class RunnableAdapter<R(*)(Args...)> {
public:
    typedef R RunType(Args...); 

    using IsMethod = std::false_type;

    explicit RunnableAdapter(R(*f)(Args...))
        : m_function(f)
    {
    }

    template <typename... RunArgs>
    R run(RunArgs&&... args) 
    {
        return m_function(std::forward<RunArgs>(args)...);
    }

private:
    R (*m_function)(Args...);
};

template <typename R, typename T, typename... Args>
class RunnableAdapter<R(T::*)(Args...)> {
public:
    typedef R RunType(T*, Args...);

    using IsMethod = std::true_type;

    explicit RunnableAdapter(R(T::*f)(Args...))
        : m_function(f)
    {
    }

    template <typename... RunArgs>
    R run(T* obj, RunArgs&&... args) 
    {
        return (obj->*m_function)(std::forward<RunArgs>(args)...);
    }

private:
    R (T::*m_function)(Args...);
};

template <typename R, typename T, typename... Args>
class RunnableAdapter<R(T::*)(Args...) const> {
public:
    typedef R RunType(const T*, Args...);

    using IsMethod = std::true_type;

    explicit RunnableAdapter(R(T::*f)(Args...) const)
        : m_function(f)
    {
    }

    template <typename... RunArgs>
    R run(const T* obj, RunArgs&&... args) 
    {
        return (obj->*m_function)(std::forward<RunArgs>(args)...);
    }

private:
    R (T::*m_function)(Args...) const;
};

template <typename T>
struct FunctorTraits {
    using RunnableType = RunnableAdapter<T>;
    using RunType = typename RunnableType::RunType;
};

template <typename T>
typename FunctorTraits<T>::RunnableType MakeRunnable(const T& t) {
    return RunnableAdapter<T>(t);
}

template <bool IsRefCall, typename R, typename Runnable>
struct InvokeHelper;

template <typename R, typename Runnable>
struct InvokeHelper<false, R, Runnable> {
    template <typename... RunArgs>
    static R makeItSo(Runnable r, RunArgs&&... args) 
    {
        return r.run(std::forward<RunArgs>(args)...);
    }
};

template <typename Runnable>
struct InvokeHelper<false, void, Runnable> {
    template <typename... RunArgs>
    static void makeItSo(Runnable r, RunArgs&&... args) 
    {
        r.run(std::forward<RunArgs>(args)...);
    }
};

template <typename R, typename Runnable>
struct InvokeHelper<true, R, Runnable> {
    template <typename RefPtr, typename... RunArgs>
    static R makeItSo(Runnable r, RefPtr p, RunArgs&&... args) 
    {
        return r.run(p.get(), std::forward<RunArgs>(args)...);
    }
};

template <typename Runnable>
struct InvokeHelper<true, void, Runnable> {
    template <typename RefPtr, typename... RunArgs>
    static void makeItSo(Runnable r, RefPtr p, RunArgs&&... args) 
    {
        r.run(p.get(), std::forward<RunArgs>(args)...);
    }
};

template <typename T>
const T& Unwrap(const T& o) {
    return o;
}

template <typename T> 
struct IsMoveOnlyType : public std::false_type {};

template <typename T, typename D>
struct IsMoveOnlyType<std::unique_ptr<T, D> > : public std::true_type {};

template <typename T, typename A>
struct IsMoveOnlyType<std::vector<T, A> > : public IsMoveOnlyType<T> {};

template <typename T>
struct ParamForwardTypeNonMoveOnly {
    using ForwardType = const T&;
};

template <typename T, size_t N>
struct ParamForwardTypeNonMoveOnly<T[N]> {
    using ForwardType = const T*;
};

template <typename T>
struct ParamForwardTypeNonMoveOnly<T[]> {
    using ForwardType = const T*;
};

template <typename T>
struct ParamForwardTypeMoveOnly {
    using ForwardType = T;
};

template <typename T>
enable_if_t<!IsMoveOnlyType<T>::value, T>& ParamForward(T& t)
{
    return t;
}

template <typename T>
enable_if_t<IsMoveOnlyType<T>::value, T>& ParamForward(T& t)
{
    return std::move(t);
}

template <typename T>
struct ParamTraits : public std::conditional<IsMoveOnlyType<T>::value, 
                ParamForwardTypeMoveOnly<T>, 
                ParamForwardTypeNonMoveOnly<T> >::type {};

template <typename BoundIndices, typename StorageType,
         typename InvokeHelperType, typename UnboundForwardRunType>
struct Invoker;

template <size_t... BoundIndices,
         typename StorageType,
         typename InvokeHelperType,
         typename R,
         typename... UnboundForwardArgs>
struct Invoker<IndexSequence<BoundIndices...>, StorageType,
               InvokeHelperType, R(UnboundForwardArgs...)> {
    static R run(BindStateBase* base, UnboundForwardArgs... unbound_args) 
    {
        StorageType* storage = static_cast<StorageType*>(base);
        return InvokeHelperType::makeItSo(storage->m_runnable,
                Unwrap(std::get<BoundIndices>(storage->m_boundArgs))...,
                ParamForward(unbound_args)...);
    }
};

template <bool IsMethod, typename... Args>
struct IsRefMethod : public std::false_type {};

template <typename T, typename... Args>
struct IsRefMethod<true, std::shared_ptr<T>, Args...> 
                : public std::true_type {};

template <typename Runnable, typename RunType, typename... BoundArgs>
struct BindState;

template <typename Runnable, 
         typename R, 
         typename... Args, 
         typename... BoundArgs>
struct BindState<Runnable, R(Args...), BoundArgs...> final 
                : public BindStateBase {
private:
    using StorageType = BindState<Runnable, R(Args...), BoundArgs...>;
    using RunnableType = Runnable;

    using IsRefCall = IsRefMethod<Runnable::IsMethod::value, 
          typename std::decay<BoundArgs>::type...>;

    using BoundIndices = MakeIndexSequence<sizeof...(BoundArgs)>;
    using UnboundForwardArgs = tl::DropTypeListItem<sizeof...(BoundArgs),
          tl::TypeList<typename ParamTraits<Args>::ForwardType...> >;

    using UnboundForwardRunType = f::MakeFunctionType<R, UnboundForwardArgs>;
    using InvokeHelperType = InvokeHelper<IsRefCall::value, R, Runnable>;
    using UnboundArgs = tl::DropTypeListItem<sizeof...(BoundArgs), 
          tl::TypeList<Args...> >;

public:
    using InvokerType = Invoker<BoundIndices, 
          StorageType, 
          InvokeHelperType, 
          UnboundForwardRunType>;

    using UnboundRunType = f::MakeFunctionType<R, UnboundArgs>;

    template <typename... ForwardArgs>
    BindState(const Runnable& runnable, ForwardArgs&&... boundArgs)
        : m_runnable(runnable)
        , m_boundArgs(std::forward<ForwardArgs>(boundArgs)...)
    {
    }

    RunnableType m_runnable;
    MakeArgsStorage<IsRefCall::value, BoundArgs...> m_boundArgs;
};

} // end namespace bind

} // end namespace tars

#endif
