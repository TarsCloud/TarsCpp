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

#ifndef _TARS_BIND_H_
#define _TARS_BIND_H_

#include "promise/bind_internal.h"
#include "promise/callback.h"

namespace tars {

template <typename Functor, typename... Args>
struct MakeUnboundRunTypeImpl {
    using Type = typename bind::BindState<
        typename bind::FunctorTraits<Functor>::RunnableType,
                 typename bind::FunctorTraits<Functor>::RunType,
                 Args...>::UnboundRunType;

};

template <typename Functor, typename... Args>
using MakeUnboundRunType = typename MakeUnboundRunTypeImpl<Functor, Args...>::Type;

template <typename Functor, typename... Args>
Callback<MakeUnboundRunType<Functor, Args...> > Bind(Functor f, Args&&... args)
{
    using RunnableType = typename bind::FunctorTraits<Functor>::RunnableType;
    using RunType = typename bind::FunctorTraits<Functor>::RunType;
    using BindState = bind::BindState<RunnableType, RunType, Args...>;

    return Callback<typename BindState::UnboundRunType>(
            new BindState(bind::MakeRunnable(f), std::forward<Args>(args)...));
}

} // end namespace tars

#endif
