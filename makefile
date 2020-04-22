build-dir=cmake-build-debug
install-prefix=

check_install_prefix_defined = $(if $(value $(string ${install-prefix})),, $(error The variable 'install-prefix' is empty - set it on the command line))

t:
	$(check_install_prefix_defined)
	@echo target t done

build: ${build-dir}
	cmake -S ./ -B ${build-dir}
	cmake --build ${build-dir}

build_to_install: ${build-dir}
	cmake -DCCMAKE_INSTALL_PREFIX=${install-prefix} -S ./ -B ${build-dir} 
	cmake --build ${build-dir}

install: build_to_install
	cmake --build ${build-dir} --target install

${build-dir}:
	mkdir -p ${build-dir}

tests: build
	cd ${build-dir}; ctest

.PHONY:
run_performance: build
	cd ${build-dir}/performance; ./dedicated_thread_speed
	cd ${build-dir}/performance; ./user_thread_speed

.PHONY:
clean_performance:
	rm ./${build-dir}/performance/trog*.log

.PHONY:
run_sample: build
	cd ${build-dir}/sample-app; ./sample

.PHONY:
clean_sample:
	rm ./${build-dir}/sample-app/trog*.log
