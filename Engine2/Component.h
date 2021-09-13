#pragma once

class Object;

class Component
{
	friend class Container;

protected:
	bool enabled;
	Object* pOwner;

private:
	void SetOwner(Object * pOwner);

public:
	virtual ~Component();

	virtual bool Init() = 0;
	virtual void PostInit();
	virtual void FixedUpdate();
	virtual void Update(float gameTime) = 0;
	virtual void Draw() = 0;

	virtual void OnCollision(Object * obj);
	virtual void OnCollisionEnter(Object * obj);
	virtual void OnCollisionStay(Object * obj);
	virtual void OnCollisionExit(Object * obj);

	virtual void Enabled(bool value);
	virtual bool Enabled() const;

};