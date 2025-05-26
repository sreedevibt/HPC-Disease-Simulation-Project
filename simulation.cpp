#include "simulation.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <numeric>
#include <fstream>
#include <cmath>
#include <unordered_map>



// Utility function to generate random numbers
static int getRandomNumber(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}


// ----- Population Implementation -----
Population::Population(const std::string& name, int size, double vaccinationRate) : name(name) {
    int vaccinatedCount = static_cast<int>(size * vaccinationRate);
    for (int i = 0; i < size; ++i) {
        if (i < vaccinatedCount)
            individuals.emplace_back(State::Vaccinated);
        else{
            individuals.emplace_back(State::Susceptible);
    }
}
}
void Population::initializeInfection() {
    int index = getRandomNumber(0, individuals.size() - 1);
   individuals[index].state = State::Infectious;
     individuals[index].infectionDuration = 0;
    
}

int Population::countByState(State state) const{
    
    int NumOfStates = 0;

    for(const Person& p: this->individuals){
        if(p.state == state){
            NumOfStates++;
        }
    }

    return NumOfStates;
}




void Population::simulateDay(int diseaseDuration) {
    const double transmissibility = 0.15; // Fixed transmissibility for the entire project
    std::vector<int> newInfections; // Track newly infected individuals

    for (size_t i = 0; i < individuals.size(); ++i) {
        if (individuals[i].state == State::Infectious) {
             

             
            // Infectious individual contacts 5 random people
            for (int j = 0; j < 5; ++j) {
                int contactIndex = getRandomNumber(0, individuals.size() - 1);

                //Skip vaccinated individuals
                if (individuals[contactIndex].state == State::Vaccinated) {
                    continue; // Vaccinated individuals do not get infected
                }

                // Infect susceptible individuals probabilistically
                if (individuals[contactIndex].state == State::Susceptible) {
                    double randomChance = static_cast<double>(getRandomNumber(0, 100)) / 100.0;
                    if (randomChance < transmissibility) {
                        newInfections.push_back(contactIndex);
                    }
                }
            }
            
            
            // Update infection duration and recover individuals after disease duration
            individuals[i].infectionDuration++;
            if (individuals[i].infectionDuration >= diseaseDuration) {
                individuals[i].state = State::Recovered;

            }
        }
    }

    // Apply new infections at the end of the day
    for (int index : newInfections) {

         if (individuals[index].state == State::Susceptible) {
            individuals[index].state = State::Infectious;
            individuals[index].infectionDuration = 0; // Initialize infection duration
        }
    }
}



// ----- Simulation Implementation -----
Simulation::Simulation(const std::vector<Population>& pops, int diseaseDuration, double transmissibility)
    : populations(pops), diseaseDuration(diseaseDuration), transmissibility(transmissibility), dayCount(0) {}

void Simulation::simulateInterPopulationContacts() {
    if (populations.size() < 2) return;

    int idx1 = getRandomNumber(0, populations.size() - 1);
    int idx2;
    do {
        idx2 = getRandomNumber(0, populations.size() - 1);
    } while (idx2 == idx1);

    Population& pop1 = populations[idx1];
    Population& pop2 = populations[idx2];

    int contactCount = static_cast<int>(0.05 * std::min(pop1.individuals.size(), pop2.individuals.size()));
    for (int i = 0; i < contactCount; ++i) {
        int person1 = getRandomNumber(0, pop1.individuals.size() - 1);
        int person2 = getRandomNumber(0, pop2.individuals.size() - 1);

        if (pop1.individuals[person1].state == State::Infectious &&
            pop2.individuals[person2].state == State::Susceptible) {
            pop2.individuals[person2].state = State::Infectious;
        }
    }
}
double Simulation::calculateStandardDeviation(const std::vector<double>& data, double mean) const {
    double sum = 0.0;
    for (double value : data) {
        sum += (value - mean) * (value - mean);
    }
    return std::sqrt(sum / data.size());
}

