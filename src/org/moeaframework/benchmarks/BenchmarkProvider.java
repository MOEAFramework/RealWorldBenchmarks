package org.moeaframework.benchmarks;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import org.moeaframework.benchmarks.CarSideImpact.CarSideImpact;
import org.moeaframework.benchmarks.GAA.GAA;
import org.moeaframework.benchmarks.HBV.HBV;
import org.moeaframework.benchmarks.WDS.WDS;
import org.moeaframework.benchmarks.WDS.WDSInstance;
import org.moeaframework.core.FrameworkException;
import org.moeaframework.core.NondominatedPopulation;
import org.moeaframework.core.PopulationIO;
import org.moeaframework.core.Problem;
import org.moeaframework.core.spi.ProblemProvider;

public class BenchmarkProvider extends ProblemProvider {

	@Override
	public Problem getProblem(String problemName) {
		if (problemName.equalsIgnoreCase("GAA")) {
			return new GAA();
		} else if (problemName.equalsIgnoreCase("CarSideImpact")) {
			return new CarSideImpact();
		} else if (problemName.equalsIgnoreCase("HBV")) {
			try {
				return new HBV();
			} catch (IOException e) {
				throw new FrameworkException(
						"unable to run HBV executable", e);
			}
		} else if (problemName.startsWith("WDS(") &&
				problemName.endsWith(")")) {
			String variant = problemName.substring(4, problemName.length()-1);
			
			try {
				return new WDS(WDSInstance.valueOf(variant.toUpperCase()));
			} catch (IllegalArgumentException e) {
				throw new FrameworkException(
						"no WDS instance found with name " + variant, e);
			} catch (IOException e) {
				throw new FrameworkException(
						"unable to run WDS executable", e);
			}
		} else {
			return null;
		}
	}

	@Override
	public NondominatedPopulation getReferenceSet(String problemName) {
		InputStream stream = null;
		
		if (problemName.equalsIgnoreCase("GAA")) {
			stream = GAA.class.getResourceAsStream("GAA.reference");
		} else if (problemName.equalsIgnoreCase("CarSideImpact")) {
			stream = CarSideImpact.class.getResourceAsStream("CarSideImpact.reference");
		} else if (problemName.equalsIgnoreCase("HBV")) {
			stream = HBV.class.getResourceAsStream("HBV.reference");
		} else if (problemName.startsWith("WDS(") &&
				problemName.endsWith(")")) {
			String variant = problemName.substring(4, problemName.length()-1);
			
			try {
				WDSInstance instance = WDSInstance.valueOf(
						variant.toUpperCase());
				stream = WDS.class.getResourceAsStream(
						instance.getName() + ".reference");
			} catch (IllegalArgumentException e) {
				stream = null;
			}
		} else {
			return null;
		}
		
		if (stream == null) {
			return null;
		} else {
			try {
				return new NondominatedPopulation(
						PopulationIO.readObjectives(new BufferedReader(
								new InputStreamReader(stream))));
			} catch (IOException e) {
				return null;
			} finally {
				try {
					stream.close();
				} catch (IOException e) {
					// ignore
				}
			}
		}
	}

}
