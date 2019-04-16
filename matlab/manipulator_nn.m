% clear;
% clc;
% clf('reset');

%% Manipulator setup:

robot = robotics.RigidBodyTree;

% Denavit-Hartenberg manipulator parameters:
dhparams = [0        pi/2  0.3796   0;
            0.6438   0     0        0;
            0.6438 	 0     0        0;
            0.1650   0     0        0];

% Setup manipulator nodes:
node1 = robotics.RigidBody('node1');
node2 = robotics.RigidBody('node2');
node3 = robotics.RigidBody('node3');
node4 = robotics.RigidBody('node4');
effectorName = 'node4';

% Setup manipulator joints:
joint1 = robotics.Joint('joint1', 'revolute');
joint2 = robotics.Joint('joint2', 'revolute');
joint3 = robotics.Joint('joint3', 'revolute');
joint4 = robotics.Joint('joint4', 'revolute');

% Set position limits for joints:
limit = [-pi/2 pi/2];
joint1.PositionLimits = [0.1745 6.1087];
joint2.PositionLimits = [0 pi];
joint3.PositionLimits = limit;
joint4.PositionLimits = limit;

% Transform joints with DH parameters:
setFixedTransform(joint1, dhparams(1,:), 'dh');
setFixedTransform(joint2, dhparams(2,:), 'dh');
setFixedTransform(joint3, dhparams(3,:), 'dh');
setFixedTransform(joint4, dhparams(4,:), 'dh');

% Assign joints to manipulator nodes:
node1.Joint = joint1;
node2.Joint = joint2;
node3.Joint = joint3;
node4.Joint = joint4;

% Assemble manipulator:
addBody(robot, node1, robot.BaseName);
addBody(robot, node2, 'node1');
addBody(robot, node3, 'node2');
addBody(robot, node4, 'node3');

%% Initializations:

% Create inverse kinematics solver:
ik = robotics.InverseKinematics('RigidBodyTree', robot);
ik.RigidBodyTree = robot;

% Setup values needed to solver:
configuration = homeConfiguration(robot);
transform = getTransform(robot, configuration, effectorName, robot.BaseName); % End effector transformation matrix.
weights = [0 0 0 1 1 1];

%% Neural network:

doTrain = true;
genExamples = true;

% Train data:
if genExamples
    nOutput = 4;
    nInput = 3;
    nExamples = 500000;
    minAngle = -pi/2;
    maxAngle = pi/2;
    angles = (maxAngle - minAngle).* rand(nOutput, nExamples) + minAngle;
    nnTrainInputs = zeros(nInput, nExamples);
    nnTrainOutputs = zeros(nOutput, nExamples);
    for i = 1:nExamples   
        configuration = setPositionsToConfiguration(configuration, angles(:, i)');
        T = getTransform(robot, configuration, effectorName, robot.BaseName);
        target = T * [0; 0; 0; 1];
        nnTrainInputs(:, i) = target(1:3);
        nnTrainOutputs(:, i) = angles(:, i);
    end
end

% NN:
if doTrain  
    net = fitnet(ones(1, 4) * nOutput^3, 'trainbr');
    net.divideParam.trainRatio = 1;
    net.divideParam.valRatio = 0;
    net.divideParam.testRatio = 0;
    net = train(net, nnTrainInputs, nnTrainOutputs);
end

%% Plotting:

a = -1;
b = 1;
% target = (b - a).* rand(1, 3) + a; % Desired end effector position.
target = nnTrainInputs(:, randi(nExamples))'; % Desired end effector position.

% Update initial guess with new position:
prediction = net(target')';
configuration = setPositionsToConfiguration(configuration, prediction);

% Update manipulator plot:
clf('reset');
show(robot, configuration);
hold all;
scatter3(target(1), target(2), target(3), 'r*', 'linewidth', 5);
hold off;
drawnow;

%% Function definitions:

function f = solutionPositions(solution)
    % Get vector of positions from inverse kinematics solution. 
    f = arrayfun(@(x) x.JointPosition, solution);
end

function f = setPositionsToConfiguration(configuration, positions)
    s = size(positions);
    for i = 1:s(2)
        configuration(i).JointPosition = positions(i);
    end
    f = configuration;
end