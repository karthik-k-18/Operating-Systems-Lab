set title "Page Replacement Algorithm using LRU Policy"
set xlabel "Number of Pages Frames"
set ylabel "Page Faults"
set terminal png
set output "LRU5.png"
set xrange [1:80]
set yrange [1:60]
plot "LRU.dat" using 1:2 with linespoints title "LRU"