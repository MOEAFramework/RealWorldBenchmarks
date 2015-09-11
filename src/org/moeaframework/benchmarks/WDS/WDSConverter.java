package org.moeaframework.benchmarks.WDS;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

/**
 * Simple script to convert from the best Pareto fronts obtained from
 * http://emps.exeter.ac.uk/engineering/research/cws/resources/benchmarks/design-resiliance-pareto-fronts/data-files/
 * to a format that matches the outputs from the WDS instances.
 */
public class WDSConverter {

	public static void main(String[] args) throws IOException {
		for (WDSInstance instance : WDSInstance.values()) {
			File input = new File("src/org/moeaframework/benchmarks/WDS/bestPF_" + instance.getName() + ".txt");
			
			if (!input.exists()) {
				input = new File("src/org/moeaframework/benchmarks/WDS/truePF_" + instance.getName() + ".txt");
			}
			
			File output = new File("src/org/moeaframework/benchmarks/WDS/" + instance.getName() + ".reference");
			
			BufferedReader reader = new BufferedReader(new FileReader(input));
			String line = null;
			
			PrintWriter writer = new PrintWriter(new FileWriter(output));
			
			while ((line = reader.readLine()) != null) {
				String[] tokens = line.trim().split("\\s+");
				double v1 = Double.parseDouble(tokens[0]);
				double v2 = Double.parseDouble(tokens[1]);
					
				writer.println(v2 + " " + (-v1));
			}
			
			writer.close();
			reader.close();
		}
	}
	
}
