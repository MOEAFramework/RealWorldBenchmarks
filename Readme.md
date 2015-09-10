# Real World Benchmarks #
## Multiobjective Optimization Problems ##

### General Aviation Aircraft ###
#### 27 Variables, 10 Objectives, 1 Constraint ####
The General Aviation Aircraft (GAA) problem seeks the design of three
propellar-driven aircraft for general aviation use.  The three planes
must seat 2, 4, and 6 passengers and satisfy a number of performance
and economic constraints.  See [1]-[4] for more details.

##### Decision Variables #####
    Index   Description
      0     2-seat CSPD
      1     2-seat AR
      2     2-seat SWEEP
      3     2-seat DPROP
      4     2-seat AF
      5     2-seat SEATW
      6     2-seat ELODT
      7     2-seat TAPER
      8     4-seat CSPD
      9     4-seat AR
     10     4-seat SWEEP
     11     4-seat DPROP
     12     4-seat AF
     13     4-seat SEATW
     14     4-seat ELODT
     15     4-seat TAPER
     16     6-seat CSPD
     17     6-seat AR
     18     6-seat SWEEP
     19     6-seat DPROP
     20     6-seat AF
     21     6-seat SEATW
     22     6-seat ELODT
     23     6-seat TAPER

##### Objectives #####
    Index   Description
      0     Takeoff Noise
      1     Empty Weight
      2     Direct Operating Cost
      3     Ride Roughness
      4     Fuel Weight
      5     Purchase Price
      6     Flight Range*
      7     Max Lift/Drag Ratio*
      8     Max Cruise Speed*
      9     Product Family Penalty Function
      
The * marks objectives that are maximized.  The values of maximized objectives
are negated for optimization and must be corrected in all output files.


### References ###
1. T. W. Simpson, W. Chen, J. K. Allen, and F. Mistree (1996),
   "Conceptual design of a family of products through the use of
   the robust concept exploration method," in 6th AIAA/USAF/NASA/
   ISSMO Symposium on Multidiciplinary Analysis and Optimization,
   vol. 2, pp. 1535-1545.

2. T. W. Simpson, B. S. D'Souza (2004), "Assessing variable levels
   of platform commonality within a product family using a
   multiobjective genetic algorithm," Concurrent Engineering:
   Research and Applications, vol. 12, no. 2, pp. 119-130.

3. R. Shah, P. M. Reed, and T. W. Simpson (2011), "Many-objective
   evolutionary optimization and visual analytics for product
   family design," Multiobjective Evolutionary Optimisation for
   Product Design and Manufacturing, Springer, London, pp. 137-159.

4. D. Hadka, P. M. Reed, and T. W. Simpson (2012), "Diagnostic
   Assessment of the Borg MOEA on Many-Objective Product Family
   Design Problems,"  WCCI 2012 World Congress on Computational
   Intelligence, Congress on Evolutionary Computation, Brisbane,
   Australia, pp. 986-995.