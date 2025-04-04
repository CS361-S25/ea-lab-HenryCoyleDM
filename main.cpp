#include <iostream>
#include "Empirical/include/emp/math/Random.hpp"

class Organism {
    public: double behavior;
    public:
    Organism(double behavior) {
        this->behavior = behavior;
    }

    public:
    Organism* reproduce(emp::Random random) {
        Organism *offspring = new Organism(behavior);
        offspring->mutate(random); // Mutate the offspring's behavior
        return offspring;
    }

    public:
    void mutate(emp::Random random) {
        behavior += random.GetRandNormal(0, 0.02); // Mutate behavior with a Gaussian distribution
    }
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    emp::vector<Organism> population;
    for (int i = 0; i < 100; ++i) {
        population.push_back(*new Organism(0.5)); // Example behavior value
    }
    emp::Random random = *new emp::Random(1); // Initialize random number generator
    Organism first_organism = population[0];
    std::cout << "Example organism: " << first_organism.behavior << std::endl;
    Organism second_organism = *first_organism.reproduce(random);
    std::cout << "Reproduced organism behavior: " << second_organism.behavior << std::endl;
    int number_of_generations = 100;
    for (int i = 0; i < number_of_generations; i++) {
        // simulate a generation by finding the best organism, reproducing it, and killing the worst organism
        int worst_index = 0;
        int best_index = 0;
        for (int j = 1; j < population.size(); j++) {
            if (population[j].behavior < population[worst_index].behavior) {
                worst_index = j; // find the worst organism
            }
            if (population[j].behavior > population[best_index].behavior) {
                best_index = j; // find the best organism
            }
        }
        Organism offspring = *population[best_index].reproduce(random); // reproduce the best organism
        population[worst_index] = offspring; // replace the worst organism with the new offspring
    }
    // print the fitness of the best organism after 100 generations
    double best_behavior = population[0].behavior;
    for (int i = 1; i < population.size(); i++) {
        if (population[i].behavior > best_behavior) {
            best_behavior = population[i].behavior;
        }
    }
    std::cout << "Best organism behavior after " << number_of_generations << " generations: " << best_behavior << std::endl;
}