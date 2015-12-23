#pragma once

class World;

class Creature
{
public:
	const int DNA_LEN = 50;
	const int CROSSOVER_RATIO = 10;
	const int MUTATION_RATIO = 20;

	Creature(int x, int y, World*);
	Creature(Creature&);
	~Creature();

	void mutate();
	void crossover(Creature& other);
	Creature child();

	int countValue(std::function<int(std::vector<int>)>);

	void print();

	void move();
	bool isReadyForCrossover();

private:
	std::vector<int> dna;
	std::thread thread;

	World* field;
	int x;
	int y;

	int readyForCrossover = CROSSOVER_RATIO;
	int readyForMutation = MUTATION_RATIO;

};

