#ifndef WIN32CONTROLS_H
#define WIN32CONTROLS_H

#include <windows.h>

namespace TankTrouble {

//按钮的起始编号
#define BUTTON_BEGIN		0x2000

//按钮编号的宏定义
#define NOSELECT			(BUTTON_BEGIN + 0)
#define SINGLE_GAME			(BUTTON_BEGIN + 1)
#define ONLINE_GAME			(BUTTON_BEGIN + 2)
#define CAMPAIGN			(BUTTON_BEGIN + 3)
#define BEGIN_GAME			(BUTTON_BEGIN + 4)
#define BACK				(BUTTON_BEGIN + 5)

//文本框编号的宏定义
#define PLAYER_NUMBER		0x2010
#define MAP_TYPE			0x2011
#define TANK_COLOR			0x2012

	//控件信息
	struct ControlsInfo {
		long id;
		wchar_t* text;
	};
	//本本框控件的信息
	struct EditInfo {
		ControlsInfo ctrInfo;
		//文本框长度是根据Info中字符串的的长度得出的
		int Width;
	};

	//控件参数
	static int PushButtonWidth = 300;
    static int PushButtonHeight = 80;
	static int RadioButtonWidth = 100;
    static int RadioButtonHeight = 50;
	static int ButtonGap = 20;
	static int EditHeight = 30;

	//按钮
	static HWND hwndButtonSingleGame;
	static HWND hwndButtonOnlineGame;
	static HWND hwndButtonCampaign;
	static HWND hwndButtonBeginGame;
	static HWND hwndButtonBack;
	static HWND hwndRadioGroupPlayerNumber[10];
	static HWND hwndRadioGroupMapType[10];
	static HWND hwndRadioGroupTankColor[10];

	//文本框
	static HWND hwndEditPlayerNumber;
	static HWND hwndEditMapType;
	static HWND hwndEditTankColor;

	//字体对象
    extern HFONT hFont;

	/*
	* 用于生成一组水平方向排列的单选按钮
	* 其中，(x,y)为最左侧按钮的左上角坐标，(width,height)为控件大小
	* ControlsInfo表示控件信息，text为按钮显示的文本信息
	*/
	void CreateRadioGroupHorizontal(
		HWND hwnd, int x, int y, int width, int height,
		int num, ControlsInfo* info, HWND radioGroup[]
	);

	/*
	* 用于生成一组竖直方向排列的单选按钮
	* 其中，(x,y)为最左侧按钮的左上角坐标，(width,height)为控件大小
	* ControlsInfo表示控件信息，text为按钮显示的文本信息
	*/
	void CreateRadioGroupVertical(
		HWND hwnd, int x, int y, int width, int height,
		int num, ControlsInfo* info, HWND radioGroup[]
	);

}

#endif
