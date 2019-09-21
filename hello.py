# My first python program. Reads data from Arduino and parses it into numerial data
# Used with Arduino program Processing_accel
# This program was originally used with Processing language
#    before I got to frustrated with its unexpected quirks
#Working on statup problem ....
# ???

"""
	Program to read data supplied by Arduino over serial port
	Arduino is connected to ADXL345 accelerometer via R2C.
	 
	Arduino will send data as strings with the following form
	
	ax,ay,az,time_in_millis
	
"""

# create serial object called arduinoSerialData
import serial										# Serial library
arduinoSerialData = serial.Serial('/dev/ttyACM0',9600)			

for i in range(0,100):							# for loop to limit time program runs
	
												# do nothing ...wait for data from Arduino (threading here??)
	while (arduinoSerialData.inWaiting()==0):
		pass										
	
	
	myData = arduinoSerialData.readline()		# get the data string
	print(myData)								# print the string
	myData = myData.decode('utf-8')				# not an issue in Python2 but in Python3 this decode must be done
												# see https://stackoverflow.com/questions/31529421/weird-output-value-bvalue-r-n-python-serial-read
	myData = myData.strip()						# strip all non-character and white spaces from right side of myData
	print(myData)								# display stripped data 
	data = myData.split(",")					# Split data using "," as delimiter
	print(data)									# Print the split data
	ax = 0.0									# convert data string into float
	ax = float(data[0])
	print(ax)									# display acceleration ax as test

