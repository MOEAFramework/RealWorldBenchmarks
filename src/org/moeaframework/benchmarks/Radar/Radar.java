package org.moeaframework.benchmarks.Radar;

import java.io.File;
import java.io.IOException;

import org.apache.commons.lang3.SystemUtils;
import org.moeaframework.core.PRNG;
import org.moeaframework.core.Solution;
import org.moeaframework.core.variable.RealVariable;
import org.moeaframework.problem.ExternalProblem;
import org.moeaframework.util.io.RedirectStream;

public class Radar extends ExternalProblem {
	
	public static final String PATH = "./native/Radar/bin/";
	
	public Radar() throws IOException {
		super((String)null, startProcess());
	}
	
	public static int startProcess() throws IOException {
		int port = PRNG.nextInt(10000, 65536);
		String command = SystemUtils.IS_OS_WINDOWS ?
				"matlab.exe" :
				"/usr/global/matlab/R2013a/bin/matlab";
	
		 Process process = new ProcessBuilder()
				.command(command, "-r", "startEval(8,9,0,'radar','" + port + "')")
				.directory(new File(PATH))
				.start();
/*
		Process process = Runtime.getRuntime().exec(
			new String[] {
				"/usr/global/matlab/R2013a/bin/matlab",
				"-r",
				"startEval(8,9,0,'radar','" + port + "')" },
			null,
			new File(PATH));
*/

		RedirectStream.redirect(process.getInputStream(), System.out);
		RedirectStream.redirect(process.getErrorStream(), System.err);

		try {
			Thread.sleep(30000);
		} catch (InterruptedException e) {
			// do nothing
		}

		return port;
	}

	@Override
	public String getName() {
		return "Radar";
	}

	@Override
	public int getNumberOfConstraints() {
		return 0;
	}

	@Override
	public int getNumberOfObjectives() {
		return 9;
	}

	@Override
	public int getNumberOfVariables() {
		return 8;
	}

	@Override
	public Solution newSolution() {
		Solution solution = new Solution(8, 9, 0);
		
		for (int i = 0; i < 8; i++) {
			solution.setVariable(i, new RealVariable(0.0, 1.0));
		}
		
		return solution;
	}

}
