#ifndef WIN32CONTROLS_H
#define WIN32CONTROLS_H

#include <windows.h>

namespace TankTrouble {

	//控件信息
	struct ControlsInfo {
		long id;
		wchar_t* text;
	};

	static int PushButtonWidth = 300;
    static int PushButtonHeight = 80;
	static int RadioButtonWidth = 150;
    static int RadioButtonHeight = 60;
	static int ButtonGap = 20;

	//按钮
	static HWND hwndButtonSingleGame;
	static HWND hwndButtonOnlineGame;
	static HWND hwndButtonCampaign;
	static HWND hwndButtonBeginGame;
	static HWND hwndButtonBack;
	static HWND hwndRadioGroupPlayerNumber[5];

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
