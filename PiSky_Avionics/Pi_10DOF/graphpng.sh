set terminal png
set output "10dofgraph.png"
set title "10 DOF Sensor Data"
set xlabel "Time (s)"
set datafile separator ","
plot "log.csv" using 2:3, "log.csv" using 2:4, "log.csv" using 2:5 