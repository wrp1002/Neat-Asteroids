#include "History.h"


History::History(int high, int average, int median, int lowest, vector<Genome *> bestGenomes) {
	this->highScore = high;
	this->averageScore = average;
	this->medianScore = median;
	this->lowestScore = lowest;
	this->bestGenomes = bestGenomes;
}

History::~History()
{
}

int History::GetHighScore()
{
	return highScore;
}

int History::GetLowScore()
{
	return lowestScore;
}

int History::GetAverageScore()
{
	return averageScore;
}

int History::GetMedianScore()
{
	return medianScore;
}

vector<Genome *> History::GetBestGenomes()
{
	return bestGenomes;
}
