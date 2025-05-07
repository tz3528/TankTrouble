#include "OnlineGame/OnlineGame.h"

namespace TankTrouble
{
	RoomWidget Room[5];
	std::atomic<bool> selectRoom = false;

	int fd;
	bool OnlineGame = true;
	bool SocketHeartbeat = true;
	int Status;

	msg_header header;
	std::list<std::unique_ptr<MemoryBlock>> MessageCache;

	MemoryPool memoryPool;

	TankInfo tankInfo;

	LRESULT CALLBACK OnlineGameWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		WidgetInfo* params = (WidgetInfo*)lParam;
		
		switch (message)
		{
		case WM_COMMAND:
			buttonDown(hwnd,wParam);
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
			if (params != nullptr) {
				params->Widget = CreateWindow(
					params->type, params->text, params->style,
					params->x, params->y, params->width, params->height,
					params->hwnd, (HMENU)params->Menu,
					params->hInstance, nullptr
				);
				if (params->Widget == NULL) {
					DWORD errorCode = GetLastError();
					
					char tmp[256] = { 0 };
					HANDLE g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
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

	static void buttonDown(HWND hwnd, WPARAM wParam) {
		switch (LOWORD(wParam))
		{
		case BACK:
			switch (Status)
			{
			case SELECT_ROOM:
				OnlineGame = false;
				repickMode(hwnd);
				DisConnect();
				SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)StartWndProc);
				break;
			case IN_ROOM:
				selectionShow(hwnd);
				ShowWindow(hwndButtonBeginGame, SW_HIDE);
				ShowWindow(hwndButtonBack, SW_HIDE);
				RoomHide();
				SendRoomListInfo();
				break;
			}
			break;
		case BEGIN_GAME:
		{
			string message = toString(BEGIN_GAME);
			send(fd, message.c_str(), message.size(), 0);
			break;
		}
		default:
			Status = IN_ROOM;
			string message = toString(ROOM_INFO);
			message += toString(LOWORD(wParam));
			send(fd, message.c_str(), message.size(), 0);
			break;
		}
	}

	static void keyDown(HWND hwnd, WPARAM wParam) {
		if (Status != GAMING) return;
		switch (wParam)
		{
		case 'J':
			tankInfo.isAttack = true;
			break;
		case 'W':
            tankInfo.isForward = true;
			break;
		case 'S':
            tankInfo.isBackward = true;
			break;
		case 'A':
            tankInfo.isLeft = true;
			break;
		case 'D':
            tankInfo.isRight = true;
			break;
		default:
			break;
		}
	}

	static void keyUp(HWND hwnd, WPARAM wParam) {
		if (Status != GAMING) return;
		switch (wParam)
		{
		case 'J':
            tankInfo.isAttack = false;
			break;
		case 'W':
            tankInfo.isForward = false;
			break;
		case 'S':
            tankInfo.isBackward = false;
			break;
		case 'A':
            tankInfo.isLeft = false;
			break;
		case 'D':
            tankInfo.isRight = false;
			break;
		default:
			break;
		}
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
		inet_pton(AF_INET, IP, &saddr.sin_addr);
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

	void DisConnect() {
		closesocket(fd);
		WSACleanup();
	}

	void onlineGameInit(HWND hwnd) {
		Status = SELECT_ROOM;
		OnlineGame = true;

		HANDLE g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		char tmp[256] = { 0 };
		sprintf_s(tmp, sizeof(tmp), "1\n");
		WriteConsoleA(g_hOutput, tmp, (DWORD)strlen(tmp), nullptr, nullptr);
	
		//1.初始化网络库
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			return;
		}

		selectRoom = true;

		fd = EstablishSocket();

		timerManager.addTask(SOCKET_HEARTBEAT, 5000, SendSocketHeartbeat);

		send(fd, toString(ROOM_LIST_INFO).c_str(), toString(ROOM_LIST_INFO).size(), 0);

		while (OnlineGame) {
			char message[1024] = { 0 };
			int len = recv(fd, message, sizeof(message), 0);
			if (len > 0) {
				SplicedMessage(hwnd,message, len);
			}
			else if (len == 0) {
				break;
			}
			else {
				int errorCode = WSAGetLastError();
				char tmp[256] = { 0 };
				HANDLE g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
				sprintf_s(tmp, sizeof(tmp), "%d\n", errorCode);
				WriteConsoleA(g_hOutput, tmp, (DWORD)strlen(tmp), nullptr, nullptr);
				break;
			}
		}

		closesocket(fd);

	}

	void SplicedMessage(HWND hwnd,const char* message, int len) {
		HANDLE g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		char tmp[256] = { 0 };
		sprintf_s(tmp, sizeof(tmp), "recv message.len:%d\n",len);
		WriteConsoleA(g_hOutput, tmp, (DWORD)strlen(tmp), nullptr, nullptr);
	
		while (len > 0) {
			//先获取包头
			if (header.bodySize == -1) {
				//没有获取过包头
				if (MessageCache.empty()) {
					//如果消息的长度足够一个包头
					if (len >= sizeof(msg_header)) {
                        header.bodySize = CtoI(message, len);
					}
					//消息不够一个包头的长度
					else {
						auto block = memoryPool.allocate();
						memcpy(block->data.get(), message, len);
						block->current_size = len;
						MessageCache.emplace_back(std::move(block));
						return ;
					}
				}
				//上一次获取了包头，但是不够包头的长度
				else {
					//仍然不够长
					auto& frontBlock = MessageCache.front();
					size_t needed = sizeof(msg_header) - frontBlock->current_size;
					if (len < needed) {
						memcpy(frontBlock->data.get() + frontBlock->current_size, message, len);
						frontBlock->current_size += len;
						return ;
					}
					else {
						memcpy(frontBlock->data.get() + frontBlock->current_size, message, needed);
						message += needed;
						len -= needed;
						const char* tmp = reinterpret_cast<const char*>(frontBlock->data.get());

						MessageCache.pop_front();
						header.bodySize = CtoI(tmp, len);
					}
				}
			}

			int maxLength = min(len, header.bodySize);

			if (!MessageCache.empty() && MessageCache.back()->remain() > 0) {
				if (MessageCache.back()->remain() <= maxLength) {
					maxLength = MessageCache.back()->remain();
				}
				memcpy(MessageCache.back()->data.get() + MessageCache.back()->current_size, message, maxLength);
				MessageCache.back()->current_size += maxLength;
				len -= maxLength;
				header.bodySize -= maxLength;
				message += maxLength;
			}			

			sprintf_s(tmp, sizeof(tmp), "bodysize len:%d %d\n",header.bodySize,len);
			WriteConsoleA(g_hOutput, tmp, (DWORD)strlen(tmp), nullptr, nullptr);
	

			while (len > 0 && header.bodySize > 0) {
				maxLength = min(len, header.bodySize);
				auto block = memoryPool.allocate();
				if (maxLength > block->remain()) {
					maxLength = block->remain();
				}
				memcpy(block->data.get(), message, maxLength);
				block->current_size = maxLength;
				len -= block->current_size;
				message += block->current_size;
				header.bodySize -= block->current_size;
				MessageCache.emplace_back(std::move(block));
			}

			if (header.bodySize == 0) {
                header.bodySize = -1;
				size_t totalSize = 0;
				for (const auto& block : MessageCache) {
					totalSize += block->current_size;
				}
				char* result = new char[totalSize];
				char* current = result;

				while (!MessageCache.empty()) {
					memcpy(current, MessageCache.front()->data.get(), MessageCache.front()->current_size);
                    current += MessageCache.front()->current_size;
					MessageCache.pop_front();
				}
                Dispatch(hwnd, fd, result, totalSize);
			}
		}
	}

	void RoomShow() {
		for (int i = 0;i < 5;i++) {
			Room[i].Show();
		}
	}

	void RoomHide(){
		for (int i = 0;i < 5;i++) {
            Room[i].Hide();
		}
	}

	int CtoI(const char*& str, int& len) {
		len -= 4;
		int sum = (uint8_t)str[0] | ((uint8_t)str[1] << 8) | ((uint8_t)str[2] << 16) | ((uint8_t)str[3] << 24);
		str += 4;
		return sum;
	}

	void Dispatch(HWND hwnd, int fd,const char* message,int len){
		int type = MessageType(message, len);

		switch (type)
		{
		case SOCKET_HEARTBEAT:
			SocketHeartbeat = true;
			break;
		case ROOM_LIST_INFO:
			updateRoomList(hwnd, message, len);
			break;
		case ROOM_INFO:
			updateRoomInfo(hwnd, message, len);
			break;
		case BEGIN_GAME:
			BeginGame(hwnd,message,len);
			break;
		default:
			break;
		}
		
	}

	int SendSocketHeartbeat() {
		if (!SocketHeartbeat) {
			OnlineGame = false;
			return 0;
		}
        string heartbeat = toString(SOCKET_HEARTBEAT);
        send(fd, heartbeat.c_str(), heartbeat.size(), 0);
		return 5000;
	}

	void SendRoomListInfo(){
		std::string message = toString(ROOM_LIST_INFO);
		size_t result = send(fd, message.c_str(), message.size(), 0);
		if (result == SOCKET_ERROR) {
			int errorCode = WSAGetLastError();
			// 处理错误
			char tmp[256] = { 0 };
			HANDLE g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
			sprintf_s(tmp, sizeof(tmp), "%d\n", errorCode);
			WriteConsoleA(g_hOutput, tmp, (DWORD)strlen(tmp), nullptr, nullptr);
		}
	}

	void SendRoomInfo(int id){
		string message = toString(ROOM_INFO)+toString(id);
        send(fd, message.c_str(), message.size(), 0);
	}

	void SendTankStatus() {
		string message = toString(TANK_STATUS)+toString(tankInfo);
		send(fd, message.c_str(), message.size(), 0);
	}

	void updateRoomList(HWND hwnd,const char* message,int len) {
		int num = CtoI(message,len);

		HANDLE g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		char tmp[256] = { 0 };
		sprintf_s(tmp, sizeof(tmp), "room count:%d\n",num);
		WriteConsoleA(g_hOutput, tmp, (DWORD)strlen(tmp), nullptr, nullptr);
	

		int gap = (WindowWidth - num * 200) / (num + 1);
		int left = gap;

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
		int Players = CtoI(message, len);
		SendMessage(hwndRadioGroupPlayerNumber[Players], BM_SETCHECK, BST_CHECKED, 0);
		int Map = CtoI(message, len);
        SendMessage(hwndRadioGroupMapType[Map], BM_SETCHECK, BST_CHECKED, 0);


	}

	void BeginGame(HWND hwnd,const char* message, int len) {
		selectionShow(hwnd);
		ShowWindow(hwndButtonBeginGame, SW_HIDE);
		ShowWindow(hwndButtonBack, SW_HIDE);

		int WallNum = CtoI(message, len);
		for (int i = 0;i < WallNum;i++) {
			//WallPool.emplace_back(toObject<Wall>(message));
		}

		int TankNum = CtoI(message, len);
        for (int i = 0;i < TankNum;i++) {
			//TankPool.emplace_back(toObject<Tank>(message));
		}

		timerManager.addTask(TANK_STATUS, 40, []() {
			
            SendTankStatus();
			if (Status == GAMING) {
				return 40;
			}
			return 0;
			});
	}

}
