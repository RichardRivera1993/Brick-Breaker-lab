#pragma once
#include "BaseObject.h"

class Box : public BaseObject
{
public:
	bool doubleThick = false;
	int width = 2;
	int height = 2;
	int hitCount = 0; // Adding this line to track the number of hits.
	void Draw() const override;
	bool Contains(int x, int y);
};