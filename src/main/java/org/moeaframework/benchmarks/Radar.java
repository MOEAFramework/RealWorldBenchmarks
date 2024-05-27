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

import java.io.File;
import java.time.Duration;

import org.moeaframework.core.FrameworkException;
import org.moeaframework.core.PRNG;
import org.moeaframework.core.Settings;
import org.moeaframework.core.Solution;
import org.moeaframework.core.variable.RealVariable;
import org.moeaframework.problem.ExternalProblem;

public class Radar extends ExternalProblem {

	public Radar() throws Exception {
		super(createBuilder());
	}

	public static Builder createBuilder() {
		if (!new File("./native/Radar/bin/", "testpris.p").exists()) {
			throw new FrameworkException("testpris.p missing, see installation instructions");
		}
		
		int port = Settings.PROPERTIES.getInt("matlab.port", PRNG.nextInt(10000, 65536));
		int retries = Settings.PROPERTIES.getInt("matlab.retries", 30);
		String command = Settings.PROPERTIES.getString("matlab.path", "matlab");
		
		return new Builder()
				.withCommand(command, "-batch", "startEval(8,9,0,'radar','" + port + "')")
				.withWorkingDirectory(new File("./native/Radar/bin/"))
				.withSocket("127.0.0.1", port)
				.withRetries(retries, Duration.ofSeconds(1));
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
