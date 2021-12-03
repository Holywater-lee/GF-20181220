#include "TextureManager.h"
#include "SDL_image.h"
#include <iostream>
#include "Game.h"

TextureManager* TextureManager::s_pInstance = nullptr;

bool TextureManager::load(std::string fileName, std::string id)
{
	SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());

	if (pTempSurface == 0) return false;

	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(TheGame::Instance()->getRenderer(), pTempSurface);
	SDL_FreeSurface(pTempSurface);
	if (pTexture != 0)
	{
		m_textureMap[id] = pTexture;
		return true;
	}

	return false;
}

// UIObject가 사용
void TextureManager::draw(std::string id, int x, int y, int width, int height, SDL_RendererFlip flip)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;

	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = x;
	destRect.y = y;

	SDL_RenderCopyEx(TheGame::Instance()->getRenderer(), m_textureMap[id], &srcRect, &destRect, 0, 0, flip);
}

// 게임오브젝트들이 사용
void TextureManager::drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_RendererFlip flip)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;

	srcRect.x = width * currentFrame;
	srcRect.y = height * currentRow;
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = x;
	destRect.y = y;

	SDL_RenderCopyEx(TheGame::Instance()->getRenderer(), m_textureMap[id], &srcRect, &destRect, NULL, NULL, flip);
}

void TextureManager::drawBackground(std::string id)
{
	SDL_RenderCopy(TheGame::Instance()->getRenderer(), m_textureMap[id], NULL, NULL);
}

// 텍스쳐 삭제
void TextureManager::DestroyTex(std::string id)
{
	SDL_DestroyTexture(m_textureMap[id]);
	m_textureMap[id] = nullptr;
}

void TextureManager::Clean()
{
	delete s_pInstance;
	s_pInstance = nullptr;
}