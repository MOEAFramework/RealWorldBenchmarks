# Real World Benchmarks #

This repository contains a collection of multi and many-objective optimization problems
with real-world applications for benchmarking multiobjective evolutionary algorithms (MOEAs).

### Available Benchmarks ###

The following benchmark problems are available.

| Problem                                     | Variables | Objectives | Constraints | References |
| ------------------------------------------- | :-------: | :--------: | :---------: | ---------- |
| General Aviation Aircraft (GAA)             | 27        | 10         | 1           | [1]-[4]    |
| HBV Rainfall-Runoff Model Calibration (HBV) | 14        | 4          | 0           | [5]        |
| Radar Waveform Optimization                 | 8         | 9          | 0           | [6]        |
| Car Side Impact                             | 7         | 3          | 10          | [7]        |
| Water Supply Portfolio Planning (LRGV)      | 8         | 5          | 4           | [5], [8]   |
| Lake Pollution Control Policy               | 100       | 4          | 1           | [9]-[11]   |

In addition, this repository contains twelve bi-objective water distribution system (WDS) design problems [12]
ranging from 8 to 567 decision variables:

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

Additional information for specific problems can be found in the cited papers as well as
the README files and other documentation for each problem.

### License ###

Most of the software contained in this repository is copyright by the respective authors
who developed the benchmark problem.  We have provided citations to the original papers
introducing these benchmark problems.  We encourage all users to cite the appropriate
authors if using these problems in publications.

### Installation ###

Several of these benchmark problems include compiled software.  For convenience, we provide
executables for Windows users.  Linux users should ensure basic build tools are installed
(e.g., `make`, `gcc`, `g++`) and run `make` from the root directory of this repository.

### References ###

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

12. Q. Wang, M. Guidolin, D. Savic, and Z. Kapelan (2015). "Two-Objective Design of
    Benchmark Problems of a Water Distribution System via MOEAs: Towards the
    Best-Known Approximation of the True Pareto Front." Journal of Water Resources
    Planning and Management, 141(3), 04014060.
    ([Link](http://ascelibrary.org/doi/abs/10.1061/%28ASCE%29WR.1943-5452.0000460))
