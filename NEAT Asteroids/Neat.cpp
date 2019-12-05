#include "Neat.h"



Neat::Neat(vector<string> inputLabels, vector<string> outputLabels, int maxAgents) {
	this->inputNodes = inputLabels.size();
	this->outputNodes = outputLabels.size();
	this->maxAgents = maxAgents;

	for (int i = 0; i < inputNodes; i++)
		NewNode(0.1, float(i) / float(inputNodes) + 1.0 / float(inputNodes) / 2, NodeGene::INPUT, inputLabels[i]);

	for (int i = 0; i < outputNodes; i++)
		NewNode(0.9, float(i) / float(outputNodes) + 1.0 / float(outputNodes) / 2, NodeGene::OUTPUT, outputLabels[i]);

	for (int i = 0; i < maxAgents; i++) {
		Agent *newAgent = new Agent(NewGenome());
		for (int j = 0; j < (inputNodes * outputNodes); j++) newAgent->GetGenome()->MutateAddConnection();
		agents.push_back(newAgent);
	}

	speciesCreatedTotal = 0;
	generation = 0;
	simIndex = 0;
	cycle = 0;
	SetSpeed(1);

	/*
	printf("Agents:\n");
	for (auto i : agents)
		i->Print();
	*/
}

Neat::~Neat() {
	vector<NodeGene *> allNodes;
	unordered_map<string, ConnectionGene *> allConnections;
	vector<Agent *> agents;
	vector<Species *> species;

	for (auto node : allNodes)
		delete node;
	for (auto connection : allConnections)
		delete connection.second;
	for (auto agent : agents)
		delete agent;
	for (auto s : species)
		delete s;
}

void Neat::Update() {
	cycle++;
}

void Neat::DrawInfo(ALLEGRO_FONT *font) {
	if (!drawOptions["info"])
		return;

	vector<string> info;
	info.push_back("Generation: " + to_string(generation));
	info.push_back("Species: " + to_string(species.size()));
	info.push_back("Population: " + to_string(simIndex) + "/" + to_string(agents.size()));
	info.push_back("Cycle: " + to_string(cycle));
	info.push_back("Speed: " + to_string(speed) + "x");

	for (unsigned i = 0; i < info.size(); i++)
		al_draw_text(font, al_map_rgb(255, 255, 255), 1, i * 18, NULL, info[i].c_str());
}

void Neat::Draw() {

}

void Neat::DrawGenome(ALLEGRO_FONT * font) {
	sort(agents.begin(), agents.end());
	int y = 0;
	int x = 0;
	for (unsigned i = 0; i < agents.size(); i++) {
		agents[i]->GetGenome()->Draw(x, y, 400, 300, font);
		al_draw_textf(font, al_map_rgb(255, 255, 255), x + 150, y + 10, ALLEGRO_ALIGN_CENTER, "Score: %f", agents[i]->GetFitness());
		al_draw_textf(font, al_map_rgb(255, 255, 255), x + 150, y + 20, ALLEGRO_ALIGN_CENTER, "Nodes: %i", agents[i]->GetGenome()->allNodes.size());
		x += 400;
		if (x >= 800) {
			x = 0;
			y += 300;
		}
	}
}

void Neat::Print() {
	printf("\n=====================================================\n");
	printf("Species:\n");
	for (auto s : species) {
		printf("ID:%i  Spec-Fit:%f  Normalized-Fit:%f  Best:%i  Size:%i  Average-fit:%f \n", int(s), s->GetScore(), s->GetScoreNormalized(), s->GetBestFitness(), s->Size(), s->GetAverageFitness());
	}
	printf("Generation:%i  Species Created:%i  Nodes:%i  Connections:%i\n", generation, speciesCreatedTotal, allNodes.size(), allConnections.size());
}

