cmake_minimum_required(VERSION 3.10)

project(unimail-cpp-sdk VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(UNIMAIL_MASTER_PROJECT OFF)
set(UNIMAIL_GENERATED_INCLUDE_DIR ${PROJECT_BINARY_DIR}/generated/include)


if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  # message(STATUS "CMAKE_CURRENT_SOURCE_DIR = CMAKE_SOURCE_DIR")
  set(UNIMAIL_MASTER_PROJECT ON)
  if (NOT DEFINED UNIMAIL_TEST)
    set(UNIMAIL_TEST "build unimail tests" ON)
  endif()
  # message(STATUS "CMake version: ${CMAKE_VERSION}")
else()
  # message(STATUS "CMAKE_CURRENT_SOURCE_DIR != CMAKE_SOURCE_DIR")
endif()

include(GNUInstallDirs)

file(MAKE_DIRECTORY ${UNIMAIL_GENERATED_INCLUDE_DIR}/allcloud)
configure_file(
  ${PROJECT_SOURCE_DIR}/include/allcloud/unimail-version.h.in
  ${UNIMAIL_GENERATED_INCLUDE_DIR}/allcloud/unimail-version.h
  @ONLY)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SOURCE)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(restclient-cpp CONFIG REQUIRED)

add_library(unimail STATIC ${SOURCE})
add_library(allcloud::unimail ALIAS unimail)

set_target_properties(
  unimail PROPERTIES VERSION ${PROJECT_VERSION}
                    SOVERSION ${PROJECT_VERSION_MAJOR})
target_include_directories(unimail PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                $<BUILD_INTERFACE:${UNIMAIL_GENERATED_INCLUDE_DIR}>
                $<INSTALL_INTERFACE:include>)
target_link_libraries(unimail PUBLIC nlohmann_json::nlohmann_json restclient-cpp)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${UNIMAIL_GENERATED_INCLUDE_DIR}/allcloud/unimail-version.h
      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/allcloud)


# 进行库安装
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
install(
  TARGETS unimail
  EXPORT ${TARGETS_EXPORT_NAME}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION include
  FRAMEWORK DESTINATION ".")

# 添加后续使用 cmake 导入的相关配置
include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake
  ${PROJECT_BINARY_DIR}/cmake/${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION lib/cmake/${PROJECT_NAME})

install(FILES "${PROJECT_BINARY_DIR}/cmake/${PROJECT_NAME}Config.cmake"
  DESTINATION lib/cmake/${PROJECT_NAME})

install(EXPORT ${TARGETS_EXPORT_NAME}
  NAMESPACE icurve::
  DESTINATION "lib/cmake/${PROJECT_NAME}")

if (UNIMAIL_TEST)
    message(STATUS "unimail tests...")
    enable_testing()
    add_subdirectory(test)
endif()