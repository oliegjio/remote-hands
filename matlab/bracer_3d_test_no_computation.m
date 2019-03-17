t = tcpip('0.0.0.0', 7247, 'NetworkRole', 'server');
fopen(t);
disp('New connection');

% Position of the effector:
p = [0 0 0];

% Velocity of the effector:
v = [0 0 0];

while true
    if t.BytesAvailable       
        data = fscanf(t);
        
        % Split data string (with spaces):
        splitted = strsplit(data);
        if (size(splitted) < 3); continue; end
        
        % Convert strings to doubles:
        ad = arrayfun(@(x) str2double(x), splitted);
        a = ad(1:3);
        
        % Update velocity with acceleration:
%         v = (v + a) * 0.99;
        v = v + a;
        
        % Update position with velocity:
        p = p + v;
        
        % Debug:
%         fprintf('dt: %f ', dt);
        fprintf('p: %f %f %f ', p(1), p(2), p(3));
%         fprintf('ag: %f %f %f ', ag(1), ag(2), ag(3));
        fprintf('v: %f %f %f ', v(1), v(2), v(3));
        fprintf('a: %f %f %f ', a(1), a(2), a(3));
%         fprintf('ar: %f %f %f ', ar(1), ar(2), ar(3));
%         fprintf('q: %f %f %f %f ', q(1), q(2), q(3), q(4));
        fprintf('\n');
        
        % Draw:
        plot3(p(1), p(2), p(3), '.');
        limit = [-250 250];
        xlim(limit);
        ylim(limit);
        zlim(limit);
        grid on;
        drawnow;
    end
end

%% Clear:

fclose(f);
delete(t);
clear t;

%% Function definitions:

function f = visnan(v)
    f = fold(@(a, x) x || a, arrayfun(@(x) isnan(x), v));
end