void Neat::GraphHistory(string option) {
	ofstream file;
	file.open("./Resources/history.txt");

	if (!file.is_open()) {
		printf("Unable to open file\n");
		return;
	}

	if (option == "all" || option == "best") {
		file << "Best";
		for (auto h : history)
			file << " " + to_string(h.GetHighScore());
		file << "\n";
	}

	if (option == "all" || option == "average") {
		file << "Average";
		for (auto h : history)
			file << " " + to_string(h.GetAverageScore());
		file << "\n";
	}

	if (option == "all" || option == "median") {
		file << "Median";
		for (auto h : history)
			file << " " + to_string(h.GetMedianScore());
		file << "\n";
	}

	if (option == "all" || option == "worst") {
		file << "Worst";
		for (auto h : history)
			file << " " + to_string(h.GetLowScore());
	}

	file.close();


	string cmd = "python Resources/GraphFile.py history.txt";
	system(cmd.c_str());
}

void Neat::RecordHistory() {
	int total = 0;
	for (auto agent : agents) {
		//agent->CalculateFitness();
		total += agent->GetFitness();
	}
	std::sort(agents.begin(), agents.end(), [](const Agent *a, const Agent *b) {return a->GetFitness() > b->GetFitness(); });
	int best = agents[0]->GetFitness();
	int worst = agents[agents.size() - 1]->GetFitness();
	int average = total / agents.size();
	int median = agents[agents.size() / 2]->GetFitness();

	vector<Genome *> bestGenomes;
	for (unsigned i = 0; i < species.size(); i++) {
		if (species[i]->GetChamp() != nullptr)
			bestGenomes.push_back(species[i]->GetChamp()->GetGenome()->Copy());
	}

	history.push_back(History(best, average, median, worst, bestGenomes));
}

Agent * Neat::GetNextAgent() {
	cycle = 0;
	return agents[simIndex++];
}

vector<Agent *> Neat::GetHistoryAgents(int index) {
	vector<Agent *> agents;
	if ((unsigned)index >= history.size())
		return agents;

	vector<Genome *> genomes = history[index].GetBestGenomes();
	for (unsigned i = 0; i < genomes.size(); i++)
		agents.push_back(new Agent(genomes[i]->Copy()));

	return agents;
	
}

void Neat::Evolve() {
	GenerateSpecies();
	RecordHistory();
	Kill();
	RemoveExtinct();
	RemoveDead();

	printf("Size after killing: %i\n", agents.size());

	while (agents.size() < (unsigned)this->maxAgents)
		Reproduce();
	
	printf("Done with evolving\n");

	//Mutate();

	generation++;
	simIndex = 0;
	cycle = 0;
}

void Neat::GenerateSpecies() {
	for (auto agent : agents)
		agent->CalculateFitness();

	for (auto s : species)
		s->Reset();

	for (auto agent : agents) {
		if (agent->GetSpecies() != nullptr) {
			continue;
		}

		for (unsigned i = 0; i < species.size(); i++) {
			if (species[i]->Add(agent)) {
				break;
			}
		}

		if (agent->GetSpecies() == nullptr) {
			//printf("New Species!\n");
			Species *newSpecies = new Species(agent);
			species.push_back(newSpecies);
			speciesCreatedTotal++;
		}
	}

	for (auto s : species)
		s->CalculateFitness();

	NormalizeSpecies();
}



void Neat::NormalizeSpecies() {
	long int total = 0;
	double count = 0;

	for (auto s : species)
		total += s->GetScore();

	for (auto s : species) {
		s->SetScoreNormalized(double(s->GetScore()) / double(total));
	}

	for (auto s : species) {
		count += s->GetScoreNormalized();
	}

	totalFitness = total;

	printf("Count: %f    Total: %i", count, total);
}

void Neat::Kill() {
	for (auto s : species)
		s->Kill(KILL_PERCENT);
}

void Neat::RemoveExtinct() {
	for (auto it = species.begin(); it != species.end(); ) {
		if ((*it)->Size() <= 1) {
			(*it)->GoExtinct();
			delete * it;
			it = species.erase(it);
		}
		else {
			++it;
		}
	}
}

void Neat::RemoveDead() {
	for (int i = agents.size() - 1; i >= 0; i--) {
		if (agents[i]->GetSpecies() == nullptr) {
			delete agents[i];
			agents[i] = nullptr;
			agents.erase(agents.begin() + i);
		}
		else
			agents[i]->Reset();
	}
}

