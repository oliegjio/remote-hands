import serial
import math
from kinematics import solve_3dof_kinematics
from server import Server
from client import Client
from utils import reset_in_range, clamp
from pyquaternion import Quaternion
import time


def handle_data(data):
    """
    Takes data string received from the server.
    Tries to transform that data to tuple of quaternion (4 value list) and acceleration (3 value list).

    Raises:
        ValueError: If could not convert incoming data to meaningful format.
    """

    if data is None:
        raise ValueError("Data is empty")

    try:
        transforms = list(map(lambda x: float(x), data.split(b' ')))
    except ValueError:
        raise ValueError("Error splitting data or converting values to float")

    if len(transforms) != 7:
        raise ValueError("Wrong number of data values")

    quaternion = [transforms[0], transforms[1], transforms[2], transforms[3]]
    acceleration = [transforms[4], transforms[5], transforms[6]]

    return quaternion, acceleration


def handle_transforms(quaternion, acceleration):
    new_quaternion = Quaternion(*quaternion)
    new_acceleration = new_quaternion.rotate(acceleration)  # Rotate acceleration to global basis.
    new_acceleration[2] -= 1  # Subtract gravity vector.)

    for index, value in enumerate(new_acceleration):
        # if value > 0.5:
        #     new_acceleration[index] = 1
        # elif value < -0.5:
        #     new_acceleration[index] = -1
        # else:
        #     new_acceleration[index] = 0

        # Ignore values below...
        # lower_bound = 0.1
        # value = reset_in_range(value, 0, -lower_bound, lower_bound)

        # Set upper bound to acceleration:
        # upper_bound = 0.5
        # new_acceleration[index] = clamp(value, -upper_bound, upper_bound)

        # Scale acceleration:
        new_acceleration[index] *= 10

    return new_quaternion, new_acceleration


def update_translation(translation, acceleration):
    new_translation = list(translation)

    for i, t in enumerate(new_translation):
        new_translation[i] += acceleration[i]
        bound = 27
        new_translation[i] = clamp(new_translation[i], -bound, bound)  # Restring end effector position to a square.

    return new_translation


def handle_output(angles):
    """ Convert list of angles to output string for serial port. Returns a list of 3 angles in degrees. """

    radians = list(map(lambda x: x * (180 / math.pi), angles))  # Convert degrees to radians.

    angle_freedom = 110
    restricted = list(map(lambda x: clamp(x, -angle_freedom, angle_freedom), radians))  # Constrain angles between...

    strings = list(map(lambda x: str(int(x)), restricted))  # Convert angles to integers and then to strings.

    output = ' '.join(strings) + ' \n'  # Construct output string.
    array = output.encode(encoding='ascii')  # Convert output string to byte array with ASCII encoding.

    return array


def main():
    localhost = '10.42.0.1'
    # localhost = '127.0.0.1'
    # serial_port = serial.Serial('/dev/ttyUSB1', 9600)  # Connect to manipulator controller.
    server = Server(localhost, 7247)  # Connect to bracer controller.
    server.start()
    client = Client(localhost, 5677)
    client.start()
    # graphics = Graphics(800, 600)
    # graphics.start()

    translation = [15, 15, 15]  # Origin point of end effector.

    time.sleep(20)  # Wait until gyroscope calibration finished.

    while True:
        data = server.receive()

        try:
            quaternion, acceleration = handle_data(data)
        except ValueError as error:
            print(error)
            continue

        quaternion, acceleration = handle_transforms(quaternion, acceleration)

        translation = update_translation(translation, acceleration)

        try:
            print(translation)
            # angles = solve_3dof_kinematics(translation[0], translation[1], 94.7, 71.2, 46)
            angles = solve_3dof_kinematics(translation[1], translation[2], 10, 10, 10)
        except ValueError as error:
            print(error)
            continue

        # graphics.window.animate_square(translation[0], translation[1])

        output = handle_output(angles)
        try:
            client.send(output)
        except OSError:
            continue
        # serial_port.write(output)


if __name__ == '__main__':
    main()
