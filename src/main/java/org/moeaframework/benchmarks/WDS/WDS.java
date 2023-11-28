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
package org.moeaframework.benchmarks.WDS;

import java.io.File;

import org.apache.commons.lang3.SystemUtils;
import org.moeaframework.core.Solution;
import org.moeaframework.core.variable.EncodingUtils;
import org.moeaframework.problem.NativeProblem;

/**
 * The water distribution system (WDS) problem.
 */
public class WDS extends NativeProblem {
	
	public static final String PATH = "./native/WDS/bin/";
	
	private final WDSInstance instance;

	public WDS(WDSInstance instance) {
		super(createProcess(instance));
		this.instance = instance;
	}
	
	public static ProcessBuilder createProcess(WDSInstance instance) {
		String command = SystemUtils.IS_OS_WINDOWS ?
				PATH + instance.getName() + ".exe" :
				"./" + instance.getName();
		
		return new ProcessBuilder()
				.command(command)
				.directory(new File(PATH));
	}

	@Override
	public String getName() {
		return instance.getName();
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
			solution.setVariable(i, EncodingUtils.newInt(0, instance.getNumberOfOptions()-1));
		}
		
		return solution;
	}
	
}
