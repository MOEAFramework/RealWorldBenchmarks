# Radar Waveform Optimization

This code is adapted from Evan J. Hughes EMO 2007 Many-Objective Radar
Waveform Software available at http://code.evanhughes.org/.

## Setup

The actual problem definition is not distributed in this repository since the
code is only licensed for academic use.  To setup this problem, download the
Radar Waveform Matlab code from http://code.evanhughes.org/ and extract
`testpris.p` into `native/Radar/bin`.

## Usage

Sockets are used to communicate with the Radar Waveform problem written in
Matlab.  Consequently, this problem is only supported on POSIX-compatible systems,
such as Linux.

To optimize the problem using the Borg MOEA, run:

```bash

./borg.exe -n 10000 -v 8 -o 9 -S -D 30 -P 25001 -f out.set -- \
    matlab -batch "startEval(8, 9, 0, 'radar', '25001')"
```

These options include:

```
  -n 10000 - Run for 10,000 function evaluations
  -v 8     - 8 decision variables ranging from [0, 1]
  -o 9     - 9 minimization objectives
  -S       - Connect to Matlab using sockets
  -D 30    - Wait 30 seconds after launching Matlab before connecting
  -P 25001 - Connect to port 25001
```

The Borg MOEA will start a new Matlab process and invoke the command:

```matlab

startEval(8, 9, 0, 'radar', '25001')
```

`startEval` reads decision variables from the socket (port 25001) and calls the Radar
Waveform problem defined in `radar.m`.  The objectives and constraints (if any) are
sent back to the Borg MOEA through the same socket.  The first three numbers specify the
number of variables, objectives, and constraints, respectively.

## Troubleshooting

```
bind: Address already in use
Unable to establish socket connection
```

This could indicate (1) another process is running and consuming the port;
(2) the port hasn't been released by a previous invocation.  Consider changing
the port number used, both in the `-P` option and in the `startEval` command.

```
execv: No such file or directory
```

This occurs when the Matlab executable is not found.  You might need to add
the Matlab `bin/` folder to the system path.  Alternatively,
set `matlab.path` in `moeaframework.properties`.
