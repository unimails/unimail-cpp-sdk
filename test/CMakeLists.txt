# find_package
find_package(GTest CONFIG REQUIRED)
find_package(fmt)
find_package(nlohmann_json CONFIG REQUIRED)
set(CMAKE_CXX_STANDARD 20)

message(STATUS "test")
include_directories(${PROJECT_SOURCE_DIR}/include)

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/test)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/attachment.txt ${PROJECT_BINARY_DIR}/test/attachment.txt COPYONLY)

add_executable(unimail_test unimail_test.cpp)
target_link_libraries(unimail_test PRIVATE fmt::fmt-header-only GTest::gtest GTest::gtest_main allcloud::unimail)
target_link_directories(unimail_test PUBLIC ${PROJECT_SOURCE_DIR}/include)
#set_target_properties(unimail_test PROPERTIES
# 	VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
# )
add_test(NAME unimail_test COMMAND unimail_test)
# set_tests_properties(unimail_test PROPERTIES WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
