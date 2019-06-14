#pragma once
#include <iostream>
#include <conio.h>
#include <lmcons.h>

namespace System
{
	namespace Console
	{
		namespace Output
		{
			template<typename VarType>
			static void Write(VarType item)
			{
				std::cout << item;
			}

			template<typename VarType>
			static void WriteLine(VarType item)
			{
				std::cout << item << std::endl;
			}

			template<typename VarType>
			static void Write(VarType item, DWORD duration, int lenth)
			{
				for (int i = 0; i < lenth; i++)
				{
					Write(item[i]);
					Sleep(duration);
				}
			}

			static void AddLines();
			template<typename VarType>
			static void WriteLine(VarType item, DWORD duration, int lenth)
			{
				for (int i = 0; i < lenth; i++)
				{
					Write(item[i]);
					Sleep(duration);
				}
				AddLines();
			}

			static void AddLines()
			{
				std::cout << std::endl;
			}

			static void AddLines(short count)
			{
				for (short i = 0; i < count; i++)
				{
					std::cout << std::endl;
				}
			}

		}
		namespace Input
		{
			template<typename VarType>
			static void EnterL(VarType& var)
			{
				std::cin >> var;
			}

			namespace Keyboard
			{
				static bool IsKeyPressed()
				{
					return _kbhit();
				}

				static int ReadKey()
				{
					return _getch();
				}
			}
		}

		namespace
		{
			static void Refresh()
			{
				system("cls");
			}

			static int Pause() {
				return _getch();
			}

			template<typename VarType>
			static int Pause(VarType pauseText) {
				Output::Write(pauseText);
				return _getch();
			}
		}

		namespace Cursor
		{
			static void SetCursorVisible(bool visible)
			{
				int size = 20;
				HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
				CONSOLE_CURSOR_INFO lpCursor;
				lpCursor.bVisible = visible;
				lpCursor.dwSize = size;
				SetConsoleCursorInfo(consoleHandle, &lpCursor);
			}

			static void SetCursorPosition(int x, int y)
			{
				COORD CursorPosition;
				HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
				CursorPosition.X = x;
				CursorPosition.Y = y;
				SetConsoleCursorPosition(consoleHandle, CursorPosition);
			}
		}

		namespace Globalization
		{
			void SetRegion(std::string region)
			{
				setlocale(LC_ALL, region.c_str());
			}
		}

		namespace Window
		{

			static void Maximize()
			{
				ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
			}

			static void Minimize()
			{
				ShowWindow(GetConsoleWindow(), SW_MINIMIZE);
			}

			static void Hide()
			{
				ShowWindow(GetConsoleWindow(), SW_HIDE);
			}

			static void Show()
			{
				ShowWindow(GetConsoleWindow(), SW_SHOW);
			}

			static void ChangeScreenW10()
			{
				SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
			}

			static void SetScreenSize(unsigned int cols, unsigned int lines)
			{
				std::string command = "mode con cols=" + std::to_string(cols) + " lines=" + std::to_string(lines);
				system(command.c_str());
			}

			static void SetTitle(std::string title)
			{
				SetConsoleTitle(title.c_str());
			}
		}

		namespace Graphic
		{
			class ConsoleStyle
			{
			private:
				int _BACKGROUND_COLOR = 0;
				int _FOREGROUND_COLOR = 7;
				void _SET() {
					HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
					SetConsoleTextAttribute(hcon, (_FOREGROUND_COLOR + (_BACKGROUND_COLOR * 16)));
				}
			public:
				void SetBackground(int color)
				{
					_BACKGROUND_COLOR = color;
					_SET();
				}

				void SetForeground(int color)
				{
					_FOREGROUND_COLOR = color;
					_SET();
				}
			} _CONSOLE_STYLE_;

			namespace Color
			{
				enum Colors
				{
					Black = 0,
					DarkBlue = 1,
					DarkGreen = 2,
					DarkCyan = 3,
					DarkRed = 4,
					DarkMagenta = 5,
					DarkYellow = 6,
					Gray = 7,
					DarkGray = 8,
					Blue = 9,
					Green = 10,
					Cyan = 11,
					Red = 12,
					Magenta = 13,
					Yellow = 14,
					White = 15
				};
			}

			namespace Buffer
			{
				struct ConsoleLayout3Cell
				{
				public:
					Color::Colors BackgroundColorLayout;
					Color::Colors ForegroundColorLayout;
					char		  TextCharLayout;
				};

				class ConsoleLayout3Buffer2Coords
				{
				public:
					ConsoleLayout3Cell** BufferCellMass;
					int W;
					int H;
				public:

					ConsoleLayout3Cell** GetCellMass()
					{
						return BufferCellMass;
					}

