package org.moeaframework.problem;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class NativeCommand {
	
	private final String executableName;
	
	private final String[] arguments;
	
	private final File workingDirectory;
	
	public NativeCommand(String executableName, String[] arguments) {
		this(executableName, arguments, null);
	}

	public NativeCommand(String executableName, String[] arguments, File workingDirectory) {
		super();
		this.executableName = executableName;
		this.arguments = arguments;
		this.workingDirectory = workingDirectory;
	}

	public String getExecutableName() {
		return executableName;
	}

	public String[] getArguments() {
		return arguments;
	}
	
	public File getWorkingDirectory() {
		return workingDirectory;
	}
	
	public Process exec() throws IOException {
		return toProcessBuilder(OsType.getOsType()).start();
	}
	
	public ProcessBuilder toProcessBuilder(OsType osType) {
		List<String> command = new ArrayList<String>();
		String executableName = getExecutableName();
		File workingDirectory = getWorkingDirectory();
		
		if (osType == OsType.WINDOWS && !executableName.toLowerCase().endsWith(".exe")) {
			executableName += ".exe";
		}

		if (new File(workingDirectory, executableName).exists()) {
			// Executable exists in working directory, run directly.
			File relativePath = osType == OsType.WINDOWS ? workingDirectory : new File(".");
			command.add(new File(relativePath, executableName).getPath());
		} else {
			// Executable not found, attempt to run from PATH.
			command.add(executableName);
		}

		command.addAll(List.of(getArguments()));
		
		return new ProcessBuilder()
				.command(command)
				.directory(getWorkingDirectory());
	}

}
