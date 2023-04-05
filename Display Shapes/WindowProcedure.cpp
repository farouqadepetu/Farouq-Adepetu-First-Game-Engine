#include "WindowProcedure.h"
#include "DisplayShapesGlobalVariables.h"

using namespace GlobalVariables;

namespace WindowProc
{
	//Window Procedure
	LRESULT DisplayShapesWindowProc(HWND windowHandle, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			case WM_ACTIVATE: //handles the events where the window gets activated/deactivated.
			{
				if (LOWORD(wParam) == WA_INACTIVE)
				{
					isAppPaused = true;
					frameTime.Stop();
				}
				else
				{
					isAppPaused = false;
					frameTime.Start();

				}
				return 0;
			}

			case WM_SIZE: //when the window gets resized.
			{
				if (scene != nullptr && window != nullptr)
				{
					window->SetWidth(LOWORD(lParam));
					window->SetHeight(HIWORD(lParam));

					if (wParam == SIZE_MINIMIZED) //window gets minimized
					{
						isAppPaused = true;
						isMinimized = true;
						isMaximized = false;
					}
					if (wParam == SIZE_MAXIMIZED) //window gets maximized
					{
						isAppPaused = false;
						isMinimized = false;
						isMaximized = true;
						scene->Resize(window->GetWidth(), window->GetHeight(), windowHandle,
							isMSAAEnabled, isTextEnabled);
					}
					if (wParam == SIZE_RESTORED)
					{
						//restoring from a minimized state
						if (isMinimized)
						{
							isAppPaused = false;
							isMinimized = false;
							scene->Resize(window->GetWidth(), window->GetHeight(), windowHandle,
								isMSAAEnabled, isTextEnabled);
						}
						//restoring from a maximized state
						else if (isMaximized)
						{
							isMaximized = false;
							scene->Resize(window->GetWidth(), window->GetHeight(), windowHandle,
								isMSAAEnabled, isTextEnabled);
						}
						//will resize when the resize bars are released.
						else if (!isResizing)
						{
							scene->Resize(window->GetWidth(), window->GetHeight(), windowHandle,
								isMSAAEnabled, isTextEnabled);
						}
					}
				}

				return 0;
			}

			//when user grabs resize bars.
			case WM_ENTERSIZEMOVE:
			{
				isAppPaused = true;
				isResizing = true;
				frameTime.Stop();
				return 0;
			}

			//when the user releases the resize bars.
			case WM_EXITSIZEMOVE:
			{
				isAppPaused = false;
				isResizing = false;
				frameTime.Start();
				scene->Resize(window->GetWidth(), window->GetHeight(), windowHandle,
					isMSAAEnabled, isTextEnabled);
				return 0;
			}

			case WM_CHAR: //if they user every presses a char key.
			{
				switch (wParam)
				{
				case '1':
					isMSAAEnabled = (isMSAAEnabled) ? false : true;
					scene->Resize(window->GetWidth(), window->GetHeight(), windowHandle,
						isMSAAEnabled, isTextEnabled);
					break;

				case '2':
					isTextEnabled = (isTextEnabled) ? false : true;
					scene->Resize(window->GetWidth(), window->GetHeight(), windowHandle,
						isMSAAEnabled, isTextEnabled);
					break;

				case '3':
					isSolid = (isSolid) ? false : true;
				}
				return 0;
			}

			case WM_DESTROY: //when the user exits the window.
			{
				PostQuitMessage(0);
				return 0;
			}
		}

		return DefWindowProc(windowHandle, uMsg, wParam, lParam); //handles events that weren't manually handled.
	}
}