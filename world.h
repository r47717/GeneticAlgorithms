#pragma once

class Creature;

class World
{
public:
	World(std::function<int(std::vector<int>)>);
	~World();

	void printAll();

	void timeTick();
	void evaluateBest();
	void printBest();

	void printField();

	bool isEmpty(int x, int y);
	std::mutex fieldMutex;
	void occupy(int x, int y);
	void release(int x, int y);

	bool simulationStopped = false;

private:
	static const int CREATURES_NUM = 20;
	static const int WIDTH = 30;
	static const int HEIGHT = 30;
	int field[HEIGHT][WIDTH];

	std::vector<Creature *> creatures;
	std::function<int(std::vector<int>)> func;

	int best;
	int bestIndex = -1;

	int mutationRate = 100;

	void runMutation();
	void runCrossover();

};

int random_n_m(int n, int m);