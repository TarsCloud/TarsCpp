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

#ifndef _TARS_CALL_BACK_H_
#define _TARS_CALL_BACK_H_

#include <memory>
#include "promise/bind_internal.h"

namespace tars {

namespace cb {

// class CallbackBase
class CallbackBase {
public:
    bool isNull() const { return m_bindState.get() == nullptr; }
    void reset() { m_bindState = nullptr, m_function = nullptr; }

    virtual ~CallbackBase() {}

    explicit operator bool () const
    {
        return !isNull();
    }

protected:

    using InvokeFunctionStorage = void(*)();
	
    bool equals(const CallbackBase& other) const
    {
        return m_bindState.get() == other.m_bindState.get()
            && m_function == other.m_function;
    }

    explicit CallbackBase(bind::BindStateBase* bindState)
        : m_bindState(bindState)
        , m_function(nullptr)
    {
    }

    CallbackBase(const CallbackBase& o)
        : m_bindState(o.m_bindState)
        , m_function(o.m_function)
    {
    }

    CallbackBase(CallbackBase&& o)
        : m_bindState(std::move(o.m_bindState))
        , m_function(o.m_function)
    {
        o.m_function = nullptr;
    }


    // bind state with specific parameter.
    std::shared_ptr<bind::BindStateBase> m_bindState;

    // pointer to function address.
    InvokeFunctionStorage m_function;
};

} // end namespace cb

template <typename T>
class Callback;

template <typename R, typename... Args>
class Callback<R(Args...)> : public cb::CallbackBase {
    typedef R(*PolymorphicInvoke)(bind::BindStateBase*, typename bind::ParamTraits<Args>::ForwardType...);
public:
    typedef R RunType(Args...);
    
    Callback() 
        : cb::CallbackBase(nullptr) 
    { 
    }

	template <typename Runnable, typename BindRunType, typename... BoundArgsType>
    Callback(bind::BindState<Runnable, BindRunType, BoundArgsType...>* bindState)
        : cb::CallbackBase(bindState)
    {
        PolymorphicInvoke f = &bind::BindState<Runnable, BindRunType, BoundArgsType...>::InvokerType::run;
        m_function = reinterpret_cast<InvokeFunctionStorage>(f);
    }

    bool equals(const Callback& other) const 
    {
        return CallbackBase::equals(other);
    }

    R run(typename bind::ParamTraits<Args>::ForwardType... args) const 
    {
        PolymorphicInvoke f =  reinterpret_cast<PolymorphicInvoke>(m_function);
        return f(m_bindState.get(), bind::ParamForward(args)...);
    }
};

// Syntactic sugar to make Callback<void()> easier to declare since it
// will be used in a lot of APIs with delayed execution.
using Closure = Callback<void()>;

} // end namespace tars

#endif
