package org.moeaframework.benchmarks;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import org.moeaframework.benchmarks.GAA.GAA;
import org.moeaframework.benchmarks.HBV.HBV;
import org.moeaframework.benchmarks.WDS.WDS;
import org.moeaframework.benchmarks.WDS.WDSInstance;
import org.moeaframework.core.NondominatedPopulation;
import org.moeaframework.core.PopulationIO;
import org.moeaframework.core.Problem;
import org.moeaframework.core.spi.ProblemProvider;
import org.moeaframework.core.spi.ProviderNotFoundException;

public class BenchmarkProvider extends ProblemProvider {

	@Override
	public Problem getProblem(String problemName) {
		if (problemName.equalsIgnoreCase("GAA")) {
			return new GAA();
		} else if (problemName.equalsIgnoreCase("HBV")) {
			try {
				return new HBV();
			} catch (IOException e) {
				throw new ProviderNotFoundException(
						"unable to run HBV executable", e);
			}
		} else if (problemName.startsWith("WDS(") &&
				problemName.endsWith(")")) {
			String variant = problemName.substring(4, problemName.length()-1);
			
			try {
				return new WDS(WDSInstance.valueOf(variant.toUpperCase()));
			} catch (IllegalArgumentException e) {
				throw new ProviderNotFoundException(
						"no WDS instance found with name " + variant, e);
			} catch (IOException e) {
				throw new ProviderNotFoundException(
						"unable to run WDS executable", e);
			}
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
		} else if (problemName.equalsIgnoreCase("HBV")) {
			InputStream stream = HBV.class.getResourceAsStream("HBV.reference");
			
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
		} else if (problemName.startsWith("WDS(") &&
				problemName.endsWith(")")) {
			String variant = problemName.substring(4, problemName.length()-1);
			
			try {
				WDSInstance instance = WDSInstance.valueOf(
						variant.toUpperCase());
				InputStream stream = WDS.class.getResourceAsStream(
						instance.getName() + ".reference");
				
				try {
					return new NondominatedPopulation(
							PopulationIO.readObjectives(new BufferedReader(
									new InputStreamReader(stream))));
				} catch (IOException e) {
					return null;
				}
			} catch (IllegalArgumentException e) {
				return null;
			}
		} else {
			return null;
		}
	}

}
