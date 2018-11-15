import socket


class Networking:
    def __init__(self, address, port):
        self.address = address
        self.port = port

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind((self.address, self.port))
        self.sock.listen(1)

        self.connection, self.client_address = self.sock.accept()

    def __close__(self):
        self.sock.close()

    def receive(self):
        buffer = b''

        while True:
            try:
                data = self.connection.recv(1024)
            except OSError:
                self.connection, self.client_address = self.sock.accept()

            if not data:
                return None

            pre, separator, post = data.partition(b'\r\n')
            if not separator:
                buffer += data
            else:
                data = buffer + pre + separator
                return data
