function [] = startEval(nvars, nobjs, ncons, functionHandle, port)

if ~libisloaded('libmoeaframework')
    loadlibrary('libmoeaframework', 'moeamatlab.h');
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
calllib('libmoeaframework', 'MOEA_Init_socket', nobjs, ncons, port);
disp('Connection established...');

while strcmp(calllib('libmoeaframework', 'MOEA_Next_solution'), 'MOEA_SUCCESS')
    vars = zeros(1, nvars);
    pvars = libpointer('doublePtr', vars);
    calllib('libmoeaframework', 'MOEA_Read_doubles', nvars, pvars);
    vars = get(pvars, 'Value');

    [objs, cons] = functionHandle(vars);

    pobjs = libpointer('doublePtr', objs);
    pcons = libpointer('doublePtr', cons);
    calllib('libmoeaframework', 'MOEA_Write', pobjs, pcons);
end

calllib('libmoeaframework', 'MOEA_Terminate');
disp('Shutting down...');
exit
