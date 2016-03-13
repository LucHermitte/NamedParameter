/**@file	tests/UT_NOK_redundant_parameter-4.cpp
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


// ===[ copy_no_default4 ]==================================== {{{1
template <typename ...Args>
void f_copy_with_default4(Args&& ...args)
{
    auto a = na::get("a"_na = 1, std::forward<Args>(args)...);
    auto b = na::get("b"_na = 2, std::forward<Args>(args)...);
    auto c = na::get("c"_na = 3, std::forward<Args>(args)...);
    auto d = na::get("d"_na = 4, std::forward<Args>(args)...);
}

BOOST_AUTO_TEST_CASE(redundant_parameter4)
{
    f_copy_with_default4(
            "a"_na=42,
            "b"_na=42,
            "c"_na="bar",
            "d"_na=42,
            "b"_na="foo"
            );
}

// =========================================================== }}}1
// vim:set fdm=marker:
