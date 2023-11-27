package org.moeaframework.problem;

import java.io.IOException;

public abstract class NativeProblem extends ExternalProblem {

	public NativeProblem(ProcessBuilder builder) throws IOException {
		super(builder.start());
	}

}
