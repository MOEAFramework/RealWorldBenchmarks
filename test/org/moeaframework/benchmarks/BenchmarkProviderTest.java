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
	
	protected void test(String problemName) {
		new Executor()
				.withAlgorithm("NSGAII")
				.withProblem(problemName)
				.withMaxEvaluations(1000)
				.run();

		Assert.assertNotNull(
				ProblemFactory.getInstance().getReferenceSet(problemName));
	}
	
	@Test
	public void testCarSideImpact() {
		test("CarSideImpact");
	}
	
	@Test
	public void testElectricMotor() {
		test("ElectricMotor");
	}
	
	@Test
	public void testGAA() {
		test("GAA");
	}
	
	@Test
	public void testHBV() {
		test("HBV");
	}
	
	@Test
	public void testLakeProblem() {
		test("LakeProblem");
	}
	
	@Test
	public void testLRGV() {
		test("LRGV");
	}
	
	@Test
	@Ignore("requires matlab")
	public void testRadar() {
		test("Radar");
	}
	
	@Test
	public void testWDS() {
		test("WDS(GOY)");
	}

}
