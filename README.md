# exchange
An exchange engine built in C++.

## Make Quickstart
This project uses CMake.

- `make run`: run the main engine
- `make generate`: generate sample binary input file
- `make test`: run test suite
- `make test-asan`: run test suite with ASan and UBSan
- `make test-tsan`: run test suite with TSan
- `make clean`: clean build cache

## Manual Build

### Running the Engine

```
mkdir build
cmake -S . -B build
cmake --build build
./build/engine
```

### Testing

To run unit tests:
```
mkdir -p build
cmake -S . -B build
cmake --build build
./build/tests/engine_tests
```

Optionally, the following compiler flags can be set to enable sanitizers:
```
-DENABLE_SANITIZER_ADDRESS=ON
-DENABLE_SANITIZER_UNDEFINED=ON
-DENABLE_SANITIZER_THREAD=ON
```

Run tests with Address Sanitizer (ASan) and Undefined Behavior Sanitizer (UBSan):
```
mkdir -p build
cmake -S . -B build -DENABLE_SANITIZER_ADDRESS=ON -DENABLE_SANITIZER_UNDEFINED=ON
cmake --build build
./build/tests/engine_tests
```

Run tests with Thread Sanitizer (TSan):
```
mkdir -p build
cmake -S . -B build -DENABLE_SANITIZER_THREAD=ON
cmake --build build
./build/tests/engine_tests
```