#ifndef WIN32CONTROLS_H
#define WIN32CONTROLS_H

#include <windows.h>

namespace TankTrouble {

	//�ؼ���Ϣ
	struct ControlsInfo {
		long id;
		wchar_t* text;
	};

	static int PushButtonWidth = 300;
    static int PushButtonHeight = 80;
	static int RadioButtonWidth = 150;
    static int RadioButtonHeight = 60;
	static int ButtonGap = 20;

	//��ť
	static HWND hwndButtonSingleGame;
	static HWND hwndButtonOnlineGame;
	static HWND hwndButtonCampaign;
	static HWND hwndButtonBeginGame;
	static HWND hwndButtonBack;
	static HWND hwndRadioGroupPlayerNumber[5];

	/*
	* ��������һ��ˮƽ�������еĵ�ѡ��ť
	* ���У�(x,y)Ϊ����ఴť�����Ͻ����꣬(width,height)Ϊ�ؼ���С
	* ControlsInfo��ʾ�ؼ���Ϣ��textΪ��ť��ʾ���ı���Ϣ
	*/
	void CreateRadioGroupHorizontal(
		HWND hwnd, int x, int y, int width, int height,
		int num, ControlsInfo* info, HWND radioGroup[]
	);

	/*
	* ��������һ����ֱ�������еĵ�ѡ��ť
	* ���У�(x,y)Ϊ����ఴť�����Ͻ����꣬(width,height)Ϊ�ؼ���С
	* ControlsInfo��ʾ�ؼ���Ϣ��textΪ��ť��ʾ���ı���Ϣ
	*/
	void CreateRadioGroupVertical(
		HWND hwnd, int x, int y, int width, int height,
		int num, ControlsInfo* info, HWND radioGroup[]
	);

}

#endif
