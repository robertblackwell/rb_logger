set(TARGET terminate)
add_executable(${TARGET} 
	terminate_main.cpp
)
target_include_directories(${TARGET} 
	PUBLIC SYSTEM 
	./ ${TROG_VENDOR_INCLUDE_DIR}
)


target_link_libraries(${TARGET} PUBLIC
	trog::trog
	Threads::Threads
)


# add_test(${TARGET} ${TARGET})

