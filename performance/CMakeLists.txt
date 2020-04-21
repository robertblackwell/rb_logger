set(TARGET dedicated_thread_speed)
add_executable(${TARGET} 
	dedicated_thread_speed.cpp
	../../include/trog/trog.hpp
	# ../../include/trog/loglevel.cpp
)
target_include_directories(${TARGET} PUBLIC SYSTEM ${TROG_INCLUDE_DIRS})
target_link_libraries(${TARGET} PUBLIC ${TROG_LINK_LIBS})

set(TARGET2 user_thread_speed)
add_executable(${TARGET2} 
	user_thread_speed.cpp
	../../include/trog/trog.hpp
	# ../../include/trog/loglevel.cpp
)
target_include_directories(${TARGET2} PUBLIC SYSTEM ${TROG_INCLUDE_DIRS})
target_link_libraries(${TARGET2} PUBLIC ${TROG_LINK_LIBS})
