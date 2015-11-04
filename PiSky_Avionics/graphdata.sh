set terminal png
set output "10dofgraph.png"
set title "10 DOF Sensor Data"
set xlabel "Time (s)"
set ylabel "Temp (degrees)"
set datafile separator ","
plot "log.csv" using 2:7
set term x11