#include "GameManager.h"



GameManager::GameManager() {
	shipImage = al_load_bitmap("Resources/ship.png");
	font = al_load_font("Resources/Minecraft.ttf", 12, NULL);
	genomeFont = al_load_font("Resources/arial.ttf", 8, NULL);

	Reset();
}

GameManager::GameManager(Agent * newAgent) {
	shipImage = al_load_bitmap("Resources/ship.png");
	font = al_load_font("Resources/Minecraft.ttf", 12, NULL);
	genomeFont = al_load_font("Resources/arial.ttf", 8, NULL);

	Reset(newAgent);
}


GameManager::~GameManager() {
	al_destroy_bitmap(shipImage);
	al_destroy_font(font);
}

void GameManager::Reset() {
	cycles = 0;
	ship = nullptr;
	done = false;
	playerControlled = false;
	spawnAsteroids = true;

	asteroids.clear();
	bullets.clear();
}

void GameManager::Reset(Agent * newAgent) {
	Reset();

	ship = newAgent;
	ship->SetPos(Globals::screenWidth / 2, Globals::screenHeight / 2);

	int dist = 300;
	float dir = Globals::RandomDir();
	float x = ship->GetX() + cos(dir) * dist;
	float y = ship->GetY() + sin(dir) * dist;
	asteroids.push_back(Asteroid(x, y, dir + Globals::PI, 2, BIG));

	for (int i = 0; i < 5; i++)
		NewAsteroid();
}

void GameManager::NewAsteroid() {
	int pos = rand() % 4;
	int x = -BIG, y = -BIG;
	float speed = (rand() % 2000) * 0.001 + 1;

	//speed /= 10;

	if (pos == 0)
		x = rand() % Globals::screenWidth;
	else if (pos == 1)
		y = rand() % Globals::screenHeight;
	else if (pos == 2) {
		x = Globals::screenWidth + BIG;
		y = rand() % Globals::screenHeight;
	}
	else if (pos == 3) {
		x = rand() % Globals::screenWidth;
		y = Globals::screenHeight + BIG;
	}

	asteroids.push_back(Asteroid(x, y, Globals::RandomDir(), speed, rand() % 2 == 0 ? BIG : MEDIUM));
}

void GameManager::SetKey(int key, bool pressed) {
	if (!playerControlled)
		return;

	if (ship != nullptr) {
		if (key == ALLEGRO_KEY_UP)
			ship->SetMoving(pressed);
		else if (key == ALLEGRO_KEY_LEFT)
			ship->ChangeDir(-1 * pressed);
		else if (key == ALLEGRO_KEY_RIGHT)
			ship->ChangeDir(1 * pressed);
		else if (key == ALLEGRO_KEY_SPACE) {
			if (ship->CanShoot() && ship->isAlive() && pressed) {
				bullets.push_back(Bullet(ship->GetX(), ship->GetY(), ship->GetDir(), ship));
				ship->Shoot();
			}
		}
	}
}

void GameManager::Update() {
	for (auto &i : asteroids)
		i.Update();

	for (auto &i : bullets)
		i.Update();

	if (ship != nullptr) {
		ship->Update();
		Collide();
		if (ship->ShouldShoot()) {
			ship->Shoot();
			bullets.push_back(Bullet(ship->GetX(), ship->GetY(), ship->GetDir(), ship));
		}
	}


	Cleanup();

	if (spawnAsteroids && asteroids.size() < maxAsteroids && rand() % 100 == 0)
		NewAsteroid();

	cycles++;

	if (!ship->isAlive()) {
		done = true;
	}
}

void GameManager::Collide() {
	for (auto &asteroid : asteroids) {
		for (auto &bullet : bullets) {
			float distance = Globals::GetDistance(asteroid.GetX(), asteroid.GetY(), bullet.GetX(), bullet.GetY());
			if (distance <= asteroid.GetRadius() + bullet.GetRadius()) {
				bullet.GetParent()->AddScore();
				bullet.GetParent()->AddHitCount();
				bullet.Die();
				asteroid.Die();
			}
		}

		float distance = Globals::GetDistance(ship->GetX(), ship->GetY(), asteroid.GetX(), asteroid.GetY());
		if (distance <= ship->GetRadius() + asteroid.GetRadius()) {
			ship->Die();
			asteroid.Die();
		}
	}

	ship->UpdateEyes(asteroids);
}

void GameManager::Draw() {
	for (auto &i : asteroids)
		i.Draw();

	for (auto &i : bullets)
		i.Draw();

	if (ship != nullptr) {
		ship->Draw(shipImage, genomeFont);
		al_draw_textf(font, al_map_rgb(255, 255, 255), Globals::screenWidth / 2, 2, ALLEGRO_ALIGN_CENTER, "Score: %i", ship->GetScore());
	}
}

void GameManager::Cleanup() {
	for (int i = bullets.size() - 1; i >= 0; i--) {
		if (bullets[i].IsDead()) {
			bullets[i].GetParent()->Reload();
			bullets.erase(bullets.begin() + i);
		}
	}

	for (int i = asteroids.size() - 1; i >= 0; i--) {
		if (asteroids[i].IsDead()) {
			if (asteroids[i].GetRadius() > SMALL) {
				for (int j = 0; j < 2; j++) {
					int radius = asteroids[i].GetRadius() == BIG ? MEDIUM : SMALL;
					asteroids.push_back(Asteroid(asteroids[i].GetX(), asteroids[i].GetY(), Globals::RandomDir(), (rand() % 2000) * 0.001 + 1, radius));
				}
			}
			asteroids.erase(asteroids.begin() + i);
		}
	}

}

void GameManager::SetPlayerControlled(bool val) {
	playerControlled = val;
	if (ship != nullptr)
		ship->SetPlayerControlled(val);
}

void GameManager::SetSpawnAsteroids(bool val) {
	spawnAsteroids = val;
}

bool GameManager::Done()
{
	return done;
}
