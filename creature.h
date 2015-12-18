#pragma once

class World;

class Creature
{
public:
	const int DNA_LEN = 50;
	Creature(int x, int y, World*);
	Creature(Creature&);
	~Creature();

	void mutate(int rate);
	void crossover(Creature& other);
	Creature child();

	int countValue(std::function<int(std::vector<int>)>);

	void print();

	void move();

private:
	std::vector<int> dna;
	std::thread thread;

	World* field;
	int x;
	int y;

};

