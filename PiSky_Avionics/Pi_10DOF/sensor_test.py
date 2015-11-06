from tendof import * 

S = Unified_Sensor()
while(True):
	time.sleep(0.1)
	print S.get_xyz()
	print S.get_temperature()