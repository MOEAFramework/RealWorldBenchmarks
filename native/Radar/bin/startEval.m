function [] = startEval(nvars, nobjs, ncons, functionHandle, port)

if ispc
    libname = 'moeaframework';
else
    libname = 'libmoeaframework';
end

if ~libisloaded(libname)
    loadlibrary(libname, 'moeamatlab.h');
end

if (isa(functionHandle, 'function_handle'))
    %already a function handle, ok
elseif (isa(functionHandle, 'char'))
    functionHandle = str2func(functionHandle);
else
    error('Function is not a valid function name or function handle');
end

if (nargin < 5)
    port = libpointer
end

disp('Waiting for connection...');
calllib(libname, 'MOEA_Init_socket', nobjs, ncons, port);
disp('Connection established...');

while strcmp(calllib(libname, 'MOEA_Next_solution'), 'MOEA_SUCCESS')
    vars = zeros(1, nvars);
    pvars = libpointer('doublePtr', vars);
    calllib(libname, 'MOEA_Read_doubles', nvars, pvars);
    vars = get(pvars, 'Value');

    [objs, cons] = functionHandle(vars);

    pobjs = libpointer('doublePtr', objs);
    pcons = libpointer('doublePtr', cons);
    calllib(libname, 'MOEA_Write', pobjs, pcons);
end

calllib(libname, 'MOEA_Terminate');
disp('Shutting down...');
exit
