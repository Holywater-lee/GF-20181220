#include "ScoreManager.h"
#include "Game.h"

ScoreManager* ScoreManager::s_pInstance = nullptr;

// ���ھ� ��������
int ScoreManager::GetScore()
{
	return currentScore;
}

// ���� ����
void ScoreManager::SetScore(int score)
{
	currentScore = score;
	TheGame::Instance()->RefreshScore();
}

// ���� ���ϱ�
void ScoreManager::AddScore(int score)
{
	currentScore += score;
	TheGame::Instance()->RefreshScore();
	printf("currentScore: %d\n", currentScore);
}

// ����
void ScoreManager::Clean()
{
	delete s_pInstance;
	s_pInstance = nullptr;
}