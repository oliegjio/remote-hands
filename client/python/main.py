import serial
from server import Server
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

    if len(transforms) != 3:
        raise ValueError("Wrong number of data values")

    angles = [transforms[0], transforms[1], transforms[2]]

    return angles


def main():
    localhost = '10.42.0.1'
    # localhost = '127.0.0.1'
    serial_port = serial.Serial('/dev/ttyUSB1', 9600)  # Connect to manipulator controller.
    server = Server(localhost, 7247)  # Connect to bracer controller.
    server.start()

    time.sleep(20)  # Wait until gyroscope calibration finished.

    while True:
        data = server.receive()

        try:
            angles = handle_data(data)  # Angles in degrees (from 0 to 360).
        except ValueError as error:
            print(error)
            continue

        output = str(' '.join(angles) + ' \n').encode(encoding='ascii')
        serial_port.write(output)


if __name__ == '__main__':
    main()
