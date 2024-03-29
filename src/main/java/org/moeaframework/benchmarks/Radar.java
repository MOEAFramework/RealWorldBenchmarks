package org.moeaframework.benchmarks;

import java.io.File;

import org.moeaframework.core.FrameworkException;
import org.moeaframework.core.PRNG;
import org.moeaframework.core.Settings;
import org.moeaframework.core.Solution;
import org.moeaframework.core.variable.RealVariable;
import org.moeaframework.problem.ExternalProblem;
import org.moeaframework.problem.NativeCommand;
import org.moeaframework.util.io.RedirectStream;

public class Radar extends ExternalProblem {

	public static final String PATH = "./native/Radar/bin/";

	public Radar() throws Exception {
		super("127.0.0.1", startProcess());
	}

	public static int startProcess() throws Exception {
		validate();
		
		int port = Settings.PROPERTIES.getInt("matlab.port",
				PRNG.nextInt(10000, 65536));
		
		NativeCommand command = new NativeCommand(
				Settings.PROPERTIES.getString("matlab.path", "matlab"),
				new String[] { "-batch", "startEval(8,9,0,'radar','" + port + "')" },
				new File(PATH));

		Process process = command.exec();

		RedirectStream.redirect(process.getInputStream(), System.out);
		RedirectStream.redirect(process.getErrorStream(), System.err);

		Thread.sleep(Settings.PROPERTIES.getInt("matlab.sleep", 30000));

		return port;
	}

	public static void validate() {
		if (!new File(PATH, "testpris.p").exists()) {
			throw new FrameworkException("testpris.p missing, see installation instructions");
		}
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
