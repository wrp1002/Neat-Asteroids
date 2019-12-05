#pragma once

#include <allegro5/allegro_primitives.h>

#include <math.h>

#include "Globals.h"

class Asteroid
{
private:
	float x, y;
	float velX, velY;
	int radius;
	bool alive;

public:
	Asteroid(float x, float y, float dir, float speed, int radius);
	~Asteroid();

	void Update();
	void Draw();
	void Die();

	float GetX() { return x; }
	float GetY() { return y; }
	int GetRadius() { return radius; }
	bool IsDead() { return !alive; }
};

