package org.moeaframework.problem;

import org.apache.commons.lang3.SystemUtils;

public enum OsType {
	
	/**
	 * Windows.
	 */
	WINDOWS,
	
	/**
	 * POSIX compatible systems, such as Unix, Linux, etc.
	 */
	POSIX;
	
	public static OsType getOsType() {
		if (SystemUtils.IS_OS_WINDOWS) {
			return WINDOWS;
		} else {
			return POSIX;
		}
	}

}
