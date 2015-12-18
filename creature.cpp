#include <vector>
#include <functional>
#include <memory>
#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include "world.h"
#include "creature.h"


Creature::Creature(int x, int y, World* field)
	:dna(DNA_LEN, 0), x(x), y(y), field(field)
{
	for (auto p = dna.begin(); p != dna.end(); p++)
		*p = random_n_m(0, 1);

	thread = std::thread(&Creature::move, this);
}

Creature::Creature(Creature& parent)
	: dna(parent.dna)
{
}

Creature::~Creature()
{
	thread.join();
}

void Creature::mutate(int rate)
{
	int bit = random_n_m(0, DNA_LEN - 1);
	double p = double(rand()) / RAND_MAX * rate;
	if (p < 1) {
		dna[bit] = dna[bit] == 1 ? 0 : 1;
		//std::cout << "\nbit mutated\n";
	}
		
}

void Creature::crossover(Creature& other)
{
	for (int i = 0; i < DNA_LEN / 2; i++) {
		int tmp = this->dna[i];
		this->dna[i] = other.dna[i];
		other.dna[i] = tmp;
	}
}

Creature Creature::child()
{
	return Creature(*this);
}

int Creature::countValue(std::function<int(std::vector<int>)> func)
{
	return func(this->dna);
}

void Creature::print()
{
	for (auto p = dna.begin(); p != dna.end(); p++)
		std::cout << *p;
}

void Creature::move() // creature thread runs this method
{
	int x;
	int y;
	int cnt = 0;

	while (!field->simulationStopped) 
	{

		std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // sleep for 1 sec (it is so easy in C++...)

		std::unique_lock<std::mutex> lck(field->fieldMutex);

		int dir = random_n_m(1, 4); // randomly select a direction

		do {
			x = this->x;
			y = this->y;
			switch (dir) {
			case 1: // up
				y--;
				break;
			case 2: // right
				x++;
				break;
			case 3: // down
				y++;
				break;
			case 4: // left
				x--;
				break;
			}
			if (field->isEmpty(x, y)) // found free direction
				break;
		
			dir++;
			if (dir == 5) // this direction is busy - change dir
				dir = 1;
	
		} while (++cnt < 4); // ctn == 4 means all 4 directions are busy
	
		if (cnt < 4) {
			field->occupy(x, y);
			this->x = x;
			this->y = y;
		}
	}
}


