#ifndef MAP_H
#define MAP_H

#include "pch.h"

#include <vector>
#include <memory>
#include <Windows.h>
#include <random>

using std::vector, std::shared_ptr, std::pair;
using std::shuffle, std::random_device, std::mt19937, std::uniform_int_distribution;

namespace TankTrouble
{

//地图大小的宏定义
#define SMALL_MAP			(0x2200 + 1)
#define MEDIUM_MAP			(0x2200 + 2)
#define LARGE_MAP			(0x2200 + 3)

//不同地图大小的行列数
#define SMALL_ROW			4
#define SMALL_COLUMN		5
#define MEDIUM_ROW			6
#define MEDIUM_COLUMN		8
#define LARGE_ROW			8
#define LARGE_COLUMN		10

	class Wall
	{
	public:
		Wall() = default;
		Wall(point u, point v, int size);
		~Wall();

		void draw(HDC hdcMem);

		double HalfWidth;
		point LeftUp, LeftDown, RightUp, RightDown;
	};

	const double ConnectionProbability = 1.0 / 3;

	extern random_device rd;
	extern mt19937 gen;
	extern uniform_int_distribution<> IntervalRN;

	/*位置结构体
	* 将原地图进行栅格化
	* 每个格的位置用其所在行和列表示
	*/
	struct GridPosition {
		int row, column;
	};

	extern int MapSize;
	extern int Row, Column;
	extern int permutation[110];
	extern int xGap, yGap;
	extern vector<shared_ptr<Wall>> WallPool;
	extern vector<vector<int>> edge;

	int PtoN(GridPosition pos);
	GridPosition NtoP(int num);
	int square(int x);
	point getCentrePoint(GridPosition pos);

	void addWall(GridPosition a, GridPosition b);
	void GenerateMap(int mapSize);
	
}

#endif // MAP_H
