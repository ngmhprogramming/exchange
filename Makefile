.PHONY: run test test-asan test-tsan clean

run:
	@mkdir -p build
	@cmake -S . -B build -DBUILD_TESTING_SUITE=OFF
	@cmake --build build --target engine
	@./build/engine

test:
	@mkdir -p build
	@cmake -S . -B build -DBUILD_TESTING_SUITE=ON
	@cmake --build build --target engine_tests
	@./build/tests/engine_tests

test-asan:
	@mkdir -p build
	@cmake -S . -B build -DBUILD_TESTING_SUITE=ON -DENABLE_SANITIZER_ADDRESS=ON -DENABLE_SANITIZER_UNDEFINED=ON
	@cmake --build build --target engine_tests
	@./build/tests/engine_tests

test-tsan:
	@mkdir -p build
	@cmake -S . -B build -DBUILD_TESTING_SUITE=ON -DENABLE_SANITIZER_THREAD=ON
	@cmake --build build --target engine_tests
	@./build/tests/engine_tests

clean:
	@echo "Purging build artifacts..."
	@rm -rf build