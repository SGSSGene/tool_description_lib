cmake_minimum_required (VERSION 3.12)

# check if this is the main project
if (NOT DEFINED PROJECT_NAME)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(TDL_ROOT_PROJECT TRUE)
endif ()

project (tdl LANGUAGES CXX VERSION "${TDL_PROJECT_VERSION}"
             DESCRIPTION "tdl -- tool description library")

add_library(tdl INTERFACE)
target_include_directories(tdl INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>)



if (${TDL_ROOT_PROJECT})
    enable_testing()
    file(GLOB TEST_CPP_FILES
         LIST_DIRECTORIES false
         RELATIVE ${PROJECT_SOURCE_DIR}
         CONFIGURE_DEPENDS
         src/test_tdl/*.cpp)
    add_executable(test_tdl ${TEST_CPP_FILES})
    target_link_libraries(test_tdl tdl)
    add_test(NAME test_tdl COMMAND test_tdl)
endif()
