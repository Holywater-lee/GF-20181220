#include "TextManager.h"
#include "Game.h"

TextManager* TextManager::s_pInstance = nullptr;

bool TextManager::InitFont(const char* fontFile, int fontSize)
{
	if (TTF_Init() < 0) { printf("오류: 폰트를 초기화할 수 없습니다. (%s)\n", TTF_GetError()); return false; }
	m_pFont = TTF_OpenFont(fontFile, fontSize);
	if (m_pFont == NULL) { printf("오류: 폰트를 열 수 없습니다. (%s)\n", TTF_GetError()); return false; }

	return true;
}

bool TextManager::LoadText(SDL_Color fgColor, SDL_Color bgColor, int x, int y, int width, int height, const char* textContents)
{
	SDL_Texture* t_texture;
	SDL_Surface* t_textSurface = TTF_RenderText_Shaded(m_pFont, textContents, fgColor, bgColor);

	if (t_textSurface == 0) return false;

	t_texture = SDL_CreateTextureFromSurface(TheGame::Instance()->getRenderer(), t_textSurface);
	SDL_FreeSurface(t_textSurface);

	if (t_texture == 0) return false;

	Text* tempText = new Text(x, y, width, height);
	tempText->SetTexture(t_texture);
	TheGame::Instance()->CreateText(tempText);

	return true;
}

void TextManager::RefreshIntText(Text* text, SDL_Color fgColor, SDL_Color bgColor, int contents)
{
	text->FreeTexture();

	SDL_Texture* t_texture;
	char tempContents[128];
	sprintf_s(tempContents, sizeof(tempContents), "%4d", contents);

	SDL_Surface* t_textSurface = TTF_RenderText_Blended(m_pFont, tempContents, fgColor);
	t_texture = SDL_CreateTextureFromSurface(TheGame::Instance()->getRenderer(), t_textSurface);
	SDL_FreeSurface(t_textSurface);

	text->SetTexture(t_texture);
}

Text* TextManager::LoadIntToText(SDL_Color fgColor, SDL_Color bgColor, int x, int y, int width, int height, int contents)
{
	SDL_Texture* t_texture;
	char tempContents[128];
	sprintf_s(tempContents, sizeof(tempContents), "%4d", contents);

	SDL_Surface* t_textSurface = TTF_RenderText_Blended(m_pFont, tempContents, fgColor);

	if (t_textSurface == 0) return 0;

	t_texture = SDL_CreateTextureFromSurface(TheGame::Instance()->getRenderer(), t_textSurface);
	SDL_FreeSurface(t_textSurface);

	if (t_texture == 0) return 0;

	Text* tempText = new Text(x, y, width, height);
	tempText->SetTexture(t_texture);

	return tempText;
}

bool TextManager::LoadHanguelText(SDL_Color fgColor, SDL_Color bgColor, int x, int y, int width, int height, const wchar_t* textContents)
{
	SDL_Texture* t_texture;
	SDL_Surface* t_textSurface = TTF_RenderUNICODE_Shaded(m_pFont, (Uint16*)textContents, fgColor, bgColor);

	if (t_textSurface == 0) return false;

	t_texture = SDL_CreateTextureFromSurface(TheGame::Instance()->getRenderer(), t_textSurface);
	SDL_FreeSurface(t_textSurface);

	if (t_texture == 0) return false;

	Text* tempText = new Text(x, y, width, height);
	tempText->SetTexture(t_texture);
	TheGame::Instance()->CreateText(tempText);

	return true;
}

void TextManager::clean()
{
	TTF_CloseFont(m_pFont);
	TTF_Quit();
}