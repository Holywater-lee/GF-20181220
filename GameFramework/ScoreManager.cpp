#include "ScoreManager.h"
#include "Game.h"

ScoreManager* ScoreManager::s_pInstance = nullptr;

// 스코어 가져오기
int ScoreManager::GetScore()
{
	return currentScore;
}

// 점수 설정
void ScoreManager::SetScore(int score)
{
	currentScore = score;
	TheGame::Instance()->RefreshScore();
}

// 점수 더하기
void ScoreManager::AddScore(int score)
{
	currentScore += score;
	TheGame::Instance()->RefreshScore();
	printf("currentScore: %d\n", currentScore);
}

// 삭제
void ScoreManager::Clean()
{
	delete s_pInstance;
	s_pInstance = nullptr;
}