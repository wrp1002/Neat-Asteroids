#pragma once

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include <math.h>

#include "Neat.h"
#include "Agent.h"
#include "Genome.h"
#include "Globals.h"
#include "Asteroid.h"
#include "Bullet.h"

using namespace std;

class GameManager
{
private:
	int cycles;
	Agent *ship;
	ALLEGRO_BITMAP *shipImage;
	ALLEGRO_FONT *font;
	ALLEGRO_FONT *genomeFont;

	vector<Asteroid> asteroids;
	vector<Bullet> bullets;
	const unsigned int maxAsteroids = 30;
	bool done;
	bool playerControlled;
	bool spawnAsteroids;

	enum ASTEROID_SIZES { BIG = 40, MEDIUM = 20, SMALL = 10 };

public:
	GameManager();
	GameManager(Agent *newAgent);
	~GameManager();
	void Reset();
	void Reset(Agent *newAgent);

	void NewAsteroid();
	void SetKey(int key, bool pressed);
	void Update();
	void Collide();
	void Draw();
	void Cleanup();

	void SetPlayerControlled(bool val);
	void SetSpawnAsteroids(bool val);

	bool Done();
};

