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

import org.moeaframework.core.Solution;
import org.moeaframework.core.variable.EncodingUtils;
import org.moeaframework.core.variable.RealVariable;
import org.moeaframework.problem.AbstractProblem;

/**
 * The car side impact problem.  Aims to minimize the weight of the car while minimizing the pubic force experienced by
 * a passenger and the average velocity of the V-Pillar responsible for withstanding the impact load.
 * <p>
 * References:
 * <ol>
 *   <li>Jain, H. and K. Deb.  "An Evolutionary Many-Objective Optimization Algorithm Using Reference-Point-Based
 *       Nondominated Sorting Approach, Part II: Handling Constraints and Extending to an Adaptive Approach."
 *       IEEE Transactions on Evolutionary Computation, 18(4):602-622, 2014.
 * </ol>
 */
public class CarSideImpact extends AbstractProblem {

	/**
	 * Constructs the car side impact problem instance.
	 */
	public CarSideImpact() {
		super(7, 3, 10);
	}

	@Override
	public void evaluate(Solution solution) {
		double x1 = EncodingUtils.getReal(solution.getVariable(0));
		double x2 = EncodingUtils.getReal(solution.getVariable(1));
		double x3 = EncodingUtils.getReal(solution.getVariable(2));
		double x4 = EncodingUtils.getReal(solution.getVariable(3));
		double x5 = EncodingUtils.getReal(solution.getVariable(4));
		double x6 = EncodingUtils.getReal(solution.getVariable(5));
		double x7 = EncodingUtils.getReal(solution.getVariable(6));

		double F = 4.72 - 0.5*x4 - 0.19*x2*x3;
		double Vmbp = 10.58 - 0.674*x1*x2 - 0.67275*x2;
		double Vfd = 16.45 - 0.489*x3*x7 - 0.843*x5*x6;
		double f1 = 1.98 + 4.9*x1 + 6.67*x2 + 6.98*x3 + 4.01*x4 + 1.78*x5 + 0.00001*x6 + 2.73*x7;
		double f2 = F;
		double f3 = 0.5*(Vmbp + Vfd);
		double g1 = 1.16 - 0.3717*x2*x4 - 0.0092928*x3;
		double g2 = 0.261 - 0.0159*x1*x2 - 0.06486*x1 - 0.019*x2*x7 + 0.0144*x3*x5 + 0.0154464*x6;
		double g3 = 0.214 + 0.00817*x5 - 0.045195*x1 - 0.0135168*x1 + 0.03099*x2*x6 - 0.018*x2*x7 + 0.007176*x3 +
				0.023232*x3 - 0.00364*x5*x6 - 0.018*x2*x2;
		double g4 = 0.74 - 0.61*x2 - 0.031296*x3 - 0.031872*x7 + 0.227*x2*x2;
		double g5 = 28.98 + 3.818*x3 - 4.2*x1*x2 + 1.27296*x6 - 2.68065*x7;
		double g6 = 33.86 + 2.95*x3 - 5.057*x1*x2 - 3.795*x2 - 3.4431*x7 + 1.45728;
		double g7 = 46.36 - 9.9*x2 - 4.4505*x1;
		double g8 = F;
		double g9 = Vmbp;
		double g10 = Vfd;
		
		solution.setObjective(0, f1);
		solution.setObjective(1, f2);
		solution.setObjective(2, f3);
		solution.setConstraint(0, g1 <= 1.0 ? 0.0 : g1-1.0);
		solution.setConstraint(1, g2 <= 0.32 ? 0.0 : g2-0.32);
		solution.setConstraint(2, g3 <= 0.32 ? 0.0 : g3-0.32);
		solution.setConstraint(3, g4 <= 0.32 ? 0.0 : g4-0.32);
		solution.setConstraint(4, g5 <= 32.0 ? 0.0 : g5-32.0);
		solution.setConstraint(5, g6 <= 32.0 ? 0.0 : g6-32.0);
		solution.setConstraint(6, g7 <= 32.0 ? 0.0 : g7-32.0);
		solution.setConstraint(7, g8 <= 4.0 ? 0.0 : g8-4.0);
		solution.setConstraint(8, g9 <= 9.9 ? 0.0 : g9-9.9);
		solution.setConstraint(9, g10 <= 15.7 ? 0.0 : g10-15.7);
	}

	@Override
	public Solution newSolution() {
		Solution solution = new Solution(7, 3, 10);
		solution.setVariable(0, new RealVariable(0.5, 1.5));
		solution.setVariable(1, new RealVariable(0.45, 1.35));
		solution.setVariable(2, new RealVariable(0.5, 1.5));
		solution.setVariable(3, new RealVariable(0.5, 1.5));
		solution.setVariable(4, new RealVariable(0.875, 2.625));
		solution.setVariable(5, new RealVariable(0.4, 1.2));
		solution.setVariable(6, new RealVariable(0.4, 1.2));
		return solution;
	}

}
