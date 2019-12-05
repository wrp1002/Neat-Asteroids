#include "Genome.h"
#include "Neat.h"


Genome::Genome(Neat *neat) {
	this->neat = neat;
	otherGenes = {rand() % 6};
}

Genome::~Genome() {
	/*for (auto it = allNodes.begin(); it != allNodes.end(); ) {
		delete * it;
		it = allNodes.erase(it);
	}*/
	for (auto it = connections.begin(); it != connections.end(); ) {
		delete * it;
		it = connections.erase(it);
	}
}

void Genome::PrepareCalculations() {
	this->output.clear();
	for (auto node : allNodes)
		node->ClearConnections();

	for (auto connection : connections) {
		connection->GetTo()->AddConnection(connection);
	}
}

vector<double> Genome::Calculate(vector<double> inputs) {
	PrepareCalculations();
	this->input = inputs;

	if (inputNodes.size() != inputs.size()) {
		printf("\nErrpr: Wrong size of input\n");
		printf("Input Size:%i  Correct Size:%i\n\n", inputs.size(), inputNodes.size());
		printf("Input: ");
		for (unsigned i = 0; i < inputs.size(); i++)
			printf(" %f", inputs[i]);
		printf("\n");
		system("pause");
		exit(EXIT_FAILURE);
	}

	for (unsigned i = 0; i < inputs.size(); i++) {
		inputNodes[i]->SetOutput(inputs[i]);
	}

	for (auto node : hiddenNodes)
		node->Calculate();

	for (auto node : outputNodes) {
		node->Calculate();
		output.push_back(node->GetOutput());
	}

	return output;
}

vector<double> Genome::GetOutput() {
	return output;
}

Genome * Genome::Crossover(Genome * g1, Genome * g2) {
	Genome *newGenome = neat->NewGenome();

	unsigned index1 = 0;
	unsigned index2 = 0;

	while (index1 < g1->GetConnections().size() && index2 < g2->GetConnections().size()) {
		ConnectionGene *gene1 = g1->GetConnections().at(index1);
		ConnectionGene *gene2 = g2->GetConnections().at(index2);

		int in1 = gene1->GetInnovationNumber();
		int in2 = gene2->GetInnovationNumber();

		if (in1 == in2) {
			if (rand() % 2 == 0)
				newGenome->AddConnection(neat->CopyConnection(gene1));
			else
				newGenome->AddConnection(neat->CopyConnection(gene2));

			index1++;
			index2++;
		}
		else if (in1 > in2) {
			//newGenome->AddConnection(neat->CopyConnection(gene2));	// maybe try this?
			index2++;
		}
		else {
			newGenome->AddConnection(neat->CopyConnection(gene1));
			index1++;
		}
	}

	while (index1 < g1->GetConnections().size()) {
		ConnectionGene *gene = g1->GetConnections().at(index1);
		newGenome->AddConnection(neat->CopyConnection(gene));
		index1++;
	}

	for (ConnectionGene *c : newGenome->GetConnections()) {
		newGenome->AddNode(c->GetFrom());
		newGenome->AddNode(c->GetTo());
	}

	for (unsigned i = 0; i < otherGenes.size(); i++) {
		if (rand() % 2 == 0)
			newGenome->AddOtherGene(g1->GetOtherGene(i));
		else
			newGenome->AddOtherGene(g2->GetOtherGene(i));
	}

	return newGenome;
}

Genome * Genome::Copy() {
	Genome *copy = neat->NewGenome();
	for (unsigned i = 0; i < connections.size(); i++) {
		copy->AddConnection(neat->CopyConnection(connections[i]));

		copy->AddNode(connections[i]->GetFrom());
		copy->AddNode(connections[i]->GetTo());
	}
	for (unsigned i = 0; i < otherGenes.size(); i++)
		copy->AddOtherGene(otherGenes[i]);

	return copy;
}

void Genome::Print() {
	printf("Input Nodes:\n");
	for (auto node : this->inputNodes) {
		node->Print();
		printf("\n");
	}

	printf("\nHidden Nodes:\n");
	for (auto node : this->hiddenNodes) {
		node->Print();
		printf("\n");
	}

	printf("\nOutput Nodes:\n");
	for (auto node : this->outputNodes) {
		node->Print();
		printf("\n");
	}

	printf("\nConnections:\n");
	for (auto connection : this->connections) {
		connection->Print();
		printf("\n");
	}
}

