#pragma once

#include <vector>


#include "NodeGene.h"
#include "Globals.h"

using namespace std;

class ConnectionGene
{
private:
	

	double weight;
	bool enabled;
	int innovationNumber;

public:
	NodeGene *fromNode;
	NodeGene *toNode;
	NodeGene *childNode;
	ConnectionGene(NodeGene *from, NodeGene *to);
	~ConnectionGene();

	void Print();
	void RandomWeight();

	void SetFrom(NodeGene *val) { this->fromNode = val; }
	void SetTo(NodeGene *val) { this->toNode = val; }
	void SetChild(NodeGene *val) { this->childNode = val; }
	void SetEnabled(bool val) { this->enabled = val; }
	void SetInnovationNumber(int val) { this->innovationNumber = val; }
	void SetWeight(double val) { this->weight = val; }

	NodeGene *GetFrom() { return fromNode; }
	NodeGene *GetTo() { return toNode; }
	NodeGene *GetChild() { return childNode; }
	double GetWeight() { return weight; }
	bool GetEnabled() { return enabled; }
	int GetInnovationNumber() { return innovationNumber; }
};

