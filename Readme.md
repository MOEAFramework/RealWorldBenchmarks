# Real World Benchmarks #

### Available Benchmarks ###
* **General Aviation Aircraft (GAA)** - 27 Vars, 10 Objs, 1 Constr
* **HBV Rainfall-Runoff Model Calibration (HBV)** - 14 Vars, 4 Objs
* **Radar Waveform Optimization** - 8 Vars, 9 Objs
* **Water Distribution System Design**
  * **Two-reservior Network (TRN)** - 8 Vars, 2 Objs, 1 Constr
  * **Two-loop Network (TLN)** - 8 Vars, 2 Objs, 1 Constr
  * **BakRyan Network (BAK)** - 9 Vars, 2 Objs, 1 Constr
  * **New York Tunnel Network (NYT)** - 21 Vars, 2 Objs, 1 Constr
  * **Blacksburg Network (BLA)** - 23 Vars, 2 Objs, 1 Constr
  * **Hanoi Network (HAN)** - 34 Vars, 2 Objs, 1 Constr
  * **GoYang Network (GOY)** - 30 Vars, 2 Objs, 1 Constr
  * **Fossolo Network (FOS)** - 58 Vars, 2 Objs, 1 Constr
  * **Pescara Network (PES)** - 99 Vars, 2 Objs, 1 Constr
  * **Modena Network (MOD)** - 317 Vars, 2 Objs, 1 Constr
  * **Belerma Irrigation Network (BIN)** - 454 Vars, 2 Objs, 1 Constr
  * **Exeter Network (EXN)** - 567 Vars, 2 Objs, 1 Constr

### General Aviation Aircraft - 27 Vars, 10 Objs, 1 Constraint ###
The General Aviation Aircraft (GAA) problem seeks the design of three
propeller-driven aircraft for general aviation use.  The three planes
must seat 2, 4, and 6 passengers and satisfy a number of performance
and economic constraints.  See [1]-[4] for more details.

##### Decision Variables #####
    Index   Description
      0     2-seat Cruise Speed
      1     2-seat Aspect Ratio
      2     2-seat Sweep Angle
      3     2-seat Propeller Diameter
      4     2-seat Wing Loading
      5     2-seat Engine Activity Factor
      6     2-seat Seat Width
      7     2-seat Tail Length/Diameter Ratio
      8     2-seat Taper Ratio
      9     4-seat Cruise Speed
     10     4-seat Aspect Ratio
     11     4-seat Sweep Angle
     12     4-seat Propeller Diameter
     13     4-seat Wing Loading
     14     4-seat Engine Activity Factor
     15     4-seat Seat Width
     16     4-seat Tail Length/Diameter Ratio
     17     4-seat Taper Ratio
     18     6-seat Cruise Speed
     19     6-seat Aspect Ratio
     20     6-seat Sweep Angle
     21     6-seat Propeller Diameter
     22     6-seat Wing Loading
     23     6-seat Engine Activity Factor
     24     6-seat Seat Width
     25     6-seat Tail Length/Diameter Ratio
     26     6-seat Taper Ratio

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

Implementations in C and Python are available at
https://github.com/matthewjwoodruff/generalaviation.

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