void Neat::Reproduce() {
	Species *s = PickWithWeight();
	if (s == nullptr)
		return;

	//printf("Picked species with score %f\n", s->GetScoreNormalized());

	Genome *g = s->Crossover();
	if (g == nullptr)
		return;

	g->Mutate();

	Agent *newAgent = new Agent(g);
	s->AddForce(newAgent);
	agents.push_back(newAgent);
}

Genome *Neat::NewGenome() {
	Genome *g = new Genome(this);
	
	for (int i = 0; i < inputNodes; i++)
		g->AddNode(allNodes[i]);

	for (int i = inputNodes; i < inputNodes + outputNodes; i++)
		g->AddNode(allNodes[i]);

	return g;
}

void Neat::Mutate() {
	for (auto agent : agents) {
		agent->GetGenome()->Mutate();
	}
}

void Neat::AddChild(ConnectionGene * con, NodeGene *child) {
	string key = NodesToString(con->GetFrom(), con->GetTo());
	allConnections[key]->SetChild(child);
}

NodeGene *Neat::NewNode(float x, float y, int type, string name) {
	NodeGene *newNode = new NodeGene(x, y, type, name);
	allNodes.push_back(newNode);
	return newNode;
}

NodeGene *Neat::GetChild(ConnectionGene * con) {
	return GetChild(con->GetFrom(), con->GetTo());
}

NodeGene *Neat::GetChild(NodeGene *from, NodeGene *to) {
	string key = NodesToString(from, to);
	NodeGene *child = allConnections[key]->GetChild();

	return child;
}

ConnectionGene * Neat::GetConnection(NodeGene * from, NodeGene * to) {
	string val = NodesToString(from, to);

	//printf("Checking connection... ");

	if (allConnections.count(val) > 0) {
		//printf("Existing connection\n");
		return CopyConnection(allConnections.at(val));
	}

	//printf("New connection\n");

	ConnectionGene *connection = new ConnectionGene(from, to);
	connection->SetInnovationNumber(allConnections.size() + 1);
	allConnections[val] = connection;

	return CopyConnection(connection);
}

ConnectionGene * Neat::GetConnection(ConnectionGene * con) {
	NodeGene *from = con->GetFrom();
	NodeGene *to = con->GetTo();

	string val = NodesToString(from, to);

	//printf("Checking connection... ");

	if (allConnections.count(val) > 0) {
		//printf("Existing connection\n");
		return CopyConnection(allConnections.at(val));
	}

	//printf("New connection\n");

	ConnectionGene *connection = new ConnectionGene(from, to);
	connection->SetInnovationNumber(allConnections.size() + 1);
	allConnections[val] = connection;

	return CopyConnection(connection);
}

ConnectionGene * Neat::CopyConnection(ConnectionGene * con) {
	ConnectionGene *copy = new ConnectionGene(con->GetFrom(), con->GetTo());
	copy->SetInnovationNumber(con->GetInnovationNumber());
	copy->SetEnabled(con->GetEnabled());
	copy->SetWeight(con->GetWeight());
	return copy;
}

string Neat::NodesToString(NodeGene * from, NodeGene * to)
{
	return to_string((int)from) + to_string((int)to);
}

Species * Neat::PickWithWeight() {
	unsigned index = 0;
	double r = rand() % totalFitness;

	while (r > 0 && index < species.size()) {
		r -= species[index]->GetScore();
		index++;
	}

	index--;
	if (index < 0 || index >= species.size())
		return nullptr;
	return species[index];
}

double Neat::GetDisjointCoef()
{
	return disjointCoef;
}

double Neat::GetExcessCoef()
{
	return excessCoef;
}

double Neat::GetWeightCoef()
{
	return weightCoef;
}

double Neat::GetCP()
{
	return CP;
}

void Neat::ToggleDrawOption(string val) {
	drawOptions[val] = !drawOptions[val];
}

bool Neat::GetDrawOption(string val) {
	return drawOptions[val];
}
