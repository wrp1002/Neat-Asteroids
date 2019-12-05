#include "Eye.h"



Eye::Eye(float x, float y, float dirOffset) {
	this->x = x;
	this->y = y;
	this->dirOffset = dirOffset;
	this->dir = this->dirOffset;

	eyeColor = al_map_rgb(0, 100, 255);
	seeColor = al_map_rgba(50, 100, 100, 50);

	dist = 0;
	maxDist = 300;

	drawRadius = 5;
	seeRadius = 10;
	seeInterval = 10;
}

Eye::~Eye()
{
}



bool Eye::Collides(Asteroid &asteroid, float checkX, float checkY) {
	float dist = Globals::GetDistance(asteroid.GetX(), asteroid.GetY(), checkX, checkY);
	if (dist < asteroid.GetRadius() + seeRadius)
		return true;
	return false;
}

void Eye::Update(float x, float y, float _dir, vector<Asteroid> &asteroids) {
	this->x = x;
	this->y = y;
	this->dir = _dir + this->dirOffset;

	float checkX = x;
	float checkY = y;

	for (dist = 0; dist < maxDist; dist += seeInterval) {
		checkX += cos(dir) * seeInterval;
		checkY += sin(dir) * seeInterval;

		if (checkX < -Globals::wrapRadius)
			checkX = Globals::screenWidth + Globals::wrapRadius;
		else if (checkX > Globals::screenWidth + Globals::wrapRadius)
			checkX = -Globals::wrapRadius;
		if (checkY < -Globals::wrapRadius)
			checkY = Globals::screenHeight + Globals::wrapRadius;
		else if (checkY > Globals::screenHeight + Globals::wrapRadius)
			checkY = -Globals::wrapRadius;

		for (auto asteroid : asteroids) {
			if (Collides(asteroid, checkX, checkY))
				return;
		}
	}
}

void Eye::Draw() {
	float dx = x;
	float dy = y;
	for (int i = 0; i < dist; i += seeInterval) {
		al_draw_line(dx, dy, dx + cos(dir) * seeInterval, dy + sin(dir) * seeInterval, seeColor, 1);
		dx += cos(dir) * seeInterval;
		dy += sin(dir) * seeInterval;

		if (dx < -Globals::wrapRadius)
			dx = Globals::screenWidth + Globals::wrapRadius;
		else if (dx > Globals::screenWidth + Globals::wrapRadius)
			dx = -Globals::wrapRadius;
		if (dy < -Globals::wrapRadius)
			dy = Globals::screenHeight + Globals::wrapRadius;
		else if (dy > Globals::screenHeight + Globals::wrapRadius)
			dy = -Globals::wrapRadius;
	}
}
