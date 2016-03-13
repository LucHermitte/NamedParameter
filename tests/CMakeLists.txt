## ======[ Options ]=================================================== {{{1

# GCC specifics
if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -std=c++14")
  # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb")
  message(STATUS "GCC detected")
endif()

# Visual Studio specifics
if(MSVC)
  # Warning Level 4 for Debug builds
  list(APPEND CMAKE_CXX_FLAGS_DEBUG " /W4")
  list(REMOVE_DUPLICATES CMAKE_CXX_FLAGS_DEBUG)
endif()

## ======[ Dependencies ]============================================== {{{1
# set(Boost_DEBUG 1)
# set(Boost_DEBUG TRUE)
# set(Boost_DETAILED_FAILURE_MSG TRUE)
find_package(Boost 1.50.0 COMPONENTS unit_test_framework REQUIRED)
IF(Boost_FOUND)
    MESSAGE(STATUS "Boost_INCLUDE_DIR: ${Boost_INCLUDE_DIR}")
    MESSAGE(STATUS "Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
    MESSAGE(STATUS "Boost_UNIT_TEST_FRAMEWORK_LIBRARY: ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}")
    # MESSAGE(STATUS "Boost_FILESYSTEM_LIBRARY: ${Boost_FILESYSTEM_LIBRARY}")
    # MESSAGE(STATUS "Boost_DATE_TIME_LIBRARY: ${Boost_DATE_TIME_LIBRARY}")
ELSE(Boost_FOUND)
  MESSAGE(STATUS "Boost_DIR is: $ENV{Boost_DIR}")
  MESSAGE(FATAL_ERROR
      "Cannot build tests without Boost. Please set Boost_DIR.")
ENDIF(Boost_FOUND)

## ======[ Tests ]===================================================== {{{1

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)

# ===[ Help function for testing compilation errors {{{2
function(CHECK_COMPILATION_FAILS target error)
    # Thanks to Fraser's answer on SO:
    # http://stackoverflow.com/a/30191576/15934

    add_executable(${target} ${target}.cpp)
    # Avoid building these targets normally
    set_target_properties(${target} PROPERTIES
        EXCLUDE_FROM_ALL TRUE
        EXCLUDE_FROM_DEFAULT_BUILD TRUE)

    # Provide a PP definition to target the appropriate part of
    # "${target}.cpp", or provide separate files per test.
    target_compile_definitions(${target} PRIVATE ${target})

    # Add the test. This invokes "cmake --build ..." which is a cross-platform way of building the given
    # target.
    add_test(NAME ${target}
        COMMAND ${CMAKE_COMMAND} --build . --target ${target} --config $<CONFIGURATION>
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

    # Expect the test to fail (i.e. cmake --build should return a non-zero value)
    set_tests_properties(${target} PROPERTIES
        # WILL_FAIL TRUE
        PASS_REGULAR_EXPRESSION ${error}
        )
endfunction()

# ===[ OK {{{2
add_executable(UT_OK UT_OK.cpp)
target_link_libraries(UT_OK
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )
add_test(UT_OK UT_OK)

# ===[ Missing mandatory {{{2
CHECK_COMPILATION_FAILS(UT_NOK_missing_mandatory-1 "This parameter is mandatory, no default value is acceptable")
CHECK_COMPILATION_FAILS(UT_NOK_missing_mandatory-4 "This parameter is mandatory, no default value is acceptable")

# ===[ Argument filled passed several times {{{2
CHECK_COMPILATION_FAILS(UT_NOK_redundant_parameter-1 "A parameter cannot be instanciated several times")
CHECK_COMPILATION_FAILS(UT_NOK_redundant_parameter-4 "A parameter cannot be instanciated several times")

# ===[ Type mismatch beetween default and parameter {{{2
CHECK_COMPILATION_FAILS(UT_NOK_compatible_to_default_int "The parameter passed and the default value declared don't have compatible types!")
CHECK_COMPILATION_FAILS(UT_NOK_compatible_to_default_str "The parameter passed and the default value declared don't have compatible types!")

# ==================================================================== }}}1
# vim:set fdm=marker:
