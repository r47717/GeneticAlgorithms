#include <vector>
#include <functional>
#include <thread>
#include <mutex>
#include <chrono>
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

	for (int i = 0; i < 1000; i++) {
		cout << "Time: " << i << "\n";
		world.evaluateBest();
		world.printBest();
		world.timeTick();
	}

	return 0;
}

