#include "PlatformPlayer.h"
#include "Rigidbody.h"
#include "Object.h"
#include "Floor.h"

PlatformPlayer::~PlatformPlayer()
{
}

bool PlatformPlayer::Init()
{
	bGrounded = false;
	enabled = true;

	return true;
}

void PlatformPlayer::PostInit()
{
	collisionPlayer = pOwner->Components()->GetComponent<Collision>();
	rigidbodyPlayer = pOwner->Components()->GetComponent<Rigidbody>();

	bboxPlayer = (Rect*)collisionPlayer->BoundingBox();
	bboxTop = Rect(2, bboxPlayer->Width() - 1);
	bboxBottom = Rect(2, bboxPlayer->Width() - 1);
}

void PlatformPlayer::Update(float gameTime)
{
	UpdateCollisionFlag();
}

void PlatformPlayer::Draw()
{
}

void PlatformPlayer::Gravity(bool value)
{
	bGravity = value;
}

bool PlatformPlayer::Gravity() const
{
	return bGravity;
}

void PlatformPlayer::UpdateGroundFlag()
{
	bNoPlatform = true;

	if (collisionPlayer->IsColliding())
	{
		bboxBottom.MoveTo(bboxPlayer->X(), bboxPlayer->Y() + bboxPlayer->Height() / 2);

		if (collisionPlayer->CollisorTable()->size() > 0)
		{
			for (auto it : *collisionPlayer->CollisorTable())
			{
				if (it->Type() == PLATFORM)
				{
					bNoPlatform = false;

					Rect * bbox_collisor = (Rect *)it->Components()->GetComponent<Collision>()->BoundingBox();

					if (Engine::CurrentGame()->GetScene()->Collision((Geometry*)&bboxBottom, bbox_collisor, nullptr) && bboxPlayer->Bottom() <= bbox_collisor->Top())
					{
						bGrounded = true;
						groundObject = it;
						break;
					}
					else
					{
						bGrounded = false;
					}
				}
			}
		}
		else
		{
			bGrounded = false;
			groundObject = nullptr;
		}
	}
	else
	{
		bGrounded = false;
		groundObject = nullptr;
	}

	if (bNoPlatform)
	{
		bGrounded = false;
		groundObject = nullptr;
	}
}

void PlatformPlayer::UpdateCollisionFlag()
{
	UpdateGroundFlag();
}

Collision * PlatformPlayer::CollisionPlayer()
{
	return collisionPlayer;
}

Object * PlatformPlayer::GroundObject() const
{
	return groundObject;
}

bool PlatformPlayer::GroundCollision(Object * obj)
{
	bboxBottom.MoveTo(bboxPlayer->X(), bboxPlayer->Y() + bboxPlayer->Height() / 2);

	Geometry * bbox_collisor = obj->Components()->GetComponent<Collision>()->BoundingBox();

	if (Engine::CurrentGame()->GetScene()->Collision((Geometry*)&bboxBottom, bbox_collisor, nullptr) && bboxPlayer->Y() < bbox_collisor->Y())
		return true;

	return false;
	
}

bool PlatformPlayer::PerfectGroundCollision(Object * obj)
{
	bboxBottom.MoveTo(bboxPlayer->X(), bboxPlayer->Y() + bboxPlayer->Height() / 2);

	Rect * bbox_collisor = (Rect *)obj->Components()->GetComponent<Collision>()->BoundingBox();

	if (Engine::CurrentGame()->GetScene()->Collision((Geometry*)&bboxBottom, bbox_collisor, nullptr) && bboxPlayer->Bottom() <= bbox_collisor->Top())
		return true;

	return false;
}

void PlatformPlayer::SaveAccelerationVector(Vector2 vector)
{
	acceleration = vector;
}

void PlatformPlayer::ForceNotGrounded()
{
	bGrounded = false;
}

bool PlatformPlayer::IsGrounded() const
{
	return bGrounded;
}
