
set(TARGET test_logger)


add_executable(${TARGET} 
	main.cpp
	../rb_logger/rb_logger.cpp
	../rb_logger/rb_logger.hpp
)
target_include_directories(${TARGET} PUBLIC SYSTEM
	../ 
	../rb_logger.hpp ${TEST_INCLUDE_DIRS}
)
target_compile_features(${TARGET} PRIVATE cxx_std_17)
target_link_libraries(${TARGET} PUBLIC ${TEST_LINK_LIBS})
# target_precompile_headers(${TARGET} REUSE_FROM marvin_library)

add_test(${TARGET} ${TARGET})
# SET_TESTS_PROPERTIES(${TARGET}
#     PROPERTIES ENVIRONMENT "SSL_CERT_FILE=/usr/local/etc/openssl@1.1/cert.pem")

