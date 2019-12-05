#include "Asteroid.h"






Asteroid::Asteroid(float x, float y, float dir, float speed, int radius) {
	this->x = x;
	this->y = y;
	this->velX = cos(dir) * speed;
	this->velY = sin(dir) * speed;
	this->radius = radius;
	alive = true;
}

Asteroid::~Asteroid()
{
}

void Asteroid::Update() {
	x += velX;
	y += velY;

	if (x < -Globals::wrapRadius)
		x = Globals::screenWidth + Globals::wrapRadius;
	else if (x > Globals::screenWidth + Globals::wrapRadius)
		x = -Globals::wrapRadius;
	if (y < -Globals::wrapRadius)
		y = Globals::screenHeight + Globals::wrapRadius;
	else if (y > Globals::screenHeight + Globals::wrapRadius)
		y = -Globals::wrapRadius;
}

void Asteroid::Draw() {
	al_draw_circle(x, y, radius, al_map_rgb(100, 100, 100), 2);
}

void Asteroid::Die() {
	this->alive = false;
}