void Simulation::runMultipleSimulations(int runs) {
     std::cout << "\nRunning " << runs << " simulation runs...\n";
   
 // Generate unique filenames for each run
        // Data storage for total values for each population across runs
    std::unordered_map<std::string, std::vector<int>> totalSusceptible;
    std::unordered_map<std::string, std::vector<int>> totalRecovered;
    std::unordered_map<std::string, std::vector<int>> totalVaccinated;


    for (int i = 0; i < runs; ++i) {
        std::string detailsFilename = "disease_details_run_" + std::to_string(i + 1) + ".csv";
        std::string statsFilename = "disease_stats_run_" + std::to_string(i) + ".csv";
        
        for (auto& population : populations) {
            
            population.initializeInfection();
        }
        // Run the simulation
        start(detailsFilename);
         writeSummaryStatistics(statsFilename);
//Total for each population
for (const auto& population : populations) {
            totalSusceptible[population.name].push_back(population.countByState(State::Susceptible));
            totalRecovered[population.name].push_back(population.countByState(State::Recovered));
            totalVaccinated[population.name].push_back(population.countByState(State::Vaccinated));
        }

          std::cout << "Run #" << i << " completed. Results saved.\n";

        
    }




    // Calculate and print average and standard deviation for each property
    auto calculateStats = [](const std::vector<int>& data) {
        double mean = std::accumulate(data.begin(), data.end(), 0.0) / data.size();
        double variance = 0.0;
        for (int value : data) {
            variance += (value - mean) * (value - mean);
        }
        variance /= data.size();
        return std::make_pair(mean, std::sqrt(variance));
    };

    std::cout << "\nSimulation Statistics Across " << runs << " Runs:\n";
    std::cout << "--------------------------------------------------\n";
    std::cout << "Population       | Property       | Mean          | Std Dev\n";
    std::cout << "--------------------------------------------------\n";

    // Print stats for each population
    for (const auto& [name, values] : totalSusceptible) {
        auto [mean, stdDev] = calculateStats(values);
        std::cout << name << "       | Susceptible    | " << mean << "       | " << stdDev << "\n";
    }
    for (const auto& [name, values] : totalRecovered) {
        auto [mean, stdDev] = calculateStats(values);
        std::cout << name << "       | Recovered      | " << mean << "       | " << stdDev << "\n";
    }
    for (const auto& [name, values] : totalVaccinated) {
        auto [mean, stdDev] = calculateStats(values);
        std::cout << name << "       | Vaccinated     | " << mean << "       | " << stdDev << "\n";
    }

    std::cout << "--------------------------------------------------\n";
}

    


void Simulation::writeSummaryStatistics(const std::string& statsFilename) {
    std::ofstream statsFile(statsFilename); // Open the file
    if (!statsFile.is_open()) {
        std::cerr << "Error: Could not open file " << "disease_stats.csv" << " for writing summary statistics.\n";
        return;
    }

    // Write the header
    statsFile << "Total Days,Total Susceptible,Total Recovered,Total Vaccinated\n";

    // Initialize counters for global statistics
    int totalSusceptible = 0, totalRecovered = 0, totalVaccinated = 0;

    // Aggregate statistics across all populations
    for (const auto& pop : populations) {
        totalSusceptible += pop.countByState(State::Susceptible);
        totalRecovered += pop.countByState(State::Recovered);
        totalVaccinated += pop.countByState(State::Vaccinated);
    }

    // Write the aggregated statistics
    statsFile << dayCount << ","
              << totalSusceptible << ","
              << totalRecovered << ","
              << totalVaccinated << "\n";

    statsFile.close(); // Close the file
    std::cout << "Summary statistics written to " << statsFilename << "\n";
}
void Simulation::start(const std::string& detailsFilename) {
    bool hasInfectious = true;

    // Open a CSV file to store daily results
    std::ofstream outputFile(detailsFilename);
    outputFile << "Day,Population,Susceptible,Infectious,Recovered,Vaccinated\n";

    while (hasInfectious) {
        dayCount++;
        hasInfectious = false;

                

         

        for (auto& pop : populations) {
            pop.simulateDay(3);

int infectious = pop.countByState(State::Infectious);
            int recovered = pop.countByState(State::Recovered);
            int susceptible = pop.countByState(State::Susceptible);
            int vaccinated = pop.countByState(State::Vaccinated);

             
            // Write results to the CSV file
            outputFile << dayCount << ","
                        << pop.name << ","
                        << susceptible << ","
                        << infectious << ","
                        << recovered << ","
                        << vaccinated << "\n";
 
            // Write detailed results to disease_details.csv
            
            // Check if any infectious individuals remain
            if (pop.countByState(State::Infectious) > 0) {
                
                hasInfectious = true;
            }
        }

        simulateInterPopulationContacts();
    }

    outputFile.close();

 //writeSummaryStatistics(statsFilename);

    // Print summary to the terminal
    std::cout << "\nSimulation Results:\n";
    std::cout << "Total Days: " << dayCount << "\n";
    for (const auto& pop : populations) {
        std::cout << "Population: " << pop.name << "\n";
        std::cout << "  Susceptible: " << pop.countByState(State::Susceptible) << "\n";
        std::cout << "  Recovered: " << pop.countByState(State::Recovered) << "\n";
        std::cout << "  Vaccinated: " << pop.countByState(State::Vaccinated) << "\n";
    }
    std::cout << "Results saved to"<<  detailsFilename << "'.\n";
    
    
}

