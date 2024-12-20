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

public class WDSTest extends AbstractProblemTest {

	@Test
	public void testSolve() {
		testSolve("WDS(GOY)");
	}
	
	@Test
	public void testReferenceSet() {
		testReferenceSet("WDS(GOY)");
	}

	@Test
	public void testLowerBound() {
		testSolution("WDS(GOY)",
				new double[] { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 },
				new double[] { 0.17467288672924042, -6.119567394256592 },
				new double[] { -2390.6025390625 },
				false);
	}
	
	@Test
	public void testUpperBound() {
		testSolution("WDS(GOY)",
				new double[] { 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999, 7.999999999999999 },
				new double[] { 0.3297256529331207, 0.7125067114830017 },
				new double[] { 0.0 },
				true);
	}
	
}
