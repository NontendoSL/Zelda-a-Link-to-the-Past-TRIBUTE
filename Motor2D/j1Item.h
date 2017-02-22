#pragma once
#ifndef __ITEMS_H_
#define __ITEMS_H_

#include "j1SceneElements.h"


class j1Item : public j1SceneElement
{
public:
	j1Item(iPoint position);

	~j1Item();

	bool Update();

	void Draw();

private:
	bool picked=false, equiped, equipable;
};

#endif