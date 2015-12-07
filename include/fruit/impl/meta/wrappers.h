/*
 * Copyright 2014 Google Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FRUIT_META_WRAPPERS_H
#define FRUIT_META_WRAPPERS_H

#include <memory>

namespace fruit {
namespace impl {
namespace meta {

struct ConsSignature {
  template <typename ReturnType, typename... Args>
  struct apply;
  
  template <typename ReturnType, typename... Args>
  struct apply<Type<ReturnType>, Type<Args>...> {
    using type = Type<ReturnType(Args...)>;
  };
};

struct ConsStdFunction {
  template <typename Signature>
  struct apply;
  
  template <typename Signature>
  struct apply<Type<Signature>> {
    using type = Type<std::function<Signature>>;
  };
};

struct ConsUniquePtr {
  template <typename T>
  struct apply;
  
  template <typename T>
  struct apply<Type<T>> {
    using type = Type<std::unique_ptr<T>>;
  };
};

struct ConsReference {
  template <typename T>
  struct apply;
  
  template <typename T>
  struct apply<Type<T>> {
    using type = Type<T&>;
  };
};

struct IsEmpty {
  template <typename T>
  struct apply;
  
  template <typename T>
  struct apply<Type<T>> {
    using type = Bool<std::is_empty<T>::value>;
  };
};

struct IsTriviallyCopyable {
  template <typename T>
  struct apply;
  
  template <typename T>
  struct apply<Type<T>> {
    
    using type = Bool<
#if defined(__clang__)

#if __has_feature(is_trivially_copyable) && defined(_LIBCPP_VERSION)
    std::is_trivially_copyable<T>::value
#else
  // The standard library might not support is_trivially_copyable.
  // Use the internal name instead.
  __is_trivially_copyable(T)
#endif

#else
  
#if defined(__GNUC__) && __GNUC__ >= 5
    std::is_trivially_copyable<T>::value
#else
  // The compiler doesn't support __is_trivially_copyable (nor is std::is_trivially_copyable
  // supported by the library). We use this check as a proxy, but it's not exactly the same thing.
  __has_trivial_copy(T)
#endif
  
#endif
    >;
  };
};

struct IsPointer {
  template <typename T>
  struct apply;
  
  template <typename T>
  struct apply<Type<T>> {
    using type = Bool<std::is_pointer<T>::value>;
  };
};

struct IsAbstract {
  template <typename T>
  struct apply;
  
  template <typename T>
  struct apply<Type<T>> {
    using type = Bool<std::is_abstract<T>::value>;
  };
};

struct IsBaseOf {
  template <typename I, typename C>
  struct apply;
  
  template <typename I, typename C>
  struct apply<Type<I>, Type<C>> {
    using type = Bool<std::is_base_of<I, C>::value>;
  };
};

} // namespace meta
} // namespace impl
} // namespace fruit


#endif // FRUIT_META_WRAPPERS_H
