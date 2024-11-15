set output 'graphs/mapeo1.png'
set xlabel "Speed"
set ylabel "Pulses"
plot 'graphs/mapeo1.txt' using 1:2 with lines title "Motor 1" linewidth 1, 'graphs/mapeo1.txt' using 1:3 with lines title "Motor 2" linewidth 1
plot 'graphs/mapeo3.txt' using 1:2 with lines title "Motor 1"
