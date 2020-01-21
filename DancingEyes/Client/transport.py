import socket

class Transport(object):
    def __init__(self, addr, fake = False):
        self.s = socket.socket()
        self.addr = addr
        self.fake = fake
        
    def send(self, cmd):
        print("Send: {}".format(cmd))
        if self.fake: return True
        
        for i in range(1, 3):
            try:
                b = bytes(cmd, 'utf-8')
                self.s.sendall(b)
                return True
            except socket.error as msg:
                if i == 2:
                    print("sendall error: {}".format(msg))
                self.s.connect((self.addr, 80))
                
        return False
        
    def recv(self):
        if self.fake: return ''
        str = []
        while True:
            try:
                s = self.s.recv(1).decode('utf-8')
                str.append(s)
                if '\n' in s:
                    break
            except:
                print("Recv failed")
                break
        return ''.join(str).replace('\r', '').replace('\n', '')

    def send_cmd(self, cmd):
        self.send(cmd + "\r")
        self.recv()        
        
    def move(self, cmd, points):
        addComma = False
        for p in points:
            if addComma:
                cmd += ","
            cmd += "{}".format(p)
            addComma = True
        self.send(cmd + "\r")
        self.recv()
        
    def left(self, points):
        self.move("L:", points)

    def right(self, points):
        self.move("R:", points)

    def eyes(self, points):
        self.left (points)
        self.right(points)
