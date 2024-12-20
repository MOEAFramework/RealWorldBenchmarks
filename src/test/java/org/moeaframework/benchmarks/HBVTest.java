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

public class HBVTest extends AbstractProblemTest {

	@Test
	public void testSolve() {
		testSolve("HBV");
	}
	
	@Test
	public void testReferenceSet() {
		testReferenceSet("HBV");
	}
	
	@Test
	public void testLowerBound() {
		testSolution("HBV",
				new double[] { 0.0, 0.5, 1.0, 10.0, 0.0, 0.3, 0.0, 0.0, 24.0, -3.0, 0.0, 0.0, 0.0, 0.0 },
				new double[] { 9.910673, 3.527903, 1.225312, 1.327312 },
				new double[] { },
				true);
	}
	
	@Test
	public void testUpperBound() {
		testSolution("HBV",
				new double[] { 100.0, 20.0, 100.0, 20000.0, 100.0, 1.0, 2000.0, 7.0, 120.0, 3.0, 20.0, 1.0, 0.8, 7.0 },
				new double[] { 0.9951112, 3.319143, 0.6665916, 0.9936301 },
				new double[] { },
				true);
	}
	
}
