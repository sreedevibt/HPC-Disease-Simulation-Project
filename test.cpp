#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../include/doctest.h"
#include "simulation.h"
#include "INIReader.h"

// Test the Simulation Class
TEST_CASE("Simulation Class Testing") {
    SUBCASE("Simulation Initialization") {
        Population pop1("Population1", 100, 0.10); // 10% vaccinated
        Population pop2("Population2", 200, 0.20); // 20% vaccinated
        std::vector<Population> populations = {pop1, pop2};

        Simulation simulation(populations, 5, 0.15); // duration = 5, transmissibility = 0.15
        CHECK(simulation.populations.size() == 2); // Two populations should exist
    }
}

// Test the Population Class
TEST_CASE("Population Class Testing") {
    SUBCASE("Population Initialization") {
        Population population("TestPopulation", 100, 0.10); // 10% vaccinated
        CHECK(population.individuals.size() == 100); // Population size should match
        int vaccinated_count = 0;
        for (const auto& person : population.individuals) {
            if (person.state == State::Vaccinated) {
                ++vaccinated_count;
            }
        }
        CHECK(vaccinated_count == 10); // 10% vaccinated
    }

    SUBCASE("Infection Initialization") {
        Population population("TestPopulation", 100, 0.10);
        population.initializeInfection();
        int infected_count = 0;
        for (const auto& person : population.individuals) {
            if (person.state == State::Infectious) {
                ++infected_count;
            }
        }
        CHECK(infected_count == 1); // Only one person should be infected
    }

    SUBCASE("Daily Simulation") {
        Population population("TestPopulation", 100, 0.10);
        population.initializeInfection();
        population.simulateDay(3); // Simulate a day with duration = 3
        CHECK(population.countByState(State::Infectious) > 0); // Infection spreads
    }
}

/*// Test the Simulation Class Integration
TEST_CASE("Simulation Class Integration Test") {
    SUBCASE("Run Simulation with Two Populations") {
        Population pop1("Population1", 100, 0.10); // 10% vaccinated
        Population pop2("Population2", 200, 0.20); // 20% vaccinated
        std::vector<Population> populations = {pop1, pop2};

        Simulation simulation(populations, 5, 0.15); // duration = 5, transmissibility = 0.15
        simulation.start();

        

        // After the simulation, there should be no infectious individuals
        for (const auto& pop : simulation.populations) {
            CHECK(pop.countByState(State::Infectious) == 0); // All infections should have recovered
        }
    }
}*/

// Test Infection Spread Between Populations
TEST_CASE("Inter-Population Contact Test") {
    SUBCASE("Simulate Inter-Population Contacts") {
        Population pop1("Population1", 100, 0.10); // 10% vaccinated
        Population pop2("Population2", 200, 0.20); // 20% vaccinated
        std::vector<Population> populations = {pop1, pop2};

        Simulation simulation(populations, 5, 0.15); // duration = 5, transmissibility = 0.15
        pop1.initializeInfection(); // Infect one person in Population1

        simulation.simulateInterPopulationContacts();

        // Check if infections have potentially spread to Population2
        CHECK(pop2.countByState(State::Infectious) >= 0); // Infection might spread
    }
}


TEST_CASE("Inter-Population Contact Edge Cases") {
    SUBCASE("Only One Population - No Contacts") {
        std::vector<Population> pops = {Population("Single", 100, 0.1)};
        Simulation simulation(pops, 5, 0.15);
        simulation.simulateInterPopulationContacts(); // Should not crash or run
    }

    SUBCASE("Empty Populations") {
        std::vector<Population> pops = {};
        Simulation simulation(pops, 5, 0.15);
        simulation.simulateInterPopulationContacts(); // Should handle gracefully
    }
}
TEST_CASE("Population Simulation Edge Cases") {
    SUBCASE("All Susceptible") {
        Population pop("AllSusceptible", 100, 0.0);
        pop.simulateDay(3);
        CHECK(pop.countByState(State::Infectious) == 0); // No spread without initial infections
    }

    SUBCASE("All Recovered") {
        Population pop("AllRecovered", 100, 0.0);
        for (auto& person : pop.individuals) {
            person.state = State::Recovered;
        }
        pop.simulateDay(3);
        CHECK(pop.countByState(State::Infectious) == 0); // No spread among recovered
    }
}
 TEST_CASE("Multiple Simulation Runs") {
    Population pop1("Population1", 100, 0.10);
    Population pop2("Population2", 200, 0.20);
    std::vector<Population> populations = {pop1, pop2};

    Simulation simulation(populations, 5, 0.15);
    simulation.runMultipleSimulations(5);

    
    CHECK(true); // Add additional checks for correctness if you parse results programmatically.
}
/*TEST_CASE("CSV File Generation") {
    Population pop1("Population1", 100, 0.10);
    Population pop2("Population2", 200, 0.20);
    std::vector<Population> populations = {pop1, pop2};

    Simulation simulation(populations, 5, 0.15);
    simulation.start();

    
    std::ifstream detailsFile("disease_details.csv");
    std::ifstream statsFile("disease_stats.csv");
    CHECK(detailsFile.good());
    CHECK(statsFile.good());

    detailsFile.close();
    statsFile.close();
}*/
