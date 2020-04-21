set(TARGET sample)
add_executable(${TARGET} 
	main.cpp
	class_a.cpp 
	class_b.cpp
	../../trog/loglevel.cpp
	../../trog/call_data.cpp
)
target_include_directories(${TARGET} PUBLIC SYSTEM ./ ${TEST_INCLUDE_DIRS} )
target_link_libraries(${TARGET} PUBLIC ${TEST_LINK_LIBS})
add_test(${TARGET} ${TARGET})

