import smbus
import time
import struct

filename = "log.txt"
logData = True 

mybus = smbus.SMBus(1)

"""
	Manages i2c communication with arduinos.  
"""
class I2C: 
	def __init__(self, addresses):
		self.addresses = addresses # sets Pi's arduino addresses to result of search 
		return 

	def writeData(self, value, address):
		mybus.write_byte(address, value)
		return -1
	
	def readData(self, address):
		number = mybus.ead_byte(address)
		return number

	def gatherData(self): 
		data = []
		for add in addresses: 
			data.append(readData(add))
		return data # this is indexed by the Arduino 

"""
1 byte read write from: http://blog.oscarliang.net/raspberry-pi-arduino-connected-i2c/
"""
def testAddress(address):
	try: 
		writeNumber(1, address) 
	except: 
		return False 
	time.sleep(1)
	number = readNumber(address)

	if number == 1: 
		return True 
	return False 

"""
	Write a specific number to an address
"""
def writeNumber(value, address):
	mybus.write_byte(address, value)
	# bus.write_byte_data(address, 0, value)
	return 

"""
	Read a byte (number) from an address 
"""
def readNumber(address):
	number = mybus.read_byte(address)
	# number = bus.read_byte_data(address, 1)
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

	print "connected i2c:"
	print connected_i2c

	return connected_i2c 

def readPacket(address):
    data = []
    length = int(mybus.read_byte(address));
    print 'length of data: ' + str(length)
    for ii in xrange(length):
    	try: 
        	k = mybus.read_byte(address)
		data.append(k)
       	except: 
       		break
    
    data_arr = []
    for jj in xrange(5):
    	ByteArray = data[jj*4:(jj+1)*4]
    	b = ''.join(chr(i) for i in ByteArray)
    	f = struct.unpack('f',b)
    	s = float(str(f)[1:-2])
    	data_arr.append(s)

    return data_arr

"""
	Write data to file (not to i2c). 
"""
def writeData(log, data):
	log.write("Arduino Data:")
	for ii in xrange(len(data)): 
		log.write(ii + ": " + data[ii])
	log.write("\n")

# ping rate for data
rescan_rate = 10
data_count = 0

if __name__ == "__main__":
	devices = scan_i2c()

	# loop infinitely to get data
	f = open('log.txt', 'r+')
	f.truncate()
	f.close()

	with open("log.txt", "a") as log:
		#log.write("**** BEGINNING OF FILE ****")
		while(True):
			data_count = data_count + 1
			# loop through each arduino
			if(data_count % rescan_rate == 0): 
				devices = scan_i2c() # rescan at end of 10 cycles
									  # put them into test mode 
    
			for item in devices: 
				# which arduino are we looking for 
				try: 
					print 'reading to [%s]' % (item)
					log.write("Reading from Arduino on port: %i \n" % (item))
					data_back = readPacket(item)
					print data_back, len(data_back)
					for ii in xrange(len(data_back)):
						print data_back[ii]
						log.write(data_back[ii])
						log.write(",")
						log.write("\n")
				except:
					print 'fail'
					devices = scan_i2c()  # lost an arduino = rescan
				# sleep a bit 
				time.sleep(0.5)