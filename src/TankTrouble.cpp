#include <windows.h>
#include "Window.h"

using namespace TankTrouble;

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow) {

    return start(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

}
