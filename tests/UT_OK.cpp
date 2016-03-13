/**@file	tests/UT_OK.cpp
 * @author	Luc Hermitte <EMAIL:luc{dot}hermitte{at}gmail{dot}com>
 *
 * Distributed under the Boost Software License, Version 1.0.
 * See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt
 */
#define BOOST_TEST_MODULE "Named parameters related tests"
#define BOOST_TEST_DYN_LINK

#include "named-parameters.hpp"
#include <boost/test/unit_test.hpp>
#include <vector>
#include <complex>

using namespace na::literals;

// ===[ copy_no_default1 ]==================================== {{{1
template <typename F, typename ...Args>
void f_copy_no_default1(F checks, Args&& ...args)
{
    auto a = na::get("a"_na, std::forward<Args>(args)...);
    checks(a);
}

BOOST_AUTO_TEST_CASE(copy_no_default1)
{
    f_copy_no_default1(
            [](int a) { BOOST_CHECK_EQUAL(a, 42); },
            "a"_na=42);
    f_copy_no_default1(
            [](std::string const& a) { BOOST_CHECK_EQUAL(a, "foo"); },
            "a"_na="foo");
}

// ===[ too_many_params ]===================================== {{{1
// too many parameters
BOOST_AUTO_TEST_CASE(too_many_params)
{
    f_copy_no_default1(
            [](std::string const& a) { BOOST_CHECK_EQUAL(a, "foo"); },
            "a"_na="foo", "b"_na=42);

    // redunding an unused parameter
    f_copy_no_default1(
            [](std::string const& a) { BOOST_CHECK_EQUAL(a, "foo"); },
            "a"_na="foo", "b"_na="foo", "b"_na=42);
}

// ===[ copy_with_default1 ]==================================== {{{1
template <typename F, typename ...Args>
void f_copy_with_default1(F checks, Args&& ...args)
{
    auto a = na::get("a"_na=5, std::forward<Args>(args)...);
    checks(a);
}

BOOST_AUTO_TEST_CASE(copy_with_default1)
{
    f_copy_with_default1(
            [](auto && a) { BOOST_CHECK_EQUAL(a, 42); },
            "a"_na=42);
    f_copy_with_default1(
            [](auto && a) { BOOST_CHECK_EQUAL(a, 5); }
            );
}

// ===[ copy_with_compatible_default_int ]====================== {{{1
BOOST_AUTO_TEST_CASE(copy_with_compatible_default_int)
{
    f_copy_with_default1(
            [](auto && a) { BOOST_CHECK_EQUAL(a, true); },
            "a"_na=true);
    f_copy_with_default1(
            [](auto && a) { BOOST_CHECK_EQUAL(a, 42.5); },
            "a"_na=42.5);
    f_copy_with_default1(
            [](auto && a) { BOOST_CHECK_EQUAL(a, 8); },
            "a"_na=short{8});
    auto c = std::complex<double>(1,5);
    f_copy_with_default1(
            [&](auto && a) { BOOST_CHECK_EQUAL(a, c); },
            "a"_na=c);
}

// ===[ copy_with_compatible_default_str ]====================== {{{1
template <typename F, typename ...Args>
void f_copy_with_default_str(F checks, Args&& ...args)
{
    auto s = std::string{"bar"};
    auto a = na::get("a"_na="foo", std::forward<Args>(args)...);
    auto b = na::get("b"_na=s    , std::forward<Args>(args)...);
    checks(a, b);
}

BOOST_AUTO_TEST_CASE(copy_with_compatible_default_str)
{
    auto checker = [](auto && a, auto && b) {
        BOOST_CHECK_EQUAL(a, "toto");
        BOOST_CHECK_EQUAL(b, "titi");
    } ;

    auto       s  = std::string{"titi"};
    auto const sc = std::string{"titi"};

    // same types
    f_copy_with_default_str(checker, "a"_na="toto", "b"_na=s);
    // mixings!
    // f_copy_with_default_str(checker, "a"_na=s, "b"_na="toto");
    // f_copy_with_default_str(checker, "a"_na=sc, "b"_na=sc);
}

