import socket


class Client:
    def __init__(self, address, port):
        self.address = address
        self.port = port

        self.started = False
        self.sock = None

    def __del__(self):
        self.sock.close()

    def start(self):
        self.started = True

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

        self.sock.connect((self.address, self.port))

    def stop(self):
        self.started = False
        self.sock.close()

    def send(self, message):
        if not self.started:
            return

        try:
            if type(message) is bytes:
                self.sock.send(message)
            else:
                self.sock.send(str(message).encode(encoding='ascii'))
        except OSError:
            self.stop()
            raise OSError('Failed to send data to the server')

