# Set the output file format and name
set terminal png
set output 'plott.png'

# Set the title and labels for the plot
set title 'Recovered Count and Vaccination Rate'
set xlabel 'Vaccination Rate'
set ylabel 'Recovered Count'

# Define the data file and format
datafile = 'simulation/herdimm.csv'
set datafile separator ','
set style line 1 lc rgb '#0060ad' pt 7 ps 1.5
set grid

# Plot data from columns 1 and 2.
plot datafile using 2:1 with points title 'Line plot ',\
     datafile using 2:1 with lines title 'Recovered'
     