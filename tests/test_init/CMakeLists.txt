set(TARGET test_init)
add_executable(${TARGET} 
	test_init.cpp
	../../trog/trog.cpp
	../../trog/trog.hpp
	../../trog/trog_call_data.cpp
)
target_include_directories(${TARGET} PUBLIC SYSTEM ${TEST_INCLUDE_DIRS})
target_link_libraries(${TARGET} PUBLIC ${TEST_LINK_LIBS})
add_test(${TARGET} ${TARGET})

