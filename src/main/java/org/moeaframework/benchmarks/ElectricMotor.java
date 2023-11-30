<<<<<<< Updated upstream:src/main/java/org/moeaframework/benchmarks/ElectricMotor/ElectricMotor.java
/* Copyright 2009-2023 David Hadka and other contributors
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
package org.moeaframework.benchmarks.ElectricMotor;
=======
package org.moeaframework.benchmarks;
>>>>>>> Stashed changes:src/main/java/org/moeaframework/benchmarks/ElectricMotor.java

import org.moeaframework.core.Solution;
import org.moeaframework.core.variable.EncodingUtils;
import org.moeaframework.core.variable.RealVariable;
import org.moeaframework.problem.AbstractProblem;

/**
 * The electric motor product platform design problem.
 * <p>
 * References:
 * <ol>
 *   <li>Simpson, T.W., J.R.A. Maier, and F. Mistree (2001).  "Product platform
 *       design: method and application."  Res Eng Design, 13:2-22.
 * </ol>
 */
public class ElectricMotor extends AbstractProblem {
	
	public static final double[] EPSILON = new double[] { 0.001 };

	public static double[] REQUIRED_TORQUE = new double[] {
		0.05, 0.1, 0.125, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.5
	};

	public static double REQUIRED_POWER = 300.0;

	public static double MAXIMUM_MASS = 2.0;

	public static double MINIMUM_EFFICIENCY = 0.15;

	public static double MAXIMUM_INTENSITY = 5000.0;

	public static double TORQUE_EPSILON = 0.001;

	public static double POWER_EPSILON = 0.1;
	
	public ElectricMotor() {
		super(80, 20, 60);
	}

	@Override
	public void evaluate(Solution solution) {
		for (int i = 0; i < 10; i++) {
			double[] input = new double[8];
			input[0] = EncodingUtils.getReal(solution.getVariable(8*i + 0));
			input[1] = EncodingUtils.getReal(solution.getVariable(8*i + 1));
			input[2] = EncodingUtils.getReal(solution.getVariable(8*i + 2));
			input[3] = EncodingUtils.getReal(solution.getVariable(8*i + 3));
			input[4] = EncodingUtils.getReal(solution.getVariable(8*i + 4));
			input[5] = EncodingUtils.getReal(solution.getVariable(8*i + 5));
			input[6] = EncodingUtils.getReal(solution.getVariable(8*i + 6));
			input[7] = EncodingUtils.getReal(solution.getVariable(8*i + 7));

			// convert to integers
			input[0] = Math.round(input[0]);
			input[2] = Math.round(input[2]);

			double[] output = evaluateMotor(input);

			solution.setObjective(2*i + 0, output[1]);
			solution.setObjective(2*i + 1, -output[2]);

			solution.setConstraint(6*i + 0,
					Math.abs(output[0] - REQUIRED_TORQUE[i]) <= TORQUE_EPSILON ?
							0.0 :
							Math.abs(output[0] - REQUIRED_TORQUE[i]));
			solution.setConstraint(6*i + 1,
					Math.abs(output[4] - REQUIRED_POWER) <= POWER_EPSILON ?
							0.0 :
							Math.abs(output[4] - REQUIRED_POWER));
			solution.setConstraint(6*i + 2,
					output[5] >= 1.0 ?
							0.0 :
							1.0 - output[5]);
			solution.setConstraint(6*i + 3,
					output[3] / 2.0 <= MAXIMUM_INTENSITY ?
							0.0 :
							output[3] / 2.0 - MAXIMUM_INTENSITY);
			solution.setConstraint(6*i + 4,
					output[1] <= MAXIMUM_MASS ?
							0.0 :
							output[1] - MAXIMUM_MASS);
			solution.setConstraint(6*i + 5,
					output[2] > MINIMUM_EFFICIENCY ?
							0.0 :
							output[2] - MINIMUM_EFFICIENCY);
		}
	}

