#pragma once
#include <random> // GetRandomInt의 기능들을 사용하기 위해 include

class Utility
{
public:
	// min ~ max 중의 int값을 얻어오는 함수
	static int GetRandomInt(int min, int max)
	{
		std::random_device rd;
		std::mt19937 mersenne(rd()); // 메르센 트위스터라는 방법을 사용해 rd로부터 시드를 얻는다
		std::uniform_int_distribution<> value(min, max);
		return value(mersenne);
	}

	static int Lerp(int a, int b, float dampTime)
	{
		return (a + (b - a) * dampTime);
	}
};