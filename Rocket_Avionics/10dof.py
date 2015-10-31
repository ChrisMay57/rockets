from L3GD20 import L3GD20
import smbus
import time

dt = 0.02
x = 0
y = 0
z = 0

mybus = smbus.SMBus(1)

s = L3GD20(busId = 1, slaveAddr = 0x6b, ifLog = False, ifWriteBlock=False)
s.Set_PowerMode("Normal")
s.Set_FullScale_Value("250dps")
s.Set_AxisX_Enabled(True)
s.Set_AxisY_Enabled(True)
s.Set_AxisZ_Enabled(True)
s.Init()
s.Calibrate()

sensor = BMP085.BMP085()
lsm = Adafruit_LSM303()


while(1):
	print 'Sensor 1'
	print 'Temp = {0:0.2f} *C'.format(sensor.read_temperature())
	print 'Pressure = {0:0.2f} Pa'.format(sensor.read_pressure())
	print 'Altitude = {0:0.2f} m'.format(sensor.read_altitude())
	print 'Sealevel Pressure = {0:0.2f} Pa'.format(sensor.read_sealevel_pressure())

	print 'Sensor 2'
	print lsm.read()

	print 'Sensor 3'
	dxyz = s.Get_CalOut_Value()
	x += dxyz[0]*dt;
	y += dxyz[1]*dt;
	z += dxyz[2]*dt;
	print("{:7.2f} {:7.2f} {:7.2f}".format(x, y, z))

	sleep(1)