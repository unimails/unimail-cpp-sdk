@PACKAGE_INIT@

find_package(nlohmann_json CONFIG REQUIRED)
find_package(restclient-cpp CONFIG REQUIRED)

include("${CMAKE_CURRENT_LIST_DIR}/unimail-cpp-sdkTargets.cmake")
check_required_components("@PROJECT_NAME@")