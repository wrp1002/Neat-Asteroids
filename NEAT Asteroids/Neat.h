#pragma once

#include <allegro5/allegro_font.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <math.h>
#include <Windows.h>
#include <fstream>

#include "NodeGene.h"
#include "ConnectionGene.h"
#include "Genome.h"
#include "Agent.h"
#include "Species.h"
#include "Globals.h"
#include "History.h"

using namespace std;

class Neat
{
private:
	vector<NodeGene *> allNodes;
	unordered_map<string, ConnectionGene *> allConnections;
	vector<Agent *> agents;
	vector<Species *> species;
	vector<History> history;
	unordered_map<string, bool> drawOptions = { {"nn", true}, {"info", true}, {"eyes", false} };

	int inputNodes, outputNodes;
	int maxAgents;

	const double disjointCoef	= 0.75;
	const double excessCoef		= 0.75;
	const double weightCoef		= 0.8;
	const double CP				= 1.0;

	const double KILL_PERCENT	= 0.5;

	const double WEIGHT_SHIFT_STRENGTH	= 0.3;
	const double WEIGHT_RANDOM_STRENGTH = 1.0;

	/*
	float excessCoeff = 1.5;
	float weightDiffCoeff = 0.8;
	float compatibilityThreshold = 1;
	*/

	const double PROBABILITY_MUTATE_CONNECTION		= 0.05;
	const double PROBABILITY_MUTATE_ADD_NODE		= 0.03;
	const double PROBABILITY_MUTATE_WEIGHT_SHIFT	= 0.6;
	const double PROBABILITY_MUTATE_WEIGHT_RANDOM	= 0.2;
	const double PROBABILITY_MUTATE_TOGGLE			= 0.00;

	long int totalFitness = 0;

	int speciesCreatedTotal;
	int generation;
	int simIndex;
	int cycle;
	int speed;

public:
	Neat(vector<string> inputLabels, vector<string> outputLabels, int maxAgents);
	~Neat();

	void CalculateFitness();
	void Update();
	void DrawInfo(ALLEGRO_FONT *font);
	void Draw();
	void DrawGenome(ALLEGRO_FONT *font);
	void Print();
	void GraphHistory(string option);
	void RecordHistory();

	Agent *GetNextAgent();
	vector<Agent *> GetHistoryAgents(int index);
	void Evolve();
	void GenerateSpecies();
	void NormalizeSpecies();
	void Kill();
	void RemoveExtinct();
	void RemoveDead();
	void Reproduce();
	void Mutate();

	void AddChild(ConnectionGene *con, NodeGene *child);

	ConnectionGene *GetConnection(NodeGene *from, NodeGene *to);
	ConnectionGene *GetConnection(ConnectionGene *con);
	ConnectionGene *CopyConnection(ConnectionGene *con);
	NodeGene *NewNode(float x, float y, int type, string name = "");
	NodeGene *GetChild(ConnectionGene *con);
	NodeGene *GetChild(NodeGene *from, NodeGene *to);
	string NodesToString(NodeGene *from, NodeGene *to);
	Genome *NewGenome();
	Agent *GetAgent(int index);
	Species *PickWithWeight();

	double GetDisjointCoef();
	double GetExcessCoef();
	double GetWeightCoef();
	double GetCP();
	const double GetWEIGHT_SHIFT_STRENGTH() const { return WEIGHT_SHIFT_STRENGTH; }
	const double GetWEIGHT_RANDOM_STRENGTH() const { return WEIGHT_RANDOM_STRENGTH; }
	const double GetPROBABILITY_MUTATE_CONNECTION() const { return PROBABILITY_MUTATE_CONNECTION; }
	const double GetPROBABILITY_MUTATE_WEIGHT_SHIFT() const { return PROBABILITY_MUTATE_WEIGHT_SHIFT; }
	const double GetPROBABILITY_MUTATE_WEIGHT_RANDOM() const { return PROBABILITY_MUTATE_WEIGHT_RANDOM; }
	const double GetPROBABILITY_MUTATE_ADD_NODE() const { return PROBABILITY_MUTATE_ADD_NODE; }
	const double GetPROBABILITY_MUTATE_TOGGLE() const { return PROBABILITY_MUTATE_TOGGLE; }
	bool GenerationDone() { return simIndex == agents.size(); }
	int GetSpeed() const { return speed; }
	void SetSpeed(int val) { speed = val; }

	void ToggleDrawOption(string val);
	bool GetDrawOption(string val);
};

