package org.moeaframework.benchmarks;

import java.io.IOException;

import org.moeaframework.Analyzer;
import org.moeaframework.Executor;
import org.moeaframework.benchmarks.WDS.WDSInstance;
import org.moeaframework.core.NondominatedPopulation;
import org.moeaframework.core.Solution;
import org.moeaframework.problem.CDTLZ.C3_DTLZ4;

public class Test2 {
	
	public static void main(String[] args) throws IOException {
		//configure and run this experiment
		NondominatedPopulation result = new Executor()
				.withProblemClass(CarSideImpact.class)
				.withAlgorithm("NSGAIII")
				.withMaxEvaluations(500000)
				//.distributeOnAllCores()
				.run();
		
		//display the results
		//System.out.format("Objective1  Objective2%n");
		
		for (Solution solution : result) {
			if (!solution.violatesConstraints()) {
				System.out.format("%.4f, %.4f, %.4f,%n",
						solution.getObjective(0),
						solution.getObjective(1),
						solution.getObjective(2));
			}
		}
	}

}
