#pragma once
#include <random> // �Ʒ� ��ɵ��� ����ϱ� ���� include

class GetRandom
{
public:
	// min ~ max ���� int���� ������ �Լ�
	static int GetRandomInt(int min, int max)
	{
		std::random_device rd;
		std::mt19937 mersenne(rd()); // �޸��� Ʈ�����Ͷ�� ����� ����� rd�κ��� �õ带 ��´�
		std::uniform_int_distribution<> value(min, max);
		return value(mersenne);
	}};