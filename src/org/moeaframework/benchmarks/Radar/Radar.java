package org.moeaframework.benchmarks.Radar;

import java.io.File;
import java.io.IOException;

import org.apache.commons.lang3.SystemUtils;
import org.moeaframework.core.FrameworkException;
import org.moeaframework.core.PRNG;
import org.moeaframework.core.Solution;
import org.moeaframework.core.variable.RealVariable;
import org.moeaframework.problem.ExternalProblem;

public class Radar extends ExternalProblem {
	
	public static final String PATH = "./native/Radar/bin/";
	
	private final String port;
	
	private Process matlabProcess;

	public Radar() throws IOException {
		this(Integer.toString(PRNG.nextInt(10000, 65536)));
	}
	
	Radar(String port) throws IOException {
		super(null, port);
		this.port = port;
		
		validate();
		matlabProcess = createProcess().start();
		
		try {
			Thread.sleep(30000);
		} catch (InterruptedException e) {
			// do nothing
		}
	}
	
	public void validate() {
		if (!new File(PATH, "testpris.p").exists()) {
			throw new FrameworkException("testpris.p missing, see installation instructions");
		}
	}
	
	public ProcessBuilder createProcess() {
		String command = SystemUtils.IS_OS_WINDOWS ?
				"matlab.exe" :
				"matlab";
		
		return new ProcessBuilder()
				.command(command, "-r", "startEval(8, 9, 0, 'radar', '" + port + "')")
				.directory(new File(PATH));
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

	@Override
	public synchronized void close() {
		if (matlabProcess != null) {
			matlabProcess.destroy();
		}
		
		super.close();
	}

}
