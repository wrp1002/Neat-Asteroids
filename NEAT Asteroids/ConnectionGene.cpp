#include "ConnectionGene.h"


ConnectionGene::ConnectionGene(NodeGene * from, NodeGene * to) {
	this->fromNode = from;
	this->toNode = to;
	this->childNode = nullptr;

	this->weight = 0;
	this->enabled = true;
	this->innovationNumber = 0;
}

ConnectionGene::~ConnectionGene() {

}

void ConnectionGene::Print() {
	printf("Connection{ ");
	this->fromNode->Print();
	printf("  =>  ");
	this->toNode->Print();
	printf(", Enabled: %s }", (enabled ? "True" : "False"));
}

void ConnectionGene::RandomWeight() {
	this->weight = Globals::Random() * 2 - 1;
}
