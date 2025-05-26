# HPC Disease Simulation 

This project models the spread of a disease across multiple populations with a vaccination rate and fixed transmibility rate. It also generates detailed simulation statistics, performs testing with coverage reports, and visualizes data using GNUPlot.

---

## Features

- Simulates disease spread across populations with vaccination effects.
- Outputs statistics to CSV files (`disease_stats.csv`, `disease_details.csv`).
- Includes unit and integration tests with coverage reports.
- Generates data visualizations with GNUPlot.

---
## Project Structure

Below is a brief explanation of the key C++ files and their purpose in the project:

### C++ Files

- **`main.cpp`**:  
  Entry point for the simulation. It reads the configuration file (`disease_in.ini`) and initializes the simulation. It handles both single-population and multi-population simulations and supports multiple runs for statistical analysis.

- **`simulation.cpp`**:  
  Contains the implementation of the simulation logic, including:
  - Disease spread within a population.
  - Inter-population interactions.
  - Statistical computations like averages and standard deviations.
  - Writing simulation results to CSV files.

- **`simulation.h`**:  
  The header file that defines the classes and methods used in the simulation:
  - **`Person`**: Represents an individual and their infection state.
  - **`Population`**: Models a population with individuals and simulates disease spread.
  - **`Simulation`**: Manages the overall simulation across multiple populations.

- **`test.cpp`**:  
  Contains unit and integration tests for verifying the correctness of the simulation logic, using the `doctest` testing framework.

---

## Configuration File: `disease_in.ini`

The `disease_in.ini` file defines all simulation parameters.
## Prerequisites

Before running the project, ensure you have:
- **Docker** installed: [Install Docker](https://docs.docker.com/get-docker/)
- **GNUPlot** installed: [Install GNUPlot](http://www.gnuplot.info/).
---

## How to Run the Simulation

### **Step 1: Clone the Repository**

Clone the repository to your local system:

```bash
git clone https://github.com/your-repo/disease-simulation.git
cd <nameofthefolder>
```
## Step 2:Build the project

### With CMake:
```bash
rm -rf build
mkdir build && cd build
cmake ..
make
```
### Step 3 : Run the simulation
For simulation, 
```bash
./disease_simulation 
./disease_tests
```
Inside the build folder there would be two csv files which generated,  simulation details for each timestep to file disease_details.csv and  summary statistics in csv format to file disease_stats.csv. Also  Before termination, the application shall print runtime information and summary
 statistics on the terminal (stdout):– Number of timesteps until finish– Number of vaccinated, recovered and susceptiple persons


 ## Visualizing results
 After running the simulation, using plot.gp script in the root folder we can generate  the SIR status over time plot. This plot is based for a single population and single simulation run. The file is saved as single_population_results.csv here. 
 ```bash
 g++ -std=c++17 -o disease_simulation main.cpp simulation.cpp
 ./disease_simulation
 cd ..
 gnuplot plot.gp
``` 
 Further to analyze the herd immunity, an experiment been done for a single population of size 15000.
 The vaccination rate sampled from 0% to 100% in steps manually in the disease_in.ini file, for each time ,for a population with one simulation run.Transimissibility and number of days to recover from the disease being kept constant throughout and the data is saved in herdimm.csv. The plot shows the number of recovered people vs. The vaccination_rate at the end of each simulation and is given by plott.gp

 ```bash
 gnuplot plott.gp
```
 The graphs would be saved be saved in the roor folder in png format.

 ## Code Coverage Analysis
 The code coverage analysis of the project was performed using gcov, which evaluates how much of the source code is executed during tests. It can be checked with following steps

 ```bash
 cd build/CMakeFiles/disease_simulation.dir/simulation
 gcov simulation.cpp.o


 ````
 Overall Coverage: 84.86% 

 









