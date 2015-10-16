import smbus
import time

filename = "log.txt"
logData = True 

"""
	Manages i2c communication with arduinos.  
"""
class I2C: 
	def __init__(self, addresses):
		self.addresses = addresses # sets Pi's arduino addresses to result of search 
		self.bus = smbus.SMBus(1)
		return 

	def writeData(self, value, address):
		self.bus.write_byte(address, value)
		return -1
	
	def readData(self, address):
		number = self.bus.read_byte(address)
		return number

	def gatherData(self): 
		data = []
		for add in addresses: 
			data.append(readData(add))
		return data # this is indexed by the Arduino 

"""
1 byte read write from: http://blog.oscarliang.net/raspberry-pi-arduino-connected-i2c/
"""
def writeNumber(bus, address, value):
	mybus = smbus.SMBus(1)
	mybus.write_byte(address, value)
	# bus.write_byte_data(address, 0, value)
	return 
def readNumber(bus, address):
	number = bus.read_byte(address)
	# number = bus.read_byte_data(address, 1)
	return number

"""
	Finds all connected i2c addresses
"""	
def scan_i2c():
	connected_i2c = [] 
	cur_address = 0
	test_bus = smbus.SMBus(1)
	for jj in xrange(cur_address, 120): 
		try: 
			print 'sending'
			writeNumber(test_bus, int(jj), 1)
			print 'sent 1'
			time.sleep(1) # wait one second for response 
			return_data = readNumber(test_bus, jj)
			print "returned:"
			print return_data
		except: 
			continue 

		if(return_data == "1"): # test what this should be 
			connected_i2c.append(jj)

	print "connected i2c:"
	print connected_i2c

	return connected_i2c 

"""
	Write data to file (not to i2c). 
"""
def writeData(log, data):
	log.write("Arduino Data:")
	for ii in xrange(len(data)): 
		log.write(ii + ": " + data[ii])
	log.write("\n")

pingRate = 50 

if __name__ == "__main__":
	arduinos = scan_i2c() 
	pi2c = I2C(arduinos)

	# with open(filename, "a") as log:
 #    	while logData:
	# 		currentData = pi2c.gatherData()
	# 		writeData(log, currentData)
	# 		time.sleep(pingRate / 1000) # 50 milliseconds