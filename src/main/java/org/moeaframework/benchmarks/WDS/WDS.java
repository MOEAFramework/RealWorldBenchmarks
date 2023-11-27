package org.moeaframework.benchmarks.WDS;

import java.io.File;
import java.io.IOException;

import org.apache.commons.lang3.SystemUtils;
import org.moeaframework.core.Solution;
import org.moeaframework.core.variable.EncodingUtils;
import org.moeaframework.problem.NativeProblem;

/**
 * The water distribution system (WDS) problem designed for use with the
 * MOEA Framework.
 */
public class WDS extends NativeProblem {
	
	public static final String PATH = "./native/WDS/bin/";
	
	private final WDSInstance instance;

	public WDS(WDSInstance instance) throws IOException {
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
			solution.setVariable(i, EncodingUtils.newInt(0,
					instance.getNumberOfOptions()-1));
		}
		
		return solution;
	}
	
}
