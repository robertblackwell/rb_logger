set(TARGET test_doctest)
add_executable(${TARGET} 
	test_doctest.cpp
)
target_include_directories(${TARGET} PUBLIC ${TEST_INCLUDE_DIRS})
target_link_libraries(${TARGET} PUBLIC ${TEST_LINK_LIBS})

add_test(${TARGET} ${TARGET})

