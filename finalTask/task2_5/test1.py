import serial
import sys
import pty
import os
import time
import select
from time import ctime
from time import sleep
import threading  #multi thread working mode

#send & recv msg using mkpty port
def Vpsend(slaveName,master): #slaveName is the parameter from mkpty()
    print "\n"
    print " Open the port.Please Waiting...\n"
    sleep(1) 
    ser=serial.Serial(slaveName,9600,timeout=0.5) #pass parameter:slaveName
    print "Open port successful! the port information:"
    print ser
    print "\n"
    while ser.isOpen(): #the return is True or Faulse
        print "please write the msg(exit to break)"
        msg=raw_input()

        #add a break reason:::kill the child thread
        if msg == 'exit':
            print "\n"
            print "Please waiting to close the connection......"
            sleep(1)
            break;  

        msg=msg + '\r' + '\n'  #AT form define

        #data=ser.write(msg)
    os.write(master, msg)
    sys.stdout.flush()  #flush the buffer
    print "\n"
    print ("waiting to recv the data...")
    i=0
    for i in range(600):
    	msg_recv = os.read(master,20)
    	print ("\n")
    	print ("%s"%(msg_recv))
	sleep(1)
#create only one virtual port
def mkpty():
    #make pair of pseudo tty
    master, slave = pty.openpty()
    slaveName = os.ttyname(slave)

    print "slave device names:", slaveName

    #set the Vpsend() as the child thread and set the 'slaveName' as pass parameter, Vpsend will use 'slaveName'
    t=threading.Thread(target=Vpsend,args=(slaveName,master))
    t.start()   #start this thread
    while True:
        if t.is_alive() == False:  #when the child thread killed, then this father thead break...
            break
    
mkpty()

