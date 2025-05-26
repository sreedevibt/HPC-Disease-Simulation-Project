

#include "simulation.h"
#include "INIReader.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    bool singlePopulationExperiment = false;
    
    // Check for single population experiment flag in the command line
    if (argc > 1 && std::string(argv[1]) == "--single-population") {
        singlePopulationExperiment = true;
    }

    if (singlePopulationExperiment) {
        // Single Population Experiment parameters
        std::string populationName = "SinglePopulation";
        int populationSize = 15000;
        double vaccinationRate = 0.1;   // No vaccinated individuals initially
        int diseaseDuration = 3;            // Disease duration of 3 days
        double transmissibility = 0.15; // Transmissibility of 0.15

        // Initialize the single population
        Population singlePopulation(populationName, populationSize, vaccinationRate);
        singlePopulation.initializeInfection();  // Start with one infectious person

        // Initialize the simulation with only one population
        std::vector<Population> populations = {singlePopulation};
        Simulation sim(populations, diseaseDuration, transmissibility);

        // Start the simulation and record the results
        sim.startSinglePopulationExperiment();

        std::cout << "Single Population experiment completed. Results saved to 'single_population_results.csv'.\n";
    } 
    
    else 
    
    {
        // Multi Population simulation from configuration file
        INIReader reader("disease_in.ini");
        if (reader.ParseError() < 0) {
            std::cerr << "Can't load configuration file.\n";
            return 1;
        }

        int numPopulations = reader.GetInteger("global", "num_populations", 1);
        int diseaseDuration = reader.GetInteger("disease", "duration", 3);
        double transmissibility = reader.GetReal("disease", "transmissibility", 0.15);
        int simulationRuns = reader.GetInteger("global", "simulation_runs", 3); 


        std::vector<Population> populations;
        for (int i = 1; i <= numPopulations; ++i) {
            std::string section = "population_" + std::to_string(i);
            std::string name = reader.Get(section, "name", "Unknown");
            int size = reader.GetInteger(section, "size", 100);
            double vaccinationRate = reader.GetReal(section, "vaccination_rate", 0.0);

            Population pop(name, size, vaccinationRate);
            pop.initializeInfection();  // Start with one infectious person
            populations.push_back(pop);
        }

        // Initialize the simulation with multiple populations
        Simulation sim(populations, diseaseDuration, transmissibility);

         if (simulationRuns > 1) {
    sim.runMultipleSimulations(simulationRuns);

    std::cout << "Multiple simulation runs completed.\n";
} else {
     std::string detailsFilename = "disease_details.csv";
    std::string statsFilename = "disease_stats.csv";
    sim.start(detailsFilename);
    sim.writeSummaryStatistics(statsFilename);
}

    }
    return 0;
}
