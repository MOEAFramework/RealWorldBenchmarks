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

public class LRGVTest extends AbstractProblemTest {

	@Test
	public void testSolve() {
		testSolve("LRGV");
	}

	@Test
	public void testLowerBound() {
		testSolution("LRGV",
				new double[] { 0.0, 0.0, 0.1, 0.1, 0.0, 0.0, 0.0, 0.0 },
				new double[] { 0.0678, 0.5427500000000001, 0.052332950946083, 0.0, 0.0, 8.237323224016374E-67 },
				new double[] { -0.6038095238095238, -0.4517676767676767, 0.0, 0.0 },
				false);
	}
	
	@Test
	public void testUpperBound() {
		testSolution("LRGV",
				new double[] { 1.0, 1.0, 1.0, 0.4, 3.0, 0.0, 3.0, 0.0 },
				new double[] { 0.14687273772049875, 1.0, 0.7396398491493245, 0.0012601974979137415, 7.8E-4, 8.237323224016374E-67 },
				new double[] { 0.0, 0.0, -0.0059376725451394385, 0.0 },
				false);
	}
	
}
