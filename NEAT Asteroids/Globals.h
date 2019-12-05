#pragma once
#include <allegro5/allegro_font.h>
#include <random>
#include <math.h>

class Globals {
public:
	static const int screenWidth = 800;
	static const int screenHeight = 600;
	static const int wrapRadius = 40;
	static const float PI;

	static double Random() {
		return double(rand()) / RAND_MAX;
	}

	static float RandomDir() {
		return (rand() % 360 - 180) * (3.14159265358979 / 180);
	}

	static float GetDistance(float x1, float y1, float x2, float y2) {
		return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	}

	static float RandomNormal() {
		float amount = 16;
		double sum = 0;
		for (int i = 0; i < amount; i++)
			sum += Random();
		sum /= amount;
		return sum;
	}
};