// ===[ copy_no_default5 ]==================================== {{{1
template <typename F, typename ...Args>
void f_copy_no_default5(F checks, Args&& ...args)
{
    auto a = na::get("a"_na, std::forward<Args>(args)...);
    auto b = na::get("b"_na, std::forward<Args>(args)...);
    auto c = na::get("c"_na, std::forward<Args>(args)...);
    auto d = na::get("d"_na, std::forward<Args>(args)...);
    auto e = na::get("e"_na, std::forward<Args>(args)...);
    checks(a, b, c, d, e);
    b.push_back(42);
    e.push_back(42);
}

BOOST_AUTO_TEST_CASE(copy_no_default5)
{
    auto       v = std::vector<int>{ 1, 2, 3, 4, 5};
    auto const x = std::vector<int>{ 11, 12, 13, 14, 15};
    f_copy_no_default5(
            [&](auto&& a, auto&& b, auto&& c, auto&& d, auto&& e) {
            BOOST_CHECK_EQUAL(a, 42);
            BOOST_CHECK_EQUAL_COLLECTIONS(
                    std::begin(b), std::end(b),
                    std::begin(v), std::end(v));
            BOOST_CHECK_EQUAL(c, "foo");
            BOOST_CHECK_EQUAL(d, 12.5);
            BOOST_CHECK_EQUAL_COLLECTIONS(
                    std::begin(e), std::end(e),
                    std::begin(x), std::end(x));
            },
            "a"_na=42,
            "b"_na=v,
            "c"_na="foo",
            "d"_na=12.5,
            "e"_na=x
            );
    BOOST_CHECK_EQUAL(v.size(), 5); // check v was copied
}


// ===[ copy_with_default5 ]==================================== {{{1
template <typename F, typename ...Args>
void f_copy_with_default5(F checks, Args&& ...args)
{
    auto a = na::get("a"_na = 1                 , std::forward<Args>(args)...);
    auto b = na::get("b"_na = std::vector<int>{}, std::forward<Args>(args)...);
    auto c = na::get("c"_na = "bar"             , std::forward<Args>(args)...);
    auto d = na::get("d"_na = 24.5              , std::forward<Args>(args)...);
    auto e = na::get("e"_na, std::forward<Args>(args)...);
    checks(a, b, c, d, e);
    b.push_back(42);
    e.push_back(42);
}

BOOST_AUTO_TEST_CASE(copy_with_default5)
{
    // const std::vector<int> v = { 1, 2, 3, 4, 5};
    auto       v = std::vector<int>{ 1, 2, 3, 4, 5};
    auto const x = std::vector<int>{ 11, 12, 13, 14, 15};
    f_copy_with_default5(
            [&](auto&& a, auto&& b, auto&& c, auto&& d, auto&& e) {
            BOOST_CHECK_EQUAL(a, 42);
            BOOST_CHECK_EQUAL_COLLECTIONS(
                    std::begin(b), std::end(b),
                    std::begin(v), std::end(v));
            BOOST_CHECK_EQUAL(c, "foo");
            BOOST_CHECK_EQUAL(d, 12.5);
            BOOST_CHECK_EQUAL_COLLECTIONS(
                    std::begin(e), std::end(e),
                    std::begin(x), std::end(x));
            },
            "a"_na=42,
            "b"_na=v,
            "c"_na="foo",
            "d"_na=12.5,
            "e"_na=x
            );
    BOOST_CHECK_EQUAL(v.size(), 5); // check v was copied

    // unsorted
    f_copy_with_default5(
            [&](auto&& a, auto&& b, auto&& c, auto&& d, auto&& e) {
            BOOST_CHECK_EQUAL(a, 42);
            BOOST_CHECK_EQUAL_COLLECTIONS(
                    std::begin(b), std::end(b),
                    std::begin(v), std::end(v));
            BOOST_CHECK_EQUAL(c, "foo");
            BOOST_CHECK_EQUAL(d, 12.5);
            BOOST_CHECK_EQUAL_COLLECTIONS(
                    std::begin(e), std::end(e),
                    std::begin(x), std::end(x));
            },
            "d"_na=12.5,
            "a"_na=42,
            "c"_na="foo",
            "b"_na=v,
            "e"_na=x
            );
    BOOST_CHECK_EQUAL(v.size(), 5); // check v was copied

    f_copy_with_default5(
            [&](auto&& a, auto&& b, auto&& c, auto&& d, auto&& e) {
            BOOST_CHECK_EQUAL(a, 1);
            BOOST_CHECK_EQUAL(b.size(), 0);
            BOOST_CHECK_EQUAL(c, "bar");
            BOOST_CHECK_EQUAL(d, 24.5);
            BOOST_CHECK_EQUAL_COLLECTIONS(
                    std::begin(e), std::end(e),
                    std::begin(x), std::end(x));
            },
            "e"_na=x
            );
    BOOST_CHECK_EQUAL(v.size(), 5); // check v was copied
}


