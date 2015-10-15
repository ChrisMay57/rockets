import smbus
import time

filename = "log.txt"
logData = True 

"""
	Manages i2c communication with arduino.  
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
	Finds all connected arduinos
"""	
def scan_i2c():
	arduinos = [] 
	return arduinos 

"""
	Write data to file (not to i2c). 
"""
def writeData(log, data):
	log.write("Arduino Data:")
	for ii in xrange(len(data)): 
		log.write(ii + ": " + data[ii])
	log.write("\n")

if __name__ == "__main__":
	arduinos = scan_i2c() 
	pi2c = I2C(arduinos)

	with open(filename, "a") as log:
    	while logData:
			currentData = pi2c.gatherData()
			writeData(log, currentData)
			time.sleep(50 / 1000) # 50 milliseconds