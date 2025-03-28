#pragma once

#ifndef ONLINE_GAME_H
#define ONLINE_GAME_H

#include "pch.h"
#include "Win32Controls.h"
#include "Window.h"
#include "Container.hpp"

namespace TankTrouble
{
//Socket–≈œ¢
#define SOCKET_INFO         0xFF00
#define SOCKET_heartbeat    (SOCKET_INFO + 0)
#define ROOM_LIST_INFO	    (SOCKET_INFO + 1)
#define ROOM_INFO			(SOCKET_INFO + 2)

#define ROOM_NUMBER			5

	struct RoomListInfo {
		int id;
		int maxPlayers;
        int Players;
	};
	
	LRESULT CALLBACK OnlineGameWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	int EstablishSocket();

	void onlineGameInit(HWND hwnd);

	int CtoI(const char*& str, int& len);
	inline int (*MessageType)(const char*& str, int& len) = CtoI;


	void Dispatch(HWND hwnd,int fd,const char* message, int len);

	void updateRoomList(HWND hwnd,const char* message, int len);
	void updateRoomInfo(HWND hwnd,const char* message, int len);

}

#endif
