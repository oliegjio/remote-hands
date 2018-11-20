import serial
from graphics import *
from kinematics import *
from mathematics import *
import base64
from networking import *
from utils import *
from pyquaternion import Quaternion
import time


def restrict_rotation(x):
    if x > 110:
        return 110
    elif x < -110:
        return -110
    return x


def millis():
    return int(round(time.time() * 1000))


def main():
    serial_port = serial.Serial('/dev/ttyUSB1', 9600)
    # graphics = Graphics(800, 600)
    # graphics.start()
    networking = Networking('10.42.0.1', 7247)
    networking.start()

    translation = [50, 50, 0]

    past_time = millis()

    while True:
        if millis() < past_time + 10000:
            continue

        data = networking.receive()

        if data is None:
            continue

        # Splits line, received from the server, to the list of value:
        try:
            transforms = list(map(lambda x: float(x), data.split(b' ')))
        except ValueError:
            continue
        # This values are: quaternion (w, x, y, z), translations (x, y, z).

        if len(transforms) != 7:
            continue

        quaternion = [transforms[0], transforms[1], transforms[2], transforms[3]]
        acceleration = [transforms[4], transforms[5], transforms[6]]

        q = Quaternion(*quaternion)

        acceleration = q.rotate(acceleration)  # Rotate acceleration to global basis.
        acceleration[2] -= 1  # Subtract gravity vector.

        for i, t in enumerate(acceleration):
            if t >= 0.25:
                acceleration[i] = 0.25

        scale = 10
        # canvas_scale = 10

        # Save translations:
        for i, t in enumerate(translation):
            translation[i] += acceleration[i] * scale
            if t >= 190:
                translation[i] = 190
            if t <= 60:
                translation[i] = 60
            if t <= -190:
                translation[i] = -190
            if t >= -60:
                translation[i] = -60

        # Solve kinematics (angles in radians):
        try:
            print(translation[0], translation[1])
            angles = solve_3dof(translation[0], translation[1], 94.7, 71.2, 46)
            # print("ANGLES: ", angles)
        except ValueError:
            print("Solve error")
            continue

        # graphics.window.clear_canvas()
        # graphics.window.draw_square((translation[0] / scale) * canvas_scale, (translation[1] / scale) * canvas_scale)

        output = ' '.join(list(map(lambda x: str(int(restrict_rotation(x * (180 / math.pi)))), angles))) + ' \n'
        arr = output.encode(encoding='ascii')

        # debug_lists(quaternion, acceleration, angles, arr, translation)

        serial_port.write(arr)


if __name__ == '__main__':
    main()
