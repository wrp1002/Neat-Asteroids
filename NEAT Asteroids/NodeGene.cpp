#include "NodeGene.h"
#include "ConnectionGene.h"

NodeGene::NodeGene(float x, float y, int type, string name) {
	this->x = x;
	this->y = y;
	this->type = type;
	this->name = name;
	this->output = 0;
	this->innovationNumber = 0;
}


NodeGene::~NodeGene() {

}

void NodeGene::Print() {
	printf("Node(X:%f, Y:%f, Connections:%i)", this->x, this->y, this->connections.size());
}

void NodeGene::Calculate() {
	double sum = 0;
	for (auto connection : connections) {
		if (connection->GetEnabled()) {
			sum += connection->GetFrom()->GetOutput() * connection->GetWeight();
		}
	}

	this->output = ActivationFunction(sum);
}

double NodeGene::ActivationFunction(double input) {
	return tanh(input);
}

void NodeGene::AddConnection(ConnectionGene * connection) {
	this->connections.push_back(connection);
}

void NodeGene::ClearConnections() {
	this->connections.clear();
}
