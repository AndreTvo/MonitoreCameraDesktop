#include "Container.h"

unordered_map<type_index, Component *>& Container::Data()
{
	return container;
}

Container::Container(Object * pOwner) : pOwner(pOwner)
{
}

Container::~Container()
{
}
