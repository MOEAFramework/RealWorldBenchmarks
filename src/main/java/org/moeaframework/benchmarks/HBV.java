/*
Copyright (C) 2011-2012 Josh Kollat, Jon Herman, Patrick Reed and others.

The HBV Benchmark Problem is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

The HBV Benchmark Problem is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the HBV Benchmark Problem.  If not, see <http://www.gnu.org/licenses/>.
*/
package org.moeaframework.benchmarks;

import java.io.File;

import org.apache.commons.lang3.SystemUtils;
import org.moeaframework.core.Epsilons;
import org.moeaframework.core.Solution;
import org.moeaframework.core.objective.Maximize;
import org.moeaframework.core.objective.Minimize;
import org.moeaframework.core.variable.RealVariable;
import org.moeaframework.problem.ExternalProblem;

public class HBV extends ExternalProblem {
	
	public static final Epsilons EPSILON = Epsilons.of(0.01, 0.025, 0.01, 0.01);
	
	public HBV() {
		super(new Builder()
				.withCommand(SystemUtils.IS_OS_WINDOWS ? "hbv.exe" : "hbv")
				.withWorkingDirectory(new File("./native/HBV/bin/")));
	}
	
	@Override
	public String getName() {
		return "HBV";
	}

	@Override
	public int getNumberOfVariables() {
		return 14;
	}

	@Override
	public int getNumberOfObjectives() {
		return 4;
	}

	@Override
	public int getNumberOfConstraints() {
		return 0;
	}

	@Override
	public Solution newSolution() {
		Solution solution = new Solution(14, 4, 0);
		
		solution.setVariable(0, new RealVariable("L (mm)", 0.0, 100.0));
		solution.setVariable(1, new RealVariable("K0 (d)", 0.5, 20.0));
		solution.setVariable(2, new RealVariable("K1 (d)", 1.0, 100.0));
		solution.setVariable(3, new RealVariable("K2 (d)", 10.0, 20000.0));
		solution.setVariable(4, new RealVariable("Perc (mm/d)", 0.0, 100.0));
		solution.setVariable(5, new RealVariable("LP", 0.3, 1.0));
		solution.setVariable(6, new RealVariable("Fcap (mm)", 0.0, 2000.0));
		solution.setVariable(7, new RealVariable("B", 0.0, 7.0));
		solution.setVariable(8, new RealVariable("MaxBas (d)", 24.0, 120.0));
		solution.setVariable(9, new RealVariable("TT (C)", -3.0, 3.0));
		solution.setVariable(10, new RealVariable("DDF (mm/C*d)", 0.0, 20.0));
		solution.setVariable(11, new RealVariable("CFR", 0.0, 1.0));
		solution.setVariable(12, new RealVariable("CWH", 0.0, 0.8));
		solution.setVariable(13, new RealVariable("TTI (C)", 0.0, 7.0));
		
		solution.setObjective(0, new Maximize("nse"));
		solution.setObjective(1, new Minimize("trmse"));
		solution.setObjective(2, new Minimize("roce"));
		solution.setObjective(3, new Minimize("sfdce"));
		
		return solution;
	}

}
