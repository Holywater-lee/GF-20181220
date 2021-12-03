#include "UIObject.h"
#include "TextureManager.h"
#include "Game.h"

UIObject::UIObject(const LoaderParams* pParams) : x(pParams->getX()), y(pParams->getY()), width(pParams->getWidth()), height(pParams->getHeight()), textureID(pParams->getTextureID())
{

}

// �������� ���� �������� �����Ƿ� drawFrame�� �ƴ� draw ���
void UIObject::Draw()
{
	TheTextureManager::Instance()->draw(textureID, x, y, width, height, TheGame::Instance()->getRenderer());
}