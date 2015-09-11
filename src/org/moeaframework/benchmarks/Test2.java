package org.moeaframework.benchmarks;

import java.io.IOException;

import org.moeaframework.Analyzer;
import org.moeaframework.Executor;
import org.moeaframework.benchmarks.WDS.WDSInstance;

public class Test2 {
	
	public static void main(String[] args) throws IOException {
		Analyzer analyzer = new Analyzer()
				.withProblem("WDS(NYT)")
				.withEpsilon(WDSInstance.NYT.getEpsilon())
				.includeAllMetrics();
		
		Executor executor = new Executor()
				.withSameProblemAs(analyzer)
				.withAlgorithm("NSGAII");
		
		analyzer.add("NSGAII", executor.run());
		
		analyzer.printAnalysis();
	}

}
