#include <iostream>
#include "Empirical/include/emp/math/Random.hpp"

// these organisms try to have their behvarior close to sqrt(2)
class Sqrt2Organism {
    public: double behavior;
    public:
    Sqrt2Organism(double behavior, emp::Random random) {
        this->behavior = behavior;
        // mutate organisms at start to speed up evolution
        this->mutate(random);
    }

    public:
    Sqrt2Organism* reproduce(emp::Random random) {
        Sqrt2Organism *offspring = new Sqrt2Organism(behavior, random);
        // offspring->mutate(random); // Constructor allready mutates behavior
        return offspring;
    }

    public:
    void mutate(emp::Random random) {
        behavior += random.GetRandNormal(0, 0.02); // Mutate behavior with a Gaussian distribution
    }

    public:
    double fitness() {
        // -abs(behavior^2-2)
        double difference = behavior * behavior - 2;
        if (difference > 0) {
            difference = -difference;
        }
        return difference; // organisms try to have behaviors close to sqrt(2)
    }
};

class AlternatingStringOrganism {
    public: emp::vector<bool> behavior;
    public:
    AlternatingStringOrganism(int length, emp::Random random) {
        for (int i = 0; i < length; ++i) {
            behavior.push_back(false); // Randomly initialize the behavior
        }
        this->mutate(random);
    }

    public:
    AlternatingStringOrganism* reproduce(emp::Random random) {
        AlternatingStringOrganism *offspring = new AlternatingStringOrganism(behavior.size(), random);
        for (int i = 0; i < behavior.size(); ++i) {
            // Copy the parent's behavior to the offspring
            offspring->behavior[i] = behavior[i];
        }
        offspring->mutate(random);
        return offspring;
    }

    public:
    void mutate(emp::Random random) {
        double mutation_rate = 0.1;
        for (int i = 0; i < behavior.size(); ++i) {
            if (random.GetDouble(0.0, 1.0) < mutation_rate) {
                // Flip the bit with a probability of mutation_rate
                behavior[i] = !behavior[i];
            }
        }
    }

    public:
    double fitness() {
        // Fitness is the number of alternating bits
        int count = 0;
        for (size_t i = 1; i < behavior.size(); ++i) {
            if (behavior[i] != behavior[i - 1]) count++;
        }
        return count;
    }

    public:
    void print() {
        for (int i = 0; i < behavior.size(); ++i) {
            std::cout << (behavior[i] ? '1' : '0'); // Print '1' for true, '0' for false
        }
    }
};

int main() {
    emp::Random random = *new emp::Random(1); // Initialize random number generator
    // std::cout << "Hello, World!" << std::endl;
    // population of 100 random organisms
    emp::vector<Sqrt2Organism> population;
    for (int i = 0; i < 100; ++i) {
        // organisms start with behavior around 0.5, to make sure we get the positive sqrt(2)
        population.push_back(*new Sqrt2Organism(0.5, random));
    }
    // prints some debug information about the first organisms
    Sqrt2Organism first_organism = population[0];
    std::cout << "Example organism: " << first_organism.behavior << std::endl;
    Sqrt2Organism second_organism = *first_organism.reproduce(random);
    std::cout << "Reproduced organism behavior: " << second_organism.behavior << std::endl;
    int number_of_generations = 100;
    // run 100 generations
    for (int i = 0; i < number_of_generations; i++) {
        // simulate a generation by finding the best organism
        int best_index = 0;
        for (int j = 1; j < population.size(); j++) {
            if (population[j].fitness() > population[best_index].fitness()) {
                best_index = j; // find the best organism
            }
        }
        // replace a random organism
        int replace = random.GetUInt(population.size());
        // reproduce the best organism
        Sqrt2Organism offspring = *population[best_index].reproduce(random); // reproduce the best organism
        population[replace] = offspring; // replace the worst organism with the new offspring
    }
    // print the fitness of the best organism after 100 generations
    double best_behavior_index = 0;
    for (int i = 1; i < population.size(); i++) {
        if (population[i].fitness() > population[best_behavior_index].fitness()) {
            best_behavior_index = i; // find the best organism after the simulation
        }
    }
    // print approximately sqrt(2)
    std::cout << "Best organism behavior after " << number_of_generations << " generations: " << population[best_behavior_index].behavior << std::endl;

    // I attempted to do a second simulation where the organisms store a sequence of 0s and 1s, and organisms
    // with sequences that alternate more are more fit
    // same algorithm as above, with AlternatingStringOrganism
    emp::vector<AlternatingStringOrganism> population2;
    for (int i = 0; i < 100; ++i) {
        population2.push_back(*new AlternatingStringOrganism(50, random));
    }
    AlternatingStringOrganism first_organism2 = population2[0];
    std::cout << "Example organism: ";
    first_organism2.print();
    std::cout << std::endl;
    AlternatingStringOrganism second_organism2 = *first_organism2.reproduce(random);
    std::cout << "Reproduced organism behavior: ";
    second_organism2.print();
    std::cout << std::endl;
    for (int i = 0; i < number_of_generations; i++) {
        // simulate a generation by finding the best organism, reproducing it, and killing a random organism
        int best_index = 0;
        for (int j = 1; j < population.size(); j++) {
            if (population2[j].fitness() > population2[best_index].fitness()) {
                best_index = j; // find the best organism
            }
        }
        int replace = random.GetUInt(population.size());
        AlternatingStringOrganism offspring = *population2[best_index].reproduce(random); // reproduce the best organism
        population2[replace] = offspring; // replace the worst organism with the new offspring
    }
    // print the fitness of the best organism after 100 generations
    best_behavior_index = 0;
    for (int i = 1; i < population.size(); i++) {
        if (population2[i].fitness() > population2[best_behavior_index].fitness()) {
            best_behavior_index = i;
        }
    }
    std::cout << "Best organism behavior after " << number_of_generations << " generations: ";
    population2[best_behavior_index].print();
    std::cout << std::endl;
}