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

import java.io.File;

import org.moeaframework.core.Solution;
import org.moeaframework.core.variable.RealVariable;
import org.moeaframework.problem.NativeCommand;
import org.moeaframework.problem.NativeProblem;

public class LRGV extends NativeProblem {
		
	public static final double[] EPSILON = {
		0.0009, 0.002, 0.03, 0.004, 0.004
	};
	
	public static final NativeCommand COMMAND = new NativeCommand("lrgv",
			new String[] { "-m", "std-io", "-b", "AllDecAll", "-c", "ten-year" },
			new File("./native/LRGV/bin/"));
	
	public LRGV() {
	    super(COMMAND);
	}

	@Override
	public String getName() {
		return "LRGV";
	}

	@Override
	public int getNumberOfVariables() {
		return 8;
	}

	@Override
	public int getNumberOfObjectives() {
		return 6;
	}

	@Override
	public int getNumberOfConstraints() {
		return 4;
	}

	@Override
	public Solution newSolution() {
		Solution solution = new Solution(8, 6, 4);
		solution.setVariable(0, new RealVariable(0.0, 1.0));
		solution.setVariable(1, new RealVariable(0.0, 1.0));
		solution.setVariable(2, new RealVariable(0.1, 1.0));
		solution.setVariable(3, new RealVariable(0.1, 0.4));
		solution.setVariable(4, new RealVariable(0.0, 3.0));
		solution.setVariable(5, new RealVariable(0.0, 3.0));
		solution.setVariable(6, new RealVariable(0.0, 3.0));
		solution.setVariable(7, new RealVariable(0.0, 3.0));
		return solution;
	}

	@Override
	public void evaluate(Solution solution) {
		double precision = 100;

		//get variables
		RealVariable rv2 = (RealVariable)solution.getVariable(2); 
		RealVariable rv3 = (RealVariable)solution.getVariable(3); 
		RealVariable rv4 = (RealVariable)solution.getVariable(4); 
		RealVariable rv5 = (RealVariable)solution.getVariable(5); 
		RealVariable rv6 = (RealVariable)solution.getVariable(6); 
		RealVariable rv7 = (RealVariable)solution.getVariable(7); 

		double v2 = rv2.getValue();
		double v3 = rv3.getValue();
		double v4 = rv4.getValue();
		double v5 = rv5.getValue();
		double v6 = rv6.getValue();
		double v7 = rv7.getValue();

		//delta_o_high discretization
		if (v2 < 0.05) v2 = 0;
		else if (v2 < 0.15) v2 = 0.1;
		else if (v2 < 0.25) v2 = 0.2;
		else if (v2 < 0.35) v2 = 0.3;
		else if (v2 < 0.45) v2 = 0.4;
		else if (v2 < 0.55) v2 = 0.5;
		else if (v2 < 0.65) v2 = 0.6;
		else if (v2 < 0.75) v2 = 0.7;
		else if (v2 < 0.85) v2 = 0.8;
		else if (v2 < 0.95) v2 = 0.9;
		else v2 = 1.0;

		//xi discretization
		if (v3 < 0.125) v3 = 0.10;
		else if (v3 < 0.175) v3 = 0.15;
		else if (v3 < 0.225) v3 = 0.20;
		else if (v3 < 0.275) v3 = 0.25;
		else if (v3 < 0.325) v3 = 0.30;
		else if (v3 < 0.375) v3 = 0.35;
		else v3 = 0.40;		

		//alpha/beta discretization
		v4 = Math.floor(v4 * precision +.5)/precision;
		v5 = Math.floor(v5 * precision +.5)/precision;
		v6 = Math.floor(v6 * precision +.5)/precision;
		v7 = Math.floor(v7 * precision +.5)/precision;

		//alpha/beta 3.0 constraint
		if (v4 + v5 > 3.0) v5 = 3.0 - v4; //transform
		if (v6 + v7 > 3.0) v7 = 3.0 - v6;
		
		//set values
		rv2.setValue(v2);
		rv3.setValue(v3);		
		rv4.setValue(v4);
		rv5.setValue(v5);
		rv6.setValue(v6);
		rv7.setValue(v7);
		
		super.evaluate(solution);
	}

}