// ===[ reference_no_default1 ]==================================== {{{1
template <typename F, typename ...Args>
void f_reference_no_default1(F checks, Args&& ...args)
{
    auto&& a = na::get("a"_na, std::forward<Args>(args)...);
    checks(a);
}

BOOST_AUTO_TEST_CASE(reference_no_default1)
{
    {
        int a0 = 42;
        f_reference_no_default1(
                [](auto && a) { BOOST_CHECK_EQUAL(a, 42); a+=1; },
                "a"_na=a0);
        BOOST_CHECK_EQUAL(a0, 43);
    }
    {
        std::string a0 = "foo";
        f_reference_no_default1(
                [](auto && a) { BOOST_CHECK_EQUAL(a, "foo"); a+= "bar"; },
                "a"_na=a0);
        BOOST_CHECK_EQUAL(a0, "foobar");
    }
}

// ===[ reference_no_default5 ]==================================== {{{1
template <typename F, typename ...Args>
void f_reference_no_default5(F checks, Args&& ...args)
{
    auto && a = na::get("a"_na, std::forward<Args>(args)...);
    auto && b = na::get("b"_na, std::forward<Args>(args)...);
    auto && c = na::get("c"_na, std::forward<Args>(args)...);
    auto && d = na::get("d"_na, std::forward<Args>(args)...);
    auto && e = na::get("e"_na, std::forward<Args>(args)...);
    checks(a, b, c, d, e);
    b.push_back(42);
}

BOOST_AUTO_TEST_CASE(reference_no_default5)
{
    auto       v = std::vector<int>{ 1, 2, 3, 4, 5};
    auto const x = std::vector<int>{ 11, 12, 13, 14, 15};
    f_reference_no_default5(
            [&](auto&& a, auto&& b, auto&& c, auto&& d, auto&& e) {
            BOOST_CHECK_EQUAL(a, 42);
            BOOST_CHECK_EQUAL_COLLECTIONS(
                    std::begin(b), std::end(b),
                    std::begin(v), std::end(v));
            BOOST_CHECK_EQUAL(c, "foo");
            BOOST_CHECK_EQUAL(d, 12.5);
            BOOST_CHECK_EQUAL_COLLECTIONS(
                    std::begin(e), std::end(e),
                    std::begin(x), std::end(x));
            },
            "a"_na=42,
            "b"_na=v,
            "c"_na="foo",
            "d"_na=12.5,
            "e"_na=x
            );
    BOOST_CHECK_EQUAL(v.size(), 6); // check v was copied
}

// ===[ rvref_cref_ref ]=========================================== {{{1
template <typename F, typename ...Args>
void f_rvref_cref_ref(F checks, Args&& ...args)
{
    auto && a = na::get("a"_na, std::forward<Args>(args)...);
    auto && b = na::get("b"_na, std::forward<Args>(args)...);
    auto && c = na::get("c"_na, std::forward<Args>(args)...);
    auto && d = na::get("d"_na, std::forward<Args>(args)...);
    auto && e = na::get("e"_na, std::forward<Args>(args)...);
    checks(a, b, c, d, e);
    a.push_back(42);
    // b.push_back(42);
    c.i ++;
    // d.i ++;
    e += "bar"; // <- changing a rvref
}

