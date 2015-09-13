package org.moeaframework.benchmarks;

import java.io.File;
import java.io.IOException;

import org.moeaframework.Executor;
import org.moeaframework.benchmarks.HBV.HBV;
import org.moeaframework.core.EpsilonBoxDominanceArchive;
import org.moeaframework.core.PopulationIO;

public class GenerateReferenceSet {
	
	public static void main(String[] args) throws IOException {
		String[] algorithms = new String[] {
				"NSGAII",
				"NSGAIII",
				"eMOEA",
				"GDE3",
				"IBEA",
				"MOEAD",
				"OMOPSO",
				"PAES",
				"PESA2",
				"SMPSO",
				"SPEA2"
		};
		
		String problem = "CarSideImpact";
		double[] epsilon = new double[] { 0.95, 0.02, 0.0875 };
		
		EpsilonBoxDominanceArchive referenceSet =
				new EpsilonBoxDominanceArchive(epsilon);
		
		for (String algorithm : algorithms) {
			System.out.println("Processing " + algorithm + "...");
			
			for (int i = 0; i < 10; i++) {
				System.out.println("  Running seed " + (i+1));
				
				referenceSet.addAll(new Executor()
						.withProblem(problem)
						.withAlgorithm(algorithm)
						.withMaxEvaluations(500000)
						.withEpsilon(epsilon)
						.run());
				
				PopulationIO.writeObjectives(
						new File(problem + ".reference"),
						referenceSet);
			}
		}
	}

}
