from tendof import * 
import csv

S = Unified_Sensor()

if __name__ == "__main__":
	with open("log.csv", "a+") as CSVlog: 
		while(True):
			time.sleep(0.1)
			xyz = S.get_xyz()
			temp = S.get_temperature()
			lsm = S.get_lsm()
			data_line += xyz[1:-1] + "," + temp + "," + lsm
			data_line += "\n"
			print data_line

			CSVlog.close() # close for plotting 
			subprocess.call('gnuplot', 'graphpng.sh') # replot png file 
			CSVlog = open('log.csv', 'a+')	# reopen for appending data 
			# CSVlog.write(data_line) # write to csv 
			# implement above line after testing 