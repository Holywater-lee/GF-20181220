#include "ScoreManager.h"
#include "Game.h"

ScoreManager* ScoreManager::s_pInstance = nullptr;

int ScoreManager::GetScore()
{
	return currentScore;
}

void ScoreManager::SetScore(int score)
{
	currentScore = score;
	TheGame::Instance()->RefreshScore();
}

void ScoreManager::AddScore(int score)
{
	currentScore += score;
	TheGame::Instance()->RefreshScore();
	printf("currentScore: %d\n", currentScore);
}