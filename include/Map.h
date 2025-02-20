#ifndef WALL_H
#define WALL_H

#include "point.h"

#include <list>
#include <memory>
#include <Windows.h>
#include <random>

using std::list, std::shared_ptr, std::pair;
using std::shuffle, std::random_device, std::mt19937;

namespace TankTrouble
{

//地图大小的宏定义
#define SMALL_MAP			(0x2200 + 1)
#define MEDIUM_MAP			(0x2200 + 2)
#define LARGE_MAP			(0x2200 + 3)

//不同地图大小的行列数
#define SMALL_ROW			4
#define SMALL_COLUMN		5
#define MEDIUM_ROW			8
#define MEDIUM_COLUMN		10
#define LARGE_ROW			12
#define LARGE_COLUMN		15

	class Wall
	{
	public:
		Wall(point u, point v, int size);
		~Wall();

		void draw(HDC hdcMem);

		double HalfWidth;
		point LeftUp, LeftDown, RightUp, RightDown;
	};

	const double ConnectionProbability = 1.0 / 3;

	static random_device rd;

	/*位置结构体
	* 将原地图进行栅格化
	* 每个格的位置用其所在行和列表示
	*/
	struct position {
		int row, column;
	};

	static int Row, Column;
	static int xGap, yGap;
	static int beginNum;

	extern list<shared_ptr<Wall>> WallPool;

	int PtoN(position pos);
    position NtoP(int num);
	int square(int x);

	void addWall(position a, position b);
	void GenerateMap(int mapSize);
	
}

#endif // WALL_H
