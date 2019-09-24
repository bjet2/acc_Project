# My first python program. Reads data from Arduino and parses it into numerial data
# Used with Arduino program Processing_accel


"""
	Program to read data supplied by Arduino over serial port
	Arduino is connected to ADXL345 accelerometer via R2C.
	 
	Arduino will send data as strings with the following form
	
	ax,ay,az,time_in_millis
	
"""

# create serial object called arduinoSerialData
import serial										# Serial library
import math											# Math library
#from visual import *								# vpython library
arduinoSerialData = serial.Serial('/dev/ttyACM0',115200)	

t_i = 0.0		

at_beginning = False								# used to clear buffers of old data 
													# will be set to True if "0.0,0.0,0.0,0.0" string is received

for i in range(0,10000):								# for loop to limit time program runs
		
	myData = "0.0,0.0,0.0,0.0"						# initialize data string
													
	while (arduinoSerialData.inWaiting()==0):		# do nothing ...wait for data from Arduino (threading here??)
		pass										
	
	data_valid = True								# data will be tested. If data is not expected set to False and data is ignored
	myData = arduinoSerialData.readline()			# get the data string
	
	try:
		myData = myData.decode('utf-8')				# not an issue in Python2 but in Python3 this decode must be done
													# see https://stackoverflow.com/questions/31529421/weird-output-value-bvalue-r-n-python-serial-read
	except ValueError:
		data_valid = False
	else:
		myData = myData.strip()						# strip all non-character and white spaces from right side of myData
		data = myData.split(",")					# Split data using "," as delimiter
		if( len(data) != 4):						# check to see if 4 pieces of data were received
			data_valid = False
		else:
			ax = 0.0									# convert data string into floats
			ay = 0.0
			az = 0.0
			t = 0.0
			
			try:										# check to see if data is valid
				ax = float(data[0])
			except ValueError:
				data_valid = False
				
			try:
				ay = float(data[1])
			except ValueError:
				data_valid = False
				
			try:
				az = float(data[2])
			except ValueError:
				data_valid = False
				
			try:
				t = float(data[3])
			except ValueError:
				data_valid = False	
			
			if (t==t_i):											# check to see if data is in fact new
				#print('"  "  "')									# " " " added to monitor to indicate data is duplicate
				data_valid = False									# maxiumum data rate seems to be 100 Hz
			t_i=t
						
			if(data_valid == True and at_beginning == False and ax==0.0 and ay == 0.0 and az ==0.0 and t==0.0):
				at_beginning = True
				print("Start of new data found ....")


		if (data_valid == True and at_beginning == True) :
			angle_in_xy_plane = -10.0
			#print("\n a =",ax/255*9.8," ",ay/255*9.8," ",az/255*9.8)			# display acceleration ax in N/kg as test
																				# data not calibrated here
																				# this will be done on arduino side
			mag_a = ((ax/255*9.8)**2+(ay/255*9.8)**2+(az/255*9.8)**2)**(0.5)	# magnitude for angle calculations
			if (mag_a != 0):
				angle_from_z_axis= math.acos((az/255*9.8)/mag_a)
				mag_a_xy_plane = ((ax/255*9.8)**2+(ay/255*9.8)**2)**(0.5)		# magnitude in x-y plane only
				
				if (mag_a_xy_plane == 0):
					angle_in_xy_plane = 0										# actually undefined but this will do
				else:
					if(ax==0):
						if(ay>0):
							angle_in_xy_plane = 3.141592654
						else:
							angle_in_xy_plane = -3.141592654
					else:
						angle_in_xy_plane = math.atan(ay/ax)
			
				
				#print(" axy mag =",mag_a_xy_plane, " angle in x-y plane", angle_in_xy_plane*180/3.141592654)
				if (mag_a > 10.3 or mag_a<9.3):
					print("G = ", mag_a, " angle from vertical", angle_from_z_axis*180/3.141592654,"Moving")		# success! 9.8 to -9.8 for all three axiesprint("movement")
				else:
					print("G = ", mag_a, " angle from vertical", angle_from_z_axis*180/3.141592654)		# success! 9.8 to -9.8 for all three axiesprint("movement")

			
																	
arduinoSerialData.close()

