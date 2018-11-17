# import serial
from graphics import *
from kinematics import *
from mathematics import *
from networking import *
from utils import *
from pyquaternion import Quaternion


''' SERIAL CODE BACKUP:
serial_port = serial.Serial('/dev/ttyUSB0', 9600)
output = ' '.join(list(map(lambda x: str(x), angles)))
serial_port.write(output)
'''


def main():
    graphics = Graphics(800, 600)
    graphics.start()
    networking = Networking('10.42.0.1', 7247)

    translations = [0, 0, 0]
    rotations = [0, 0, 0]

    while True:
        data = networking.receive()

        if data is None:
            continue

        # Splits line, received from the server, to the list of value:
        transforms = list(map(lambda x: float(x), data.split(b' ')))
        # This values are: rotations (x, y, z) in degrees, translations (x, y, z).

        if len(transforms) != 6:
            continue

        new_translations = [transforms[0], transforms[1], transforms[2]]
        new_rotations = [transforms[3], transforms[4], transforms[5]]
        new_rotations = list(map(lambda x: x * (math.pi / 180), new_rotations))  # Convert degrees to radians.

        for i in range(3):
            rotations[i] += new_rotations[i]

        axis_x = [1, 0, 0]
        axis_y = [0, 1, 0]
        axis_z = [0, 0, 1]

        qx = Quaternion(axis=axis_x, angle=rotations[0])
        qy = Quaternion(axis=axis_y, angle=rotations[1])
        qz = Quaternion(axis=axis_z, angle=rotations[2])

        qq = qx * qy * qz

        new_translations = qq.rotate(new_translations)

        # x y z
        # z x y
        # z y x

        # Transform local translations to global ones:
        # new_translations = rotation_3d(axis_y, rotations[0]).dot(new_translations)
        # new_translations = rotation_3d(axis_z, rotations[1]).dot(new_translations)
        # new_translations = rotation_3d(axis_x, rotations[2]).dot(new_translations)

        # Save translations:
        for i in range(3):
            translations[i] += new_translations[i]

        # Solve kinematics (angles in radians):
        try:
            angles = solve_3dof(translations[0], translations[1], 10, 10, 10)
        except ValueError:
            pass

        debug_lists([new_rotations, new_translations, angles])
        graphics.window.clear_canvas()
        graphics.window.draw_square(translations[0], translations[1])


if __name__ == '__main__':
    main()
