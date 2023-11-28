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
package org.moeaframework.benchmarks;

import java.io.File;
import java.io.IOException;

import org.moeaframework.Executor;
import org.moeaframework.benchmarks.LakeProblem.LakeProblem;
import org.moeaframework.core.EpsilonBoxDominanceArchive;
import org.moeaframework.core.PopulationIO;

public class GenerateReferenceSet {
	
	public static void main(String[] args) throws IOException {
		String[] algorithms = new String[] {
				"NSGAII",
				"NSGAIII",
				"eMOEA",
				"GDE3",
				//"IBEA",
				"MOEAD",
				"OMOPSO",
				//"PAES",
				//"PESA2",
				"SMPSO",
				"SPEA2"
		};
		
		String problem = "LakeProblem";
		double[] epsilon = LakeProblem.EPSILON;
		
		EpsilonBoxDominanceArchive referenceSet = new EpsilonBoxDominanceArchive(epsilon);
		
		for (String algorithm : algorithms) {
			System.out.println("Processing " + algorithm + "...");
			
			for (int i = 0; i < 10; i++) {
				System.out.println("  Running seed " + (i+1));
				
				referenceSet.addAll(new Executor()
						.withProblem(problem)
						.withAlgorithm(algorithm)
						.withMaxEvaluations(100000)
						.withEpsilon(epsilon)
						.run());
				
				PopulationIO.writeObjectives(
						new File(problem + ".reference"),
						referenceSet);
			}
		}
	}

}
