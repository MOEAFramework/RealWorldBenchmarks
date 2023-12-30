package org.moeaframework.problem;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class NativeCommand {
	
	private final String executableName;
	
	private final String[] arguments;
	
	private final File workingDirectory;

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
		
		switch (osType) {
		case WINDOWS:
			command.add(new File(getWorkingDirectory(), getExecutableName() + ".exe").getPath());
			break;
		case POSIX:
			command.add(new File(".", getExecutableName()).getPath());
			break;
		default:
			throw new IllegalArgumentException("Unsupported OS " + osType);
		}
		
		command.addAll(List.of(getArguments()));
		
		return new ProcessBuilder()
				.command(command)
				.directory(getWorkingDirectory());
	}

}