	public static double[] evaluateMotor(double[] input) {
		double NARM = input[0]; // number of wire turns on the armature
		double AWA = input[1]; // cross-sectional area of armature wire
		double NFIELD = input[2]; // number of wire turns on each field pole
		double AWF = input[3]; // cross-sectional area of field wire
		double RADIUS = input[4]; // radius of the motor
		double THICK = input[5]; // thickness of the stator
		double LENGTH = input[6]; // stack length of the motor
		double CURRNT = input[7]; // current drawn by the motor

		double LGAP = 0.0007;
		double VOLTAG = 115.0;
		double RESIST = 1.69E-8;
		double DCOPPR = 8960.0;
		double DSTEEL = 7850.0;
		double SATLEV = 220.0;
		double SATLV2 = 1000.0;
		double MUO = 4 * 3.14159E-7;

		THICK = THICK / 1000.0;
		RADIUS = RADIUS / 100.0;
		LENGTH = LENGTH / 100.0;
		AWF = AWF / 1000000.0;
		AWA = AWA / 1000000.0;

		double RDIAM = 2.0 * (RADIUS - THICK - LGAP);
		double FEAS = RADIUS / THICK;

		// power
		double RA = RESIST * NARM * (2.0 * LENGTH + 2.0 * RDIAM) / AWA;
		double RS = RESIST * 2.0 * NFIELD * (2.0 * LENGTH + 4.0 * (RADIUS - THICK)) / AWF;
		double LOSS = Math.pow(CURRNT, 2.0) * (RA + RS) + 2.0 * CURRNT;
		double POWER = VOLTAG * CURRNT - LOSS;
		double EFFIC = POWER / (VOLTAG * CURRNT);

		// torque
		double KT = NARM / Math.PI;
		double LC = Math.PI * (2.0 * RADIUS + THICK) / 2.0;
		double SAT = 2.0 * NFIELD * CURRNT / (LC + RDIAM + 2.0 * LGAP);

		double MUR;
		if (SAT <= SATLEV) {
			MUR = -0.22791 * Math.pow(SAT, 2.0) + 52.411 * SAT + 3115.8;
		} else if (SAT >= SATLV2) {
			MUR = 1000.0;
		} else {
			MUR = 11633.5 - 1486.33 * Math.log(SAT);
		}

		double AS = THICK * LENGTH;
		double AR = RDIAM * LENGTH;
		double AA = RDIAM * LENGTH;

		double RRS = LC / (2.0 * MUR * MUO * AS);
		double RRR = RDIAM / (MUR * MUO * AR);
		double RRA = LGAP / (MUO * AA);
		double FFF = NFIELD * CURRNT;

		double RR = RRS + RRR + 2.0 * RRA;
		double PHI = FFF / RR;
		double TORQUE = KT * PHI * CURRNT;
		double SPEED = POWER / TORQUE;

		// mass
		double MSTATOR = Math.PI * LENGTH * DSTEEL * (Math.pow(RADIUS, 2.0) - Math.pow(RADIUS - THICK, 2.0));

		double MROTOR = Math.PI * LENGTH * DSTEEL * Math.pow(RDIAM / 2.0, 2.0);

		double MWIND = ((2.0 * LENGTH + 2.0 * RDIAM) * AWA * NARM +
				(2.0 * LENGTH + 4.0 * (RADIUS - THICK)) * AWF * 2.0 * NFIELD) * DCOPPR;

		double MASS = MSTATOR + MROTOR + MWIND;

		return new double[] { TORQUE, MASS, EFFIC, SAT, POWER, FEAS, SPEED };
	}

	@Override
	public Solution newSolution() {
		Solution solution = new Solution(80, 20, 60);

		for (int i = 0; i < 10; i++) {
			// number of wire turns on the armature
			solution.setVariable(8 * i + 0, new RealVariable(100, 1500));

			// cross-sectional area of armature wire
			solution.setVariable(8 * i + 1, new RealVariable(0.01, 1.0));

			// number of wire turns on each field pole
			solution.setVariable(8 * i + 2, new RealVariable(1, 500));

			// cross-sectional area of field wire
			solution.setVariable(8 * i + 3, new RealVariable(0.01, 1.0));

			// radius of the motor
			solution.setVariable(8 * i + 4, new RealVariable(0.01, 0.1));

			// thickness of the stator
			solution.setVariable(8 * i + 5, new RealVariable(0.0005, 0.1));

			// stack length of the motor
			solution.setVariable(8 * i + 6, new RealVariable(0.001, 0.1));

			// current drawn by the motor
			solution.setVariable(8 * i + 7, new RealVariable(0.1, 6.0));
		}

		return solution;
	}

}
