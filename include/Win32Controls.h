#ifndef WIN32CONTROLS_H
#define WIN32CONTROLS_H

#include <windows.h>

namespace TankTrouble {

#define CREATE_WIDGET		0x3000

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

	/**
	 * @brief 窗口信息结构体
	 */
	struct WidgetInfo {
		const wchar_t* type;
		const wchar_t* text;
		int style;
		int x,y;
		int width,height;
        HWND hwnd;
		HMENU Menu;
        HINSTANCE hInstance;
		LPARAM iParam;
		HWND Widget;
	};
	/**
	 * @brief 房间窗口类
	 */
	class RoomWidget {
	public:
		RoomWidget() = default;
		RoomWidget(
			HWND hwnd,
			int id,int MaxPlayers,int Players,
			int x, int y
		);

		void Hide();
        void Show();

	private:
		HWND hwndRoot;
		HWND hwndButtonJoin;
		HWND hwndEditId;
		HWND hwndEditPlayers;
	};

	/**
	 * @brief 控件信息
	 */
	struct ControlsInfo {
		long id;
		const wchar_t* text;
	};
	/**
	 * @brief 文本框控件信息
	 */
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
	static int RoomWidgetWidth = 200;
    static int RoomWidgetHeight = 300;

	//按钮
	extern HWND hwndButtonSingleGame;
	extern HWND hwndButtonOnlineGame;
	extern HWND hwndButtonCampaign;
	extern HWND hwndButtonBeginGame;
	extern HWND hwndButtonBack;
	extern HWND hwndRadioGroupPlayerNumber[10];
	extern HWND hwndRadioGroupMapType[10];
	extern HWND hwndRadioGroupTankColor[10];

	//文本框
	extern HWND hwndEditPlayerNumber;
	extern HWND hwndEditMapType;
	extern HWND hwndEditTankColor;

	//字体对象
    extern HFONT hFont;

	void menuShow(HWND hwnd);
	void menuHide(HWND hwnd);
	void selectionShow(HWND hwnd);
	void selectionHide(HWND hwnd);

	/**
	 * @brief 用于生成一组竖直方向排列的单选按钮,
	 * 其中，(x,y)为最左侧按钮的左上角坐标，(width,height)为控件大小
	 * ControlsInfo表示控件信息，text为按钮显示的文本信息
	 * @param hwnd			窗口句柄
	 * @param x				最左侧按钮左上角的横坐标
	 * @param y				最左侧按钮左上角的纵坐标
	 * @param width			窗口宽度
	 * @param height		窗口高度
	 * @param num			按钮数量
	 * @param info			按钮信息数组
	 * @param radioGroup	按钮句柄数组
	 */
	void CreateRadioGroupHorizontal(
		HWND hwnd, int x, int y, int width, int height,
		int num, ControlsInfo* info, HWND radioGroup[]
	);

	/**
	 * @brief 用于生成一组竖直方向排列的单选按钮,
	 * 其中，(x,y)为最左侧按钮的左上角坐标，(width,height)为控件大小
	 * ControlsInfo表示控件信息，text为按钮显示的文本信息
	 * @param hwnd			窗口句柄
	 * @param x				最上方按钮左上角的横坐标
	 * @param y				最上方按钮左上角的纵坐标
	 * @param width			窗口宽度
	 * @param height		窗口高度
	 * @param num			按钮数量
	 * @param info			按钮信息数组
	 * @param radioGroup	按钮句柄数组
	 */
	void CreateRadioGroupVertical(
		HWND hwnd, int x, int y, int width, int height,
		int num, ControlsInfo* info, HWND radioGroup[]
	);

}

#endif
