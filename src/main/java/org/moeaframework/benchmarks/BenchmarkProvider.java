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
		
		register("GAA", () -> new GAA(), locateReferenceSet(GAA.class, "GAA.reference"));
		register("CarSideImpact", () -> new CarSideImpact(), locateReferenceSet(CarSideImpact.class, "CarSideImpact.reference"));
		register("ElectricMotor", () -> new ElectricMotor(), locateReferenceSet(ElectricMotor.class, "ElectricMotor.reference"));
		register("HBV", () -> new HBV(), locateReferenceSet(HBV.class, "HBV.reference"));
		register("LRGV", () -> new LRGV(), null);
		register("LakeProblem", () -> new LakeProblem(), locateReferenceSet(LakeProblem.class, "LakeProblem.reference"));
		
		for (WDSInstance variant : WDSInstance.values()) {
			register("WDS(" + variant.getName() + ")", () -> new WDS(variant), locateReferenceSet(WDS.class, variant.getName() + ".reference"));
		}
	}
	
	private static String locateReferenceSet(Class<?> problem, String filename) {
		return problem.getPackageName().replace(".", "/") + "/" + filename;
	}

}
