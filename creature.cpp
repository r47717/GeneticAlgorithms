#include <vector>
#include <functional>
#include <memory>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>
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

void Creature::mutate()
{
	int bit = random_n_m(0, DNA_LEN - 1);
	dna[bit] = dna[bit] == 1 ? 0 : 1;
	//std::cout << "\nbit mutated\n";
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
	std::uniform_int_distribution<int> dice_distribution(1, 8);
	std::mt19937 random_number_engine;
	random_number_engine.seed(this->x * this->y * random_n_m(1, 1000));
	auto dice_roller = std::bind(dice_distribution, random_number_engine);

	while (!field->simulationStopped) 
	{

		int x;
		int y;

		std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // sleep for 1 sec (it is so easy in C++...)

		if (this->readyForCrossover < CROSSOVER_RATIO)
			this->readyForCrossover++;

		if (this->readyForMutation <= MUTATION_RATIO)
			this->readyForMutation++;
		else {
			//this->mutate();
			//readyForMutation = 0;
			//std::cout << "I mutated...\n";
		}

		std::unique_lock<std::mutex> lck(field->fieldMutex);

		int dir = dice_roller(); // randomly select a direction 1 - 8
		int cnt = 0;

		do {
			x = this->x;
			y = this->y;
			switch (dir) {
			case 1: // up
				y--;
				break;
			case 2: // up-right
				y--;
				x++;
				break;
			case 3: // right
				x++;
				break;
			case 4: // right-down
				x++;
				y++;
				break;
			case 5: // down
				y++;
				break;
			case 6: // down-left
				x--;
				y++;
				break;
			case 7: // left
				x--;
				break;
			case 8: // up-left
				x--;
				y--;
				break;
			}
			if (field->isEmpty(x, y)) // follow this free direction
				break;
		
			if (this->isReadyForCrossover()) {
				Creature* occupant = field->getOccupant(x, y);
				if (occupant != nullptr) { 
					if (occupant->isReadyForCrossover()) {
						this->crossover(*occupant);
						this->readyForCrossover = 0;
						occupant->readyForCrossover = 0;
						cnt = 8;
						std::cout << "We just had a crossover...\n";
						break;
					}
					else {
						//std::cout << "I wanted crossover but he was not ready...\n";
					}
				}
			}			
			
			dir++; // this direction is busy - change dir
			if (dir == 9) 
				dir = 1;
	
		} while (++cnt < 8);
	
		if (cnt < 8) {  // ctn == 8 means all 8 directions are busy (or a crossover happened) - do not move
			field->release(this->x, this->y);
			field->occupy(x, y, this);
			this->x = x;
			this->y = y;
		}
	}
}

bool Creature::isReadyForCrossover()
{
	return readyForCrossover >= CROSSOVER_RATIO;
}


