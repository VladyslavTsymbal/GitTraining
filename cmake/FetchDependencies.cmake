include(FetchContent)

FetchContent_Declare(
    zlib
    GIT_REPOSITORY https://github.com/madler/zlib.git
    GIT_TAG        v1.3
)
FetchContent_MakeAvailable(zlib)
set(ZLIB_INCLUDE_DIR ${zlib_SOURCE_DIR} ${zlib_BINARY_DIR})

if (ENABLE_TESTS)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
    )
    FetchContent_MakeAvailable(googletest)

    # Enable testing with GTest and compile test sources
    include(cmake/EnableTesting.cmake)
endif ()

FetchContent_Declare(
    gflags
    GIT_REPOSITORY https://github.com/gflags/gflags
    GIT_TAG        v2.2.2
)
FetchContent_MakeAvailable(gflags)

set(DEPENDENCIES_LIBS zlib)
set(DEPENDENCIES_INCLUDES ${ZLIB_INCLUDE_DIR})