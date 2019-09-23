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
arduinoSerialData = serial.Serial('/dev/ttyACM0',9600)			

at_beginning = False								# used to clear buffers of old data 
													# will be set to True if "0.0,0.0,0.0,0.0" string is received

for i in range(0,1000):								# for loop to limit time program runs
		
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
						
			if(data_valid == True and at_beginning == False and ax==0.0 and ay == 0.0 and az ==0.0 and t==0.0):
				at_beginning = True
				print("Start of new data found ....")
		
		if (data_valid == True and at_beginning == True) :
			#print(ax/255," ",ay/255," ",az/255," ",t)				# display acceleration ax in N/kg as test
																				# data not calibrated here
																				# this will be done on arduino side
			mag_a = ((ax/255*9.8)**2+(ay/255*9.8)**2+(az/255*9.8)**2)**(0.5)	# test to see if calibration is close
			print(mag_a)														# success! 9.8 to -9.8 for all three axies
																	
arduinoSerialData.close()

