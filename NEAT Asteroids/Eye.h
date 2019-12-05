#pragma once
#include <allegro5/allegro_primitives.h>

#include <math.h>
#include <vector>

#include "Asteroid.h"
#include "Globals.h"

using namespace std;

class Eye
{
	float x, y;
	float dir, dirOffset;
	float dist, maxDist;
	float drawRadius, seeRadius;
	int seeInterval;
	ALLEGRO_COLOR eyeColor;
	ALLEGRO_COLOR seeColor;

public:
	Eye(float x, float y, float dir);
	~Eye();

	bool Collides(Asteroid &asteroid, float checkX, float checkY);
	void Update(float x, float y, float _dir, vector<Asteroid> &asteroids);
	void Draw();

	float GetDistScaled() { return (dist - maxDist / 2) * 2 / maxDist; }
};

