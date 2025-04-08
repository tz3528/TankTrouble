#pragma once
#define WIN32_LEAN_AND_MEAN

#include "tools/Container.hpp"
#include "tools/geometry.h"
#include "tools/Monitor.hpp"
#include "tools/TimerManager.hpp"
#include "tools/ThreadPool.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <cstdio>

#include <memory>
#include <mutex>
#include <condition_variable>
#include <shared_mutex>
#include <list>
#include <vector>
#include <atomic>



using std::shared_lock, std::shared_mutex, std::unique_lock;
using std::condition_variable;
using std::list,std::vector;

namespace TankTrouble{

	static int WindowWidth = 1280;
	static int WindowHeight = 720;

	//边缘四面墙的坐标
	extern int LeftWall, RightWall, UpWall, BottomWall;

}


