#!/usr/bin/env python3
from OSC import *
import sys
from time import sleep
import random

cc = OSCClient()
cc.connect(('192.168.12.249', 7000))   # localhost, port 57120

def click(msg,arg0,arg1):
    global cc
    oscmsg = OSCMessage()
    print "%s %d %d" % (msg,arg0,arg1)
    oscmsg.setAddress(msg)
    oscmsg.append(arg0)
    oscmsg.append(arg1)
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
    print (path, args[0], args[1], args[2])
    #click (path, args[1], args[2])    

def quit_callback(path, tags, args, source):
    # don't do this at home (or it'll quit blender)
    global run
    run = False

server.addMsgHandler( "/Hit", hit_callback )
server.addMsgHandler( "/quit", quit_callback )

# user script that's called by the game engine every frame
def each_frame():
    # clear timed_out flag
    server.timed_out = False
    # handle all pending requests then return
    while not server.timed_out:
        server.handle_request()

# simulate a "game engine"
while run:
    # do the game stuff:
    #sleep(0.01)
    # call user script
    each_frame()

click ("/Hit", random.randint(0,5120), random.randint(0,1920))
    
server.close()