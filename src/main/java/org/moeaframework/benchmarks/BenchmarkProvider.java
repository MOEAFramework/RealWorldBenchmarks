/* Copyright 2009-2023 David Hadka and other contributors
 *
 * This file is part of the MOEA Framework.
 *
 * The MOEA Framework is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * The MOEA Framework is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with the MOEA Framework.  If not, see <http://www.gnu.org/licenses/>.
 */
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
