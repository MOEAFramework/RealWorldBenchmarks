/* Copyright 2009-2024 David Hadka and other contributors
 *
 * This file is part of the MOEA Framework.
 *
 * The MOEA Framework is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * The MOEA Framework is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with the MOEA Framework.  If not, see <http://www.gnu.org/licenses/>.
 */
package org.moeaframework.benchmarks;

import java.io.File;

import org.apache.commons.lang3.SystemUtils;
import org.moeaframework.core.Solution;
import org.moeaframework.core.constraint.Equal;
import org.moeaframework.core.objective.Maximize;
import org.moeaframework.core.objective.Minimize;
import org.moeaframework.core.variable.BinaryIntegerVariable;
import org.moeaframework.problem.ExternalProblem;

/**
 * The water distribution system (WDS) problem.
 * 
 * See https://engineering.exeter.ac.uk/research/cws/resources/benchmarks/pareto/ for more details.
 */
public class WDS extends ExternalProblem {
	
	public enum WDSInstance {
		
		TRN("TRN", 8, 8, new double[] { 0.01, 0.001 }),
		
		TLN("TLN", 8, 14, new double[] { 0.01, 0.001 }),
		
		BAK("BAK", 9, 11, new double[] { 0.001, 0.001 }),
		
		NYT("NYT", 21, 16, new double[] { 1, 0.001 }),
		
		BLA("BLA", 23, 14, new double[] { 0.001, 0.001 }),
		
		HAN("HAN", 34, 6, new double[] { 0.01, 0.001 }),
		
		GOY("GOY", 30, 8, new double[] { 0.001, 0.001 }),
		
		FOS("FOS", 58, 22, new double[] { 0.001, 0.001 }),
		
		PES("PES", 99, 13, new double[] { 0.01, 0.001 }),
		
		MOD("MOD", 317, 13, new double[] { 0.01, 0.001 }),
		
		BIN("BIN", 454, 10, new double[] { 0.01, 0.001 }),
		
		EXN("EXN", 567, 11, new double[] { 0.01, 0.001 });
		
		private final String name;
		
		private final int numberOfVariables;
		
		private final int numberOfOptions;
		
		private final double[] epsilon;
		
		WDSInstance(String name, int numberOfVariables, int numberOfOptions, double[] epsilon) {
			this.name = name;
			this.numberOfVariables = numberOfVariables;
			this.numberOfOptions = numberOfOptions;
			this.epsilon = epsilon;
		}

		public String getName() {
			return name;
		}

		public int getNumberOfVariables() {
			return numberOfVariables;
		}
		
		public int getNumberOfOptions() {
			return numberOfOptions;
		}

		public double[] getEpsilon() {
			return epsilon.clone();
		}
		
		public Builder getBuilder() {
			return new Builder()
					.withCommand(SystemUtils.IS_OS_WINDOWS ? getName() + ".exe" : getName())
					.withWorkingDirectory(new File("./native/WDS/bin/"))
					.withDebugging();
		}

	}
		
	private final WDSInstance instance;

	public WDS(WDSInstance instance) {
		super(instance.getBuilder());
		this.instance = instance;
	}

	@Override
	public String getName() {
		return "WDS(" + instance.getName() + ")";
	}

	@Override
	public int getNumberOfConstraints() {
		return 1;
	}

	@Override
	public int getNumberOfObjectives() {
		return 2;
	}

	@Override
	public int getNumberOfVariables() {
		return instance.getNumberOfVariables();
	}

	@Override
	public Solution newSolution() {
		Solution solution = new Solution(instance.getNumberOfVariables(), 2, 1);
		
		for (int i = 0; i < instance.getNumberOfVariables(); i++) {
			solution.setVariable(i, new BinaryIntegerVariable(0, instance.getNumberOfOptions()-1));
		}
		
		solution.setObjective(0, new Minimize("TotalCost ($ MM)"));
		solution.setObjective(1, new Maximize("Resilience"));
		
		solution.setConstraint(0, new Equal(0.0));
		
		return solution;
	}
	
}
