#include "Portal.h"
#include "Main.h"

int Portal::refcount = 0;

void Portal::ResetIdRef()
{
	refcount = 0;
}

Portal::Portal(float width, float height, int idFile, float playerX, float playerY) : idFile(idFile), playerX(playerX), playerY(playerY), width(width), height(height)
{
	type = PORTAL;
	idRef = refcount++;
	collision = Components()->AddComponent<Collision>();
	collision->BoundingBox(rect = new Rect(height, width));
	direction = 0;
}

Portal::~Portal()
{
}

int Portal::IdRef() const
{
	return idRef;
}

float Portal::Width() const
{
	return rect->Width();
}

float Portal::Height() const
{
	return rect->Height();
}

void Portal::Width(float value)
{
	rect->Width(value);
}

void Portal::Height(float value)
{
	rect->Height(value);
}

float Portal::PlayerX() const
{
	return playerX;
}

float Portal::PlayerY() const
{
	return playerY;
}

void Portal::Direction(int direction)
{
	// (0 = UP, 1 = LEFT, 2 = RIGHT, 3 = DOWN, 4 = STOP)

	this->direction = direction;
}

int Portal::Direction() const
{
	return direction;
}

void Portal::PlayerStartPoint(float x, float y)
{
	playerX = x;
	playerY = y;
}

void Portal::LevelId(int value)
{
	idFile = value;
}

int Portal::LevelId() const
{
	return idFile;
}

void Portal::Update(float gameTime)
{
}

void Portal::Draw()
{

}

void Portal::OnCollisionEnter(Object * obj)
{
	if (obj->Type() == PLAYER)
	{
		stringstream text;
		text << "Level" << idFile << ".txt";

		// ((Main *)Engine::CurrentGame())->TryNewLevel(text.str(), playerX, playerY);
	}
}
