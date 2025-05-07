#pragma once

#ifndef ONLINE_GAME_H
#define ONLINE_GAME_H

#include "pch.h"
#include "Win32Controls.h"
#include "Window.h"

namespace TankTrouble
{
#define IP					"192.168.1.118"

//Socket信息
#define SOCKET_INFO         0xFF00
#define SOCKET_HEARTBEAT    (SOCKET_INFO + 0)
#define ROOM_LIST_INFO	    (SOCKET_INFO + 1)
#define ROOM_INFO			(SOCKET_INFO + 2)
#define BEGIN_GAME			(SOCKET_INFO + 3)
#define TANK_STATUS			(SOCKET_INFO + 4)

#define ROOM_NUMBER			5

#define SELECT_ROOM			0xEE00
#define IN_ROOM				0xEE01
#define GAMING				0xEE02

	#pragma pack(push, 1)
	/**
	 * @brief 数据包包头
	 */
	struct msg_header {
		int32_t bodySize = -1;
	};
	#pragma pack(pop)

	/**
	 * @brief 房间列表显示的信息
	 */
	struct RoomListInfo{
		int id;
        int maxPlayers;
        int players;
	};

	struct TankInfo {
		bool isLife		= true;
		bool isAttack	= false;
		bool isForward	= false;
		bool isBackward = false;
		bool isLeft		= false;
		bool isRight	= false;
	};
	
	LRESULT CALLBACK OnlineGameWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	static void buttonDown(HWND hwnd, WPARAM wParam);

	static void keyDown(HWND hwnd, WPARAM wParam);

	static void keyUp(HWND hwnd, WPARAM wParam);

	int EstablishSocket();

	void DisConnect();
	/**
	 * @brief		初始化联机游戏资源，连接服务器
	 * @param hwnd 窗口句柄
	 */
	void onlineGameInit(HWND hwnd);
	
	void SplicedMessage(HWND hwnd,const char* message,int len);
	/**
	 * @brief 展示房间列表
	 */
	void RoomShow();
	/**
	 * @brief 隐藏房间列表
	 */
	void RoomHide();
	/**
	 * @brief		将前四个字符转换为一个int类型
	 * @param str	收到的信息
	 * @param len	字符串长度
	 * @return		
	 */
	int CtoI(const char*& str, int& len);
	inline int (*MessageType)(const char*& str, int& len) = CtoI;
	/**
	 * @brief 用于派发收到的消息
	 * @param hwnd		窗口句柄
	 * @param fd		文件描述符
	 * @param message	收到的消息
	 * @param len		消息长度
	 */
	void Dispatch(HWND hwnd,int fd,const char* message, int len);
	/**
	 * @brief	定时任务，用于发送心跳包
	 * @return	距离下一次发送心跳包的时间
	 */
	int SendSocketHeartbeat();

	void SendRoomListInfo();

	void SendRoomInfo(int id);

	void SendTankStatus();
	/**
	 * @brief			解析房间列表信息，并更新房间列表
	 * @param hwnd		窗口句柄
	 * @param message	房间列表信息
	 * @param len		信息长度
	 */
	void updateRoomList(HWND hwnd,const char* message, int len);
	/**
	 * @brief			解析房间状态信息，并更新房间状态
	 * @param hwnd		窗口句柄
	 * @param message	房间信息
	 * @param len		信息长度
	 */
	void updateRoomInfo(HWND hwnd,const char* message, int len);

	void BeginGame(HWND hwnd,const char* message, int len);

}

#endif
