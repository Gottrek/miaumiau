#include <iostream>
#include <string>
#include <vector>
#include <random>

class Parrot {
    std::vector<std::string> phrases;
    std::mt19937 rng{std::random_device{}()};

public:
    void addPhrase(const std::string& phrase) {
        phrases.push_back(phrase);
    }

    void say(int times = 1) {
        if (phrases.empty()) {
            std::cout << "(Papuga nic nie zna)\n";
            return;
        }

        std::uniform_int_distribution<> dist(0, phrases.size() - 1);
        for (int i = 0; i < times; ++i)
            std::cout << phrases[dist(rng)] << '\n';
    }
};

int main() {
    Parrot p;
    p.addPhrase("Dzwoni papuga");
    p.addPhrase("Mowi ze musi sie udac");

    p.say(5);
}
