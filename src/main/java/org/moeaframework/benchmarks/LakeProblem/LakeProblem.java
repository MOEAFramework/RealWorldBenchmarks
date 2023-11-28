/* Copyright 2009-2023 David Hadka and other contributors
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
package org.moeaframework.benchmarks.LakeProblem;

import java.io.File;

import org.apache.commons.lang3.SystemUtils;
import org.moeaframework.core.Solution;
import org.moeaframework.core.variable.RealVariable;
import org.moeaframework.problem.NativeProblem;

public class LakeProblem extends NativeProblem {
	
	public static final String PATH = "./native/LakeProblem/bin/";
	
	public static final double[] EPSILON = {
		0.01, 0.01, 0.0001, 0.0001
	};
	
	public LakeProblem() {
		super(createProcess());
	}
	
	public static ProcessBuilder createProcess() {
		String command = SystemUtils.IS_OS_WINDOWS ? PATH + "lake.exe" : "./lake";
		return new ProcessBuilder().command(command).directory(new File(PATH));
	}

	@Override
	public String getName() {
		return "LakeProblem";
	}

	@Override
	public int getNumberOfVariables() {
		return 100;
	}

	@Override
	public int getNumberOfObjectives() {
		return 4;
	}

	@Override
	public int getNumberOfConstraints() {
		return 1;
	}

	@Override
	public Solution newSolution() {
		Solution solution = new Solution(100, 4, 1);
		
		for (int i = 0; i < 100; i++) {
			solution.setVariable(i, new RealVariable(0.0, 0.1));
		}
		
		return solution;
	}

}
