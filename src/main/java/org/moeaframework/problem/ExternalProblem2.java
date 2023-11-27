package org.moeaframework.problem;

import java.io.IOException;

public abstract class ExternalProblem2 extends ExternalProblem {

	public ExternalProblem2(ProcessBuilder builder) throws IOException {
		super(builder.start());
	}

}
