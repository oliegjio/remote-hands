import serial
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
    serial_port = serial.Serial('/dev/ttyUSB1', 9600)
    graphics = Graphics(800, 600)
    graphics.start()
    networking = Networking('10.42.0.1', 7247)
    networking.start()

    translation = [0, 0, 0]

    while True:
        data = networking.receive()

        if data is None:
            continue

        # Splits line, received from the server, to the list of value:
        transforms = list(map(lambda x: float(x), data.split(b' ')))
        # This values are: quaternion (w, x, y, z), translations (x, y, z).

        if len(transforms) != 7:
            continue

        quaternion = [transforms[0], transforms[1], transforms[2], transforms[3]]
        acceleration = [transforms[4], transforms[5], transforms[6]]

        q = Quaternion(*quaternion)

        # new_translations = q.inverse.rotate(new_translations)
        acceleration = q.rotate(acceleration)
        acceleration[2] -= 1

        for i, t in enumerate(acceleration):
            if t <= 0.1:
                acceleration[i] = 0

        # Save translations:
        for i in range(3):
            translation[i] += acceleration[i]

        # Solve kinematics (angles in radians):
        try:
            angles = solve_3dof(translation[0], translation[1], 10, 10, 10)
        except ValueError:
            pass

        debug_lists([quaternion, acceleration, angles])

        graphics.window.clear_canvas()
        graphics.window.draw_square(translation[0] * 5, translation[1] * 5)

        output = ' '.join(list(map(lambda x: str(x * (180 / math.pi)), angles))) + '\n'
        serial_port.write(output)


if __name__ == '__main__':
    main()
