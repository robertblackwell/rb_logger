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
	${TROG_VENDOR_LIB_DIR}/libboost_filesystem.a
)


# add_test(${TARGET} ${TARGET})

