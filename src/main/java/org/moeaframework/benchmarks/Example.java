package org.moeaframework.benchmarks;

import org.moeaframework.algorithm.NSGAII;
import org.moeaframework.core.population.NondominatedPopulation;

public class Example {
	
	public static void main(String[] args) {
		GAA problem = new GAA();

		NSGAII algorithm = new NSGAII(problem);
		algorithm.run(10000);

		NondominatedPopulation result = algorithm.getResult();
		result.display();
	}

}
