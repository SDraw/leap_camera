#include "stdafx.h"

#include "Core/Core.h"

#ifdef _DEBUG
int main(void)
{
#else
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(pCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);
#endif

    Core *l_core = new Core();
    if(l_core->Initialize())
    {
        while(l_core->DoPulse());
    }
    l_core->Terminate();
    delete l_core;

    return 0;
}
