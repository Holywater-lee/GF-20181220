#include "UIObject.h"
#include "TextureManager.h"
#include "Game.h"

UIObject::UIObject(const LoaderParams* pParams) : x(pParams->getX()), y(pParams->getY()), width(pParams->getWidth()), height(pParams->getHeight()), textureID(pParams->getTextureID())
{

}

// 프레임이 따로 존재하지 않으므로 drawFrame이 아닌 draw 사용
void UIObject::Draw()
{
	TheTextureManager::Instance()->draw(textureID, x, y, width, height, TheGame::Instance()->getRenderer());
}