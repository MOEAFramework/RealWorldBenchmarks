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

public class LRGVTest extends AbstractProblemTest {

	@Test
	@Ignore
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
	
// Windows:
//	Starting process '.\native\LRGV\bin\lrgv.exe -m std-io -b AllDecAll -c ten-year'
//	<< 0.0 0.0 0.1 0.1 0.0 0.0 0.0 0.0
//	>> 0.067799999999999999 0.54275000000000007 0.052332950946083 0 0 0 -0.6038095238095238 -0.45176767676767671 0 1372.9499999999534
//	Actual Constraints: [-0.6038095238095238, -0.4517676767676767, 0.0, 1372.9499999999534]
//	Waiting for process to exit...
//	Process exited with code 0
//	Starting process '.\native\LRGV\bin\lrgv.exe -m std-io -b AllDecAll -c ten-year'
//	<< 1.0 1.0 1.0 0.4 3.0 0.0 3.0 0.0
//	>> 0.14688755308684337 1 0.73901009177481636 0.0013347587141167424 0.00081400000000000005 1.11011894436792e-311 0 0 -0.0080529641534905494 15
//	Actual Constraints: [0.0, 0.0, -0.00805296415349055, 15.0]

// Linux:
//	Starting process './lrgv -m std-io -b AllDecAll -c ten-year'
//	<< 0.0 0.0 0.1 0.1 0.0 0.0 0.0 0.0
//	>> 0.067799999999999999 0.54275000000000007 0.052332950946083 0 0 8.2373232240163736e-67 -0.6038095238095238 -0.45176767676767671 0 0
//	Waiting for process to exit...
//	Process exited with code 0
//	Starting process './lrgv -m std-io -b AllDecAll -c ten-year'
//	<< 1.0 1.0 1.0 0.4 3.0 0.0 3.0 0.0
//	>> 0.14688755308684337 1 0.73901009177481636 0.0013347587141167424 0.00081400000000000005 8.2373232240163736e-67 0 0 -0.0080529641534905494 0
	
}
