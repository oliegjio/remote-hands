import serial
from server import Server
import time
from mathematics import smooth_map
from utils import clamp


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
        transforms = list(map(lambda x: float(x), data.strip().split(b' ')))
    except ValueError:
        raise ValueError("Error splitting data or converting values to float")

    if len(transforms) != 3:
        raise ValueError("Wrong number of data values")

    angles = [transforms[0], transforms[1], transforms[2]]

    return angles


def main():
    serial_port = serial.Serial('/dev/ttyUSB1', 9600)  # Connect to manipulator controller.
    localhost = '10.42.0.1'
    server = Server(localhost, 7247)  # Connect to bracer controller.
    server.start()

    rotation = [0, 0, 0]
    previous_angles = [0, 0, 0]
    initial_angles = [0, 0, 0]
    data_counter = 0

    time.sleep(15)  # Wait until gyroscope calibration finished.

    while True:
        data = server.receive()

        print("DATA: ", data)

        try:
            current_angles = handle_data(data)  # Angles in degrees (from 0 to 360).
            if data_counter == 5:
                initial_angles = list(current_angles)
            data_counter += 1
        except ValueError as error:
            print(error)
            continue

        if data_counter < 6:
            print("CONTINUE")
            continue

        for i in range(3):
            current_angles[i] -= initial_angles[i]
            if abs(rotation[i]) < 360:
                rotation[i] += current_angles[i] - previous_angles[i]
                rotation[i] = clamp(rotation[i], -360, 360)
            else:
                continue
        previous_angles = list(current_angles)

        result_rotation = list(rotation)
        result_rotation[0], result_rotation[2] = result_rotation[2], result_rotation[0]

        result_rotation = list(
            map(lambda x: smooth_map(x, 0, 360, 0, 110) if 0 < x < 360 else smooth_map(x, -360, 0, -110, 0),
                result_rotation))

        output = str(' '.join(list(map(lambda x: str(x), result_rotation))) + ' \n').encode(encoding='ascii')
        print("OUTPUT: ", output)

        try:
            serial_port.write(output)
        except OSError as error:
            print(error)
            continue


if __name__ == '__main__':
    main()
