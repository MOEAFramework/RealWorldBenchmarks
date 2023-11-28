package org.moeaframework.problem;

import java.io.IOException;

import org.moeaframework.core.FrameworkException;

public abstract class NativeProblem extends ExternalProblem {

	public NativeProblem(ProcessBuilder builder) {
		super(startProcessOrThrow(builder));
	}
	
	private static final Process startProcessOrThrow(ProcessBuilder builder) {
		try {
			return builder.start();
		} catch (IOException e) {
			throw new FrameworkException("Failed to start native process", e);
		}
	}

}
