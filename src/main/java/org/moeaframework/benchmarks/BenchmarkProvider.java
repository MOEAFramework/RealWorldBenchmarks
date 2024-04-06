/* Copyright 2009-2024 David Hadka and other contributors
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

import org.moeaframework.benchmarks.WDS.WDSInstance;
import org.moeaframework.core.FrameworkException;
import org.moeaframework.core.spi.RegisteredProblemProvider;

public class BenchmarkProvider extends RegisteredProblemProvider {
	
	public BenchmarkProvider() {
		super();
		
		register("GAA", GAA::new, "./pf/GAA.reference");
		register("CarSideImpact", CarSideImpact::new, "./pf/CarSideImpact.reference");
		register("ElectricMotor", ElectricMotor::new, "./pf/ElectricMotor.reference");
		register("HBV", HBV::new, "./pf/HBV.reference");
		register("LRGV", LRGV::new, null);
		register("LakeProblem", LakeProblem::new, "./pf/LakeProblem.reference");
		
		for (WDSInstance variant : WDSInstance.values()) {
			register("WDS(" + variant.getName() + ")", () -> new WDS(variant), "./pf/WDS/" + variant.getName() + ".reference");
		}
		
		register("Radar", () -> {
			try {
				return new Radar();
			} catch (Exception e) {
				throw new FrameworkException("failed to start Radar problem", e);
			}
		}, null);
	}
	
}
