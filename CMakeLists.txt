cmake_minimum_required(VERSION 3.5.0)
project(cc VERSION 0.2.0 LANGUAGES C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(cc_Stack_test ${CMAKE_SOURCE_DIR}/testing/cc_Stack_test.c)
add_executable(cc_Queue_test ${CMAKE_SOURCE_DIR}/testing/cc_Queue_test.c)
add_executable(cc_HashMap_test ${CMAKE_SOURCE_DIR}/testing/cc_HashMap_test.c)

add_test(cc_Stack_test cc_Stack_test)
add_test(cc_Queue_test cc_Queue_test)
add_test(cc_HashMap_test cc_HashMap_test)
