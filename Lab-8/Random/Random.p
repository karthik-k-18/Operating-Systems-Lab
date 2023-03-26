set title "Page Replacement Algorithm using Random Policy"
set xlabel "Number of Pages Frames"
set ylabel "Page Faults"
set terminal png
set output "Random5.png"
set xrange [0:80]
set yrange [1:60]
plot "Random.dat" using 1:2 with linespoints title "Random"