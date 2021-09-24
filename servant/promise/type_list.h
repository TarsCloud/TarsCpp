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

#ifndef _TARS_TYPE_LIST_H_
#define _TARS_TYPE_LIST_H_

namespace tars {

namespace tl {

template <typename... Types>
struct TypeList {};

// get the Nth type in a given type list.
template <size_t N, typename List>
struct TypeAtImpl;

template <size_t N, typename T, typename... List>
struct TypeAtImpl<N, TypeList<T, List...> > 
                : TypeAtImpl<N-1, TypeList<List...> > {};

template <typename T, typename... List>
struct TypeAtImpl<0, TypeList<T, List...> > {
    using Type = T;
};

// throw error while empty list.
template <> struct TypeAtImpl<0, TypeList<> > {};

template <size_t N, typename... List>
using TypeAt = typename TypeAtImpl<N, List...>::Type;

// index of a type in a given type list.
template <typename T, typename List>
struct IndexOfImpl;

template <typename T> 
struct IndexOfImpl<T, TypeList<> > {
    enum { value = -1 };
};

template <typename T, typename... List> 
struct IndexOfImpl<T, TypeList<T, List...> > {
    enum { value = 0 };
};

template <typename T, typename U, typename... List>
struct IndexOfImpl<T, TypeList<U, List...> > {
private:
    enum { temp = IndexOfImpl<T, TypeList<List...> >::value };
public:
    enum { value = ((temp == -1) ? -1 : (1 + temp)) };
};

template <typename T, typename List>
using IndexOf = IndexOfImpl<T, List>;

// drop first N types in a given type list.
template <size_t N, typename List>
struct DropTypeListItemImpl;

template <size_t N, typename T, typename... List>
struct DropTypeListItemImpl<N, TypeList<T, List...> > 
                : DropTypeListItemImpl<N-1, TypeList<List...> > {};

template <typename T, typename... List>
struct DropTypeListItemImpl<0, TypeList<T, List...> > {
    using Type = TypeList<T, List...>;
};

template <> struct DropTypeListItemImpl<0, TypeList<> > {
    using Type = TypeList<>;
};

template <size_t N, typename List>
using DropTypeListItem = typename DropTypeListItemImpl<N, List>::Type;

// take first N types in a given type list.
template <size_t N, typename List, typename... TList>
struct TakeTypeListItemImpl;

template <size_t N, typename T, typename... List, typename... TList>
struct TakeTypeListItemImpl<N, TypeList<T, List...>, TList...> 
                : TakeTypeListItemImpl<N-1, TypeList<List...>, TList..., T> {};

template <typename T, typename... List, typename... TList>
struct TakeTypeListItemImpl<0, TypeList<T, List...>, TList...> {
    using Type = TypeList<TList...>;
};

template <typename... TList> 
struct TakeTypeListItemImpl<0, TypeList<>, TList...> {
    using Type = TypeList<TList...>;
};

template <size_t N, typename List>
using TakeTypeListItem = typename TakeTypeListItemImpl<N, List>::Type;

// concat type lists.
template <typename List1, typename List2>
struct ConcatTypeListImpl;

template <typename... ListA, typename... ListB>
struct ConcatTypeListImpl<TypeList<ListA...>, TypeList<ListB...> > {
    using Type = TypeList<ListA..., ListB...>;
};

template <typename List1, typename List2>
using ConcatTypeList = typename ConcatTypeListImpl<List1, List2>::Type;

} // end namespace tl(type list)

} // end namespace tars

#endif
