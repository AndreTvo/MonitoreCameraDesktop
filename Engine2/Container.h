#pragma once

#include "Component.h"
#include <vector>
#include <unordered_map>
#include <typeindex>
using std::type_index;
using std::vector;
using std::unordered_map;

// Componentes conhecidos
#include "Collision.h"
#include "Position.h"
#include "Sprite.h"
#include "Rigidbody.h"
#include "Animation.h"
#include "Elevator.h"

class Container
{
	friend class Scene;

private:
	unordered_map<type_index, Component *> container;
	Object * pOwner;

	unordered_map<type_index, Component *> & Data();

public:
	Container(Object * pOwner);
	~Container();

	template <class TComponent>
	TComponent * GetComponent();

	template <class TComponent>
	TComponent * AddComponent();

	template <class TComponent>
	bool RemoveComponent();

	template <class TComponent>
	bool HasComponent();
};

template <class TComponent>
TComponent * Container::GetComponent()
{
	TComponent * component = nullptr;
	type_index index(typeid(TComponent));

	if (container.count(index))
		component = static_cast<TComponent *>(container[index]);

	return component;
}

template <class TComponent>
TComponent * Container::AddComponent()
{
	if (HasComponent<TComponent>())
		return nullptr;

	TComponent * tComponent = new TComponent();
	type_index index(typeid(TComponent));

	Component * component = static_cast<Component *>(tComponent);

	if (component->Init())
	{
		component->SetOwner(pOwner);
		component->PostInit();

		container[index] = component;
	}
	else
	{
		delete tComponent;
		tComponent = nullptr;
	}

	return tComponent;
}

template <class TComponent>
bool Container::RemoveComponent()
{
	type_index index(typeid(TComponent));

	if (container.count(index))
	{
		TComponent * component = static_cast<TComponent *>(container[index]);
		delete component;
		container.erase(index);
		return true;
	}

	return false;
}

template <class TComponent>
bool Container::HasComponent()
{
	type_index index(typeid(TComponent));

	if (container.count(index))
		return true;

	return false;
}