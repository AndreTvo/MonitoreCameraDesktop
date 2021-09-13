#include "Block.h"

int Block::refcount = 0;

Block::Block(SpriteInfo * spriteInfo)
{
	idRef = refcount++;
	sprite = container->AddComponent<Sprite>();
	sprite->LoadSpriteInfo(spriteInfo);
	position->Depth(0);

	collision = Components()->AddComponent<Collision>();
	collision->BoundingBox(new Rect(8, 8));
	collision->Debug(false);

	type = BLOCK;
}

Block::~Block()
{
}

int Block::IdRef() const
{
	return idRef;
}

void Block::SetSpriteInfo(SpriteInfo * spriteInfo)
{
	sprite->LoadSpriteInfo(spriteInfo);
}

void Block::Update(float gameTime)
{
}

void Block::Draw()
{
}