void Simulation::startSinglePopulationExperiment() {
    bool hasInfectious = true;  // To track if there are still infectious individuals

    // Open CSV file for saving the results
    std::ofstream outputFile("single_population_results.csv");
    outputFile << "Day,Population,Susceptible,Infectious,Recovered,Vaccinated\n";

    // Day-by-day simulation loop
    while (hasInfectious) {
        dayCount++;
        hasInfectious = false;

        for (auto& pop : populations) {
            std::vector<int> newInfections;  // Track newly infected indices

            // Process each individual
            for (size_t i = 0; i < pop.individuals.size(); ++i) {
                if (pop.individuals[i].state == State::Infectious) {
                    // Infect 5 random susceptible individuals
                    for (int j = 0; j < ceil(5 * 0.15); ++j) {
                        int contactIndex = getRandomNumber(0, pop.individuals.size() - 1);
                        if (pop.individuals[contactIndex].state == State::Susceptible) {
                            newInfections.push_back(contactIndex);
                        }
                    }

                    // Update infection duration
                    pop.individuals[i].infectionDuration++;

                    if (pop.individuals[i].infectionDuration >= diseaseDuration) {
                        pop.individuals[i].state = State::Recovered;
                    }

                   for( int n : newInfections){
                        //pop.individuals[n].state = State::Recovered
                        if (pop.individuals[n].infectionDuration >= diseaseDuration) {
                            pop.individuals[n].state = State::Recovered;
                        }
                        else{
                            pop.individuals[n].infectionDuration++;
                        }
                    }
            }}

           // Apply new infections
            for (int index : newInfections) {
                pop.individuals[index].state = State::Infectious;
            }
            

            // Check if there are still infectious individuals
            if (pop.countByState(State::Infectious) > 0) {
                hasInfectious = true;
                std::cout << pop.countByState(State::Infectious) << std::endl;
            }

            // Write daily stats to the CSV file
            outputFile << dayCount << ","
                       << pop.name << ","
                       << pop.countByState(State::Susceptible) << ","
                       << pop.countByState(State::Infectious) << ","
                       << pop.countByState(State::Recovered) << ","
                       << pop.countByState(State::Vaccinated) << "\n";
        }
    }

    outputFile.close();

    // Print summary results to terminal
    std::cout << "\nSingle Population Experiment Results:\n";
    std::cout << "Total Days: " << dayCount << "\n";
    for (const auto& pop : populations) {
        std::cout << "Population: " << pop.name << "\n";
        std::cout << "Infectious" << pop.countByState(State::Infectious) << std::endl;
        std::cout << "  Susceptible: " << pop.countByState(State::Susceptible) << "\n";
        std::cout << "  Recovered: " << pop.countByState(State::Recovered) << "\n";
        std::cout << "  Vaccinated: " << pop.countByState(State::Vaccinated) << "\n";
    }
    //std::cout << "Results saved to 'single_population_results.csv'.\n";
}
