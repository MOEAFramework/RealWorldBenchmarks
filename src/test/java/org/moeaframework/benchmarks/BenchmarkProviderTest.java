package org.moeaframework.benchmarks;

import org.junit.Assert;
import org.junit.Ignore;
import org.junit.Test;
import org.moeaframework.Executor;
import org.moeaframework.core.spi.ProblemFactory;

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
	@Ignore("requires matlab")
	public void testRadar() {
		test("Radar", false);
	}
	
	@Test
	public void testWDS() {
		test("WDS(GOY)", true);
	}

}
