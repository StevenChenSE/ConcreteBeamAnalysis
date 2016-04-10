set term png size 1024,768
set output 'fai.png'
set xlabel "ø"
set ylabel "M(N*m)"
plot 'fai.dat' with lines
