#pragma once

#include "Object.h"

class Block : public Object
{
private:
	static int refcount;

private:
	int idRef;
	Collision * collision;
	Sprite * sprite;

public:
	Block(SpriteInfo * spriteInfo);
	~Block();

	int IdRef() const;
	void SetSpriteInfo(SpriteInfo * spriteInfo);

	void Update(float gameTime);
	void Draw();
};