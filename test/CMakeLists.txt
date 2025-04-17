# find_package
find_package(GTest CONFIG REQUIRED)
find_package(fmt)
find_package(nlohmann_json CONFIG REQUIRED)
set(CMAKE_CXX_STANDARD 20)

message(STATUS "test")
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(unimail_test unimail_test.cpp)
target_link_libraries(unimail_test PRIVATE fmt::fmt-header-only GTest::gtest GTest::gtest_main icurve::unimail)
target_link_directories(unimail_test PUBLIC ${PROJECT_SOURCE_DIR}/include)
add_test(NAME unimail_test COMMAND unimail_test)
