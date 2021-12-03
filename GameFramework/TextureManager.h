#pragma once

#include "SDL.h"
#include <string>
#include <map>

class TextureManager
{
public:
	static TextureManager* Instance()
	{
		if (s_pInstance == nullptr) s_pInstance = new TextureManager();
		return s_pInstance;
	}
	~TextureManager() {}

	bool load(std::string fileName, std::string id);
	void draw(std::string id, int x, int y, int width, int height, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void drawBackground(std::string id);
	void DestroyTex(std::string id);

	void Clean();

private:
	TextureManager() {}
	static TextureManager* s_pInstance;
	std::map<std::string, SDL_Texture*> m_textureMap;
};
typedef TextureManager TheTextureManager;