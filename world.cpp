#include <vector>
#include <functional>
#include <memory>
#include <thread>
#include <mutex>
#include <chrono>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include "creature.h"
#include "world.h"



World::World(std::function<int(std::vector<int>)> f)
	: creatures(CREATURES_NUM), func(f)
{
	srand(std::time(0));

	for (int i = 0; i < HEIGHT * WIDTH; i++)
		for (int j = 0; j < WIDTH; j++)
			field[i][j] = 0;

	std::unique_lock<std::mutex> lck(fieldMutex);

	for (auto p = creatures.begin(); p != creatures.end(); p++) {
		int x = 0;
		int y = 0;
		do {
			x = random_n_m(0, WIDTH - 1);
			y = random_n_m(0, HEIGHT - 1);
		} while (!isEmpty(x, y));

		*p = new Creature(x, y, this);
	}
}


World::~World()
{
	for (auto p : creatures)
		delete p;
}

void World::printAll()
{
	std::cout << "\n";
	for (auto p : creatures) {
		p->print();
		std::cout << "\n";
	}
		
}

void World::timeTick()
{
	runMutation();
	runCrossover();
}

void World::evaluateBest()
{
	best = creatures[0]->countValue(func);
	bestIndex = 0;
	int size = creatures.size();

	for (int i = 1; i < size; i++ ) {
		int res = creatures[i]->countValue(func);
		if (res > best) {
			bestIndex = i;
			best = res;
		}
	}
}

void World::printBest() 
{
	std::cout << "Best in population: #" << bestIndex << " value = " << best << "\n";
}

bool World::isEmpty(int x, int y)
{
	return x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT && field[y][x] == 0;
}

void World::occupy(int x, int y)
{
	field[y][x] = 1;
}

void World::runMutation() 
{
	int size = creatures.size();
	for (int i = 0; i < size; i++) {
		if(i != bestIndex)
			creatures[i]->mutate(mutationRate);  // best creatures never mutate
	}
}

void World::runCrossover()
{
	int one = random_n_m(0, CREATURES_NUM - 1);
	int two = random_n_m(0, CREATURES_NUM - 1); // if one == two then no problem, don't care

	if(rand() > RAND_MAX / 2)
		creatures[one]->crossover(*creatures[two]);
}

int random_n_m(int n, int m)
{
	return rand()*(m - n) / RAND_MAX + n;
}
