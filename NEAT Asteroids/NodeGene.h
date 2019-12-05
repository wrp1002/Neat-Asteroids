#pragma once
#include <stdio.h>
#include <vector>
#include <math.h>
#include <string>

using namespace std;

class ConnectionGene;

class NodeGene
{
private:
	double x, y;
	double output;
	int innovationNumber;
	int type;
	string name;
	vector<ConnectionGene *> connections;

public:
	enum NODE_TYPE { INPUT, HIDDEN, OUTPUT };

	NodeGene(float x, float y, int type, string name = "");
	~NodeGene();

	void Print();

	void Calculate();
	double ActivationFunction(double input);
	void AddConnection(ConnectionGene *connection);
	void ClearConnections();

	void SetInnovationNumber(int val) { this->innovationNumber = val; }
	int GetInnovationNumber() { return innovationNumber; }
	double GetOutput() const { return output; }
	void SetOutput(double val) { output = val; }
	double GetX() const { return x; }
	void SetX(float val) { x = val; }
	double GetY() const { return y; }
	void GetY(float val) { y = val; }
	string GetName() { return this->name; }
	int GetType() const { return type; }
	void SetType(int val) { type = val; }
};

