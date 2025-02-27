#include "GameMode/OnlineGame.h"

namespace TankTrouble
{

	LRESULT CALLBACK OnlineGameWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_KEYDOWN:
			break;
		case WM_KEYUP:
			break;
		case WM_PAINT:
			break;
		case WM_ERASEBKGND:
		{
			return 1; // 告诉Windows消息已经被处理
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		return 0;
	}

	void onlineGameInit() {
		//1.创建通信的socket
		int fd = socket(AF_INET, SOCK_STREAM, 0);
		if (fd == -1) {
			return;
		}

		//2.连接服务器IP和端口
		struct sockaddr_in saddr;
		saddr.sin_family = AF_INET;
        saddr.sin_port = htons(6666);
		inet_pton(AF_INET, "192.168.10.224", &saddr.sin_addr);
		int ret = connect(fd, (struct sockaddr*)&saddr, sizeof(saddr));
		if (ret == -1) {
            return;
		}

		//3.通信
		while (1) {
			char buff[1024] = { 0 };
			sprintf(buff, "通信成功");
			send(fd, buff, strlen(buff) + 1, 0);

			memset(buff, 0, sizeof(buff));
			int len = recv(fd, buff, sizeof(buff), 0);
			if (len > 0) {

			}
			else if (len == 0) {
				break;
			}
			else {
				return;
			}
			Sleep(1);
		}

		closesocket(fd);
		return ;
	}

}
