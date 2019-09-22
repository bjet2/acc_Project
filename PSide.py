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

arduinoSerialData.reset_input_buffer()					# attempts to flush out old data from serial buffer on Python side
print("data waiting = ",arduinoSerialData.inWaiting())	# attempts to flush out old data from serial buffer on Python side
														# attempts to flush out old data from serial buffer on Python side
while (arduinoSerialData.inWaiting()>0):			# attempts to flush out old data from serial buffer on Python side
		myData = arduinoSerialData.readline()
		#pass										# suspect must flush output buffer for arduino as well
at_beginning = True									# this is a poor bit of programming until I can figure out
													# how to clear serial buffers

for i in range(0,100):								# for loop to limit time program runs
	if (i>20):										# used at end of loop to discard data that is in serial buffer before start
		at_beginning=False
	
	myData = "0.0,0.0,0.0,0.0"						# initialize data string
													# do nothing ...wait for data from Arduino (threading here??)
	while (arduinoSerialData.inWaiting()==0):
		pass										
	
	data_valid = True
	myData = arduinoSerialData.readline()			# get the data string
	try:
		myData = myData.decode('utf-8')				# not an issue in Python2 but in Python3 this decode must be done
	except ValueError:
		data_valid = False
		print("Error in reading serial data: Data Point ignored")
	
													# see https://stackoverflow.com/questions/31529421/weird-output-value-bvalue-r-n-python-serial-read
	else:
		myData = myData.strip()						# strip all non-character and white spaces from right side of myData
		data = myData.split(",")					# Split data using "," as delimiter
		#print(data)								# Print the split data
		if( len(data) != 4):						# check to see if 4 pieces of data were received
			print("Error: 4 data points were not recieved Data point ignored")
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
			
			if(data_valid == False):
				print("Error: data points were convertable to floats ... Data point ignored")
			else:
				if( t > 4.0 and at_beginning == True):
					print("Serial port not flushed???? Data discarded")
					data_valid = False				
		
		if (data_valid == True) :
			print(ax," ",ay," ",az," ",t)				# display acceleration ax as test
arduinoSerialData.close()

