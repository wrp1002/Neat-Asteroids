#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include <vector>
#include <random>

#include "NodeGene.h"
#include "ConnectionGene.h"

class Neat;

using namespace std;

class Genome
{
private:
	
	vector<NodeGene *> inputNodes;
	vector<NodeGene *> hiddenNodes;
	vector<NodeGene *> outputNodes;
	vector<ConnectionGene *> connections;
	vector<double> input;
	vector<double> output;
	vector<int> otherGenes;
	Neat *neat;

	void DrawNodes(vector<NodeGene *> nodes, float x, float y, float scaleX, float scaleY, ALLEGRO_FONT *font, int xOff = 0);

public:
	vector<NodeGene *> allNodes;

	Genome(Neat *neat);
	~Genome();

	void PrepareCalculations();
	vector<double> Calculate(vector<double> inputs);
	vector<double> GetOutput();
	double Distance(Genome * g1, Genome * g2);
	Genome *Crossover(Genome *g1, Genome *g2);
	Genome *Copy();

	void Print();
	void Draw(int x, int y, int scaleX, int scaleY, ALLEGRO_FONT *font);

	void AddNode(NodeGene *node);
	void AddConnection(ConnectionGene *connection);
	void AddOtherGene(int gene);

	bool ConnectionExists(ConnectionGene *con);
	bool ConnectionExists(NodeGene *from, NodeGene *to);
	bool NodeExists(NodeGene *node);
	vector<ConnectionGene *> GetConnections();
	int GetHighestInnovationNumber();
	int GetOtherGene(int index);
	Neat *GetNeat();

	void Mutate();
	void MutateAddConnection();
	void MutateAddNode();
	void MutateToggle();
	void MutateRandomWeight();
	void MutateShiftWeight();
};

