#include <vector>
#include <functional>
#include <memory>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include "creature.h"
#include "world.h"



World::World(std::function<int(std::vector<int>)> f)
	: func(f)
{
	srand(std::time(0));

	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++)
			field[i][j] = nullptr;

	std::unique_lock<std::mutex> lck(fieldMutex);

	for (int i = 0; i < CREATURES_NUM; i++) {
		int x = 0;
		int y = 0;
		do {
			x = random_n_m(0, WIDTH - 1);
			y = random_n_m(0, HEIGHT - 1);
		} while (!isEmpty(x, y));

		Creature* cr = new Creature(x, y, this);
		creatures.push_back(cr);
		occupy(x, y, cr);
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

void World::printField()
{
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++)
			std::cout << (field[i][j] == nullptr ? 0 : 1);
		std::cout << "\n";
	}
	std::cout << "\n";
}

bool World::isEmpty(int x, int y)
{
	return x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT && field[y][x] == nullptr;
}

void World::occupy(int x, int y, Creature* cr)
{
	field[y][x] = cr;
	//std::cout << "field (" << y << ", " << x << ") occupied\n";
}

void World::release(int x, int y) {
	field[y][x] = nullptr;
}

Creature* World::getOccupant(int x, int y) {
	return (x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT) ? field[y][x] : nullptr;
}

int random_n_m(int n, int m)
{
	//return rand()*(m - n) / RAND_MAX + n;
	return n + static_cast<int>(std::round(double(rand())/RAND_MAX * (m - n)));
}
