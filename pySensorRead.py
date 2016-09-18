# pyserial read from arduino sensor values
import serial
import serial.tools.list_ports
from firebase import firebase

#GLOBAL VARIABLES
angle = 0
rpm = 0
port_to_use = ''


def parseSerial(line):
	global angle, rpm
	line = str(line)[1:-2]
	vals = line.split(',')

	#in the case of a bad packet, just pass so that the script doesn't crash
	try:
		angle = float(vals[0])
		rpm = float(vals[1])
	except:
		pass

#print out the available ports for debugging purposes
ports = [i.device for i in list(serial.tools.list_ports.comports())]
port_to_use = ports[-1] #default

print "Available ports:"

#find the port with usbmodem in the name, and make that the active port
for i in  ports:
	print i
	if 'usbmodem' in i:
		port_to_use = i
print ""

#create the serial device, and open the port
ser = serial.Serial(port_to_use, 9600)

while True:
	#parse the line, storing it to global variables
	parseSerial(ser.readline())
	print angle, rpm



#uploading stuff to firebase!!!!
firebase = firebase.FirebaseApplication('https://hackmitproject-78bca.firebaseio.com/sensor.json', None)
new_user = 'Ozgur Vatansever'

while True:
    result_angle = firebase.post('/sensor', data={"angle":angle, "rpm":rpm} , params={'print': 'pretty'})
