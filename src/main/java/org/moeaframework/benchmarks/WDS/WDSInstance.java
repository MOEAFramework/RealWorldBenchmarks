/* Copyright 2009-2023 David Hadka and other contributors
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
package org.moeaframework.benchmarks.WDS;

public enum WDSInstance {
	
	TRN("TRN", 8, 8, new double[] { 0.01, 0.001 }),
	
	TLN("TLN", 8, 14, new double[] { 0.01, 0.001 }),
	
	BAK("BAK", 9, 11, new double[] { 0.001, 0.001 }),
	
	NYT("NYT", 21, 16, new double[] { 1, 0.001 }),
	
	BLA("BLA", 23, 14, new double[] { 0.001, 0.001 }),
	
	HAN("HAN", 34, 6, new double[] { 0.01, 0.001 }),
	
	GOY("GOY", 30, 8, new double[] { 0.001, 0.001 }),
	
	FOS("FOS", 58, 22, new double[] { 0.001, 0.001 }),
	
	PES("PES", 99, 13, new double[] { 0.01, 0.001 }),
	
	MOD("MOD", 317, 13, new double[] { 0.01, 0.001 }),
	
	BIN("BIN", 454, 10, new double[] { 0.01, 0.001 }),
	
	EXN("EXN", 567, 11, new double[] { 0.01, 0.001 });
	
	private final String name;
	
	private final int numberOfVariables;
	
	private final int numberOfOptions;
	
	private final double[] epsilon;
	
	WDSInstance(String name, int numberOfVariables, int numberOfOptions, double[] epsilon) {
		this.name = name;
		this.numberOfVariables = numberOfVariables;
		this.numberOfOptions = numberOfOptions;
		this.epsilon = epsilon;
	}

	public String getName() {
		return name;
	}

	public int getNumberOfVariables() {
		return numberOfVariables;
	}
	
	public int getNumberOfOptions() {
		return numberOfOptions;
	}

	public double[] getEpsilon() {
		return epsilon.clone();
	}

}
