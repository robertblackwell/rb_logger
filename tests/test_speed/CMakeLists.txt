set(TARGET test_speed)
add_executable(${TARGET} 
	test_speed.cpp
	../../trog/trog.cpp
	../../trog/trog.hpp
)
target_include_directories(${TARGET} PUBLIC SYSTEM ${TEST_INCLUDE_DIRS})
target_link_libraries(${TARGET} PUBLIC ${TEST_LINK_LIBS})
add_test(${TARGET} ${TARGET})

