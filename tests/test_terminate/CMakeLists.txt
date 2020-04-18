set(TARGET test_terminate)
add_executable(${TARGET} 
	test_terminate.cpp
	# ../../trog/trog.cpp
	# ../../trog/trog.hpp
	# ../../trog/trog_call_data.cpp
	# ../../trog/trog_collector.cpp
)
target_include_directories(${TARGET} PUBLIC SYSTEM ${TEST_INCLUDE_DIRS})
target_link_libraries(${TARGET} PUBLIC ${TEST_LINK_LIBS})
add_test(${TARGET} ${TARGET})

