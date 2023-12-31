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
package org.moeaframework.benchmarks;

import java.util.concurrent.TimeUnit;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.Ignore;
import org.junit.Test;
import org.moeaframework.Executor;
import org.moeaframework.core.FrameworkException;
import org.moeaframework.core.spi.ProblemFactory;
import org.moeaframework.problem.NativeCommand;

/**
 * Tests to ensure each benchmark problem can be instantiated with the MOEA
 * Framework and reference sets exist.
 */
public class BenchmarkProviderTest {
	
	protected void test(String problemName, boolean hasReferenceSet) {
		new Executor()
				.withAlgorithm("NSGAII")
				.withProblem(problemName)
				.withMaxEvaluations(200)
				.run();
		
		if (hasReferenceSet) {
			Assert.assertNotNull("Missing reference set",
					ProblemFactory.getInstance().getReferenceSet(problemName));
		}
	}
	
	protected void requires(NativeCommand command) {
		try {
			Process process = command.exec();
			process.waitFor(15, TimeUnit.SECONDS);
			
			if (process.isAlive()) {
				process.destroy();
				throw new FrameworkException("Process did not terminate within configured timeout");
				//Assume.assumeTrue("Process did not terminate within configured timeout", false);
			}
			
			if (process.exitValue() != 0) {
				throw new FrameworkException("Process exited with non-zero result code");
			}

			//Assume.assumeTrue("Process exited with non-zero result code", process.exitValue() == 0);
		} catch (Exception e) {
			throw new FrameworkException("Caught exception when invoking process", e);
			//Assume.assumeNoException("Caught exception when invoking process", e);
		}
	}
	
	@Test
	public void testCarSideImpact() {
		test("CarSideImpact", true);
	}
	
	@Test
	public void testElectricMotor() {
		test("ElectricMotor", true);
	}
	
	@Test
	public void testGAA() {
		test("GAA", true);
	}
	
	@Test
	public void testHBV() {
		test("HBV", true);
	}
	
	@Test
	public void testLakeProblem() {
		test("LakeProblem", true);
	}
	
	@Test
	public void testLRGV() {
		test("LRGV", false);
	}
	
	@Test
	public void testRadar() {
		requires(new NativeCommand("matlab", new String[] { "-batch", "exit(0)" }));
		test("Radar", false);
	}
	
	@Test
	public void testWDS() {
		test("WDS(GOY)", true);
	}

}
