% Create TCP connection:
t = tcpip('0.0.0.0', 7247, 'NetworkRole', 'server');
set(t, 'InputBufferSize', 1000);
fopen(t);
disp('New connection');

%% Main loop:

data_size = 1000;
data = zeros(1, data_size);
index = 1;

while true
    if t.BytesAvailable > 0
        data = fscanf(t); % Receive floating point values separated by spaces as string.
        splited = strsplit(data); % Split (by spaces) this string into an array of strings.
        
        % Convert array of strings to vector of reals:
        values = arrayfun(@(x) str2double(x), splited);
        disp(values(1:3));
        
        data(index) = abs(values(1));
        
        disp(index);
        
        if index == data_size
           break 
        end
        
        index = index + 1;
        
        flushinput(t);
    end
end

disp(max(data));

%% Clean up:

% Clean up TCP connection:
fclose(t); 
delete(t); 
clear t;