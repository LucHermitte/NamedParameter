/**@file	tests/UT_NOK_missing_mandatory-1.cpp
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
void f_copy_no_default1(Args&& ...args)
{
    auto a = na::get("a"_na, std::forward<Args>(args)...);
}

BOOST_AUTO_TEST_CASE(copy_no_default1)
{
    f_copy_no_default1();
}

// =========================================================== }}}1
// vim:set fdm=marker:
