import socket
import math


sock = socket.socket()
sock.bind(('localhost', 8000))
sock.listen(1)


def solve(x, y, l1, l2, l3):
    """ Solve kinematic equations for 3 DOF planar arm. """

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


def read_lines_forever(connection):
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


def main():
    while True:
        """ Message format: `<r1> <r2> <r3> <t1> <t2> <t3> <b>`. """

        connection, address = sock.accept()
        line = read_lines_forever(connection)

        data = list(map(lambda x: float(x), line.split(b' ')))

        radians = solve(data[0], data[1], 10, 10, 10)
        degrees = list(map(lambda x: x * (180 / math.pi), radians))

        print(degrees)


if __name__ == '__main__':
    main()
