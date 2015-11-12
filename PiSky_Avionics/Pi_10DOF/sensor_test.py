from tendof import * 
import subprocess
import csv

S = Unified_Sensor()

if __name__ == "__main__":
	CSVfile = open('log.csv', 'w+')
	CSVfile.truncate() # wipes file every time script is run
	CSVfile.close()

	with open("log.csv", "a+") as CSVlog: 
		while(True):
			time.sleep(0.1)
			xyz = S.get_xyz()
			temp = S.get_temperature()
			pressure = S.get_pressure()
			lsm = S.get_lsm()
			data_line = ""
			data_line += "7, " + str(xyz)[1:-1] + ", " + str(temp) + ", " + str(pressure) + "," + str(lsm).replace("(","").replace(")","")[1:-1	]
			data_line += "\n"
			print data_line

			CSVlog.close() # close for plotting 
			subprocess.call('gnuplot','graphpng.sh') 
			CSVlog = open('log.csv', 'a+')	# reopen for appending data 
			# CSVlog.write(data_line) # write to csv 
			# implement above line after testing 