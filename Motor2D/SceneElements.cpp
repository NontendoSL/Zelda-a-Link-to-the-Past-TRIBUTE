#include "SceneElements.h"

void SceneElement::SetPos(const iPoint& pos)
{
	position = { pos.x, pos.y };
}

int SceneElement::DropItem()
{
	return 0;
}
