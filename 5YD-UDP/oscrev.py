#!/usr/bin/env python3
from OSC import *
import sys
from time import sleep
import random

#         1                2              3                4                5
XY=[[32,[320,64]],   [96,[374,10]], [160,[394,-10]], [272,[398,-14]], [304,[398,-14]], #1
  [416,[398,-14]], [448,[398,-14]], [560,[398,-14]], [592,[398,-14]], [704,[398,-14]], #2
  [736,[398,-14]], [848,[398,-14]], [880,[398,-14]], [992,[398,-14]],[1024,[398,-14]], #3
 [1136,[398,-14]],[1168,[398,-14]],[1280,[398,-14]],[1312,[398,-14]],[1424,[398,-14]], #4
 [1456,[398,-14]],[1568,[398,-14]],[1600,[398,-14]],[1712,[398,-14]],[1744,[398,-14]], #5
 [1856,[394,-10]], [1920,[374,10]], [1984,[320,64]], [2112,[320,64]], [2176,[374,10]], #6
 [2240,[394,-10]],[2304,[394,-10]], [2368,[374,10]], [2432,[320,64]], [2560,[320,64]], #7
  [2624,[374,10]],[2688,[394,-10]],[2752,[394,-10]], [2816,[374,10]], [2880,[320,64]], #8
  [3008,[320,64]], [3040,[374,10]],[3136,[394,-10]],[3200,[394,-10]], [3264,[374,10]], #9
  [3328,[320,64]], [3456,[320,64]], [3520,[374,10]],[3584,[394,-10]],[3648,[394,-10]],#10
  [3712,[374,10]], [3776,[320,64]], [3904,[320,64]], [3968,[374,10]],[4032,[394,-10]],#11
 [4096,[394,-10]], [4160,[374,10]], [4224,[320,64]], [4352,[320,64]], [4416,[374,10]],#12
 [4480,[394,-10]],[4592,[398,-14]],[4624,[398,-14]],[4736,[398,-14]],[4768,[398,-14]],#13
 [4880,[398,-14]],[4912,[398,-14]],[5024,[398,-14]],[5056,[398,-14]],[5168,[398,-14]],#14
 [5200,[398,-14]],[5312,[398,-14]],[5344,[398,-14]],[5456,[398,-14]],[5488,[398,-14]],#15
 [5600,[398,-14]],[5632,[398,-14]],[5744,[398,-14]],[5776,[398,-14]],[5888,[398,-14]],#16
 [5920,[398,-14]],[6032,[398,-14]],[6064,[398,-14]],[6178,[394,-10]], [6240,[374,10]],#17
  [6304,[320,64]]]
tinyLen = XY[0][1][0] - XY[0][1][1]
tinyTop = XY[0][1][0]
smallLen = XY[1][1][0] - XY[1][1][1]
smallTop = XY[1][1][0]
midLen = XY[2][1][0] - XY[2][1][1]
midTop = XY[2][1][0]
bigLen = XY[3][1][0] - XY[3][1][1]
bigTop = XY[3][1][0]
tinyReal = 1860
smallReal = 1860
midReal = 2109
bigReal = 2209
  
cc = OSCClient()
cc.connect(('192.168.1.200', 6666))   # localhost, port 57120
dd = OSCClient()
dd.connect(('127.0.0.1', 910))   # localhost, port 57120

def over(msg,arg0):
    global cc
    oscmsg = OSCMessage()
    oscmsg.setAddress(msg)
    oscmsg.append(arg0)
    print oscmsg
    cc.send(oscmsg)

def click(msg,arg0,arg1):
    global cc
    oscmsg = OSCMessage()
    oscmsg.setAddress(msg)
    oscmsg.append(arg0)
    oscmsg.append(arg1)
    print oscmsg
    cc.send(oscmsg)
    
server = OSCServer( ("0.0.0.0", 7110) )
server.timeout = 0
run = True

# this method of reporting timeouts only works by convention
# that before calling handle_request() field .timed_out is 
# set to False
def handle_timeout(self):
    self.timed_out = True

# funny python's way to add a method to an instance of a class
import types
server.handle_timeout = types.MethodType(handle_timeout, server)

def hit_callback(path, tags, args, source):
    # which user will be determined by path:
    # we just throw away all slashes and join together what's left
    # user = ''.join(path.split("/"))
    # tags will contain 'fff'
    # args is a OSCMessage with data
    # source is where the message came from (in case you need to reply)
    print (path, args[0], args[1])
    if XY[args[0]][1][1] == -14:
        click (path, XY[args[0]][0], bigTop - (bigLen * args[1] / bigReal ))
    elif XY[args[0]][1][1] == -10:
        click (path, XY[args[0]][0], midTop - (midLen * args[1] / midReal ))            
    elif XY[args[0]][1][1] == 10:
        click (path, XY[args[0]][0], smallTop - (smallLen * args[1] / smallReal ))
    else: #XY[args[1]][1][1] == 64
        click (path, XY[args[0]][0], tinyTop - (tinyLen * args[1] / tinyReal ))

def get_callback(path, tags, args, source):
    # which user will be determined by path:
    # we just throw away all slashes and join together what's left
    # user = ''.join(path.split("/"))
    # tags will contain 'fff'
    # args is a OSCMessage with data
    # source is where the message came from (in case you need to reply)
    #/BallIn
    print (path, args[0], args[1])
    click (path, args[0], ( (220 * args[1]) >> 7 ) - 100)  #220 * Y / 128 - 100  
    
def gamemode_callback(path, tags, args, source):
    # don't do this at home (or it'll quit blender)
    #global run
    #run = False
    #/basketNumColor
    #https://gist.github.com/ndavison/6a5d97cb8a9091cffa7a
    #http://www.runoob.com/python/python-xml.html
    #https://my.oschina.net/sukai/blog/611451
    #https://pythonprogramming.net/urllib-tutorial-python-3/
    #/GameMode
    #Timer for 300 secs
    print (path, args[0])

def gameshow_callback(path, tags, args, source):
    # don't do this at home (or it'll quit blender)
    #global run
    #run = False
    print (path, args[0])

def showmode_callback(path, tags, args, source):
    # don't do this at home (or it'll quit blender)
    #global run
    #run = False
    #/ShowMode
    print (path, args[0])

def standby_callback(path, tags, args, source):
    # don't do this at home (or it'll quit blender)
    #global run
    #run = False
    #Send OSC to Max telling the basket wave dance
    #/StandbyMode
    print (path, args[0])

def winner_callback(path, tags, args, source):
    # don't do this at home (or it'll quit blender)
    #global run
    #run = False
    print (path, args[0])

server.addMsgHandler( "/Hit", hit_callback )
server.addMsgHandler( "/Get", get_callback )
server.addMsgHandler( "/GameMode", gamemode_callback )
server.addMsgHandler( "/GameShow", gameshow_callback )
server.addMsgHandler( "/ShowMode", showmode_callback )
server.addMsgHandler( "/StandbyMode", standby_callback )
server.addMsgHandler( "/GameWinner", winner_callback )

# user script that's called by the game engine every frame
def each_frame():
    # clear timed_out flag
    server.timed_out = False
    # handle all pending requests then return
    while not server.timed_out:
        server.handle_request()

#over("/GameShow", 0)
# simulate a "game engine"
while run:
    # do the game stuff:
    #sleep(0.01)
    # call user script
    each_frame()

for next in range(25, len(XY)):
    click ("/Hit", XY[next][0], 180)
    time.sleep(2)
    click ("/Hit", XY[next][0], 180)
    time.sleep(2)
    click ("/Hit", XY[next][0], 180)
    time.sleep(4)

server.close()