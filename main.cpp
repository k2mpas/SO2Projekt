#include <string>
#include <iostream>
#include "DiningPhilosophers.h"

int main(int argc, char* argv[]) {
    // default number of philosophers (if not given through argument)
    int n = 5;

    // parsing argument line
    if (argc > 1) {
        try {
            n = std::stoi(argv[1]);
            if (n <= 1) {
                std::cerr << "There has to be more than 1 philosopher!" << std::endl;
                return 1;
            }
        } catch (const std::exception& e) {
            std::cerr << "Invalid argument: " << e.what() << std::endl;
            return 1;
        }
    }

    std::cout << "Simulation of " << n << " dining philosophers." << std::endl;

    DiningPhilosophers dp(n);
    dp.start();

    std::cout << "Simulation has come to an end!" << std::endl;

    return 0;
}