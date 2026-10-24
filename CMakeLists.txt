cmake_minimum_required(VERSION 3.25)

project(ChessBOT
    VERSION 1.0.0
    LANGUAGES C CXX
)

# ============================================================
# Language Standards
# ============================================================

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)

# ============================================================
# Source Files
# ============================================================

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
)

# ============================================================
# Header Files
# ============================================================

file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hh"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hxx"
)

# ============================================================
# Production Library
# ============================================================

# Compile the implementation files into a reusable library.
# main.cpp is excluded because it belongs to the executable.
set(LIBRARY_SOURCES ${PROJECT_SOURCES})

list(FILTER LIBRARY_SOURCES EXCLUDE REGEX
    "/main\\.(c|cc|cpp|cxx)$"
)

add_library(${PROJECT_NAME}_lib STATIC
    ${LIBRARY_SOURCES}
    ${PROJECT_HEADERS}
)

target_include_directories(${PROJECT_NAME}_lib
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# ============================================================
# Application Executable
# ============================================================

set(MAIN_SOURCE "")

foreach(SOURCE IN LISTS PROJECT_SOURCES)
    if(SOURCE MATCHES "/main\\.(c|cc|cpp|cxx)$")
        if(MAIN_SOURCE)
            message(FATAL_ERROR
                "Multiple main source files found: "
                "${MAIN_SOURCE} and ${SOURCE}"
            )
        endif()

        set(MAIN_SOURCE "${SOURCE}")
    endif()
endforeach()

if(MAIN_SOURCE)
    add_executable(${PROJECT_NAME} "${MAIN_SOURCE}")

    target_link_libraries(${PROJECT_NAME}
        PRIVATE ${PROJECT_NAME}_lib
    )
else()
    message(STATUS
        "No main source file found; skipping application executable."
    )
endif()

# ============================================================
# Compiler Warnings
# ============================================================

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME}_lib PRIVATE
        $<$<COMPILE_LANGUAGE:C>:-Wall;-Wextra;-Wpedantic>
        $<$<COMPILE_LANGUAGE:CXX>:-Wall;-Wextra;-Wpedantic>
    )
endif()

# ============================================================
# GoogleTest
# ============================================================

if(BUILD_TESTING)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
        GIT_SHALLOW TRUE
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)

    # --------------------------------------------------------
    # Test Sources
    # --------------------------------------------------------

    file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cxx"
    )

    if(TEST_SOURCES)
        add_executable(${PROJECT_NAME}_tests
            ${TEST_SOURCES}
        )

        target_link_libraries(${PROJECT_NAME}_tests
            PRIVATE
                ${PROJECT_NAME}_lib
                GTest::gtest_main
        )

        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            target_compile_options(${PROJECT_NAME}_tests PRIVATE
                -Wall
                -Wextra
                -Wpedantic
            )
        endif()

        include(GoogleTest)

        gtest_discover_tests(${PROJECT_NAME}_tests)
    else()
        message(STATUS
            "No C++ test files found in tests/."
        )
    endif()
endif()