import time, math, sys, msvcrt, random
from transport import Transport

kDontChange = -1000

def constrain(x, min, max):
    if x < min: return min
    if x > max: return max
    return x

class Eyes(object):
    def __init__(self, addr):
        self.tr = Transport(addr)
        self.lids = 0
        self.left_x  = self.left_y  = self.left_lids  = self.left_offset  = 0
        self.right_x = self.right_y = self.right_lids = self.right_offset = 0

    def Left1(self, x, y, lids=kDontChange, offset=kDontChange):
        if lids   == kDontChange: lids   = self.left_lids
        if offset == kDontChange: offset = self.left_offset
        self.tr.send_cmd("L:{},{},{},{}".format(int(x), int(y), int(lids), int(offset)))

    def Right1(self, x, y, lids=kDontChange, offset=kDontChange):
        if lids   == kDontChange: lids   = self.right_lids
        if offset == kDontChange: offset = self.right_offset
        self.tr.send_cmd("R:{},{},{},{}".format(int(x), int(y), int(lids), int(offset)))

    def Left(self, x, y, lids=kDontChange, offset=kDontChange):
        self.left_x = x
        self.left_y = y
        if lids   != kDontChange: self.left_lids   = lids
        if offset != kDontChange: self.left_offset = offset
        self.tr.send_cmd("L:{},{},{},{}".format(int(x), int(y), int(self.left_lids), int(self.left_offset)))

    def Right(self, x, y, lids=kDontChange, offset=kDontChange):
        self.right_x = x
        self.right_y = y
        if lids   != kDontChange: self.right_lids   = lids
        if offset != kDontChange: self.right_offset = offset
        self.tr.send_cmd("R:{},{},{},{}".format(int(x), int(y), int(self.right_lids), int(self.right_offset)))

    def Both(self, x, y, lids=kDontChange):
        if lids != kDontChange: 
            self.lids = lids
            self.left_lids  = lids
            self.right_lids = lids
        self.Left (x, y, self.left_lids,  y/2)
        self.Right(x, y, self.right_lids, y/2)

    def Lids(self, left_lids, right_lids):
        left_lids  = constrain(left_lids,  0, 100)
        right_lids = constrain(right_lids, 0, 100)
        self.Left (self.left_x,  self.left_y,  left_lids )
        self.Right(self.right_x, self.right_y, right_lids)

    def Blink(self, delay=0.1):
        left_x = self.left_x
        left_y = self.left_y
        left_lids   = self.left_lids
        left_offset = self.left_offset

        right_x = self.right_x
        right_y = self.right_y
        right_lids   = self.right_lids
        right_offset = self.right_offset

        self.Both(0, 0, 0)
        time.sleep(delay)
        self.Left (left_x,  left_y,  left_lids,  left_offset )
        self.Right(right_x, right_y, right_lids, right_offset)

    def Move(self, x, y, x_sync, y_sync, lids=kDontChange):
        x2 = x if x_sync else -x
        y2 = y if y_sync else -y
        eyes.Left (x,   y, lids, y /2)
        eyes.Right(x2, y2, lids, y2/2)
        time.sleep(0.01)
        
    def Move1(self, x, y, lids=kDontChange):
        self.Move(x, y, True, True, lids)

    def Rotate(self, start, stop, step, x_sync, y_sync, lids=kDontChange):
        for i in range(start, stop, step):
            a = 3.14/180*i
            x = math.cos(a) * 100
            y = math.sin(a) * 90
            self.Move(x, y, x_sync, y_sync, lids)

    def HalfCircle_Move(self, step, sync=True, lids=kDontChange):
        if step > 0:
            self.Rotate(0, 181, step, sync, True, lids)
        else:
            self.Rotate(180, -1, step, sync, True, lids)

    def FullCircle_Move(self, step, sync=True, lids=kDontChange):
        if step > 0:
            self.Rotate(0, 361, step, sync, True, lids)
        else:
            self.Rotate(360, -1, step, sync, True, lids)

    def QuarterCircle_Move(self, step, sync=True, lids=kDontChange):
        if step > 0:
            self.Rotate(90, 181, step, sync, True, lids)
            self.Rotate(180, 89, -step, sync, True, lids)
        else:
            self.Rotate(90, -1, step, sync, True, lids)
            self.Rotate(0, 91, -step, sync, True, lids)
            
    def Side_Move(self, step, sync=True, lids=kDontChange):
        for i in range(0, 101, abs(step)):
            x = i if step > 0 else -i
            self.Move(x, 0, sync, sync, lids)
        for i in range(100, -1, -abs(step)):
            x = i if step > 0 else -i
            self.Move(x, 0, sync, sync, lids)

    def Vertical_Move(self, step, sync=True):
        for i in range(0, 101, abs(step)):
            y = i if step > 0 else -i
            self.Move(0, y, sync, sync, self.lids)
        for i in range(100, -1, -abs(step)):
            y = i if step > 0 else -i
            self.Move(0, y, sync, sync, self.lids)
        
    def CheckSides(self, step, sync=True, lids=kDontChange):
        self.Side_Move(step, sync, lids)
        self.Side_Move(-step, sync, lids)

    def CheckVerticals(self, step, sync=True):
        self.Vertical_Move(step, sync)
        self.Vertical_Move(-step, sync)

    def Bit_Move(self, step, x=0, max_y=70):
        for y in range(max_y, -max_y, -step):
            self.Move1(x, y, self.lids)
        for y in range(-max_y, max_y, step):
            self.Move1(x, y, self.lids)

    def Bit_NervousX(self):
        k = 1.0
        for i in range(40, 5, -8):
            self.Left1 (self.left_x -i, self.left_y )
            self.Right1(self.right_x-i, self.right_y)
            time.sleep(0.04 / k)
            self.Left1 (self.left_x +i, self.left_y )
            self.Right1(self.right_x+i, self.right_y)
            time.sleep(0.04 / k)
            k += 0.1

    def Bit_NervousY(self):
        k = 1.0
        for i in range(40, 5, -8):
            self.Left1 (self.left_x , self.left_y -i, kDontChange, (self.left_y -i)/2)
            self.Right1(self.right_x, self.right_y-i, kDontChange, (self.right_y-i)/2)
            time.sleep(0.04 / k)
            self.Left1 (self.left_x , self.left_y +i, kDontChange, (self.left_y +i)/2)
            self.Right1(self.right_x, self.right_y+i, kDontChange, (self.right_y +i)/2)
            time.sleep(0.04 / k)
            k += 0.1
            
    def Bit_Random(self):
        x = random.randint(-20, 20)
        y = random.randint(-20, 20)
        self.Left1 (self.left_x +x, self.left_y +y)
        self.Right1(self.right_x+x, self.right_y+y)

    def Wave_Move(self, step):
        for i in range(-100, 100, abs(step)):
            a1 = 3.14/100*i
            a2 = a1 + 3.14/2
            lids1 = math.sin(a1)*70
            lids2 = math.sin(a2)*70
            x = i if step > 0 else -i
            self.Left (x, 0, lids1 if lids1 > 0 else 10)
            self.Right(x, 0, lids2 if lids2 > 0 else 10)
            time.sleep(0.01)
        self.Both(0, 0, 0)
    
    def OpenClose_Move(self, step, delay=1):
        for lids in range(0, 90, step):
            self.Move1(-100, 0, lids)
            time.sleep(delay)
            self.Move1(100, -0, lids)
            time.sleep(delay)
        for lids in range(100, -1, -step):
            self.Move1(-100, 0, lids)
            time.sleep(delay)
            self.Move1(100, -0, lids)
            time.sleep(delay)
    
    def Start(self, delay=0.5, lids=70):
        self.Both(0, 0, 0)
        time.sleep(2*delay)
        self.Left(0, 0, lids)
        time.sleep(2*delay)
        self.Left(-100, 0, lids)
        time.sleep(delay)
        self.Left(100, 0, lids)
        time.sleep(delay)
        self.Left(0, 0, lids)
        time.sleep(delay)
        
        self.Both(0, 0, 0)
        time.sleep(2*delay)
        self.Right(0, 0, lids)
        time.sleep(2*delay)
        self.Right(-100, 0, lids)
        time.sleep(delay)
        self.Right(100, 0, lids)
        time.sleep(delay)
        self.Right(0, 0, lids)
        time.sleep(delay)
        self.Right(0, 0, 0)
        
    def ApplauseThanks(self, lids=50):
        self.Move1(0, 0, lids)
        time.sleep(0.5)
        self.Move1(70, -70, lids)
        time.sleep(1)
        self.Move1(50, 10, lids)
        time.sleep(0.5)
        self.Move1(0, -100, lids)
        time.sleep(1)
        self.Move1(-50, 10, lids)
        time.sleep(0.5)
        self.Move1(-70, -70, lids)
        time.sleep(1)
        
