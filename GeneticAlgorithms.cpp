#include <vector>
#include <functional>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>
#include <memory>
#include <iostream>
#include <cstdlib>
#include "world.h"
#include "creature.h"

using namespace std;

int main()
{
	World world([](std::vector<int> dna) -> int { 
		int sum = 0;
		for (auto i : dna)
			sum += i;
		return sum;
	});

	world.printField();

	for (int i = 0; i < 60; i++) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		//world.printField();
	}

	world.printField();

	world.simulationStopped = true;


	/*for (int i = 0; i < 1000; i++) {
		cout << "Time: " << i << "\n";
		world.evaluateBest();
		world.printBest();
		world.timeTick();
	}*/

	return 0;
}

