#pragma once

#include "Object.h"

class Portal : public Object
{
private:
	static int refcount;

public:
	static void ResetIdRef();

private:
	int idRef;
	Collision * collision;
	Rect * rect;

	float playerPosition;
	float playerX, playerY;
	float width;
	float height;
	int direction;
	int idFile;

public:
	Portal(float width, float height, int idFile, float playerX, float playerY);
	~Portal();

	int IdRef() const;

	float Width() const;
	float Height() const;

	void Width(float value);
	void Height(float value);

	float PlayerX() const;
	float PlayerY() const;

	void Direction(int direction);
	int Direction() const;

	void PlayerStartPoint(float x, float y);

	void LevelId(int value);
	int LevelId() const;

	void Update(float gameTime);
	void Draw();

	void OnCollisionEnter(Object * obj);
};