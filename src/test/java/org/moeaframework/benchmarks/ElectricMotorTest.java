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

import org.junit.Test;

public class ElectricMotorTest extends AbstractProblemTest {

	@Test
	public void testSolve() {
		testSolve("ElectricMotor");
	}
	
	@Test
	public void testReferenceSet() {
		testReferenceSet("ElectricMotor");
	}

	@Test
	public void testLowerBound() {
		testSolution("ElectricMotor",
				new double[] { 100.0, 0.01, 1.0, 0.01, 0.01, 5.0E-4, 0.001, 0.1, 100.0, 0.01, 1.0, 0.01, 0.01, 5.0E-4, 0.001, 0.1, 100.0, 0.01, 1.0, 0.01, 0.01, 5.0E-4, 0.001, 0.1, 100.0, 0.01, 1.0, 0.01, 0.01, 5.0E-4, 0.001, 0.1, 100.0, 0.01, 1.0, 0.01, 0.01, 5.0E-4, 0.001, 0.1, 100.0, 0.01, 1.0, 0.01, 0.01, 5.0E-4, 0.001, 0.1, 100.0, 0.01, 1.0, 0.01, 0.01, 5.0E-4, 0.001, 0.1, 100.0, 0.01, 1.0, 0.01, 0.01, 5.0E-4, 0.001, 0.1, 100.0, 0.01, 1.0, 0.01, 0.01, 5.0E-4, 0.001, 0.1, 100.0, 0.01, 1.0, 0.01, 0.01, 5.0E-4, 0.001, 0.1 },
				new double[] { -2.1178860361093237E-5, 0.9829575175304347, -2.1178860361093237E-5, 0.9829575175304347, -2.1178860361093237E-5, 0.9829575175304347, -2.1178860361093237E-5, 0.9829575175304347, -2.1178860361093237E-5, 0.9829575175304347, -2.1178860361093237E-5, 0.9829575175304347, -2.1178860361093237E-5, 0.9829575175304347, -2.1178860361093237E-5, 0.9829575175304347, -2.1178860361093237E-5, 0.9829575175304347, -2.1178860361093237E-5, 0.9829575175304347 },
				new double[] { 0.05000000000380373, 288.6959885484, 200.00000000000003, 194.5734766747823, -2.1178860361093237E-5, 0.9829575175304347, 0.10000000000380374, 288.6959885484, 200.00000000000003, 194.5734766747823, -2.1178860361093237E-5, 0.9829575175304347, 0.12500000000380374, 288.6959885484, 200.00000000000003, 194.5734766747823, -2.1178860361093237E-5, 0.9829575175304347, 0.15000000000380373, 288.6959885484, 200.00000000000003, 194.5734766747823, -2.1178860361093237E-5, 0.9829575175304347, 0.20000000000380375, 288.6959885484, 200.00000000000003, 194.5734766747823, -2.1178860361093237E-5, 0.9829575175304347, 0.25000000000380374, 288.6959885484, 200.00000000000003, 194.5734766747823, -2.1178860361093237E-5, 0.9829575175304347, 0.3000000000038037, 288.6959885484, 200.00000000000003, 194.5734766747823, -2.1178860361093237E-5, 0.9829575175304347, 0.3500000000038037, 288.6959885484, 200.00000000000003, 194.5734766747823, -2.1178860361093237E-5, 0.9829575175304347, 0.40000000000380376, 288.6959885484, 200.00000000000003, 194.5734766747823, -2.1178860361093237E-5, 0.9829575175304347, 0.5000000000038037, 288.6959885484, 200.00000000000003, 194.5734766747823, -2.1178860361093237E-5, 0.9829575175304347 },
				false);
	}
	
	@Test
	public void testUpperBound() {
		testSolution("ElectricMotor",
				new double[] { 1500.0, 1.0, 500.0, 1.0, 0.1, 0.1, 0.1, 6.0, 1500.0, 1.0, 500.0, 1.0, 0.1, 0.1, 0.1, 6.0, 1500.0, 1.0, 500.0, 1.0, 0.1, 0.1, 0.1, 6.0, 1500.0, 1.0, 500.0, 1.0, 0.1, 0.1, 0.1, 6.0, 1500.0, 1.0, 500.0, 1.0, 0.1, 0.1, 0.1, 6.0, 1500.0, 1.0, 500.0, 1.0, 0.1, 0.1, 0.1, 6.0, 1500.0, 1.0, 500.0, 1.0, 0.1, 0.1, 0.1, 6.0, 1500.0, 1.0, 500.0, 1.0, 0.1, 0.1, 0.1, 6.0, 1500.0, 1.0, 500.0, 1.0, 0.1, 0.1, 0.1, 6.0, 1500.0, 1.0, 500.0, 1.0, 0.1, 0.1, 0.1, 6.0 },
				new double[] { 0.08781367214553605, 0.9739676521739131, 0.08781367214553605, 0.9739676521739131, 0.08781367214553605, 0.9739676521739131, 0.08781367214553605, 0.9739676521739131, 0.08781367214553605, 0.9739676521739131, 0.08781367214553605, 0.9739676521739131, 0.08781367214553605, 0.9739676521739131, 0.08781367214553605, 0.9739676521739131, 0.08781367214553605, 0.9739676521739131, 0.08781367214553605, 0.9739676521739131 },
				new double[] { 0.04692963432538349, 372.03768, 10.0, 588388.4728341918, 0.08781367214553605, 0.9739676521739131, 0.0969296343253835, 372.03768, 10.0, 588388.4728341918, 0.08781367214553605, 0.9739676521739131, 0.12192963432538349, 372.03768, 10.0, 588388.4728341918, 0.08781367214553605, 0.9739676521739131, 0.14692963432538347, 372.03768, 10.0, 588388.4728341918, 0.08781367214553605, 0.9739676521739131, 0.1969296343253835, 372.03768, 10.0, 588388.4728341918, 0.08781367214553605, 0.9739676521739131, 0.24692963432538348, 372.03768, 10.0, 588388.4728341918, 0.08781367214553605, 0.9739676521739131, 0.2969296343253835, 372.03768, 10.0, 588388.4728341918, 0.08781367214553605, 0.9739676521739131, 0.3469296343253835, 372.03768, 10.0, 588388.4728341918, 0.08781367214553605, 0.9739676521739131, 0.39692963432538353, 372.03768, 10.0, 588388.4728341918, 0.08781367214553605, 0.9739676521739131, 0.4969296343253835, 372.03768, 10.0, 588388.4728341918, 0.08781367214553605, 0.9739676521739131 },
				false);
	}
	
}
