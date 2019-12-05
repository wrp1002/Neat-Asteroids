#pragma once
#include <vector>
#include <random>

#include "Agent.h"
#include "Genome.h"

class Neat;

class Species
{
private:
	vector<Agent *> agents;
	Agent *champ;
	float score;
	float averageFitness;
	double scoreNormalized;
	int bestFitness;

public:
	Species(Agent *representative);
	~Species();

	bool Add(Agent *newAgent);
	void AddForce(Agent *newAgent);
	void GoExtinct();
	void CalculateFitness();
	void Reset();
	void Kill(double percent);

	bool CompareAgents(Agent &agent1, Agent &agent2);
	void Sort();
	void Print() {
		for (auto a : agents)
			a->Print();
	}

	Agent *GetChamp();
	Agent *RandomAgent();
	Genome *Crossover();
	int Size();
	float GetScore() { return score; }
	float GetAverageFitness() { return averageFitness; }
	double GetScoreNormalized() const { return scoreNormalized; }
	int GetBestFitness();

	void SetScoreNormalized(double val) { scoreNormalized = val; }
	void SetScore(double val) { score = val; }
};

