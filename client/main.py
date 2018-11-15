# import serial
from graphics import *
from kinematics import *
from mathematics import *
from networking import *
from utils import *


''' SERIAL CODE BACKUP:
serial_port = serial.Serial('/dev/ttyUSB0', 9600)
output = ' '.join(list(map(lambda x: str(x), angles)))
serial_port.write(output)
'''


def main():
    graphics = Graphics(800, 600)
    networking = Networking('10.42.0.1', 7247)

    translations = [0, 0, 0]

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
        rotations = [transforms[3], transforms[4], transforms[5]]
        rotations = list(map(lambda x: x * (math.pi / 180), rotations))  # Convert degrees to radians.

        axis_x = [1, 0, 0]
        axis_y = [0, 1, 0]
        axis_z = [0, 0, 1]

        # x y z
        # z x y
        # z y x

        # Transform local translations to global ones:
        new_translations = rotation_3d(axis_y, rotations[0]).dot(new_translations)
        new_translations = rotation_3d(axis_z, rotations[1]).dot(new_translations)
        new_translations = rotation_3d(axis_x, rotations[2]).dot(new_translations)

        # Save translations:
        for i in range(len(translations)):
            translations[i] += new_translations[i]

        # Solve kinematics (angles in radians):
        angles = solve_3dof(translations[0], translations[1], 10, 10, 10)

        debug_lists([rotations, new_translations, angles])
        graphics.window.clear_canvas()
        graphics.window.draw_square(translations[0], translations[1])


if __name__ == '__main__':
    main()
