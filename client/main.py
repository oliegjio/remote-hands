import socket
import math
import numpy as np
import time
import serial
from graphics import GraphicsThread


sock = socket.socket()
sock.bind(('localhost', 8000))
sock.listen(1)
serial_port = serial.Serial('/dev/ttyUSB0', 9600)


def solve(x, y, l1, l2, l3):
    """
    Solve inverse kinematics equations for 3 DOF planar arm.

    Args:
        x (float): Target X position.
        y (float): Target Y position.
        l1 (float): Length of the first limb.
        l2 (float): Length of the second limb.
        l3 (float): Length of the third limb.

    Returns:
        list: List of angles (in radians) for manipulator joins.
    """

    phi = math.atan2(y, x)
    xw = x - l3 * math.cos(phi)
    yw = y - l3 * math.sin(phi)
    r = xw ** 2 + yw ** 2
    beta = math.acos((l1 ** 2 + l2 ** 2 - r) / (2 * l1 * l2))
    gamma = math.acos((r + l1 ** 2 - l2 ** 2) / (2 * (r ** 0.5) * l1))
    alpha = math.atan2(yw, xw)

    r1 = alpha - gamma
    r2 = math.pi - beta
    r3 = phi - r1 - r2

    return [r1, r2, r3]


def rotation_3d(axis, theta):
    """
    Return the rotation matrix associated with counterclockwise rotation about
    the given axis by theta radians in 3D space.

    Args:
        axis (list): 3D array. Axis of rotation.
        theta (float): Angle of rotation in radians.

    Returns:
        list: 3x3 rotation matrix.
    """
    axis = np.asarray(axis)
    axis = axis / math.sqrt(np.dot(axis, axis))
    a = math.cos(theta / 2.0)
    b, c, d = -axis * math.sin(theta / 2)
    aa, bb, cc, dd = a * a, b * b, c * c, d * d
    bc, ad, ac, ab, bd, cd = b * c, a * d, a * c, a * b, b * d, c * d
    return np.array([[aa + bb - cc - dd, 2 * (bc + ad), 2 * (bd - ac)],
                     [2 * (bc - ad), aa + cc - bb - dd, 2 * (cd + ab)],
                     [2 * (bd + ac), 2 * (cd - ab), aa + dd - bb - cc]])


def read_lines(connection):
    """
    Reads all incoming data until connection is closed. Returns data after finished.

    Args:
        connection: Socket's connection object.

    Returns:
        str: Accepted data string.
    """

    buffer = b''

    pre, separator, post = buffer.partition(b'\n')
    if separator:
        buffer = post
        return pre + separator

    while True:
        data = connection.recv(1024)
        if not data:
            return None

        pre, separator, post = data.partition(b'\n')
        if not separator:
            buffer += data
        else:
            data = buffer + pre + separator
            buffer = post
            return data


def handle_incoming_line(data):
    """
    Splits data string, received from the server, to list of value.

    Args:
        data: Data string in format:
        `<rotation_x> <rotation_y> <rotation_z> <translation_x> <translation_y> <translation_z>`.

    Returns:
        list: List of rotations and translations.
    """

    return list(map(lambda x: float(x), data.split(b' ')))


def timer(dt, f):
    """
    Calls function `f` every `dt` milliseconds.

    Args:
        dt: Time in Milliseconds
        f: Function to be called.
    """

    now = int(round(time.time() * 1000))
    past = now
    while True:
        now = int(round(time.time() * 1000))
        if now > past + dt:
            past = now
            f()


counter = 0


def main():
    graphics = GraphicsThread()
    graphics.start()

    while True:
        connection, address = sock.accept()
        line = read_lines(connection)

        data = handle_incoming_line(line)

        if len(data) != 6:
            continue

        rotation = [data[0], data[1], data[2]]
        translation = [data[3], data[4], data[5]]

        axis_x = [1, 0, 0]
        axis_y = [0, 1, 0]
        axis_z = [0, 0, 1]

        translation = rotation_3d(axis_x, rotation[0]).dot(translation)
        translation = rotation_3d(axis_y, rotation[1]).dot(translation)
        translation = rotation_3d(axis_z, rotation[2]).dot(translation)

        graphics.main_window.draw_rectangle(translation[0], translation[1])

        radians = solve(translation[0], translation[1], 10, 10, 10)
        degrees = list(map(lambda x: x * (180 / math.pi), radians))
        data_string = ' '.join(list(map(lambda x: str(x), degrees)))

        serial_port.write(data_string)


if __name__ == '__main__':
    main()
