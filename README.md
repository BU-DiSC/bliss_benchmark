# BLISS Benchmark
The purpose of this program is to benchmark the sortedness performance on various indexes.\
This research project is part of the [Data-intensive Systems and Computing (DiSC) lab](https://disc.bu.edu/) at Boston University.

[![Basic Tests for BLISS Benchmark](https://github.com/BU-DiSC/bliss_benchmark/actions/workflows/cmake-single-platform.yml/badge.svg)](https://github.com/BU-DiSC/bliss_benchmark/actions/workflows/cmake-single-platform.yml)

## Dependencies & Components
1. ```cmake``` >= `3.10`
2. [BoDS](https://github.com/BU-DiSC/bods)
3.  [ALEX](https://github.com/microsoft/ALEX)
4.  [LIPP](https://github.com/Jiacheng-WU/lipp)

## How to Compile
1. Clone this repo\
`git clone https://github.com/BU-DiSC/bliss_benchmark.git`

2.  Create the build directory\
`cd bliss_benchmark && mkdir build`

3.  Create CMake configuration\
`cmake ..`

4.  Compile the program\
`make`

## How to Run
To run the program, use:\
`./bliss_bench [OPTIONS]`

The program currently accepts the following parameters:
```
  -d, --data_file arg              Path to the data file
  -p, --preload_factor arg         Preload factor (default: 0.5)
  -w, --write_factor arg           Write factor (default: 0.25)
  -r, --read_factor arg            Read factor (default: 0.1)
  -m, --mixed_read_write_ratio arg Read write ratio (default: 0.5)
  -s, --seed arg                   Random Seed value (default: 0)
  -v, --verbosity [=arg(=1)]       Verbosity [0: Info| 1: Debug | 2: Trace] (default: 0)
  -i, --index arg                  Index type (alex|lipp) (default: btree)
```

## Contributing to this Project
If you are interested in contributing to this benchmarking effort, 
please reach out to [Aneesh Raman](aneeshr@bu.edu) / [Andy Huynh](ndhuynh@bu.edu) / [Manos Athanassoulis](mathan@bu.edu). 

### Integrating a New Index
We primarily import indexes as CMake libraries, before integrating them into the benchmarking framework. 

#### Importing using CMake
Import the library in `external/CMakeLists.txt`. Then, link the library to the `bliss` executable in the `CMakeLists.txt` file in the root project directory. 

#### Building the Adapter
Every index in the framework uses an adapter to interact with the benchmark. These adapters are found under `src/bliss`. 

- The abstract class for the adapter is found at `src/bliss/bliss_index.h`. 
- Add the adapter code for the new index `<abc>` in its own file called `bench_abc.h` under `src/bliss`.

#### Adding to the Benchmark
The benchmark code is found at `bliss_bench.cpp`. To add the index to the benchmark: 

- Include the relevant header file, e.g., `#include bliss/bench_abc.h`. 
- In the `main()` function, add the additional condition when checking `config.index` for parsing the new index. 

#### Adding Unit Tests
Currently, we support basic unit tests with `put()` and `get()` operations in the benchmark. 

For the newly integrated index (e.g., `abc`), add relevant unit tests under the `tests/` folder. 
- Create a new directory under `tests/` for the index `abc` by prefixing the folder with `test_*` (i.e., `mkdir tests/test_abc`).
- Each index folder gets its own `CMakeLists.txt` file that will link with the outer `tests/CMakeLists.txt` file. 
- Copy the `CMakeLists.txt` file from one of the existing indexes into `tests/abc` (i.e., `cp tests/test_btree/CMakeLists.txt tests/test_abc/`). 
- Modify `tests/CMakeLists.txt` to include the new subdirectory (i.e., add a new line with `add subdirectory(test_abc)`). 

You can create one or multiple cpp files under `tests/test_abc/` for your unit tests. 

- Name every unit test file prefixed with the index name (e.g., `abc_tests.cpp`). 
- Include the header file `bliss_index_tests.h` in your test file to import common util code. 

**You may refer to `tests/test_btree/btree_tests.cpp` for samples.**

## Issues & Additional Information
You may report bugs/issues directly on Github [here](https://github.com/BU-DiSC/bliss_benchmark/issues). 

For additional information, contact: 
- [Aneesh Raman](aneeshr@bu.edu)
- [Andy Huynh](ndhuynh@bu.edu)
- [Manos Athanassoulis](mathan@bu.edu)
