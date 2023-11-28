package org.moeaframework.benchmarks;

import org.moeaframework.benchmarks.CarSideImpact.CarSideImpact;
import org.moeaframework.benchmarks.ElectricMotor.ElectricMotor;
import org.moeaframework.benchmarks.GAA.GAA;
import org.moeaframework.benchmarks.HBV.HBV;
import org.moeaframework.benchmarks.LRGV.LRGV;
import org.moeaframework.benchmarks.LakeProblem.LakeProblem;
import org.moeaframework.benchmarks.WDS.WDS;
import org.moeaframework.benchmarks.WDS.WDSInstance;
import org.moeaframework.core.spi.RegisteredProblemProvider;

public class BenchmarkProvider extends RegisteredProblemProvider {
	
	public BenchmarkProvider() {
		super();
		
		register("GAA", () -> new GAA(), "./pf/GAA.reference");
		register("CarSideImpact", () -> new CarSideImpact(), "./pf/CarSideImpact.reference");
		register("ElectricMotor", () -> new ElectricMotor(), "./pf/ElectricMotor.reference");
		register("HBV", () -> new HBV(), "./pf/HBV.reference");
		register("LRGV", () -> new LRGV(), null);
		register("LakeProblem", () -> new LakeProblem(), "./pf/LakeProblem.reference");
		
		for (WDSInstance variant : WDSInstance.values()) {
			register("WDS(" + variant.getName() + ")", () -> new WDS(variant), "./pf/WDS/" + variant.getName() + ".reference");
		}
	}
	
}
