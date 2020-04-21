set(TARGET test_template)
add_executable(${TARGET} 
	test_template.cpp
	# ../../trog/call_data.cpp
	# ../../trog/writer.cpp
	myclass.cpp
	mytemplate.impl.cpp
)
target_include_directories(${TARGET} PUBLIC SYSTEM ${TEST_INCLUDE_DIRS})
target_link_libraries(${TARGET} PUBLIC ${TEST_LINK_LIBS})
add_test(${TARGET} ${TARGET})

