#pragma once
#define WIN32_LEAN_AND_MEAN

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

using std::shared_lock, std::shared_mutex, std::unique_lock;
using std::condition_variable;
using std::list,std::vector;

