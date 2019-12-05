#pragma once

#include <allegro5/allegro_primitives.h>

#include "Genome.h"
#include "NodeGene.h"
#include "ConnectionGene.h"
#include "Globals.h"
#include "Eye.h"

class Species;
class Neat;

class Agent
{
private:
	Genome * genome;
	Species *species;
	int fitness;
	vector<Eye> eyes;

	float x, y;
	float velX, velY, velMult;
	float accX, accY;
	float dir, moveDir;
	float radius;
	float rotSpeed;
	float accSpeed;
	float maxSpeed;

	bool moving;
	bool canShoot;
	bool shoot;
	bool alive;
	bool playerControlled;

	int score;
	int shootcount;
	int hitCount;
	int cycles;


public:
	Agent(Genome *genome);
	~Agent();

	void Print();
	//void DrawGenome(int x, int y);
	void Think();
	void Update();
	void UpdateEyes(vector<Asteroid> &asteroids);
	void Draw(ALLEGRO_BITMAP *image, ALLEGRO_FONT *font);
	void CalculateFitness();
	void Reset();

	void AddVel(float _velX, float _velY) { velX += _velX; velY += _velY; }
	void AddVelDir(float _dir, float _speed) { velX += cos(_dir) * _speed; velY += sin(_dir) * _speed; }
	void SetMoving(bool val);
	void ChangeDir(int val) { moveDir = val; }
	void Shoot();
	void Reload();

	Genome *GetGenome();

	double Distance(Agent *other);
	Species * GetSpecies() const { return species; }
	void SetSpecies(Species * val) { species = val; }

	int GetFitness() const { return fitness; }
	int GetScore();
	float GetX() const { return x; }
	float GetY() const { return y; }
	float GetDir() { return dir; }
	float GetRadius() { return radius; }
	bool CanShoot() { return canShoot; }
	bool isAlive() { return alive; }
	bool ShouldShoot() { return shoot; }

	void SetPos(float x, float y) { this->SetX(x); this->SetY(y); }
	void SetX(float val) { x = val; }
	void SetY(float val) { y = val; }
	void Die() { this->alive = false; }
	void AddScore() { score++; }
	void AddHitCount() { hitCount++; }
	void SetPlayerControlled(bool val) { playerControlled = val; }
};

