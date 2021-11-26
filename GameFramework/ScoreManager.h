#pragma once
#include <string>

class ScoreManager
{
public:
	static ScoreManager* Instance()
	{
		if (s_pInstance == nullptr) s_pInstance = new ScoreManager();
		return s_pInstance;
	}
	~ScoreManager() {}

	int GetScore();
	void SetScore(int score);
	void AddScore(int score);
private:
	ScoreManager() {}

	int currentScore = 0;

	static ScoreManager* s_pInstance;
};
typedef ScoreManager TheScore;