eyes = Eyes("192.168.1.100")
eyes.Both(0, 0, 50)

while True:
    c = msvcrt.getch()
    print(c)
    if c == b'`': break
    
    elif c == b']' : eyes.Lids(eyes.left_lids+25, eyes.right_lids+25)
    elif c == b'\\': eyes.Lids(eyes.left_lids-25, eyes.right_lids-25)
    
    elif c == b's': eyes.Side_Move(30)
    elif c == b'S': eyes.Side_Move(30, False)
    elif c == b'd': eyes.Side_Move(-30)
    elif c == b'D': eyes.Side_Move(-30, False)
    
    elif c == b'a': eyes.CheckSides(30)
    elif c == b'A': eyes.CheckSides(30, False)
    elif c == b'f': eyes.CheckSides(-30)
    elif c == b'F': eyes.CheckSides(-30, False)

    elif c == b'd': eyes.Side_Move(-30)
    elif c == b'D': eyes.Side_Move(30)
    
    elif c == b'b': eyes.Bit_Move(10)
    
    elif c == b'n': eyes.Bit_NervousX()
    elif c == b'N': eyes.Bit_NervousY()
    
    elif c == b'm': eyes.Bit_Random()
    
    elif c == b' ': eyes.Blink()
    elif c == b'f': eyes.FullCircle_Move(10)
    
    elif c == b'w': eyes.QuarterCircle_Move(-10)
    elif c == b'W': eyes.QuarterCircle_Move(-10, False)
    elif c == b'e': eyes.QuarterCircle_Move(10)
    elif c == b'E': eyes.QuarterCircle_Move(10, False)
    elif c == b'q': eyes.HalfCircle_Move(-10)
    elif c == b'Q': eyes.HalfCircle_Move(-10, False)
    elif c == b'r': eyes.HalfCircle_Move(10)
    elif c == b'R': eyes.HalfCircle_Move(10, False)
    
    elif c == b'r': eyes.ApplauseThanks()
    elif c == b't': eyes.Start()
    
    elif c == b'x': eyes.Wave_Move(10)
    
    elif c == b'4': eyes.Both(100, 0)
    elif c == b'6': eyes.Both(-100, 0)
    elif c == b'7': eyes.Both(50, 50)
    elif c == b'9': eyes.Both(-50, 50)
    elif c == b'8': eyes.Both(0, 90)
    elif c == b'1': eyes.Both(50, -50)
    elif c == b'2': eyes.Both(0, -90)
    elif c == b'3': eyes.Both(-50, -50)
    elif c == b'5': eyes.Both(0, 0)
    
    elif c == b'\r': eyes.Both(0, 0, 50)

