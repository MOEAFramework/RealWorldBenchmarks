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
package org.moeaframework.problem;

import java.io.IOException;

import org.moeaframework.core.FrameworkException;

public abstract class NativeProblem extends ExternalProblem {
	
	private final NativeCommand command;
	
	public NativeProblem(NativeCommand command) {
		super(startProcessOrThrow(command));
		this.command = command;
	}
	
	public NativeCommand getCommand() {
		return command;
	}
	
	private static final Process startProcessOrThrow(NativeCommand command) {
		try {
			return command.exec();
		} catch (IOException e) {
			throw new FrameworkException("Failed to start native process", e);
		}
	}

}
