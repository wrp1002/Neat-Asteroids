#include "Agent.h"
#include "Species.h"
#include "Neat.h"

Agent::Agent(Genome *genome) {
	Reset();

	this->genome = genome;
	SetX(0);
	SetY(0);
	velMult = 0.99;
	accSpeed = 0.1;
	maxSpeed = 10;
	moveDir = 0;
	rotSpeed = 0.1;
	radius = 10;
	playerControlled = false;

	for (float i = 0; i < Globals::PI * 2 - Globals::PI / 4; i += Globals::PI / 4)
		eyes.push_back(Eye(x, y, i));
}

Agent::~Agent() {
	delete this->genome;
}

/*
bool Agent::operator<(const Agent * other) {
	return fitness < other->GetFitness();
}
*/

void Agent::Print() {
	genome->Print();
}

void Agent::Think() {
	vector<double> input;
	for (auto eye : eyes)
		input.push_back(eye.GetDistScaled());
	input.push_back((dir + (dir < 0 ? Globals::PI * 2 : 0)) / (Globals::PI * 2));
	input.push_back(canShoot);
	input.push_back(velX / maxSpeed);
	input.push_back(velY / maxSpeed);

	vector<double> output = genome->GetOutput();
	if (output.size() > 0)
		input.push_back(output[output.size() - 1]);
	else
		input.push_back(0);

	output = genome->Calculate(input);

	if (!playerControlled) {
		//"Forward", "Left", "Right", "Shoot"
		if (output[0] > 0.8)
			SetMoving(true);
		else
			SetMoving(false);

		moveDir = 0;
		if (output[1] > 0.5)
			moveDir -= 1;
		if (output[2] > 0.5)
			moveDir += 1;
		if (output[3] > 0.5) {
			if (CanShoot() && isAlive())
				shoot = true;
			else
				shoot = false;
		}
	}
}

void Agent::Update() {
	if (!alive)
		return;

	if (genome != nullptr)
		Think();

	accX = 0;
	accY = 0;

	dir += rotSpeed * moveDir;

	if (dir > Globals::PI * 2)
		dir -= Globals::PI * 2;
	if (dir < -Globals::PI * 2)
		dir += Globals::PI * 2;

	if (moving) {
		accX += cos(dir) * accSpeed;
		accY += sin(dir) * accSpeed;
	}

	velX += accX;
	velY += accY;

	velX *= velMult;
	velY *= velMult;

	SetX(GetX() + velX);
	SetY(GetY() + velY);

	if (x < -Globals::wrapRadius)
		x = Globals::screenWidth + Globals::wrapRadius;
	else if (x > Globals::screenWidth + Globals::wrapRadius)
		x = -Globals::wrapRadius;
	if (y < -Globals::wrapRadius)
		y = Globals::screenHeight + Globals::wrapRadius;
	else if (y > Globals::screenHeight + Globals::wrapRadius)
		y = -Globals::wrapRadius;

	cycles++;
}

void Agent::UpdateEyes(vector<Asteroid> &asteroids) {
	if (!alive)
		return;

	for (auto &eye : eyes)
		eye.Update(x, y, dir, asteroids);
}

void Agent::Draw(ALLEGRO_BITMAP *image, ALLEGRO_FONT *font) {
	al_draw_tinted_scaled_rotated_bitmap_region(image, 16 * (moving && rand() % 2 == 0), 16 * genome->GetOtherGene(0), 16, 16, al_map_rgb(255, 255, 255), 8, 8, GetX(), GetY(), 1.5, 1.5, dir, NULL);

	if (genome->GetNeat()->GetDrawOption("eyes")) {
		for (auto eye : eyes)
			eye.Draw();
	}

	if (genome != nullptr && genome->GetNeat()->GetDrawOption("nn")) {
		genome->Draw(Globals::screenWidth - 320, 5, 300, 200, font);
	}
}

void Agent::CalculateFitness() {
	float accuracy = float(hitCount) / float(shootcount);

	fitness = (score + 1) * 10;
	fitness *= cycles;
	fitness *= accuracy * accuracy;
	//fitness *= fitness;

	//printf("Score:%i  cycles:%i   shootCount:%i   hitCount:%i   accuracy:%f   fitness:%i\n", score, cycles, shootcount, hitCount, accuracy, fitness);

	//if (fitness > 0) printf("Fitness: %f\n", fitness);
}

void Agent::Reset() {
	SetSpecies(nullptr);
	fitness = 0;

	SetX(0);
	SetY(0);
	velX = 0;
	velY = 0;
	accX = 0;
	accY = 0;
	moveDir = 0;
	dir = Globals::RandomDir();
	moving = false;
	canShoot = true;
	alive = true;
	shoot = false;
	cycles = 0;
	score = 0;
	shootcount = 4;
	hitCount = 1;
	cycles = 0;
}

void Agent::SetMoving(bool val) {
	if (!alive)
		return;

	moving = val;
}

void Agent::Shoot() {
	canShoot = false;
	shoot = false;
	shootcount++;
}

void Agent::Reload() {
	canShoot = true;
}

Genome * Agent::GetGenome() { 
	return this->genome; 
}

double Agent::Distance(Agent * other) {
	Agent *agent1 = this;
	Agent *agent2 = other;
	
	return agent1->GetGenome()->Distance(agent1->GetGenome(), agent2->GetGenome());
}

int Agent::GetScore()
{
	return score;
}
