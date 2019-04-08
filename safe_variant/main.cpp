
// MIT License
//
// Copyright (c) 2019 degski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <type_traits>
#include <variant>

#include <boost/mp11.hpp>
#include <boost/variant.hpp>

namespace mp11 = boost::mp11;

struct Foo {
    Foo ( ) = delete;
};

struct Bar {
    Bar & operator= ( const Bar & ) = delete;
};

struct Baz {
    Baz & operator= ( Baz && ) = delete;
};

template<typename... Args>
struct safe_std_variant : public std::variant<Args...> {
    static_assert ( std::conjunction<std::is_trivially_constructible<Args>...>::value, "is not trivially constructible" );
};

template<typename... Args>
struct safe_boost_variant : public boost::variant<Args...> {
    static_assert ( mp11::mp_and<std::is_trivially_constructible<Args>...>::value, "is not trivially constructible" );
};

template<typename... Args>
struct evensafer_std_variant : public std::variant<Args...> {
    static_assert ( std::conjunction<std::conjunction<std::is_nothrow_constructible<Args>...>,
                                     std::conjunction<std::is_nothrow_copy_assignable<Args>...>,
                                     std::conjunction<std::is_nothrow_move_assignable<Args>...>>::value,
                    "is not nothrow constructible or not nothrow assignable" );
};

template<typename... Args>
struct evensafer_boost_variant : public boost::variant<Args...> {
    static_assert (
        mp11::mp_and<mp11::mp_and<std::is_nothrow_constructible<Args>...>, mp11::mp_and<std::is_nothrow_copy_assignable<Args>...>,
                     mp11::mp_and<std::is_nothrow_move_assignable<Args>...>>::value,
        "is not nothrow constructible or not nothrow assignable" );
};

int main ( ) {

    safe_std_variant<int, double, bool> sv1;              // does compile
    safe_boost_variant<int, double, bool> sv2;            // does compile
    evensafer_std_variant<int, double, bool> sv3;         // does compile
    evensafer_boost_variant<int, double, bool> sv4;       // does compile
    safe_std_variant<int, double, bool, Foo> sv5;         // does not compile
    safe_boost_variant<int, double, bool, Foo> sv6;       // does not compile
    evensafer_std_variant<int, double, bool, Foo> sv7;    // does not compile
    evensafer_boost_variant<int, double, bool, Foo> sv8;  // does not compile
    safe_std_variant<int, double, bool, Bar> sv9;         // does compile
    safe_boost_variant<int, double, bool, Bar> sv10;      // does compile
    evensafer_std_variant<int, double, bool, Bar> sv11;   // does not compile
    evensafer_boost_variant<int, double, bool, Bar> sv12; // does not compile
    safe_std_variant<int, double, bool, Baz> sv13;        // does compile
    safe_boost_variant<int, double, bool, Baz> sv14;      // does compile
    evensafer_std_variant<int, double, bool, Baz> sv15;   // does not compile
    evensafer_boost_variant<int, double, bool, Baz> sv16; // does not compile
}
