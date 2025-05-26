# Set the output file format and name
set terminal png
set output 'plot.png'

# Set the title and labels for the plot
set title 'SIR Status Over Time'
set xlabel 'Days'
set ylabel 'Count(Log scale)'

# Define the data file and format
datafile = 'simulation/single_population_results.csv'
set datafile separator ','

# Set the y-axis to logarithmic scale
set logscale y

# Plot data from columns 1, 2, 3, and 4
plot datafile using 1:3 with lines title 'Susceptible ',\
     datafile using 1:4 with lines title 'Infected', \
     datafile using 1:5 with lines title 'Recovered', \
     datafile using 1:6 with lines title 'Vaccinated'