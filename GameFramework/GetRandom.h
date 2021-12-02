#pragma once
#include <random>

class GetRandom
{
public:
	static int GetRandomInt(int min, int max)
	{
		std::random_device rd;
		std::mt19937 mersenne(rd()); // 메르센 트위스터라는 방법을 사용해 rd로부터 시드를 얻는다
		std::uniform_int_distribution<> value(min, max);
		return value(mersenne);
	}
};