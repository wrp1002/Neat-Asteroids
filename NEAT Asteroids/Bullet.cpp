#include "Bullet.h"



Bullet::Bullet(float x, float y, float dir, Agent *parent) {
	this->parent = parent;
	this->x = x;
	this->y = y;
	this->dir = dir;
	this->speed = 7;
	this->life = life;
	this->dead = false;
	this->life = 50;
	this->radius = 3;
}


Bullet::~Bullet()
{
}

void Bullet::Update() {
	x += cos(dir) * speed;
	y += sin(dir) * speed;

	life--;
	if (life <= 0)
		Die();
}

void Bullet::Draw() {
	al_draw_filled_circle(x, y, radius, al_map_rgb(255, 0, 0));
}
