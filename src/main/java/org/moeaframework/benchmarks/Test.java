package org.moeaframework.benchmarks;

import org.moeaframework.Executor;

public class Test {
	
	public static void main(String[] args) {
		new Executor()
			.withProblem("HBV")
			.withAlgorithm("NSGAII")
			.withMaxEvaluations(10000)
			.run();
	}

}
