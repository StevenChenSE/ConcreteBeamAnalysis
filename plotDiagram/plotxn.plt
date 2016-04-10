set term png size 1024,768
set output 'xn.png'
set xlabel "xn(m)"
set ylabel "M(N*m)"
plot 'xn.dat' with lines
