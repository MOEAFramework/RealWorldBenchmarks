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

import org.junit.Ignore;
import org.junit.Test;

@Ignore
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
				new int[] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				new double[] { 0.17467288672924042, -6.119567394256592 },
				new double[] { -2390.6025390625 },
				false);
	}
	
	@Test
	public void testUpperBound() {
		testSolution("WDS(GOY)",
				new int[] { 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 },
				new double[] { 0.3297256529331207, 0.7125067114830017 },
				new double[] { 0.0 },
				true);
	}
	
// Windows:
//	Starting process '.\native\WDS\bin\GOY.exe'
//	<< 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
//	>> 0.17467288672924042 1.045642614364624 -1909.0999755859375
//	Actual Objectives: [0.17467288672924042, 1.045642614364624]
//	Waiting for process to exit...
//	Process exited with code 0
//	Starting process '.\native\WDS\bin\GOY.exe'
//	<< 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7
//	>> 0.32972565293312073 1.045642614364624 -1909.0999755859375
//	Actual Objectives: [0.3297256529331207, 1.045642614364624]

// Linux:
//	Starting process './GOY'
//	<< 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
//	>> 0.17467288672924042 -6.1195673942565918 -2390.6025390625
//	Waiting for process to exit...
//	Process exited with code 0
//	Starting process './GOY'
//	<< 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7 7
//	>> 0.32972565293312073 0.71250671148300171 -0
	
}
