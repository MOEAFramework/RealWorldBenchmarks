# Real World Benchmarks

This repository contains a collection of multi- and many-objective optimization problems
with real-world applications for benchmarking multiobjective evolutionary algorithms (MOEAs).

[![CI](https://github.com/MOEAFramework/RealWorldBenchmarks/actions/workflows/ci.yml/badge.svg)](https://github.com/MOEAFramework/RealWorldBenchmarks/actions/workflows/ci.yml)

## Installation

These codes are intended for use with the MOEA Framework.  Follow the steps below to setup these real-world
benchmark problems:

#### Prerequisites

Ensure your system has the following software installed:
1. Java 8+
2. Maven
3. GNU Make
4. GNU C/C++ compilers (`gcc` and `g++`)

#### Setup MOEA Framework

Download the latest MOEA Framework binaries or source code from http://moeaframework.org/ and extract the archive
to a folder on your computer.  We will refer to this as folder as `${MOEAFRAMEWORK_ROOT}` below.

Alternatively, if using Maven, start a new Maven project and add a reference to the `moeaframework` dependency in
`pom.xml`.

#### Compile Benchmark Problems

Several of the benchmark problems are written in other languages and must be compiled before use.  Please follow
these steps:

1. Clone this repository - `git clone https://github.com/MOEAFramework/RealWorldBenchmarks.git`
2. Run `make -C native`
3. Copy or link the `native/` folder into your MOEA Framework directory using either:
   * Option 1 - Copy the entire directory with `cp -R native/ ${MOEAFRAMEWORK_ROOT}/native`
   * Option 2 - Create a symbolic link with `ln -s $(realpath -s native/) ${MOEAFRAMEWORK_ROOT}/native`

#### Setup Benchmarks Library

If using the source or binary distribution of the MOEA Framework, you will need to obtain `real-world-benchmarks-{version}.jar`
and place it in the `${MOEAFRAMEWORK_ROOT}/lib` folder.  You can download this file from the
[releases page](https://github.com/MOEAFramework/MOEAFramework/releases) or build the latest by running `mvn package -DskipTests`.

Maven users can simply add a dependency for this library to `pom.xml`:

```xml
<dependency>
    <groupId>org.moeaframework</groupId>
    <artifactId>real-world-benchmarks</artifactId>
    <version>1.0.0</version>
</dependency>
```

## Usage

To run one of these real-world benchmark problems, you can then either directly construct the problem:

```java

Problem problem = new GAA();

NSGAII algorithm = new NSGAII(problem);
algorithm.run(10000);

NondominatedPopulation result = algorithm.getResult();
```

or reference it by name if using the `Executor`:

```java

NondominatedPopulation result = new Executor()
   .withProblem("GAA")
   .withAlgorithm("NSGAII")
   .withMaxEvaluations(10000)
   .run();
```

## Available Benchmarks

The following benchmark problems are available:

| Problem                                     | Problem Name    | Variables | Objectives | Constraints | References |
| ------------------------------------------- | --------------- | :-------: | :--------: | :---------: | ---------- |
| General Aviation Aircraft                   | `GAA`           | 27        | 10         | 1           | [1]-[4]    |
| HBV Rainfall-Runoff Model Calibration       | `HBV`           | 14        | 4          | 0           | [5]        |
| Radar Waveform Optimization                 | `Radar`         | 8         | 9          | 0           | [6]        |
| Car Side Impact                             | `CarSideImpact` | 7         | 3          | 10          | [7]        |
| Water Supply Portfolio Planning             | `LRGV`          | 8         | 5          | 4           | [5], [8]   |
| Lake Pollution Control Policy               | `LakeProblem`   | 100       | 4          | 1           | [9]-[11]   |
| Electric Motor Product Family               | `ElectricMotor` | 80        | 20         | 60          | [12]       |

In addition, this repository contains twelve bi-objective water distribution system (WDS) design problems [13]
ranging from 8 to 567 decision variables:

| Problem                          | Problem Name | Variables | Objectives | Constraints |
| -------------------------------- | ------------ | :-------: | :--------: | :---------: |
| Two-reservior Network (TRN)      | `WDS(TRN)`   | 8         | 2          | 1           |
| Two-loop Network (TLN)           | `WDS(TLN)`   | 8         | 2          | 1           |
| BakRyan Network (BAK)            | `WDS(BAK)`   | 9         | 2          | 1           |
| New York Tunnel Network (NYT)    | `WDS(NYT)`   | 21        | 2          | 1           |
| Blacksburg Network (BLA)         | `WDS(BLA)`   | 23        | 2          | 1           |
| Hanoi Network (HAN)              | `WDS(HAN)`   | 34        | 2          | 1           |
| GoYang Network (GOY)             | `WDS(GOY)`   | 30        | 2          | 1           |
| Fossolo Network (FOS)            | `WDS(FOS)`   | 58        | 2          | 1           |
| Pescara Network (PES)            | `WDS(PES)`   | 99        | 2          | 1           |
| Modena Network (MOD)             | `WDS(MOD)`   | 317       | 2          | 1           |
| Belerma Irrigation Network (BIN) | `WDS(BIN)`   | 454       | 2          | 1           |
| Exeter Network (EXN)             | `WDS(EXN)`   | 567       | 2          | 1           |

Additional information for specific problems can be found in the cited papers as well as
the README files and other documentation for each problem.

## License

Most of the software contained in this repository is copyright by the respective authors
who developed each benchmark problem.  Please cite these original works if using any of the
benchmark problems.

## References

1. T. W. Simpson, W. Chen, J. K. Allen, and F. Mistree (1996). "Conceptual design of a family
   of products through the use of the robust concept exploration method." In 6th AIAA/USAF/NASA/
   ISSMO Symposium on Multidiciplinary Analysis and Optimization, vol. 2, pp. 1535-1545.
   ([Link](http://www.researchgate.net/publication/236735937_Conceptual_Design_of_a_Family_of_Products_Through_the_Use_of_the_Robust_Concept_Exploration_Method))

2. T. W. Simpson, B. S. D'Souza (2004). "Assessing variable levels of platform commonality within
   a product family using a multiobjective genetic algorithm." Concurrent Engineering:
   Research and Applications, vol. 12, no. 2, pp. 119-130.
   ([Link](http://cer.sagepub.com/content/12/2/119.abstract))

3. R. Shah, P. M. Reed, and T. W. Simpson (2011). "Many-objective evolutionary optimization and
   visual analytics for product family design." Multiobjective Evolutionary Optimisation for
   Product Design and Manufacturing, Springer, London, pp. 137-159.
   ([Link](http://link.springer.com/chapter/10.1007/978-0-85729-652-8_4))

4. D. Hadka, P. M. Reed, and T. W. Simpson (2012). "Diagnostic Assessment of the Borg MOEA on 
   Many-Objective Product Family Design Problems."  WCCI 2012 World Congress on Computational
   Intelligence, Congress on Evolutionary Computation, Brisbane, Australia, pp. 986-995.
   ([Link](http://ieeexplore.ieee.org/xpl/articleDetails.jsp?arnumber=6256466))

5. P.M. Reed, D. Hadka, J.D. Herman, J.R. Kasprzyk, J.B. Kollat (2013).  "Evolutionary multiobjective
   optimization in water resources: The past, present, and future."  Advances in Water Resources,
   51:438-456. ([Link](http://www.sciencedirect.com/science/article/pii/S0309170812000073))

6. E. J. Hughes (2007).  "Radar Waveform Optimisation as a Many-Objective Application Benchmark."
   Evolutionary Multi-Criterion Optimization, Lecture Notes in Computer Science, 4403:700-714.
   ([Link](http://link.springer.com/chapter/10.1007%2F978-3-540-70928-2_53))

7. J. Jain and K. Deb.  "An Evolutionary Many-Objective Optimization Algorithm Using
   Reference-Point-Based Nondominated Sorting Approach, Part II: Handling Constraints and Extending
   to an Adaptive Approach."  IEEE Transactions on Evolutionary Computation, 18(4):602-622, 2014.
   ([Link](http://ieeexplore.ieee.org/xpl/abstractKeywords.jsp?arnumber=6595567))

8. J. R. Kasprzyk, P. M. Reed, B. R. Kirsch, and G. W. Characklis (2012). "Many-Objective de Novo
   Water Supply Portfolio Planning Under Deep Uncertainty." Environmental Modelling & Software,
   34:87-104. ([Link](http://www.sciencedirect.com/science/article/pii/S1364815211001010))

9. R. Singh, P. M. Reed, and K. Keller (2015). "Many-objective robust decision making for managing an
   ecosystem with a deeply uncertain threshold response", Ecology and Society v20, No.3, 12,
   doi:10.5751/ES-07687-200312. ([Link](http://www.ecologyandsociety.org/vol20/iss3/art12/))

10. V. Ward, R. Singh, P. M. Reed, and K. Keller (2015). "Confronting Tipping Points: Can
    Multi-objective Evolutionary Algorithms Discover Pollution Control Tradeoffs Given
    Environmental Thresholds?", Environmental Modelling & Software, v73, 27-43.
    ([Link](http://www.sciencedirect.com/science/article/pii/S1364815215300256))

11. S. R. Carpenter, D. Ludwig, and W. A. Brock (1999). "Management of eutrophication for
    lakes subject to potentially irreversible change." Ecological Applications 9:751-771.
    ([Link](http://www.jstor.org/stable/2641327))

12. T. W. Simpson, J. R. A. Maier, and F. Mistree (2001).  "Product platform
    design: method and application."  Res Eng Design, 13:2-22.

13. Q. Wang, M. Guidolin, D. Savic, and Z. Kapelan (2015). "Two-Objective Design of
    Benchmark Problems of a Water Distribution System via MOEAs: Towards the
    Best-Known Approximation of the True Pareto Front." Journal of Water Resources
    Planning and Management, 141(3), 04014060.
    ([Link](http://ascelibrary.org/doi/abs/10.1061/%28ASCE%29WR.1943-5452.0000460))
