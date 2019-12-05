#pragma once

#include <allegro5/allegro_primitives.h>

#include <math.h>

#include "Agent.h"

class Bullet
{
	Agent *parent;
	float x, y;
	float dir, speed;
	int life;
	int radius;
	bool dead;

public:
	Bullet(float x, float y, float dir, Agent *parent);
	~Bullet();

	void Update();
	void Draw();

	bool IsDead() const { return dead; }
	Agent *GetParent() { return parent; }
	float GetX() { return x; }
	float GetY() { return y; }
	int GetRadius() { return radius; }

	void Die() { dead = true; }
};

