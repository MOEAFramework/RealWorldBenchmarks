package org.moeaframework.benchmarks.LakeProblem;

import java.io.File;
import java.io.IOException;

import org.apache.commons.lang3.SystemUtils;
import org.moeaframework.core.Solution;
import org.moeaframework.core.variable.RealVariable;
import org.moeaframework.problem.ExternalProblem2;

public class LakeProblem extends ExternalProblem2 {
	
	public static final String PATH = "./native/LakeProblem/bin/";
	
	public static final double[] EPSILON = {
		0.01, 0.01, 0.0001, 0.0001
	};
	
	public LakeProblem() throws IOException {
		super(createProcess());
	}
	
	public static ProcessBuilder createProcess() {
		String command = SystemUtils.IS_OS_WINDOWS ?
				PATH + "lake.exe" :
				"./lake";
		
		return new ProcessBuilder()
				.command(command)
				.directory(new File(PATH));
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