					int GetWidth()
					{
						return W;
					}

					int GetHeight()
					{
						return H;
					}

					ConsoleLayout3Buffer2Coords(int bfWidth, int bfHeight)
					{
						W = bfWidth;
						H = bfHeight;

						BufferCellMass = new ConsoleLayout3Cell * [bfWidth];
						for (int i = 0; i < bfWidth; i++)
						{
							BufferCellMass[i] = new ConsoleLayout3Cell[bfHeight];
						}
					}
				};

				static void DrawBuffer2Coords(ConsoleLayout3Buffer2Coords* buffer2, ConsoleStyle* appStyle)
				{
					//Console::Refresh();
					for (int x = 0; x < buffer2->GetWidth(); x++)
						for (int y = 0; y < buffer2->GetHeight(); y++)
						{
							System::Console::Cursor::SetCursorPosition(x, y);

							appStyle->SetBackground(buffer2->BufferCellMass[x][y].BackgroundColorLayout);
							appStyle->SetForeground(buffer2->BufferCellMass[x][y].ForegroundColorLayout);
							System::Console::Output::Write(buffer2->BufferCellMass[x][y].TextCharLayout);
						}
				}
				/*
				void BufferAddText(ConsoleLayout3Buffer2Coords* buf, std::string text, int x, int y)
				{
					for (int i = 0; i < text.length(); i++)
					{
						buf->BufferCellMass[x + i][y].TextCharLayout = text[i];
					}
				}

				void BufferAddPixel(ConsoleLayout3Buffer2Coords* buf, ConsoleLayout3Cell cell, int x, int y)
				{
					buf->BufferCellMass[x][y] = cell;
				}

				void BufferAddLine(ConsoleLayout3Buffer2Coords* buf, ConsoleLayout3Cell cell, int x1, int y1, int x2, int y2)
				{
					const int deltaX = abs(x2 - x1);
					const int deltaY = abs(y2 - y1);
					const int signX = x1 < x2 ? 1 : -1;
					const int signY = y1 < y2 ? 1 : -1;

					int error = deltaX - deltaY;

					BufferAddPixel(buf, cell, x2, y2);
					while (x1 != x2 || y1 != y2)
					{
						BufferAddPixel(buf, cell, x1, y1);
						const int error2 = error * 2;

						if (error2 > -deltaY)
						{
							error -= deltaY;
							x1 += signX;
						}
						if (error2 < deltaX)
						{
							error += deltaX;
							y1 += signY;
						}
					}

				}

				void BufferAddFillCircle(ConsoleLayout3Buffer2Coords* buf, ConsoleLayout3Cell cell, int x1, int y1, int r)
				{
					int x = x1, y = y1;
					for (int y = -r; y <= r; y++)
						for (int x = -r; x <= r; x++)
							if (x * x + y * y <= r * r)
								BufferAddPixel(buf, cell, x1 + x, y1 + y);
				}

				void BufferAddTriangle(ConsoleLayout3Buffer2Coords* buf, ConsoleLayout3Cell cell, int x1, int y1, int x2, int y2, int x3, int y3)
				{
					BufferAddLine(buf, cell, x1, y1, x2, y2);
					BufferAddLine(buf, cell, x2, y2, x3, y3);
					BufferAddLine(buf, cell, x3, y3, x1, y1);
				}

				void FillBuffer(ConsoleLayout3Buffer2Coords* buf, char sym, Colors fore, Colors back)
				{
					for (int x = 0; x < buf->GetWidth(); x++)
					{
						for (int y = 0; y < buf->GetHeight(); y++)
						{
							buf->BufferCellMass[x][y].BackgroundColorLayout = back;
							buf->BufferCellMass[x][y].ForegroundColorLayout = fore;
							buf->BufferCellMass[x][y].TextCharLayout = sym;
						}
					}
				}
				*/
			}
			
		}
	}

	namespace Math
	{
		template<typename VarType>
		static VarType Absolute(VarType arg)
		{
			if (arg < 0)
				arg = -arg;
			return arg;
		}

		static const long double PI = 3.14159265358979323846;

		static const long double T = 3.14159265358979323846 * 2;

		static const long double E = 2.71828182845904523536;

		#define Min(num1, num2) ((num1) < (num2) ? num1 : num2)
		#define Max(num1, num2) ((num1) > (num2) ? num1 : num2)

	}

	#ifdef _WINDOWS_
	namespace Win32
	{
		namespace User
		{
			#ifdef _MBCS
			std::string UserName()
			{
					TCHAR username[UNLEN + 1];
					DWORD size = UNLEN + 1;
					GetUserName((TCHAR*)username, &size);
					std::string user = username;
					return user;
			}
			#endif
		}
	}
	#endif
}