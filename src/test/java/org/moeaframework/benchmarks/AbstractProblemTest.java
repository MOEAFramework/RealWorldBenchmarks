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

import java.util.Arrays;
import java.util.concurrent.TimeUnit;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.Ignore;
import org.moeaframework.algorithm.NSGAII;
import org.moeaframework.core.Solution;
import org.moeaframework.core.population.NondominatedPopulation;
import org.moeaframework.core.spi.ProblemFactory;
import org.moeaframework.core.variable.BinaryIntegerVariable;
import org.moeaframework.core.variable.RealVariable;
import org.moeaframework.problem.Problem;

@Ignore("abstract test class")
public class AbstractProblemTest {
	
	private static final double EPS = 0.01;
	
	protected void testSolve(String problemName) {
		try (Problem problem = ProblemFactory.getInstance().getProblem(problemName)) {
			Assert.assertNotNull("Problem not defined", problem);
			Assert.assertEquals("Problem name must match", problemName, problem.getName());
			
			NSGAII algorithm = new NSGAII(problem);
			algorithm.run(1000);
			
			NondominatedPopulation result = algorithm.getResult();
			
			Assert.assertNotNull("Expected non-null result", result);
			Assert.assertFalse("Expected non-empty result", result.isEmpty());
		}
	}
	
	protected void testReferenceSet(String problemName) {
		NondominatedPopulation referenceSet = ProblemFactory.getInstance().getReferenceSet(problemName);
		Assert.assertNotNull("Expected reference set", referenceSet);
		Assert.assertFalse("Expected non-empty reference set", referenceSet.isEmpty());
	}
	
	protected void testSolution(String problemName, double[] variables, double[] expectedObjectives, double[] expectedConstraints, boolean isFeasible) {
		try (Problem problem = ProblemFactory.getInstance().getProblem(problemName)) {
			Solution solution = problem.newSolution();
			RealVariable.setReal(solution, variables);
			
			problem.evaluate(solution);
			
			try {
				Assert.assertArrayEquals("Objectives do not match", expectedObjectives, solution.getObjectiveValues(), EPS);
			} catch (AssertionError e) {
				System.out.println("Actual Objectives: " + Arrays.toString(solution.getObjectiveValues()));
				throw e;
			}
			
			try {
				Assert.assertArrayEquals("Constraints do not match", expectedConstraints, solution.getConstraintValues(), EPS);
			} catch (AssertionError e) {
				System.out.println("Actual Constraints: " + Arrays.toString(solution.getConstraintValues()));
				throw e;
			}
			
			Assert.assertEquals("Feasibility does not match", isFeasible, solution.isFeasible());
			
		}
	}
	
	protected void testSolution(String problemName, int[] variables, double[] expectedObjectives, double[] expectedConstraints, boolean isFeasible) {
		try (Problem problem = ProblemFactory.getInstance().getProblem(problemName)) {
			Solution solution = problem.newSolution();
			BinaryIntegerVariable.setInt(solution, variables);
			
			problem.evaluate(solution);
			
			try {
				Assert.assertArrayEquals("Objectives do not match", expectedObjectives, solution.getObjectiveValues(), EPS);
			} catch (AssertionError e) {
				System.out.println("Actual Objectives: " + Arrays.toString(solution.getObjectiveValues()));
				throw e;
			}
			
			try {
			Assert.assertArrayEquals("Constraints do not match", expectedConstraints, solution.getConstraintValues(), EPS);
			} catch (AssertionError e) {
				System.out.println("Actual Constraints: " + Arrays.toString(solution.getConstraintValues()));
				throw e;
			}
			
			Assert.assertEquals("Feasibility does not match", isFeasible, solution.isFeasible());
			
		}
	}
	
	protected void requiresMatlab() {
		try {
			Process process = new ProcessBuilder()
					.command("matlab", "-batch", "exit(0)")
					.start();

			process.waitFor(30, TimeUnit.SECONDS);
			
			if (process.isAlive()) {
				process.destroy();
				Assume.assumeTrue("Process did not terminate within configured timeout", false);
			}
			
			Assume.assumeTrue("Process exited with non-zero result code", process.exitValue() == 0);
		} catch (Exception e) {
			Assume.assumeNoException("Caught exception when invoking process", e);
		}
	}

}
