set output 'graphs/mapeo1.png'
set xlabel "Speed"
set ylabel "Pulses"
plot 'graphs/mapeo3_simple.txt' using 1:2 with lines title "Motor 1" linewidth 1, 'graphs/mapeo3_simple.txt' using 1:3 with lines title "Motor 2" linewidth 1
plot 'graphs/mapeo3.txt' using 1:2 with lines title "Motor 1"