void Genome::Draw(int x, int y, int scaleX, int scaleY, ALLEGRO_FONT *font) {
	for (auto connection : connections) {
		al_draw_line(connection->GetFrom()->GetX() * scaleX + x, connection->GetFrom()->GetY() * scaleY + y, connection->GetTo()->GetX() * scaleX + x, connection->GetTo()->GetY() * scaleY + y, (connection->GetEnabled() ? (connection->GetWeight() > 0 ? al_map_rgba(50, 100, 255, 50) : al_map_rgba(255, 50, 50, 50)) : al_map_rgba(50, 50, 50, 50)), abs(connection->GetWeight()) * 3 + 1);
		//al_draw_textf(font, al_map_rgb(255, 255, 255), x + (connection->GetFrom()->GetX() * scaleX + connection->GetTo()->GetX() * scaleX) / 2, y + (connection->GetFrom()->GetY() * scaleY + connection->GetTo()->GetY() * scaleY) / 2, ALLEGRO_ALIGN_CENTER, "%.2f", connection->GetWeight());
		//al_draw_textf(font, al_map_rgb(255, 255, 255), x + (connection->GetFrom()->GetX() * scaleX + connection->GetTo()->GetX() * scaleX) / 2, y + (connection->GetFrom()->GetY() * scaleY + connection->GetTo()->GetY() * scaleY) / 2 - 10, ALLEGRO_ALIGN_CENTER, "%i", connection->GetInnovationNumber());
	}

	DrawNodes(inputNodes, x, y, scaleX, scaleY, font, -15);
	DrawNodes(hiddenNodes, x, y, scaleX, scaleY, font);
	DrawNodes(outputNodes, x, y, scaleX, scaleY, font, 15);

	//al_draw_rectangle(x, y, x + scaleX, y + scaleY, al_map_rgb(255, 0, 255), 1);
}

void Genome::DrawNodes(vector<NodeGene*> nodes, float x, float y, float scaleX, float scaleY, ALLEGRO_FONT *font, int xOff) {
	for (auto node : nodes) {
		if (input.size() > 0) Calculate(input);
		al_draw_filled_circle(x + node->GetX() * scaleX, y + node->GetY() * scaleY, 7, al_map_rgb(100 - node->GetOutput() * 100, 100 + node->GetOutput() * 100, 50));
		al_draw_circle(x + node->GetX() * scaleX, y + node->GetY() * scaleY, 7, al_map_rgb(100, 100, 100), 1);
		al_draw_textf(font, al_map_rgb(0, 0, 0), x + node->GetX() * scaleX, y + node->GetY() * scaleY - 5, ALLEGRO_ALIGN_CENTER, "%.2f", (node->GetOutput()));
		al_draw_textf(font, al_map_rgb(255, 255, 255), x + node->GetX() * scaleX + xOff, y + node->GetY() * scaleY - 7, xOff <= 0 ? ALLEGRO_ALIGN_RIGHT : ALLEGRO_ALIGN_LEFT, node->GetName().c_str());
	}
}

bool Genome::NodeExists(NodeGene * node) {
	for (auto other : allNodes) {
		if (node == other)
			return true;
	}
	return false;
}

void Genome::AddNode(NodeGene *node) {
	if (NodeExists(node))
		return;

	if (node->GetType() == NodeGene::INPUT)
		inputNodes.push_back(node);
	else if (node->GetType() == NodeGene::HIDDEN) {
		if (hiddenNodes.size() == 0)
			hiddenNodes.push_back(node);
		else {
			for (unsigned i = 0; i < hiddenNodes.size(); i++) {
				if (node->GetX() < hiddenNodes[i]->GetX()) {
					hiddenNodes.insert(hiddenNodes.begin() + i, node);
					break;
				}
				else if (i == hiddenNodes.size() - 1) {
					hiddenNodes.insert(hiddenNodes.begin() + i + 1, node);
					break;
				}
			}
		}
	}
	else if (node->GetType() == NodeGene::OUTPUT)
		outputNodes.push_back(node);

	allNodes.push_back(node);
}

void Genome::AddConnection(ConnectionGene *connection) {
	if (!ConnectionExists(connection))
		this->connections.push_back(connection);
}

void Genome::AddOtherGene(int gene) {
	otherGenes.push_back(gene);
}

double Genome::Distance(Genome * g1, Genome * g2) {
	int disjoint = 0;
	int excess = 0;
	double weightDiff = 0;
	int similar = 0;
	double distance = 0;

	unsigned index1 = 0;
	unsigned index2 = 0;

	int highestInnovationNumber1 = g1->GetHighestInnovationNumber();
	int highestInnovationNumber2 = g2->GetHighestInnovationNumber();

	if (highestInnovationNumber2 > highestInnovationNumber1) {
		Genome *tmp = g1;
		g1 = g2;
		g2 = tmp;
		//printf("Swapped! New order: %i  %i\n", g1->GetHighestInnovationNumber(), g2->GetHighestInnovationNumber());
	}

	while (index1 < g1->GetConnections().size() && index2 < g2->GetConnections().size()) {
		ConnectionGene *gene1 = g1->GetConnections().at(index1);
		ConnectionGene *gene2 = g2->GetConnections().at(index2);

		int in1 = gene1->GetInnovationNumber();
		int in2 = gene2->GetInnovationNumber();

		if (in1 == in2) {
			similar++;
			weightDiff += abs(gene1->GetWeight() - gene2->GetWeight());

			index1++;
			index2++;
		}
		else if (in1 > in2) {
			disjoint++;
			index2++;
		}
		else {
			disjoint++;
			index1++;
		}
	}

	excess = g1->GetConnections().size() - index1;
	if (similar != 0)
		weightDiff /= double(similar);

	int size1 = g1->GetConnections().size();
	int size2 = g2->GetConnections().size();
	double N = max(size1, size2);

	if (N < 20) N = 1;
	//N = 1.0;

	//printf("%f  %f  %f\n", (disjoint * neat->GetC1() / N), (excess * neat->GetC2() / N), (weightDiff * neat->GetC3()));

	distance = (disjoint * neat->GetDisjointCoef() / N) + (excess * neat->GetExcessCoef() / N) + (weightDiff * neat->GetWeightCoef());
	//printf("DIst: %f\n", distance);
	return distance;
}
bool Genome::ConnectionExists(ConnectionGene * con) {
	for (unsigned i = 0; i < connections.size(); i++) {
		if (con->GetFrom() == connections[i]->GetFrom() && con->GetTo() == connections[i]->GetTo()) {
			return true;
		}
	}
	return false;
}

