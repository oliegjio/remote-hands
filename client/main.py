import socket
import math
import numpy as np


sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
sock.bind(('10.42.0.1', 7247))
sock.listen(1)


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


def receive(connection):
    """
    Reads all incoming data until connection is closed. Returns data after finished.

    Args:
        connection: Socket's connection object.

    Returns:
        str: Accepted data string.
    """

    buffer = b''

    pre, separator, post = buffer.partition(b'\r\n')
    if separator:
        buffer = post
        return pre + separator

    while True:
        data = connection.recv(1024)

        if not data:
            return None

        pre, separator, post = data.partition(b'\r\n')
        if not separator:
            buffer += data
        else:
            data = buffer + pre + separator
            buffer = post
            return data


def debug_lists(lists):
    for l in lists:
        r = list(map(lambda x: round(x, 2), l))
        print(r)
    print()


def main():
    connection, address = sock.accept()

    while True:
        try:
            data = receive(connection)
        except OSError:
            connection, address = sock.accept()

        # Splits line, received from the server, to the list of value:
        transforms = list(map(lambda x: float(x), data.split(b' ')))
        # This values are: rotations (x, y, z) in degrees, translations (x, y, z).

        if len(transforms) != 6:
            continue

        translations = [transforms[0], transforms[1], transforms[2]]
        rotations = [transforms[3], transforms[4], transforms[5]]
        rotations = list(map(lambda x: x * (math.pi / 180), rotations))  # Convert degrees to radians.

        axis_x = [1, 0, 0]
        axis_y = [0, 1, 0]
        axis_z = [0, 0, 1]

        # Transform local translations to global ones:
        translations = rotation_3d(axis_x, rotations[0]).dot(translations)
        translations = rotation_3d(axis_y, rotations[1]).dot(translations)
        translations = rotation_3d(axis_z, rotations[2]).dot(translations)

        # Solve kinematics (angles in radians):
        angles = solve(transforms[0], transforms[1], 10, 10, 10)

        debug_lists([rotations, translations, angles])


if __name__ == '__main__':
    main()
