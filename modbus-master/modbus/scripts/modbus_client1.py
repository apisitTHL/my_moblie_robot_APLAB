#!/usr/bin/env python

import rospy
from modbus.modbus_wrapper_client import ModbusWrapperClient 
from std_msgs.msg import Int32MultiArray as HoldingRegister
from geometry_msgs.msg import Twist


NUM_REGISTERS = 2
ADDRESS_READ_START = 0
ADDRESS_WRITE_START = 10

if __name__=="__main__":
    rospy.init_node("modbus_client")
    rospy.loginfo("""
    This file shows the usage of the Modbus Wrapper Client.
    To see the read registers of the modbus server use: rostopic echo /modbus_wrapper/input
    To see sent something to the modbus use a publisher on the topic /modbus_wrapper/output
    This file contains a sample publisher.
    """)
    host = "192.168.11.5"
    port = 502
    if rospy.has_param("~ip"):
        host =  rospy.get_param("~ip")
    else:
        rospy.loginfo("For not using the default IP %s, add an arg e.g.: '_ip:=\"192.168.0.199\"'",host)
    if rospy.has_param("~port"):
        port =  rospy.get_param("~port")
    else:
        rospy.loginfo("For not using the default port %d, add an arg e.g.: '_port:=1234'",port)
    # setup modbus client    
    modclient = ModbusWrapperClient(host,port=port,rate=50,reset_registers=False,sub_topic="modbus_wrapper/output",pub_topic="modbus_wrapper/input")
    modclient.setReadingRegisters(ADDRESS_READ_START,NUM_REGISTERS)
    modclient.setWritingRegisters(ADDRESS_WRITE_START,NUM_REGISTERS)
    rospy.loginfo("Setup complete")
    
    # start listening to modbus and publish changes to the rostopic
    modclient.startListening()
    rospy.loginfo("Listener started")
    
    #################
    # Example 1
    # Sets an individual register using the python interface, which can automatically be reset, if a timeout is given.
    register = 7
    value = 956
    timeout = 5
    modclient.setOutput(register,value,timeout)
    rospy.loginfo("Set and individual output")
    #################
    
    
    
    #################
    # Example 2
    # Create a listener that show us a message if anything on the readable modbus registers change
    rospy.loginfo("All done. Listening to inputs... Terminate by Ctrl+c")
    def showUpdatedRegisters(msg):
        rospy.loginfo("Modbus server registers have been updated:")
	x = (msg.linear.x)*10000   
	z = (msg.angular.z)*10000 
	
	if x > 0:
	   modclient.setOutput(1,1) #booleen
	   modclient.setOutput(2,0) #booleen
	   modclient.setOutput(3,x) #linear
	if x == 0:
	   modclient.setOutput(1,0) #booleen
	   modclient.setOutput(2,0) #booleen
	   modclient.setOutput(3,0) #linear

	if z > 0:
	   modclient.setOutput(4,1) #booleen
	   modclient.setOutput(5,0) #booleen
	   modclient.setOutput(6,z) #angular

	if z == 0:
	   modclient.setOutput(4,0) #booleen
	   modclient.setOutput(5,0) #booleen
	   modclient.setOutput(6,0) #angula

	if x < 0:
	   x = x*-1
	   modclient.setOutput(1,0) #booleen
	   modclient.setOutput(2,1) #booleen
	   modclient.setOutput(3,x) #linear

	if z < 0:
	   z = z*-1
	   modclient.setOutput(4,0) #booleen
	   modclient.setOutput(5,1) #booleen
	   modclient.setOutput(6,z) #angular
	

    sub = rospy.Subscriber("/cmd_vel",Twist,showUpdatedRegisters,queue_size=500)

    

    #################
    
    #################
    # Example 3
    # writing to modbus registers using a standard ros publisher
    pub = rospy.Publisher("modbus_wrapper/output",HoldingRegister,queue_size=500)
    output = HoldingRegister()
    output.data = xrange(0,1)

    
    rospy.loginfo("Sending arrays to the modbus server")


    
    while not rospy.is_shutdown():
        rospy.sleep(1)
        pub.publish(output)
	modclient.setOutput(register,value,timeout)
    #################
    
    # Stops the listener on the modbus
    modclient.stopListening()
    
    
   
    
    
