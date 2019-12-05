#include "Species.h"
#include "Neat.h"


Species::Species(Agent * representative) {
	this->champ = representative;
	this->champ->SetSpecies(this);
	this->score = 0;
	this->SetScoreNormalized(0);
	this->agents.push_back(representative);
	this->bestFitness = champ->GetFitness();
	this->averageFitness = 0;
}

Species::~Species()
{
}

bool Species::Add(Agent * newAgent) {
	//if (Size() >= 300) return false;

	if (champ == nullptr || RandomAgent()->Distance(newAgent) < champ->GetGenome()->GetNeat()->GetCP()) {
		if (champ == nullptr) {
			champ = newAgent;
		}
		agents.push_back(newAgent);
		newAgent->SetSpecies(this);

		if (newAgent->GetFitness() > champ->GetFitness())
			champ = newAgent;

		bestFitness = champ->GetFitness();

		return true;
	}
	return false;
}

void Species::AddForce(Agent * newAgent) {
	if (newAgent->GetFitness() > champ->GetFitness())
		champ = newAgent;
	agents.push_back(newAgent);
	newAgent->SetSpecies(this);
}

void Species::GoExtinct() {
	for (auto agent : agents)
		agent->SetSpecies(nullptr);
	Reset();
}

void Species::CalculateFitness() {
	if (agents.size() == 0) {
		this->score = 0;
		return;
	}

	int sum = 0;
	int sum2 = 0;
	for (auto agent : agents) {
		sum += float(agent->GetFitness());// / (float(agents.size()));
		sum2 += float(agent->GetFitness());
	}

	averageFitness = float(sum2) / agents.size();
	this->score = float(sum) / agents.size();
}

void Species::Reset() {
	for (auto agent : agents)
		agent->SetSpecies(nullptr);
	this->champ = nullptr;
	this->agents.clear();
	this->score = 0;
}

void Species::Kill(double percent) {
	this->Sort();

	if (agents.size() > 1) {
		int stopIndex = (agents.size() - 1) - agents.size() * percent;
		//printf("STOP: %i\n", stopIndex);
		if (stopIndex < 0)
			stopIndex = 0;
		for (int i = agents.size() - 1; i >= stopIndex; i--) {
			agents[i]->SetSpecies(nullptr);
			agents.erase(agents.begin() + i);
		}
	}
	//printf("AFTER KILLING    Best: %i   Worst: %i\n", agents[0]->GetFitness(), agents.size() > 0 ? agents[agents.size() - 1]->GetFitness() : agents[0]->GetFitness());
}

void Species::Sort() {
	if (agents.size() > 1) {
		std::sort(agents.begin(), agents.end(), [](const Agent *a, const Agent *b) {return a->GetFitness() > b->GetFitness(); });
		//printf("Best: %f   Worst: %f\n", agents[0]->GetFitness(), agents[agents.size() - 1]->GetFitness());
	}
}

Agent * Species::GetChamp() {
	return champ;
}

Agent * Species::RandomAgent() {
	if (agents.size() == 0)
		return nullptr;
	return agents[rand() % agents.size()];
}

Genome * Species::Crossover() {
	Agent *a1 = RandomAgent();
	Agent *a2 = RandomAgent();
	if (a1 == nullptr || a2 == nullptr)
		return nullptr;

	Genome *g1 = a1->GetGenome();
	Genome *g2 = a2->GetGenome();

	if (g1->GetHighestInnovationNumber() > g2->GetHighestInnovationNumber()) 
		return g1->Crossover(g1, g2);
	else 
		return g1->Crossover(g2, g1);
}

int Species::Size() {
	return agents.size();
}

int Species::GetBestFitness() {
	return bestFitness;
}
