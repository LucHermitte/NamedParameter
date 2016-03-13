CMAKE_MINIMUM_REQUIRED(VERSION 2.8.4)

#======================================================================
# Version
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "0")

SET(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

#======================================================================
# Recurse to subdirectories
option(BUILD_TESTING "Enable tests" ON)
if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
# add_subdirectory(include)
