#pragma once

#include <vector>

#include "Genome.h"

using namespace std;

class History
{
private:
	int highScore;
	int averageScore;
	int medianScore;
	int lowestScore;
	vector<Genome *> bestGenomes;

public:
	History(int high, int average, int median, int lowest, vector<Genome *> bestGenomes);
	~History();

	int GetHighScore();
	int GetLowScore();
	int GetAverageScore();
	int GetMedianScore();
	vector<Genome *> GetBestGenomes();
};

