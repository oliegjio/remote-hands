%% Server:

t = tcpip('0.0.0.0', 7247, 'NetworkRole', 'server');
fopen(t);
disp('New connection');

while true
    if t.BytesAvailable 
        data = fscanf(t);
        disp(data);
    end
end

%% Clear:

fclose(t);
delete(t);
clear t;