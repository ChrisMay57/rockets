import picamera
import smbus
import time
import struct

filename = "log.txt"
logData = True 

mybus = smbus.SMBus(1)

"""
1 byte read write from: http://blog.oscarliang.net/raspberry-pi-arduino-connected-i2c/
"""
def testAddress(address):
	try: 
		writeNumber(1, address) 
	except: 
		return False 
	time.sleep(0.25)
	number = readNumber(address)

	if number == 1: 
		return True 
	return False 

"""
	Write a specific number to an address
"""
def writeNumber(value, address):
	mybus.write_byte(address, value)
	return 

"""
	Read a byte (number) from an address 
"""
def readNumber(address):
	number = mybus.read_byte(address)
	return number

"""
	Finds all connected i2c addresses
"""	
def scan_i2c():
	connected_i2c = [] 
	cur_address = 0
	for jj in xrange(0, 120): 
		if(testAddress(jj)): 
			connected_i2c.append(jj)

	print "connected i2c: "
	print connected_i2c
	return connected_i2c 

"""
Read a packet 
"""
def readPacket(address):
    data = ''
    length = int(mybus.read_byte(address));
    print 'length of data: ' + str(length)
    for ii in xrange(length):
    	try: 
        	data += str(mybus.read_byte(address))+ ' / ';
       	except: 
       		return data 

    data_arr = []

    for jj in xrange(5):
     	ByteArray = data2[jj*4:(jj+1)*4]
     	b = ''.join(chr(i) for i in ByteArray)
     	print struct.unpack('f',b)
 
     #print data
     #print b
     #print data2
    return data 

"""
	Communication constants. 
"""
rescan_rate = 10 
data_count = 0
cam_num = 0

if __name__ == "__main__":
	devices = scan_i2c() 
	print devices

	# loop infinitely to get data
	# reset the log file
	f = open('log.txt', 'r+')
	f.truncate()
	f.close()

	with open("log.txt", "a") as log:
		log.write("**** BEGINNING OF FILE ****")
		while(True):
			# loop through each arduino
			if(data_count % rescan_rate == 0): 
				devices = scan_i2c() # rescan at end of 10 cycles
									  # put them into test mode 
				# with picamera.PiCamera() as camera:
				# 	# take a picture
    			# 	camera.start_preview()
    			# 	time.sleep(5)
    			# 	camera.capture('/home/pi/Desktop/image%s.jpg' % (cam_num))
    			# 	camera.stop_preview()
    			# 	cam_num += 1

			for item in devices: 
				# which arduino are we looking for 
				try: 
					print 'reading to %s' % (item)
					log.write("Reading from Arduino on port: %i \n" % (item))
					dataBack = readPacket(item)
					log.write(dataBack + "\n")
				except:
					devices = scan_i2c()  # lost an arduino = rescan
				# sleep a bit 
				time.sleep(0.5)
			# sleep a bit
			time.sleep(2)
