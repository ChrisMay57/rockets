import serial
import time 

if __name__ == "__main__":
	while(True): 
		try: 
			ser = serial.Serial(0) # 1st port 
			print ser.readLine() 
		except: 
			print 'Serial connection failure.'
			time.sleep(0.05)
			pass 