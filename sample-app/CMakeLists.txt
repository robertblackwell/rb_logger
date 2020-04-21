set(TARGET sample)
add_executable(${TARGET} 
	main.cpp
	class_a.cpp 
	class_b.cpp
	# ../../include/trog/loglevel.cpp
)
target_include_directories(${TARGET} PUBLIC SYSTEM ./ ${TROG_INCLUDE_DIRS} )
target_link_libraries(${TARGET} PUBLIC ${TROG_LINK_LIBS})
add_test(${TARGET} ${TARGET})

