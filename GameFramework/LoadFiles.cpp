#include "LoadFiles.h"

LoadFiles* LoadFiles::s_pInstance = nullptr;

// ªË¡¶
void LoadFiles::Clean()
{
	delete s_pInstance;
	s_pInstance = nullptr;
}