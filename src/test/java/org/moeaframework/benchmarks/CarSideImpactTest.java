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

public class CarSideImpactTest extends AbstractProblemTest {

	@Test
	public void testSolve() {
		testSolve("CarSideImpact");
	}
	
	@Test
	public void testReferenceSet() {
		testReferenceSet("CarSideImpact");
	}

	@Test
	public void testLowerBound() {
		testSolution("CarSideImpact",
				new double[] { 0.5, 0.45, 0.5, 0.5, 0.875, 0.4, 0.4 },
				new double[] { 15.576004000000003, 4.42725, 13.091381250000001 },
				new double[] { 1.07172109999999998, 0.23405105999999998, 0.20441605000000002, 0.48307069999999996, 29.380924, 32.569465, 39.67975, 4.42725, 10.1256125, 16.05715 },
				false);
	}
	
	@Test
	public void testUpperBound() {
		testSolution("CarSideImpact",
				new double[] { 1.5, 1.35, 1.5, 1.5, 2.625, 1.2, 1.2 },
				new double[] { 42.768012, 3.58525, 10.61064375 },
				new double[] { 0.39336829999999995, 0.17596818000000003, 0.16976335, 0.24501710000000013, 24.512772, 20.246884999999995, 26.319249999999997, 3.58525, 8.3069375, 12.914349999999999 },
				true);
	}
	
}
