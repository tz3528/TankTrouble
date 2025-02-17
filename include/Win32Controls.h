#ifndef WIN32CONTROLS_H
#define WIN32CONTROLS_H

#include <windows.h>

namespace TankTrouble {

//��ť����ʼ���
#define BUTTON_BEGIN		0x2000

//��ť��ŵĺ궨��
#define NOSELECT			(BUTTON_BEGIN + 0)
#define SINGLE_GAME			(BUTTON_BEGIN + 1)
#define ONLINE_GAME			(BUTTON_BEGIN + 2)
#define CAMPAIGN			(BUTTON_BEGIN + 3)
#define BEGIN_GAME			(BUTTON_BEGIN + 4)
#define BACK				(BUTTON_BEGIN + 5)

//�ı����ŵĺ궨��
#define PLAYER_NUMBER		0x2010
#define MAP_TYPE			0x2011
#define TANK_COLOR			0x2012

	//�ؼ���Ϣ
	struct ControlsInfo {
		long id;
		wchar_t* text;
	};
	//������ؼ�����Ϣ
	struct EditInfo {
		ControlsInfo ctrInfo;
		//�ı��򳤶��Ǹ���Info���ַ����ĵĳ��ȵó���
		int Width;
	};

	//�ؼ�����
	static int PushButtonWidth = 300;
    static int PushButtonHeight = 80;
	static int RadioButtonWidth = 100;
    static int RadioButtonHeight = 50;
	static int ButtonGap = 20;
	static int EditHeight = 30;

	//��ť
	static HWND hwndButtonSingleGame;
	static HWND hwndButtonOnlineGame;
	static HWND hwndButtonCampaign;
	static HWND hwndButtonBeginGame;
	static HWND hwndButtonBack;
	static HWND hwndRadioGroupPlayerNumber[10];
	static HWND hwndRadioGroupMapType[10];
	static HWND hwndRadioGroupTankColor[10];

	//�ı���
	static HWND hwndEditPlayerNumber;
	static HWND hwndEditMapType;
	static HWND hwndEditTankColor;

	//�������
    extern HFONT hFont;

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
