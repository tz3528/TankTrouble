#ifndef AI_H
#define AI_H

#include "Tank.h"
#include "Window.h"
#include "Map.h"

#include <queue>
#include <memory>

using std::vector;
using std::shared_ptr;

namespace TankTrouble
{
	void AIControl(shared_ptr<Tank> Computer);
	vector<int> JudgmentMove(shared_ptr<Tank> Computer);
	void JudgmentAttack(shared_ptr<Tank> Computer, vector<int> path);

}

#endif
