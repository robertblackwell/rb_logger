cmake_minimum_required(VERSION 3.1)

##
## PROJECT
## name and version
##
project(trog VERSION 0.1.0 LANGUAGES CXX)

##
## INCLUDE
##
##
include(ExternalProject)
include(CMakePrintHelpers)
##
##
## find packages
##
find_package(Threads REQUIRED)
##
## OPTIONS
##
option(TROG_BuildTests "Build the unit tests when BUILD_TESTING is enabled." ON)
option(TROG_Install "Install CMake targets during install step." ON)
option(TROG_Package "trog does not yet being installed as a cmake package" OFF)
option(TROG_Verbose "Print trog related variables" ON)
##
## CONFIGURATION
##
include(GNUInstallDirs)

set(TROG_TARGET_NAME               ${PROJECT_NAME})
set(TROG_CONFIG_INSTALL_DIR        "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}" CACHE INTERNAL "")
set(TROG_INCLUDE_INSTALL_DIR       "${CMAKE_INSTALL_INCLUDEDIR}")
set(TROG_TARGETS_EXPORT_NAME       "${PROJECT_NAME}Targets")
set(TROG_CMAKE_CONFIG_TEMPLATE     "cmake/config.cmake.in")
set(TROG_CMAKE_CONFIG_DIR          "${CMAKE_CURRENT_BINARY_DIR}")
set(TROG_CMAKE_VERSION_CONFIG_FILE "${TROG_CMAKE_CONFIG_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
set(TROG_CMAKE_PROJECT_CONFIG_FILE "${TROG_CMAKE_CONFIG_DIR}/${PROJECT_NAME}Config.cmake")
set(TROG_CMAKE_PROJECT_TARGETS_FILE "${TROG_CMAKE_CONFIG_DIR}/${PROJECT_NAME}Targets.cmake")

set(TROG_INCLUDE_BUILD_DIR "${PROJECT_SOURCE_DIR}/include/")
message(STATUS "Using the header code from ${TROG_INCLUDE_BUILD_DIR}")

set(TROG_VENDOR_DIR "${PROJECT_SOURCE_DIR}/vendor")
message(STATUS "3rd Party dependencies are in ${TROG_VENDOR_DIR}")

set(TROG_VENDOR_INCLUDE_DIR "${TROG_VENDOR_DIR}/include")
message(STATUS "3rd Party dependencies HEADERS are in ${TROG_VENDOR_INCLUDE_DIR}")

set(TROG_VENDOR_LIB_DIR "${TROG_VENDOR_DIR}/lib")
message(STATUS "3rd Party dependencies LIBs are in ${TROG_VENDOR_LIB_DIR}")

if (TROG_Verbose)
    message("==============================================================================================")
    message("TROG_TARGET_NAME                ${PROJECT_NAME}")
    message("TROG_CONFIG_INSTALL_DIR         ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}" CACHE INTERNAL "")
    message("TROG_INCLUDE_INSTALL_DIR        ${CMAKE_INSTALL_INCLUDEDIR}")
    message("TROG_LIB_INSTALL_DIR            ${CMAKE_INSTALL_LIBDIR}")
    message("TROG_TARGETS_EXPORT_NAME        ${PROJECT_NAME}Targets")
    message("TROG_CMAKE_CONFIG_TEMPLATE      cmake/config.cmake.in")
    message("TROG_CMAKE_CONFIG_DIR           ${CMAKE_CURRENT_BINARY_DIR}")
    message("TROG_CMAKE_VERSION_CONFIG_FILE  ${TROG_CMAKE_CONFIG_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
    message("TROG_CMAKE_PROJECT_CONFIG_FILE  ${TROG_CMAKE_CONFIG_DIR}/${PROJECT_NAME}Config.cmake")
    message("TROG_CMAKE_PROJECT_TARGETS_FILE ${TROG_CMAKE_CONFIG_DIR}/${PROJECT_NAME}Targets.cmake")
    message("TROG_INCLUDE_BUILD_DIR          ${PROJECT_SOURCE_DIR}/include/")
    message("TROG_VENDOR_DIR                 ${TROG_VENDOR_DIR}")
    message("TROG_VENDOR_INCLUDE_DIR         ${TROG_VENDOR_INCLUDE_DIR}")
    message("TROG_VENDOR_LIB_DIR             ${TROG_VENDOR_LIB_DIR}")

    message("==============================================================================================")
endif()

##
## TARGET
## create target and add include path
##
add_library(${TROG_TARGET_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${TROG_TARGET_NAME} ALIAS ${TROG_TARGET_NAME})
if (${CMAKE_VERSION} VERSION_LESS "3.8.0")
    target_compile_features(${TROG_TARGET_NAME} INTERFACE cxx_range_for)
else()
    target_compile_features(${TROG_TARGET_NAME} INTERFACE cxx_std_17)
endif()

target_include_directories(
    ${TROG_TARGET_NAME}
    INTERFACE
    $<BUILD_INTERFACE:${TROG_INCLUDE_BUILD_DIR}>
    $<INSTALL_INTERFACE:include>
)

set(TROG_DEPENDS_INCLUDE_DIR 
    $<BUILD_INTERFACE:${TROG_VENDOR_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
)
set(TROG_DEPENDS_LIB_DIR 
    $<BUILD_PREFIX:${TROG_VENDOR_LIB_DIR}>
    $<INSTALL_PREFIX:lib>
)
message("CMAKE_INSTALL_LIBDIR          ${CMAKE_INSTALL_LIBDIR}")
message("TROG_DEPENDS_INCLUDE_DIR      ${TROG_DEPENDS_INCLUDE_DIR}")
message("TROG_DEPENDS_LIB_DIR          ${TROG_DEPENDS_LIB_DIR}")
set(X1 $<BUILD_INTERFACE:${TROG_INCLUDE_BUILD_DIR}>)
set(X2 $<INSTALL_INTERFACE:include>)
message("X1 ${X1}")
message("X2 ${X2}")
# ## add debug view definition file for msvc (natvis)
# if (MSVC)
#     set(NLOHMANN_ADD_NATVIS TRUE)
#     set(NLOHMANN_NATVIS_FILE "nlohmann_json.natvis")
#     target_sources(
#         ${TROG_TARGET_NAME}
#         INTERFACE
#             $<INSTALL_INTERFACE:${NLOHMANN_NATVIS_FILE}>
#             $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${NLOHMANN_NATVIS_FILE}>
#     )
# endif()

##
## TESTS
## create and configure the unit test target
##
include(CTest) #adds option BUILD_TESTING (default ON)

if(BUILD_TESTING AND TROG_BuildTests)

    enable_testing()
    add_subdirectory(tests)
    add_subdirectory(performance)
    add_subdirectory(sample-app)
endif()

##
## INSTALL
## install header files, generate and install cmake config files for find_package()
##
include(CMakePackageConfigHelpers)
# use a custom package version config file instead of
# write_basic_package_version_file to ensure that it's architecture-independent
# https://github.com/nlohmann/json/issues/1697
configure_file(
    "cmake/trogConfigVersion.cmake.in"
    ${TROG_CMAKE_VERSION_CONFIG_FILE}
    @ONLY
)
configure_file(
    ${TROG_CMAKE_CONFIG_TEMPLATE}
    ${TROG_CMAKE_PROJECT_CONFIG_FILE}
    @ONLY
)

if(TROG_Install)
    install(
        DIRECTORY ${TROG_INCLUDE_BUILD_DIR}
        DESTINATION ${TROG_INCLUDE_INSTALL_DIR}
    )
    if(TROG_Package)
        install(
            FILES ${TROG_CMAKE_PROJECT_CONFIG_FILE} ${TROG_CMAKE_VERSION_CONFIG_FILE}
            DESTINATION ${TROG_CONFIG_INSTALL_DIR}
        )
        if (NLOHMANN_ADD_NATVIS)
            install(
                FILES ${NLOHMANN_NATVIS_FILE}
                DESTINATION .
        )
        endif()
        export(
            TARGETS ${TROG_TARGET_NAME}
            NAMESPACE ${PROJECT_NAME}::
            FILE ${TROG_CMAKE_PROJECT_TARGETS_FILE}
        )
        install(
            TARGETS ${TROG_TARGET_NAME}
            EXPORT ${TROG_TARGETS_EXPORT_NAME}
            INCLUDES DESTINATION ${TROG_INCLUDE_INSTALL_DIR}
        )
        install(
            EXPORT ${TROG_TARGETS_EXPORT_NAME}
            NAMESPACE ${PROJECT_NAME}::
            DESTINATION ${TROG_CONFIG_INSTALL_DIR}
        )
    endif()
endif()
