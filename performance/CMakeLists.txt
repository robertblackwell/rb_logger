set(TARGET dedicated_thread_speed)
add_executable(${TARGET} 
	dedicated_thread_speed.cpp
	../../include/trog/trog.hpp
	# ../../include/trog/loglevel.cpp
)
target_include_directories(${TARGET} PUBLIC SYSTEM ${TROG_VENDOR_INCLUDE_DIR})
target_link_libraries(${TARGET} PUBLIC 
	trog::trog
	Threads::Threads
	)

set(TARGET2 user_thread_speed)
add_executable(${TARGET2} 
	user_thread_speed.cpp
	../../include/trog/trog.hpp
	# ../../include/trog/loglevel.cpp
)
target_include_directories(${TARGET2} PUBLIC SYSTEM ${TROG_VENDOR_INCLUDE_DIR})
target_link_libraries(${TARGET2} PUBLIC 
	trog::trog
	Threads::Threads
	)
