#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <string>

// Enumeration for the states of individuals
enum class State { Susceptible, Infectious, Vaccinated, Recovered };

// Class representing a person
class Person {
public:
    State state;             // Current state of the person
    int infectionDuration;   // Duration the person has been infectious

    // Constructor
    Person(State initState= State::Susceptible): state(initState), infectionDuration(0){};
};

// Class representing a population
class Population {
public:
    std::string name;                // Name of the population
    std::vector<Person> individuals; // List of individuals in the population
  
    // Constructor
    Population(const std::string& name, int size, double vaccinationRate);

    // Initialize one individual as infectious
    void initializeInfection();

    // Count the number of individuals in a given state
    int countByState(State state) const;

    // Simulate a single day in the population
    void simulateDay(int diseaseDuration);

    std::vector<int> newInfections;

    
};

// Class representing the entire simulation
class Simulation {
private:
    //std::vector<Population> populations; // List of populations
    int diseaseDuration;                 // Duration of the disease in days
    double transmissibility;             // Probability of disease transmission
    int dayCount;                        // Count of simulation days

     //  function to calculate standard deviation
    double calculateStandardDeviation(const std::vector<double>& data, double mean) const;
   
               
    std::vector<Person> individuals;
    
public:
std::vector<Population> populations; 
 
    // Constructor
    Simulation(const std::vector<Population>& populations, int diseaseDuration, double transmissibility);

   
void simulateInterPopulationContacts();
    // Run the simulation for multi-population experiments
    void start(const std::string& detailsFilename);

    // Run the simulation for single population experiment
    void startSinglePopulationExperiment();

    void runMultipleSimulations(int runs);
    void writeSummaryStatistics(const std::string& statsFilename); // Updated to accept a filename

     
};

#endif // SIMULATION_H
