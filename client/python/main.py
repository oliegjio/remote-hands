import serial
from graphics import *
from kinematics import *
from mathematics import *
from networking import *
from utils import *
from pyquaternion import Quaternion
import time


def millis():
    return int(round(time.time() * 1000))


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


def clamp(n, minimum, maximum):
    """ Constrain value between `minimum` and `maximum`. """
    return max(min(maximum, n), minimum)


def handle_transforms(quaternion, acceleration):
    new_quaternion = Quaternion(*quaternion)
    new_acceleration = new_quaternion.rotate(acceleration)  # Rotate acceleration to global basis.
    new_acceleration[2] -= 1  # Subtract gravity vector.

    for i, t in enumerate(new_acceleration):
        # Restrict acceleration values:
        bound = 0.25
        new_acceleration[i] = clamp(t, bound, -bound)

        # Scale acceleration:
        new_acceleration[i] *= 10

    return new_quaternion, new_acceleration


def handle_translation(translation, acceleration):
    new_translation = list(translation)

    for i, t in enumerate(new_translation):
        new_translation[i] += acceleration[i]
        new_translation[i] = clamp(new_translation[i], -180, 180)  # Restring end effector position to a square.

    return new_translation


def handle_output(angles):
    """ Convert list of angles to output string for serial port. """

    radians = list(map(lambda x: x * (180 / math.pi), angles))  # Convert degrees to radians.

    angle_freedom = 110
    restricted = list(map(lambda x: clamp(x, -angle_freedom, angle_freedom), radians))  # Constrain angles between...

    strings = list(map(lambda x: str(int(x)), restricted))  # Convert angles to integers and then to strings.

    output = ' '.join(strings) + ' \n'  # Construct output string.
    array = output.encode(encoding='ascii')  # Convert output string to byte array with ASCII encoding.

    return array


def main():
    serial_port = serial.Serial('/dev/ttyUSB1', 9600)  # Connect to manipulator controller.
    networking = Networking('10.42.0.1', 7247)  # Connect to bracer controller.
    networking.start()
    # graphics = Graphics(800, 600)
    # graphics.start()

    translation = [50, 50, 0]  # Origin point of end effector.

    time.sleep(10)  # Wait until gyroscope calibration finished.

    while True:
        data = networking.receive()

        try:
            quaternion, acceleration = handle_data(data)
        except ValueError as error:
            print(error)
            continue

        quaternion, acceleration = handle_transforms(quaternion, acceleration)

        translation = handle_translation(translation, acceleration)

        try:
            angles = solve_3dof_kinematics(translation[0], translation[1], 94.7, 71.2, 46)
        except ValueError as error:
            print("Kinematics Error: ", error)
            continue

        # graphics.window.animate_square(translation[0], translation[1])

        output = handle_output(angles)
        serial_port.write(output)


if __name__ == '__main__':
    main()
