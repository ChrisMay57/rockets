import smbus
import time

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
	return -1

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
    data = ''
    length = int(mybus.read_byte(address));
    print 'length of data: ' + str(length)
    for ii in xrange(length):
    	try: 
        	data += str(mybus.read_byte(address))+ ' / ';
       	except: 
       		return data 
    print data
    return data 

"""
	Write data to file (not to i2c). 
"""
def writeData(log, data):
	log.write("Arduino Data:")
	for ii in xrange(len(data)): 
		log.write(ii + ": " + data[ii])
	log.write("\n")

# ping rate for data
pingRate = 50 

if __name__ == "__main__":
	arduinos = scan_i2c() 
	# pi2c = I2C(arduinos)
	# print pi2c

	# loop infinitely to get data
	f = open('log.txt', 'r+')
	f.truncate()
	f.close()

	with open("log.txt", "a") as log:
		log.write("**** BEGINNING OF FILE ****")
		while(True):
			# loop through each arduino
			for i in range(0,10):
				for item in arduinos: 
					# which arduino are we looking for 
					try: 
						print 'reading to %s' % (item)
						log.write("Reading from Arduino on port: %i \n" % (item))
						dataBack = readPacket(item)
						log.write(dataBack + "\n")
					except:
						arduinos = scan_i2c()
						#if(testAddress(item)):
						#	print "connected i2c:"
						#	print connected_i2c
					# sleep a bit 
					time.sleep(0.5)
			arduinos = scan_i2c()	
			# sleep a bit
			time.sleep(2)


	# with open(filename, "a") as log:
	# 	while logData:
	# 		for item in pi2c: 
	# 			currentData = pi2c.gatherData()
	# 			writeData(log, currentData)
	# 		time.sleep(pingRate / 1000) # 50 milliseconds