bool Genome::ConnectionExists(NodeGene * from, NodeGene * to) {
	for (unsigned i = 0; i < connections.size(); i++) {
		//printf("Comparing %i  :  %i and %i  ==     %i and %i\n", connections[i], connections[i]->GetFrom(), connections[i]->GetTo(), from, to);
		if (from == connections[i]->fromNode && to == connections[i]->toNode) {
			return true;
		}
	}
	return false;
}

vector<ConnectionGene*> Genome::GetConnections() {
	return connections;
}

int Genome::GetHighestInnovationNumber() {
	if (connections.size() == 0)
		return 0;

	int highest = 0;
	for (auto con : connections) {
		if (con->GetInnovationNumber() > highest)
			highest = con->GetInnovationNumber();
	}
	return highest;
}

int Genome::GetOtherGene(int index) {
	if ((unsigned)index >= otherGenes.size())
		return 0;
	return otherGenes[index];
}

Neat * Genome::GetNeat() {
	return this->neat;
}

void Genome::Mutate() {
	if (Globals::Random() <= neat->GetPROBABILITY_MUTATE_CONNECTION())
		MutateAddConnection();

	if (Globals::Random() <= neat->GetPROBABILITY_MUTATE_TOGGLE())
		MutateToggle();

	if (Globals::Random() <= neat->GetPROBABILITY_MUTATE_ADD_NODE())
		MutateAddNode();

	if (Globals::Random() <= neat->GetPROBABILITY_MUTATE_WEIGHT_RANDOM())
		MutateRandomWeight();

	if (Globals::Random() <= neat->GetPROBABILITY_MUTATE_WEIGHT_SHIFT())
		MutateShiftWeight();
}

void Genome::MutateAddConnection() {
	//printf("Mutating add connection\n");
	for (int i = 0; i < 100; i++) {
		ConnectionGene *con = nullptr;
		NodeGene *node1 = allNodes[rand() % allNodes.size()];
		NodeGene *node2 = allNodes[rand() % allNodes.size()];


		if (node1->GetX() == node2->GetX())
			continue;

		if (node1->GetX() < node2->GetX()) {
			if (!ConnectionExists(node1, node2))
				con = neat->GetConnection(node1, node2);
			else
				continue;
		}
		else {
			if (!ConnectionExists(node2, node1))
				con = neat->GetConnection(node2, node1);
			else
				continue;
		}

		con->RandomWeight();
		connections.push_back(con);
		break;
	}
}

void Genome::MutateAddNode() {
	if (this->connections.size() > 0) {
		ConnectionGene *con = connections[rand() % connections.size()];
		ConnectionGene *newConnection1 = nullptr;
		ConnectionGene *newConnection2 = nullptr;

		NodeGene * newNode = neat->GetChild(con);
		if (newNode == nullptr) {
			newNode = neat->NewNode((con->GetFrom()->GetX() + con->GetTo()->GetX()) / 2, (con->GetFrom()->GetY() + con->GetTo()->GetY()) / 2 + (Globals::Random() - 0.5) / 5, NodeGene::HIDDEN);
			neat->AddChild(con, newNode);
		}

		AddNode(newNode);
		con->SetEnabled(false);

		newConnection1 = neat->GetConnection(con->GetFrom(), newNode);	//new ConnectionGene(newNode, con->GetTo());
		newConnection2 = neat->GetConnection(newNode, con->GetTo());

		newConnection1->SetWeight(1.0);
		newConnection2->SetWeight(con->GetWeight());

		AddConnection(newConnection1);
		AddConnection(newConnection2);

		//printf("Adding node x:%f   y:%f\n", newNode->GetX(), newNode->GetY());
	}
}

void Genome::MutateToggle() {
	if (connections.size() > 0) {
		ConnectionGene *con = connections[rand() % connections.size()];
		con->SetEnabled(!con->GetEnabled());
	}
}

void Genome::MutateRandomWeight() {
	for (auto con : connections)
		con->SetWeight((Globals::Random() * 2 - 1) * neat->GetWEIGHT_RANDOM_STRENGTH());
}

void Genome::MutateShiftWeight() {
	for (auto con : connections) {
		con->SetWeight(con->GetWeight() + (Globals::RandomNormal() * 2 - 1));

		if (con->GetWeight() > 1)
			con->SetWeight(1);
		else if (con->GetWeight() < -1)
			con->SetWeight(-1);
	}
}

