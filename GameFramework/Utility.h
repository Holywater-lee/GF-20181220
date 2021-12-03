#pragma once
#include <random> // GetRandomInt�� ��ɵ��� ����ϱ� ���� include

class Utility
{
public:
	// min ~ max ���� int���� ������ �Լ�
	static int GetRandomInt(int min, int max)
	{
		std::random_device rd;
		std::mt19937 mersenne(rd()); // �޸��� Ʈ�����Ͷ�� ����� ����� rd�κ��� �õ带 ��´�
		std::uniform_int_distribution<> value(min, max);
		return value(mersenne);
	}

	static int Lerp(int a, int b, float dampTime)
	{
		return (a + (b - a) * dampTime);
	}
};