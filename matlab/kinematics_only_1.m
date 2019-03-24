%% Manipulator setup:

robot = robotics.RigidBodyTree;

% Denavit-Hartenberg manipulator parameters:
dhparams = [0        pi/2  0.2372   0;
            0.6438   0     0        0;
            0.6438 	 0     0        0;
            0.3388   0     0        0;
            0        pi/2  0        0;
            0        0     0        0];

% Setup manipulator nodes:
node1 = robotics.RigidBody('node1');
node2 = robotics.RigidBody('node2');
node3 = robotics.RigidBody('node3');
node4 = robotics.RigidBody('node4');
node5 = robotics.RigidBody('node5');
node6 = robotics.RigidBody('node6');

% Setup manipulator joints:
joint1 = robotics.Joint('joint1', 'revolute');
joint2 = robotics.Joint('joint2', 'revolute');
joint3 = robotics.Joint('joint3', 'revolute');
joint4 = robotics.Joint('joint4', 'revolute');
joint5 = robotics.Joint('joint5', 'revolute');
joint6 = robotics.Joint('joint6', 'revolute');

% Transform joints with DH parameters:
setFixedTransform(joint1, dhparams(1,:), 'dh');
setFixedTransform(joint2, dhparams(2,:), 'dh');
setFixedTransform(joint3, dhparams(3,:), 'dh');
setFixedTransform(joint4, dhparams(4,:), 'dh');
setFixedTransform(joint5, dhparams(5,:), 'dh');
setFixedTransform(joint6, dhparams(6,:), 'dh');

% Assign joints to manipulator nodes:
node1.Joint = joint1;
node2.Joint = joint2;
node3.Joint = joint3;
node4.Joint = joint4;
node5.Joint = joint5;
node6.Joint = joint6;

% Assemble manipulator:
addBody(robot, node1, robot.BaseName);
addBody(robot, node2, 'node1');
addBody(robot, node3, 'node2');
addBody(robot, node4, 'node3');
addBody(robot, node5, 'node4');
addBody(robot, node6, 'node5');

%% Initializations:

% Create inverse kinematics solver:
ik = robotics.InverseKinematics('RigidBodyTree', robot);
ik.RigidBodyTree = robot;

% Setup values needed to solver:
homeConf = homeConfiguration(robot);
disp(homeConf)
effector = getTransform(robot, homeConf, 'node6', 'base'); % End effector transformation matrix.
target = [0 0 0.8]; % Desired end effector position.
effector(1:3, 4) = target;
weights = [0.01 0.01 0.01 1 1 1];

[ikSolution, ikInfo] = ik('node6', effector, weights, homeConf);

position = solutionPositions(ikSolution);
disp(position);

% Update manipulator plot:
show(robot, ikSolution);
hold all;
scatter3(target(1), target(2), target(3), 'r*', 'linewidth', 20);
hold off;
drawnow;

%% Function definitions:

function f = solutionPositions(solution)
    % Get vector of positions from inverse kinematics solution. 
    f = arrayfun(@(x) x.JointPosition, solution);
end