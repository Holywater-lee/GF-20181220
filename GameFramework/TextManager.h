#pragma once
#include "SDL.h"
#include "SDL_ttf.h"
#include "Text.h"

class TextManager
{
public:
	~TextManager() {}
	static TextManager* Instance()
	{
		if (s_pInstance == nullptr) s_pInstance = new TextManager();
		return s_pInstance;
	}
	bool InitFont(const char* fontFile, int fontSize);
	bool LoadHanguelText(SDL_Color fgColor, SDL_Color bgColor, int x, int y, int width, int height, const wchar_t* textContents, bool isUI = true);
	Text* LoadIntToText(SDL_Color fgColor, SDL_Color bgColor, int x, int y, int width, int height, int contents, bool isUI = true);
	void RefreshIntText(Text* text, SDL_Color fgColor, SDL_Color bgColor, int contents);
	void clean();

private:
	TextManager() {}
	TTF_Font* m_pFont = nullptr;

	static TextManager* s_pInstance;
};
typedef TextManager TheTextManager;