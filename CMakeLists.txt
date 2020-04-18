cmake_minimum_required(VERSION 3.17)

project(rb_logger
    VERSION 0.1
    DESCRIPTION "A logging library"
    LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)
set(PROJECT_DIR ${rb_logger_SOURCE_DIR})
set(VENDOR_DIR ${PROJECT_DIR}/vendor)

message("rb_logger Dir: ${rb_logger_SOURCE_DIR}")
message("Project Dir: ${PROJECT_DIR}")
message("Vendor Dir: ${VENDOR_DIR}")

# this is a fix for a problem with boost libraries see https://github.com/Microsoft/vcpkg/issues/4497
# also a hack and breaks "best practices" as per Damiel Pfeiffer's presentation https://www.youtube.com/watch?v=rLopVhns4Zs&feature=youtu.be
set(other_flags "${other_flags} -frtti -fvisibility-inlines-hidden")
set(other_flags "${other_flags} -fvisibility=hidden")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${other_flags} -pthread")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
set(CTEST_CUSTOM_PRE_TEST "echo THIS IS PRE TEST")
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(TEST_LINK_LIBS 
	Threads::Threads 
	${VENDOR_DIR}/lib/libboost_filesystem.a 
	)

include(CTest) 
enable_testing()
	
set(TEST_INCLUDE_DIRS 
	SYSTEM ${PROJECT_DIR} ${VENDOR_DIR}/include)
# find_program(iwyu_path NAMES include-what-you-use iwyu)
# if(NOT iwyu_path)
#   message(FATAL_ERROR "Could not find the program include-what-you-use")
# endif()

message("TEST_INCLUDE_DIRS ${TEST_INCLUDE_DIRS}")
message("TEST_LINK_DIRS ${TEST_LINK_DIRS}")
# Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here
# Let's ensure -std=c++xx instead of -std=g++xx
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(tests)

