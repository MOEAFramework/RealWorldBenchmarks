package org.moeaframework.benchmarks;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import org.moeaframework.benchmarks.GAA.GAA;
import org.moeaframework.core.NondominatedPopulation;
import org.moeaframework.core.PopulationIO;
import org.moeaframework.core.Problem;
import org.moeaframework.core.spi.ProblemProvider;

public class BenchmarkProvider extends ProblemProvider {

	@Override
	public Problem getProblem(String problemName) {
		if (problemName.equalsIgnoreCase("GAA")) {
			return new GAA();
		} else {
			return null;
		}
	}

	@Override
	public NondominatedPopulation getReferenceSet(String problemName) {
		if (problemName.equalsIgnoreCase("GAA")) {
			InputStream stream = GAA.class.getResourceAsStream("GAA.reference");
				
			if (stream == null) {
				return null;
			} else {
				try {
					return new NondominatedPopulation(
							PopulationIO.readObjectives(new BufferedReader(
									new InputStreamReader(stream))));
				} catch (IOException e) {
					return null;
				}
			}
		} else {
			return null;
		}
	}

}
