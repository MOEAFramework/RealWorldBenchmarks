package org.moeaframework.benchmarks;

import java.io.IOException;

import org.moeaframework.Executor;
import org.moeaframework.benchmarks.CarSideImpact.CarSideImpact;
import org.moeaframework.core.NondominatedPopulation;

public class Test2 {
	
	public static void main(String[] args) throws IOException {
		NondominatedPopulation result = new Executor()
				.withProblemClass(CarSideImpact.class)
				.withAlgorithm("NSGAIII")
				.withMaxEvaluations(500000)
				.run();
		
		result.display();
	}

}
