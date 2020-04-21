build-dir=cmake-build-debug

build: ${build-dir}
	cd ${build-dir}; cmake ..; make

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
