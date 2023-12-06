# Radar Waveform Optimization

This code is adapted from Evan J. Hughes EMO 2007 Many-Objective Radar
Waveform Software available at http://code.evanhughes.org/.

To use this problem, you must first download the ZIP from
http://code.evanhughes.org/ and extract the file testpris.p into the
native/Radar/bin/ folder.  This code is licensed only for academic use.  Please
read the license terms before download and extracting this file.

This example is designed to work with the MOEA Framework and Borg MOEA.
For example, Borg can be used with the following command:

  ./borg.exe -n 10000 -v 8 -o 9 -S -D 30 -P 16801 -f out.set -- matlab
    -nodesktop -nodisplay -nosplash -logfile out.log
    -r "startEval(8, 9, 0, 'radar', '16801')"

The Borg MOEA options used above include:

  -n 10000 - Run for 10,000 function evaluations
  -v 8     - 8 decision variables ranging from [0, 1]
  -o 9     - 9 minimization objectives
  -S       - Connect to Matlab using sockets
  -D 30    - Wait 30 seconds after launching Matlab before connecting
  -P 16801 - Connect to port 16801

The Borg MOEA launches Matlab and runs the command

  startEval(8, 9, 0, 'radar', '16801').

The arguments to this command are the number of variables, number of
objectives, number of constraints, the function name to optimize, and
the port used for communication. 

#### Troubleshooting

bind: Address already in use
Unable to establish socket connection
  -> Change the port number used, both in the -P option and in the startEval
     command

execv: No such file or directory
  -> Borg MOEA is unable to locate the matlab executable.  On Unix-like
     systems, you may need to provide the full path to matlab.  Run
     'which matlab' to see the full path.

