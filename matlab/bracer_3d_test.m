t = tcpip('0.0.0.0', 7247, 'NetworkRole', 'server');
fopen(t);

% Position of the effector:
p = [0 0 0];

% Velocity of the effector:
v = [0 0 0];

dt = 0;
last_time = cputime;

while true
    dt = cputime - last_time;
    last_time = cputime;
    if t.BytesAvailable       
        data = fscanf(t);
        
        % Split data string (with spaces):
        splitted = strsplit(data);
        if (size(splitted) < 7); continue; end
        
        % Convert strings to doubles:
        values = arrayfun(@(x) str2double(x), splitted);
        if values(1:4) == zeros(4); continue; end
        
        % Quaternion rotation:
        q = values(1:4);
        
        % Acceleration:
        a = values(5:7);
        
        % Rotate acceleration with quaternion:
        ar = quatrotate(quatinv(q), a);
        
        % Subtract gravity from acceleration:
        ag = ar - [0 0 1];
        if visnan(ag); continue; end
        ag = ag * dt;
        
        % Update velocity with acceleration:
        v = v + ag;
        v = v * 0.99;
        
        % Update position with velocity:
        p = p + v;
        
        % Debug:
        fprintf('dt: %f ', dt);
        fprintf('p: %f %f %f ', p(1), p(2), p(3));
        fprintf('ag: %f %f %f ', ag(1), ag(2), ag(3));
        fprintf('v: %f %f %f ', v(1), v(2), v(3));
%         fprintf('a: %f %f %f ', a(1), a(2), a(3));
%         fprintf('ar: %f %f %f ', ar(1), ar(2), ar(3));
%         fprintf('q: %f %f %f %f ', q(1), q(2), q(3), q(4));
        fprintf('\n');
        
        % Draw:
        plot3(p(1), p(2), p(3), '.');
        limit = [-50 50];
        xlim(limit);
        ylim(limit);
        zlim(limit);
        grid on;
        drawnow;
    end
end

%% Function definitions:

function f = visnan(v)
    f = fold(@(a, x) x || a, arrayfun(@(x) isnan(x), v));
end