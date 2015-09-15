# Real World Benchmarks #

This repository contains a collection of multi and many-objective optimization problems
with real-world applications for benchmarking multiobjective evolutionary algorithms (MOEAs).

### Available Benchmarks ###

The following benchmark problems are available.

| Problem                                     | Variables | Objectives | Constraints |
| ------------------------------------------- | :-------: | :--------: | :---------: |
| General Aviation Aircraft (GAA)             | 27        | 10         | 1           |
| HBV Rainfall-Runoff Model Calibration (HBV) | 14        | 4          | 0           |
| Radar Waveform Optimization                 | 8         | 9          | 0           |
| Car Side Impact                             | 7         | 3          | 10          |
| Water Supply Portfolio Planning (LRGV)      | 8         | 5          | 4           |
| Lake Pollution Control Policy               | 100       | 4          | 1           |

In addition, this repository contains a number of water distribution system (WDS) design problems:

| Problem                          | Variables | Objectives | Constraints |
| -------------------------------- | :-------: | :--------: | :---------: |
| Two-reservior Network (TRN)      | 8         | 2          | 1           |
| Two-loop Network (TLN)           | 8         | 2          | 1           |
| BakRyan Network (BAK)            | 9         | 2          | 1           |
| New York Tunnel Network (NYT)    | 21        | 2          | 1           |
| Blacksburg Network (BLA)         | 23        | 2          | 1           |
| Hanoi Network (HAN)              | 34        | 2          | 1           |
| GoYang Network (GOY)             | 30        | 2          | 1           |
| Fossolo Network (FOS)            | 58        | 2          | 1           |
| Pescara Network (PES)            | 99        | 2          | 1           |
| Modena Network (MOD)             | 317       | 2          | 1           |
| Belerma Irrigation Network (BIN) | 454       | 2          | 1           |
| Exeter Network (EXN)             | 567       | 2          | 1           |

### License ###

Most of the software contained in this repository is copyright by the respective authors
who developed the benchmark problem.  We have provided citations to the original papers
introducing these benchmark problems.  We encourage all users to cite the appropriate
authors if using these problems in publications.

### Installation ###

Several of these benchmark problems include compiled software.  For convenience, we provide
executables for Windows users.  Linux users should ensure basic build tools are installed
(e.g., `make`, `gcc`, `g++`) and run `make` from the root directory of this repository.
