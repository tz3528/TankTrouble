#include "OnlineGame/OnlineGame.h"

namespace TankTrouble
{
	RoomWidget Room[5];
	std::atomic<bool> selectRoom = false;
	int fd;

	LRESULT CALLBACK OnlineGameWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WidgetInfo* params = (WidgetInfo*)lParam;
		HANDLE g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		char tmp[256] = { 0 };
		
		switch (message)
		{
		case WM_COMMAND:

			break;
		case WM_KEYDOWN:
			break;
		case WM_KEYUP:
			break;
		case WM_PAINT:
			paintGame(hwnd);
			break;
		case WM_ERASEBKGND:
		{
			return 1; // 告诉Windows消息已经被处理
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			closesocket(fd);
			break;
		case CREATE_WIDGET:
			sprintf_s(tmp, sizeof(tmp), "message:%d\n", message);
			WriteConsoleA(g_hOutput, tmp, (DWORD)strlen(tmp), nullptr, nullptr);
			if (params != nullptr) {
				params->Widget = CreateWindow(
					params->type, params->text, params->style,
					params->x, params->y, params->width, params->height,
					params->hwnd, (HMENU)params->Menu,
					params->hInstance, nullptr
				);
				if (params->Widget == NULL) {
					DWORD errorCode = GetLastError();
					sprintf_s(tmp, sizeof(tmp), "CreateWindow failed with error: %lu\n", errorCode);
					WriteConsoleA(g_hOutput, tmp, (DWORD)strlen(tmp), nullptr, nullptr);
				}
				if (params->iParam != 0) {
					SetEvent((HANDLE)params->iParam); // 设置事件
				}
			}
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		}

		return 0;
	}

	int EstablishSocket() {

		HANDLE g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		char tmp[256] = { 0 };
		sprintf_s(tmp, sizeof(tmp), "2\n");
		WriteConsoleA(g_hOutput, tmp, (DWORD)strlen(tmp), nullptr, nullptr);
		//2.创建通信的socket
		int fd = socket(AF_INET, SOCK_STREAM, 0);
		if (fd == -1) {
			return -1;
		}

		sprintf_s(tmp, sizeof(tmp), "3\n");
		WriteConsoleA(g_hOutput, tmp, (DWORD)strlen(tmp), nullptr, nullptr);
		//3.连接服务器IP和端口
		struct sockaddr_in saddr;
		saddr.sin_family = AF_INET;
		saddr.sin_port = htons(6666);
		inet_pton(AF_INET, "192.168.131.224", &saddr.sin_addr);
		int ret = connect(fd, (struct sockaddr*)&saddr, sizeof(saddr));
		if (ret == -1) {
			int errorCode = WSAGetLastError();
			char tmp[256] = { 0 };
			sprintf_s(tmp, sizeof(tmp), "Connect failed with error: %d\n", errorCode);
			WriteConsoleA(g_hOutput, tmp, (DWORD)strlen(tmp), nullptr, nullptr);
			return -1;
		}
		
		sprintf_s(tmp, sizeof(tmp), "fd:%d\n",fd);
		WriteConsoleA(g_hOutput, tmp, (DWORD)strlen(tmp), nullptr, nullptr);
		return fd;
	}

	void onlineGameInit(HWND hwnd) {
		char tmp[256] = { 0 };
		HANDLE g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		sprintf_s(tmp, sizeof(tmp), "1\n");
		WriteConsoleA(g_hOutput, tmp, (DWORD)strlen(tmp), nullptr, nullptr);
		
		//1.初始化网络库
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			return;
		}

		selectRoom = true;

		fd = EstablishSocket();

		string init = toString(ROOM_LIST_INFO);
		send(fd, init.c_str(), init.size(), 0);
		while (true) {
			char message[1024] = { 0 };
			int len = recv(fd, message, sizeof(message), 0);
			if (len > 0) {
				Dispatch(hwnd,fd,message,len);
			}
			else if (len == 0) {
				break;
			}
			else {
				int errorCode = WSAGetLastError();
				HANDLE g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
				sprintf_s(tmp, sizeof(tmp), "%d\n", errorCode);
				WriteConsoleA(g_hOutput, tmp, (DWORD)strlen(tmp), nullptr, nullptr);
				break;
			}
		}

		closesocket(fd);

	}

	int CtoI(const char*& str, int& len) {
		len -= 4;
		int sum = str[0] + (str[1] << 8) + (str[2] << 16) + (str[3] << 24);
		str += 4;
		return sum;
	}

	void Dispatch(HWND hwnd, int fd,const char* message,int len){
		switch (MessageType(message,len))
		{
		case ROOM_LIST_INFO:
			updateRoomList(hwnd, message, len);
			break;
		case ROOM_INFO:
			break;
		default:
			break;
		}
		
	}

	void updateRoomList(HWND hwnd,const char* message,int len) {
		int num = CtoI(message,len);

		int gap = (WindowWidth - num * 200) / (num + 1);
		int left = gap;

		HANDLE g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		char tmp[256] = { 0 };

		for (int i = 0;i < num;i++) {
			int id = CtoI(message,len);
			int maxPlayers = CtoI(message, len);
			int players = CtoI(message, len);
			Room[i] = RoomWidget(
				hwnd, id, maxPlayers, players,
				left, 200
			);
			left += gap + 200;
		}
	}

	void updateRoomInfo(HWND hwnd,const char* message, int len) {

	}

}
