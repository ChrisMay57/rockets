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
	mybus.write_byte(address, 1)
	time.sleep(1)
	number = mybus.read_byte_data(address)
	if number == 1: 
		return true 
	return false 


"""
	Finds all connected i2c addresses
"""	
def scan_i2c():
	connected_i2c = [] 
	cur_address = 0
	for jj in xrange(cur_address, 120): 
		if(testAddress(jj)): 
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