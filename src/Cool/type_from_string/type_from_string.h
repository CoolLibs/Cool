#pragma once
#include <algorithm>
#include <cassert>
#include "generated/includes.inl"

/// Thanks to https://vector-of-bool.github.io/2021/10/22/string-templates.html
/// and https://ctrpeach.io/posts/cpp20-string-literal-template-parameters/

namespace Cool {

namespace internal {

template<size_t N>
struct FixedString {
    constexpr FixedString(const char (&str)[N])
    {
        std::copy_n(str, N, value);
    }

    char value[N];
};

template<FixedString>
struct from_impl {};

#include "generated/string_to_type_associations.inl"

} // namespace internal

template<internal::FixedString S>
using type_from_string = typename internal::from_impl<S>::type;

#include "generated/evaluate_function_template.inl"

} // namespace Cool
