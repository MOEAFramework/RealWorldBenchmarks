package org.moeaframework.benchmarks;

import java.io.File;

import org.apache.commons.lang3.SystemUtils;
import org.moeaframework.core.Solution;
import org.moeaframework.core.variable.EncodingUtils;
import org.moeaframework.problem.NativeProblem;

/**
 * The water distribution system (WDS) problem.
 * 
 * @see https://engineering.exeter.ac.uk/research/cws/resources/benchmarks/pareto/
 */
public class WDS extends NativeProblem {
	
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

	}
	
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
