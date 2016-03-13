/**@file	tests/UT_NOK_missing_mandatory-4.cpp
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

using namespace na::literals;


// ===[ copy_no_default1 ]==================================== {{{1
template <typename ...Args>
void f_copy_with_default4(Args&& ...args)
{
    auto a = na::get("a"_na = 1    , std::forward<Args>(args)...);
    auto b = na::get("b"_na        , std::forward<Args>(args)...);
    auto c = na::get("c"_na = "bar", std::forward<Args>(args)...);
    auto d = na::get("d"_na = 24.5 , std::forward<Args>(args)...);
}

BOOST_AUTO_TEST_CASE(copy_no_default5)
{
    auto       v = std::vector<int>{ 1, 2, 3, 4, 5};
    auto const x = std::vector<int>{ 11, 12, 13, 14, 15};
    f_copy_with_default4(
            "a"_na=42,
            // "b"_na=v,
            "c"_na="foo",
            "d"_na=12.5
            );
    BOOST_CHECK_EQUAL(v.size(), 5); // check v was copied
}

// =========================================================== }}}1
// vim:set fdm=marker:
