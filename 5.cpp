#include "stdafx.h"
#include "5.h"
#include <ctime>

using std::vector;
using std::wstring;
using std::to_wstring;

#define MAX_LOADSTRING 100

HINSTANCE hInst;

struct Coord {
	int x1, y1;
	int x2, y2;
};

BOOL                InitInstance(HINSTANCE, int);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	MainDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	DlgWin(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	DlgSet(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void				Line(HDC& hdc, int x1, int y1, int x2, int y2);
bool				GetFigurePlace(const int& x, const int& y, vector<vector<int>>& matrix, const int& crossZero);
bool				CheckWinner(vector<vector<int>>& matrix, const int& crossZero);
bool				Computer(vector<vector<int>>& matrix);
void				CheckDraw(HWND& hDlg, bool& gameover, int tapsCount, int& countDraws);
void				WinGameover(HWND hDlg, bool& gameover, int& countPlayerWins, int IDC);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY5));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   HWND hWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, MainDlg);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

int dim = 3;
bool cl = false;
std::wstring str;
INT_PTR CALLBACK MainDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	srand(time(NULL));
	UNREFERENCED_PARAMETER(lParam);
	static vector<vector<int>> matrix(dim, vector<int>(dim, 0));
	static bool gameover = false, computer = false;
	static int tapsCount = 0, count1PlayerWins = 0, count2PlayerWins = 0, countDraws = 0;
	static COLORREF linesColor = RGB(0, 0, 200), crossColor = RGB(130, 130, 130), circleColor = RGB(100, 200, 0);
	int x, y;
	bool tapWas;

	switch (message)
	{
	case WM_INITDIALOG:
		SetTimer(hDlg, 1, 100, NULL);
		return (INT_PTR)TRUE;
	case WM_TIMER:
		InvalidateRect(hDlg, NULL, 1);
		break;
	case WM_LBUTTONDOWN: {
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		tapWas = GetFigurePlace(x, y, matrix, 1);
		if (tapWas) {
			++tapsCount;
			gameover = CheckWinner(matrix, 1);
			WinGameover(hDlg, gameover, count1PlayerWins, IDC_STATIC_1WINER);
			CheckDraw(hDlg, gameover, tapsCount, countDraws);
		}
		if (computer && tapWas && !gameover) {
			tapWas = Computer(matrix);
			if (tapWas) {
				++tapsCount;
			}
			gameover = CheckWinner(matrix, 2);
			WinGameover(hDlg, gameover, count2PlayerWins, IDC_STATIC_2WINER);
		}
		CheckDraw(hDlg, gameover, tapsCount, countDraws);
		break; 
	}
	case WM_RBUTTONDOWN:
		if (!computer) {
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			tapWas = GetFigurePlace(x, y, matrix, 2);
			if (tapWas) {
				++tapsCount;
				gameover = CheckWinner(matrix, 2);
				WinGameover(hDlg, gameover, count2PlayerWins, IDC_STATIC_2WINER);
				CheckDraw(hDlg, gameover, tapsCount, countDraws);
			}
		}
		break;
	case WM_COMMAND:
		{
		    int wmId = LOWORD(wParam);
		    switch (wmId)
		    {
			case ID_GAME_EXIT:
			case IDCANCEL:
				DestroyWindow(hDlg);
				break;
			case ID_SETTINGS:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_SETTINGS), hDlg, DlgSet);
				break;
			case ID_NEW_GAME:
				cl = true;
				break;
			case ID_ENEMY_HUMAN:
				computer = false;
				break;
			case ID_ENEMY_COMPUTER:
				computer = true;
				break;
			case IDC_RADIO1:
				linesColor = RGB(200, 0, 0);
				break;
			case IDC_RADIO2:
				linesColor = RGB(0, 0, 200);
				break;
			case IDC_RADIO3:
				linesColor = RGB(0, 200, 0);
				break;
			case IDC_RADIO4:
				crossColor = RGB(130, 130, 130);
				circleColor = RGB(100, 200, 0);
				break;
			case IDC_RADIO5:
				crossColor = RGB(0, 0, 200);
				circleColor = RGB(130, 130, 130);
				break;
			case IDC_RADIO6:
				crossColor = RGB(200, 0, 0);
				circleColor = RGB(0, 0, 200);
				break;
		    case IDM_ABOUT:
		        DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hDlg, About);
		        break;
		    case IDM_EXIT:
		        DestroyWindow(hDlg);
		        break;
		    }
		}
		break;
	case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hDlg, &ps);
			HPEN linePen = CreatePen(0, 5, linesColor), crossPen = CreatePen(0, 5, crossColor),
				circlePen = CreatePen(0, 5, circleColor);
			if (cl) {
				matrix.clear();
				matrix.resize(dim, vector<int>(dim, 0));
				cl = false;
				gameover = false;
				tapsCount = 0;
			}
			// vertical lines
			SelectObject(hdc, linePen);
			for (int i = 0; i < dim; ++i) {
				Line(hdc, 450 / dim * i, 600, 450 / dim * i, 150);
				Line(hdc, 0, 150 + 450 / dim * i, 450, 150 + 450 / dim * i);
			}
			
			for (int i = 0; i < dim; ++i) {
				for (int j = 0; j < dim; ++j) {
					switch (matrix[i][j]) {
					case 1:
						SelectObject(hdc, crossPen);
						Line(hdc, 450 / dim * j + 20, 150 + 450 / dim * i + 20,
							450 / dim * (j + 1) - 20, 150 + 450 / dim * (i + 1) - 20);
						Line(hdc, 450 / dim * j + 20, 150 + 450 / dim * (i + 1) - 20,
							450 / dim * (j + 1) - 20, 150 + 450 / dim * i + 20);
						break;
					case 2:
						SelectObject(hdc, circlePen);
						Ellipse(hdc, 450 / dim * j + 20, 150 + 450 / dim * i + 20,
							450 / dim * (j + 1) - 20, 150 + 450 / dim * (i + 1) - 20);
						break;
					default:
						break;
					}
				}
			}
			DeleteObject(linePen);
			DeleteObject(crossPen);
			DeleteObject(circlePen);
			EndPaint(hDlg, &ps);
		}
		break;
	case WM_DESTROY:
		KillTimer(hDlg, 1);
		PostQuitMessage(0);
		break;

	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK DlgWin(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SetDlgItemText(hDlg, IDC_STATIC_PLAYERWON, str.c_str());
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			cl = true;
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK DlgSet(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		case ID_RADIO3X3:
			dim = 3;
			break;
		case ID_RADIO4X4:
			dim = 4;
			break;
		case ID_RADIO5X5:
			dim = 5;
			break;
		}
		cl = true;
		break;
	}
	return (INT_PTR)FALSE;
}

void Line(HDC& hdc, int x1, int y1, int x2, int y2) {
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

bool GetFigurePlace(const int& x, const int& y, vector<vector<int>>& matrix, const int& crossZero) {
	int place = -1;
	for (int i = 0; i < dim; ++i) {
		for (int j = 0; j < dim; ++j) {
			if (x > 450 / dim * j && x < 450 / dim * (j + 1) 
				&& y > 150 + 450 / dim * i && y < 150 + 450 / dim * (i + 1) && !(matrix[i][j])) {
				matrix[i][j] = crossZero;
				return true;
			}
		}
	}
	return false;
}

bool CheckWinner(vector<vector<int>>& matrix, const int& crossZero) {
	// check horizontal and vertical
	int finRes = 0;
	for (int i = 0; i < dim; ++i) {
		finRes = matrix[i][0];
		for (int j = 1; j < dim; ++j) {
			finRes *= matrix[i][j];
		}
		if (finRes == pow(crossZero, dim)) {
			return true;
		}
		finRes = matrix[0][i];
		for (int j = 1; j < dim; ++j) {
			finRes *= matrix[j][i];
		}
		if (finRes == pow(crossZero, dim)) {
			return true;
		}
	}

	finRes = matrix[0][0];
	for (int j = 1; j < dim; ++j) {
		finRes *= matrix[j][j];
	}
	if (finRes == pow(crossZero, dim)) {
		return true;
	}
	finRes = matrix[dim - 1][0];
	for (int j = 1; j < dim; ++j) {
		finRes *= matrix[dim - 1 - j][j];
	}
	if (finRes == pow(crossZero, dim)) {
		return true;
	}
	return false;
}

void CheckDraw(HWND& hDlg, bool& gameover, int tapsCount, int& countDraws) {
	if (!gameover && tapsCount == dim * dim) {
		++countDraws;
		SetDlgItemText(hDlg, IDC_STATIC_DRAWS, std::to_wstring(countDraws).c_str());
		str = L"Draw!!!";
		gameover = true;
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hDlg, DlgWin);
	}
}

void WinGameover(HWND hDlg, bool& gameover, int& countPlayerWins, int IDC) {
	if (gameover) {
		++countPlayerWins;
		SetDlgItemText(hDlg, IDC, std::to_wstring(countPlayerWins).c_str());
		if (IDC == 1035) {
			str = L"Player #1 has won!";
		}
		else {
			str = L"Player #2 has won!";
		}
		gameover = true;
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hDlg, DlgWin);
	}
}

bool Computer(vector<vector<int>>& matrix) {
	int	countEmpty = 0, lineSum = 0;
	bool cont = true;
	// check if computer can win
	// horizontally and vertically
	for (int i = 0; i < dim; ++i) {
		for (int j = 0; j < dim; ++j) {
			if (matrix[i][j] == 1) {
				cont = false;
			}
			if (matrix[i][j] == 0) {
				++countEmpty;
			}
		}
		if (countEmpty == 1 && cont) {
			for (int j = 0; j < dim; ++j) {
				if (matrix[i][j] == 0) {
					matrix[i][j] = 2;
					return true;
				}
			}
		}
		cont = true;
		countEmpty = 0;

		for (int j = 0; j < dim; ++j) {
			if (matrix[j][i] == 1) {
				cont = false;
			}
			if (matrix[j][i] == 0) {
				++countEmpty;
			}
		}
		if (countEmpty == 1 && cont) {
			for (int j = 0; j < dim; ++j) {
				if (matrix[j][i] == 0) {
					matrix[j][i] = 2;
					return true;
				}
			}
		}
		cont = true;
		countEmpty = 0;
	}
	// diagonals
	// main
	for (int i = 0; i < dim; ++i) {
		if (matrix[i][i] == 1) {
			cont = false;
		}
		if (matrix[i][i] == 0) {
			++countEmpty;
		}
	}
	if (countEmpty == 1 && cont) {
		for (int j = 0; j < dim; ++j) {
			if (matrix[j][j] == 0) {
				matrix[j][j] = 2;
				return true;
			}
		}
	}
	cont = true;
	countEmpty = 0;
	// side
	for (int i = 0; i < dim; ++i) {
		if (matrix[dim - 1 - i][i] == 1) {
			cont = false;
		}
		if (matrix[dim - 1 - i][i] == 0) {
			++countEmpty;
		}
	}
	if (countEmpty == 1 && cont) {
		for (int j = 0; j < dim; ++j) {
			if (matrix[dim - 1 - j][j] == 0) {
				matrix[dim - 1 - j][j] = 2;
				return true;
			}
		}
	}
	cont = true;
	countEmpty = 0;

	// defend horizont and vertical
	for (int i = 0; i < dim; ++i) {
		for (int j = 0; j < dim; ++j) {
			if (matrix[i][j] == 2) {
				cont = false;
			}
			if (matrix[i][j] == 0) {
				++countEmpty;
			}
		}
		if (countEmpty == 1 && cont) {
			for (int j = 0; j < dim; ++j) {
				if (matrix[i][j] == 0) {
					matrix[i][j] = 2;
					return true;
				}
			}
		}
		cont = true;
		countEmpty = 0;

		for (int j = 0; j < dim; ++j) {
			if (matrix[j][i] == 2) {
				cont = false;
			}
			if (matrix[j][i] == 0) {
				++countEmpty;
			}
		}
		if (countEmpty == 1 && cont) {
			for (int j = 0; j < dim; ++j) {
				if (matrix[j][i] == 0) {
					matrix[j][i] = 2;
					return true;
				}
			}
		}
		cont = true;
		countEmpty = 0;
	}
	// defend diagonals
	for (int i = 0; i < dim; ++i) {
		if (matrix[i][i] == 2) {
			cont = false;
		}
		if (matrix[i][i] == 0) {
			++countEmpty;
		}
	}
	if (countEmpty == 1 && cont) {
		for (int j = 0; j < dim; ++j) {
			if (matrix[j][j] == 0) {
				matrix[j][j] = 2;
				return true;
			}
		}
	}
	cont = true;
	countEmpty = 0;
	for (int i = 0; i < dim; ++i) {
		if (matrix[dim - 1 - i][i] == 2) {
			cont = false;
		}
		if (matrix[dim - 1 - i][i] == 0) {
			++countEmpty;
		}
	}
	if (countEmpty == 1 && cont) {
		for (int j = 0; j < dim; ++j) {
			if (matrix[dim - 1 - j][j] == 0) {
				matrix[dim - 1 - j][j] = 2;
				return true;
			}
		}
	}
	cont = true;
	countEmpty = 0;
	// fill central cell with circle if it is empty
	if (matrix[dim / 2][dim / 2] == 0) {
		matrix[dim / 2][dim / 2] = 2;
		return true;
	}
	// special case
	if (dim == 3 && matrix[1][1] == 1) {
		if (matrix[0][0] == 0) {
			matrix[0][0] = 2;
			return true;
		}
		if (matrix[2][0] == 0) {
			matrix[2][0] = 2;
			return true;
		}
	}
	// put circles in potentially dangerous places
	for (int i = 0; i < dim; ++i) {
		lineSum = 0;
		for (int j = 0; j < dim; ++j) {
			if (matrix[i][j] == 2) {
				break;
			}
			lineSum += matrix[i][j];
		}
		for (int j = 0; j < dim; ++j) {
			if (matrix[i][j] == 0 && lineSum != 0) {
				matrix[i][j] = 2;
				return true;
			}
		}
		lineSum = 0;
		for (int j = 0; j < dim; ++j) {
			if (matrix[j][i] == 2) {
				break;
			}
			lineSum += matrix[j][i];
		}
		for (int j = 0; j < dim; ++j) {
			if (matrix[j][i] == 0 && lineSum != 0) {
				matrix[j][i] = 2;
				return true;
			}
		}
	}
	for (int i = 0; i < dim; ++i) {
		for (int j = 0; j < dim; ++j) {
			if (matrix[i][j] == 0) {
				matrix[i][j] = 2;
			}
		}
	}
}