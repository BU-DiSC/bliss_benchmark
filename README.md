# bliss_benchmark
The purpose of this program is to benchmark the sortedness performance on various indexes.\
This research project is part of the [Data-intensive Systems and Computing (DiSC) lab](https://disc.bu.edu/) at Boston University.

## Dependencies & Components
1. ```cmake``` >= `3.10`
2. [BoDS](https://github.com/BU-DiSC/bods)
3.  [ALEX](https://github.com/microsoft/ALEX)
4.  [LIPP](https://github.com/Jiacheng-WU/lipp)

## How to compile
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
