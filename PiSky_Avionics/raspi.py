import smbus
import time
import struct
import csv
import subprocess

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
#def writeData(log, data):
#	log.write("Arduino Data:")
#	for ii in xrange(len(data)): 
#		log.write(ii + ": " + data[ii])
#	log.write("\n")

# ping rate for data
rescan_rate = 1000
data_count = 0

def writeCSV(data,CSVfile):
        writer = csv.writer(CSVfile)
        writer.writerows(data)
        
        
        


if __name__ == "__main__":
	print 'start'
	Start_Time  = time.time()
	devices = scan_i2c()

	# loop infinitely to get data
	#f = open('log.txt', 'r+')
	#f.truncate()
	#f.close()
	CSVfile = open('log.csv', 'w+')
	CSVfile.truncate() #wipes file every time script is run
	CSVfile.close()
        
	with open("log.txt", "a") as log:
		#with open("log.csv", "a") as CSVlog:
		
		log.write("**** BEGINNING OF FILE ****")
		
		CSVlog = open('log.csv', 'a+')
		subprocess.call('gnuplot', '-p','graphrealtime.sh')
		
		while(True):
			data_count += 1
			# loop through each arduino
			if(data_count % rescan_rate == 0): 
				devices = scan_i2c() # rescan at end of 10 cycles
									 # put them into test mode 
				CSVlog.close() #close for plotting
				subprocess.call('gnuplot', 'graphpng.sh') #replot png file every cycle
				CSVlog = open('log.csv', 'a+')	#reopen for appending data

				for item in devices: 
					# which arduino are we looking for 
					try: 
						print 'reading to [%s]' % (item)
						log.write("Reading from Arduino on port: %i \n" % (item))
						data_back = readPacket(item)
						data_line = "%i," % (item)
						data_line += str(time.time() - Start_Time) + ","
				
						for ii in xrange(len(data_back)):
							data_line += str(data_back[ii]) + ","
				
						data_line += "\n"
						log.write(data_line)
						CSVlog.write(data_line)
					except:
						devices = scan_i2c()  # lost an arduino = rescan
					# sleep a bit 
					time.sleep(0.05)
