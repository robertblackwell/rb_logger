set(TARGET template)
add_executable(${TARGET} 
	template_main.cpp
	myclass.cpp
	mytemplate.impl.cpp
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