std::string f() { return "toto"; }
struct my_noncopyable {
    int i;
    my_noncopyable(my_noncopyable const&) = delete;
    my_noncopyable & operator=(my_noncopyable const&) = delete;
};

BOOST_AUTO_TEST_CASE(rvref_cref_ref)
{
    // copiable stuff
    auto       lv  = std::vector<int>{ 1, 2, 3, 4, 5};
    auto const clv = std::vector<int>{ 11, 12, 13, 14, 15};
    // non-copyable stuff
    my_noncopyable        nc{1};
    my_noncopyable const cnc{2};
    f_rvref_cref_ref(
            [&](auto&& a, auto&& b, auto&& c, auto&& d, auto&& e) {
            BOOST_CHECK_EQUAL_COLLECTIONS(
                    std::begin(a), std::end(a),
                    std::begin(lv), std::end(lv));
            BOOST_CHECK_EQUAL_COLLECTIONS(
                    std::begin(b), std::end(b),
                    std::begin(clv), std::end(clv));
            BOOST_CHECK_EQUAL(c.i, 1);
            BOOST_CHECK_EQUAL(d.i, 2);
            BOOST_CHECK_EQUAL(e, "toto");
            },
            "a"_na=lv,
            "b"_na=clv,
            "c"_na=nc,
            "d"_na=cnc,
            "e"_na=f()
            );
    // BOOST_CHECK_EQUAL(v.size(), 6); // check v was copied
}

// ===[ reference_with_default5 ]==================================== {{{1
#if 0
// References are not compatible with default value!
template <typename F, typename ...Args>
void f_reference_with_default5(F checks, Args&& ...args)
{
    auto && a = na::get("a"_na = 1                 , std::forward<Args>(args)...);
    auto && b = na::get("b"_na = std::vector<int>{}, std::forward<Args>(args)...);
    auto && c = na::get("c"_na = "bar"             , std::forward<Args>(args)...);
    auto && d = na::get("d"_na = 24.5              , std::forward<Args>(args)...);
    auto && e = na::get("e"_na                     , std::forward<Args>(args)...);
    checks(a, b, c, d, e);
    b.push_back(42);
    // e.push_back(42);
}

BOOST_AUTO_TEST_CASE(reference_with_default5)
{
    auto       v = std::vector<int>{ 1, 2, 3, 4, 5};
    auto const x = std::vector<int>{ 11, 12, 13, 14, 15};

    f_reference_with_default5(
            [&](auto&& a, auto&& b, auto&& c, auto&& d, auto&& e) {
            BOOST_CHECK_EQUAL(a, 42);
            BOOST_CHECK_EQUAL_COLLECTIONS(
                    std::begin(b), std::end(b),
                    std::begin(v), std::end(v));
            BOOST_CHECK_EQUAL(c, "foo");
            BOOST_CHECK_EQUAL(d, 12.5);
            BOOST_CHECK_EQUAL_COLLECTIONS(
                    std::begin(e), std::end(e),
                    std::begin(x), std::end(x));
            },
            "d"_na=12.5,
            "a"_na=42,
            "c"_na="foo",
            "b"_na=v,
            "e"_na=x
            );
    BOOST_CHECK_EQUAL(v.size(), 6); // check v was copied

    // References are not compatible with default value!
    f_reference_with_default5(
            [&](auto&& a, auto&& b, auto&& c, auto&& d, auto&& e) {
            BOOST_CHECK_EQUAL(a, 1);
            BOOST_CHECK_EQUAL(b.size(), 0);
            BOOST_CHECK_EQUAL(c, "bar");
            BOOST_CHECK_EQUAL(d, 24.5);
            BOOST_CHECK_EQUAL_COLLECTIONS(
                    std::begin(e), std::end(e),
                    std::begin(x), std::end(x));
            },
            "e"_na=x
            );
    BOOST_CHECK_EQUAL(v.size(), 6); // check v was copied
}
#endif

// =========================================================== }}}1
// vim:set fdm=marker:
