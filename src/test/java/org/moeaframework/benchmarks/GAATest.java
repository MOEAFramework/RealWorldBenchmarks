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
public class GAATest extends AbstractProblemTest {

	@Test
	public void testSolve() {
		testSolve("GAA");
	}
	
	@Test
	public void testReferenceSet() {
		testReferenceSet("GAA");
	}
	
	@Test
	public void testLowerBound() {
		testSolution("GAA",
				new double[] { 0.24, 7.0, 0.0, 5.5, 19.0, 85.0, 14.0, 3.0, 0.46, 0.24, 7.0, 0.0, 5.5, 19.0, 85.0, 14.0, 3.0, 0.46, 0.24, 7.0, 0.0, 5.5, 19.0, 85.0, 14.0, 3.0, 0.46 },
				new double[] { 73.239998, 1880.3199970000003, 62.38500200000003, 2.1867999999999994, 480.173996, 41699.24730800003, 3032.0586889999995, 15.726500000000003, 189.25630300000014, 1.9229626863835638E-16 },
				new double[] { 0.33805332444444436 },
				false);
	}
	
	@Test
	public void testUpperBound() {
		testSolution("GAA",
				new double[] { 0.48, 11.0, 6.0, 5.968, 25.0, 110.0, 20.0, 3.75, 1.0, 0.48, 11.0, 6.0, 5.968, 25.0, 110.0, 20.0, 3.75, 1.0, 0.48, 11.0, 6.0, 5.968, 25.0, 110.0, 20.0, 3.75, 1.0 },
				new double[] { 75.19549799999994, 2097.8436029999993, 95.00900000000001, 2.078, 291.2477919999998, 47369.88729400002, 891.8127029999995, 17.929600000000004, 198.903706, 0.0 },
				new double[] { 2.3017063231666643 },
				false);
	}
	
}
