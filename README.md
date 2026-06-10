# exchange
An exchange engine built in C++.

## Build Instructions
This project uses CMake.

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