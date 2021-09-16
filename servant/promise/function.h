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

#ifndef _TARS_FUNCTION_H_
#define _TARS_FUNCTION_H_

#include "promise/template_helper.h"
#include "promise/type_list.h"

namespace tars {

namespace f {

template <typename R, typename List>
struct MakeFunctionTypeImpl;

template <typename R, typename... Args>
struct MakeFunctionTypeImpl<R, tl::TypeList<Args...> > {
    typedef R Type(Args...);
};

template <typename R, typename List>
using MakeFunctionType = typename MakeFunctionTypeImpl<R, List>::Type; 

template <typename F>
struct ExtractArgsImpl;

template <typename R, typename... Args>
struct ExtractArgsImpl<R(Args...)> {
    using Type = tl::TypeList<Args...>;
};

template <typename F>
using ExtractArgs = typename ExtractArgsImpl<F>::Type;

} // end namespace f(function)

} // end namespace tars

#endif
