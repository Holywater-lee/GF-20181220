#include "LoadFiles.h"

LoadFiles* LoadFiles::s_pInstance = nullptr;

// ����
void LoadFiles::Clean()
{
	delete s_pInstance;
	s_pInstance = nullptr;
}