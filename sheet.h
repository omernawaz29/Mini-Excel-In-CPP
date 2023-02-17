#pragma once
#include<conio.h>  
#include<iostream> 
#include<windows.h>
#include <mmsystem.h>
#include <string> 
#include <fstream>
using namespace std;

#pragma region ColorDefinitions
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHTGREY 7
#define DARKGREY 8
#define LIGHTBLUE 9
#define LIGHTGREEN 10
#define LIGHTCYAN 11
#define LIGHTRED 12
#define LIGHTMAGENTA 13
#define YELLOW 14
#define WHITE 15
#define BLINK 128
#pragma endregion

void color(int k)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, k);
	if (k > 255)
	{
		k = 1;
	}
}
void gotoRowCol(int rpos, int cpos)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	int xpos = cpos, ypos = rpos;
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos;
	scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput, scrn);
}
void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);

}


class sheet
{
	class cell
	{
		string data;
		cell* rt, * lt, * up, * dn;
		int clr;
		char align;
		friend class sheet;

	public:

		cell(string _data = "", cell* _lt = nullptr, cell* _rt = nullptr, cell* _up = nullptr, cell* _dn = nullptr)
			:data(_data), rt(_rt), lt(_lt), up(_up), dn(_dn), clr(WHITE), align('c') {}
	};

	int cWidth, cHeight;
	cell* head;
	cell* tail;
	cell* curr;
	int cols, rows;
	int currC, currR;

	int rs, cs, re, ce, co;
	bool cut;

	//utility functions
	cell* generateRow(int size)
	{
		cell* head = new cell();
		head->lt = nullptr;
		cell* itr = head;

		for (int i = 1; i < size; i++)
		{
			itr->rt = new cell();
			itr->rt->lt = itr;
			itr = itr->rt;
		}

		return head;
	}
	cell* generateCol(int size)
	{
		cell* head = new cell();
		head->up = nullptr;
		cell* itr = head;

		for (int i = 1; i < size; i++)
		{
			itr->dn = new cell();
			itr->dn->up = itr;
			itr = itr->dn;
		}

		return head;
	}
	cell* getCell(int r, int c)
	{
		if (c > cols || r > rows)
			return nullptr;

		cell* temp = head;

		for (int i = 0; i < c; i++)
			temp = temp->rt;
		for (int i = 0; i < r; i++)
			temp = temp->dn;

		return temp;
	}

	void printCellBorder(int r, int c, int clr = LIGHTGREY)
	{
		color(clr);
		r *= cHeight - 1, c *= cWidth - 1;

		if (r < 0)
			r = 0;
		if (c < 0)
			c = 0;

		for (int i = c; i < c + cWidth; i++)
		{
			gotoRowCol(r, i);
			cout << char(254);
		}
		for (int i = c; i < c + cWidth; i++)
		{
			gotoRowCol(r + cHeight - 1, i);
			cout << char(254);
		}
		for (int i = r; i < r + cHeight; i++)
		{
			gotoRowCol(i, c);
			cout << char(254);
		}
		for (int i = r; i < r + cHeight; i++)
		{
			gotoRowCol(i, c + cWidth - 1);
			cout << char(254);
		}



	}
	void printCellData(int r, int c)
	{
		cell* dataCell = getCell(r, c);
		color(dataCell->clr);

		r *= cHeight - 1, c *= cWidth - 1;

		if (r < 0)
			r = 0;
		if (c < 0)
			c = 0;


		gotoRowCol(r + 2, c + 1);
		cout << "            ";
		int als = dataCell->data.size() / 2;

		if (dataCell->align == 'c')
		{
			gotoRowCol(r + 2, c + 7 - als);
		}
		else if (dataCell->align == 'l')
		{
			gotoRowCol(r + 2, c + 2);
		}
		else if (dataCell->align == 'r')
		{
			gotoRowCol(r + 2, c + 12 - (als * 2));
		}


		cout << dataCell->data;


	}

	void printGrid()
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				printCellBorder(i, j);
			}
		}
	}
	void printData()
	{
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				printCellData(i, j);
			}
		}
	}

	void printSheet()
	{
		printGrid();
		printData();
	}

	void printRow(int r)
	{
		for (int i = 0; i < cols; i++)
		{
			printCellData(r, i);
		}
	}
	void printCol(int c)
	{
		for (int i = 0; i < rows; i++)
		{
			printCellData(i, c);
		}
	}

	void highlightCurr()
	{
		printCellBorder(currR, currC, LIGHTBLUE);
	}
	void unHighlightCurr()
	{
		printCellBorder(currR, currC, WHITE);
	}

	bool isNumber(string val)
	{
		if (val.size() == 0)
			return false;
		
		for (int i = 0; i < val.size(); i++)
		{
			if ((val[i] < 48 || val[i]>57) && val[i] != '\0')
				return false;
		}
		return true;
	}
	void clearPrompt(int i)
	{
		color(WHITE);
		i += 8;
		gotoRowCol(rows * cHeight + i, 0);
		cout << "                                                                              ";
	}
	void clearPrompts()
	{
		color(WHITE);
		for (int i = 8; i < 15; i++)
		{
			gotoRowCol(rows * cHeight + i, 0);
			cout << "                                                                              ";
		}
	}

public:

	class iterator {

		cell* p;
		friend class sheet;
	public:
		iterator(cell* ptr)
		{
			p = ptr;
		}

		iterator& operator++()
		{
			p = p->dn;
			return *this;
		}
		iterator& operator--()
		{
			p = p->up;
			return *this;
		}
		iterator& operator++(int)
		{
			p = p->rt;
			return *this;
		}
		iterator& operator--(int)
		{
			p = p->lt;
			return *this;
		}
	};

	sheet(int rsize = 5, int csize = 5) : cWidth(15), cHeight(5), cut(false)
	{
		head = generateRow(rsize);
		curr = head;
		currC = 0;
		currR = 0;


		cols = rsize;
		rows = csize;

		for (int i = 0; i < csize - 1; i++)
		{
			InsertRowBelow(i);
			rows--;
		}

		printSheet();
		highlightCurr();

	}

	void InsertRowBelow(int r)
	{
		cell* rowHead = getCell(r, 0);
		cell* newRowHead = generateRow(cols);

		while (rowHead != nullptr)
		{
			newRowHead->dn = rowHead->dn;
			if (rowHead->dn != nullptr)
				rowHead->dn->up = newRowHead;
			rowHead->dn = newRowHead;
			newRowHead->up = rowHead;

			newRowHead = newRowHead->rt;
			rowHead = rowHead->rt;
		}
		rows++;
	}
	void InsertRowAbove(int r)
	{
		cell* rowHead = getCell(r, 0);
		cell* newRowHead = generateRow(cols);

		if (rowHead == head)
			head = newRowHead;

		while (rowHead != nullptr)
		{
			newRowHead->up = rowHead->up;
			if (rowHead->up != nullptr)
				rowHead->up->dn = newRowHead;
			rowHead->up = newRowHead;
			newRowHead->dn = rowHead;

			newRowHead = newRowHead->rt;
			rowHead = rowHead->rt;
		}
		rows++;
	}

	void InsertColRight(int c)
	{
		cell* colHead = getCell(0, c);
		cell* newColHead = generateCol(rows);

		while (colHead != nullptr)
		{
			newColHead->rt = colHead->rt;
			if (colHead->rt != nullptr)
				colHead->rt->lt = newColHead;
			colHead->rt = newColHead;
			newColHead->lt = colHead;

			newColHead = newColHead->dn;
			colHead = colHead->dn;
		}
		cols++;

	}
	void InsertColLeft(int c)
	{
		cell* colHead = getCell(0, c);
		cell* newColHead = generateCol(rows);

		if (colHead == head)
			head = newColHead;

		while (colHead != nullptr)
		{
			newColHead->lt = colHead->lt;
			if (colHead->lt != nullptr)
				colHead->lt->rt = newColHead;
			colHead->lt = newColHead;
			newColHead->rt = colHead;

			newColHead = newColHead->dn;
			colHead = colHead->dn;
		}
		cols++;
	}

	void InsertCellRightShift(int r, int c)
	{
		cell* refCell = getCell(r, c);
		InsertColRight(cols - 1);

		cell* itr = getCell(r, cols - 1);
		while (itr != refCell)
		{
			itr->data = itr->lt->data;
			itr = itr->lt;
		}
		refCell->data = "";
	}
	void InsertCellDownShift(int r, int c)
	{
		cell* refCell = getCell(r, c);
		InsertRowBelow(rows - 1);
		cell* itr = getCell(rows - 1, c);
		while (itr != refCell)
		{
			itr->data = itr->up->data;
			itr = itr->up;
		}
		refCell->data = "";
	}

	void DeleteRow(int r)
	{
		cell* rowHead = getCell(r, 0);
		if (rowHead == head)
			head = rowHead->dn;
		cell* itr = rowHead;

		while (itr != nullptr)
		{
			if (itr->dn != nullptr)
				itr->dn->up = itr->up;

			if (itr->up != nullptr)
				itr->up->dn = itr->dn;

			itr = itr->rt;
		}

		while (itr != nullptr)
		{
			itr = rowHead->rt;
			delete[]rowHead;
			rowHead = itr;
		}
		rows--;
	}
	void DeleteColumn(int c)
	{
		cell* colHead = getCell(0, c);
		if (colHead == head)
			head = colHead->rt;

		cell* itr = colHead;

		while (itr != nullptr)
		{
			if (itr->lt != nullptr)
				itr->lt->rt = itr->rt;

			if (itr->rt != nullptr)
				itr->rt->lt = itr->lt;

			itr = itr->dn;
		}

		while (itr != nullptr)
		{
			itr = colHead->dn;
			delete[]colHead;
			colHead = itr;
		}
		cols--;
	}

	void ClearRow(int r)
	{
		cell* itr = getCell(r, 0);
		while (itr != nullptr)
		{
			itr->data = "";
			itr = itr->rt;
		}
		printRow(r);
	}
	void ClearColumn(int c)
	{
		cell* itr = getCell(0, c);
		while (itr != nullptr)
		{
			itr->data = "";
			itr = itr->dn;
		}
		printCol(c);
	}

	void DeleteCellLeftShift(int r, int c)
	{
		cell* refCell = getCell(r, c);
		cell* itr = refCell;

		while (itr != nullptr)
		{
			if (itr->rt == nullptr)
				itr->data = "";
			else
				itr->data = itr->rt->data;

			itr = itr->rt;
		}
	}
	void DeleteCellUpShift(int r, int c)
	{
		cell* refCell = getCell(r, c);
		cell* itr = refCell;

		while (itr != nullptr)
		{
			if (itr->dn == nullptr)
				itr->data = "";
			else
				itr->data = itr->dn->data;

			itr = itr->dn;
		}
	}

	void printExcel()
	{
		cell* rowHead = head;
		for (int i = 0; i < rows; i++)
		{
			cell* itr = rowHead;
			for (int j = 0; j < cols; j++)
			{
				cout << itr->data << " ";
				itr = itr->rt;
			}
			cout << endl;
			rowHead = rowHead->dn;
		}
	}

	string sumCells()
	{
		int result = 0;
		for (int i = rs; i <= re; i++)
		{
			for (int j = cs; j <= ce; j++)
			{
				cell* itr = getCell(i, j);
				if (isNumber(itr->data))
				{
					result += stoi(itr->data);
				}
			}
		}

		string ret = to_string(result);
		ret.resize(7);
		return ret;
	}
	string countCells()
	{
		int r = re - rs + 1;
		int c = ce - cs + 1;
		return to_string(r * c);
	}
	string averageCells()
	{
		return to_string(stoi(sumCells()) / stoi(countCells()));
	}
	string getMin()
	{
		int result = INT_MAX;
		for (int i = rs; i <= re; i++)
		{
			for (int j = cs; j <= ce; j++)
			{
				cell* itr = getCell(i, j);
				if (itr != nullptr && isNumber(itr->data))
				{
					result = min(result, stoi(itr->data));
				}
			}
		}

		return to_string(result);
	}
	string getMax()
	{
		int result = INT_MIN;
		for (int i = rs; i <= re; i++)
		{
			for (int j = cs; j <= ce; j++)
			{
				cell* itr = getCell(i, j);
				if (itr != nullptr && isNumber(itr->data))
				{
					result = max(result, stoi(itr->data));
				}
			}
		}

		return to_string(result);
	}

	void Paste()
	{
		if (rs < 0)
			return;
		cell* itr = getCell(currR, currC);
		cell* copy = getCell(rs, cs);
		int i = -1, j = -1;
		
		do
		{
			j = -1;
			do
			{
				j++;
				itr->data = copy->data;

				if (j >= ce - cs)
					break;

				if (itr->rt == nullptr)
					InsertColRight(cols - 1);
				itr = itr->rt;
				copy = copy->rt;

			} while (j < ce - cs);

			for (int i = 0; i < j; i++)
			{
				itr = itr->lt;
				copy = copy->lt;
			}

			i++;

			if (i >= re - rs)
				break;
			if (itr->dn == nullptr)
				InsertRowBelow(rows - 1);
			itr = itr->dn;
			copy = copy->dn;

		} while (i<re-rs);

		if (cut)
		{
			for (int i = rs; i <= re; i++)
			{
				for (int j = cs; j <= ce; j++)
				{
					cell* itr = getCell(i, j);
					itr->data = "";
				}
			}
			cut = false;
			rs = -1;
		}
	}

	void getRange(int& ra, int& ca)
	{
		gotoRowCol(rows * cHeight + 8, 0);
		cout << "Select Range: ";
		co = 0;
		while (co < 3)
		{
			int key = _getch();
			if (movementInput(key)) {}
			else if (key == 32)
			{
				if (co == 0)
				{
					
					rs = currR;
					cs = currC;
					gotoRowCol(rows * cHeight + 10, 0);
					cout << "Range Start: (" << rs << ',' << cs << ')';
				}
				else if (co == 1)
				{
					re = currR;
					ce = currC;
					gotoRowCol(rows * cHeight + 11, 0);
					cout << "Range End: (" << re << ',' << ce << ')';
				}
				else if (co == 2)
				{
					ra = currR;
					ca = currC;
					gotoRowCol(rows * cHeight + 12, 0);
					cout << "Answer At: (" << ra << ',' << ca << ')';
				}
				co++;
			}
		}

		if (re < rs)
			swap(re, rs);
		if (ce < cs)
			swap(ce, cs);
	}
	void getRange()
	{
		co = 0;
		gotoRowCol(rows * cHeight + 8, 0);
		cout << "Select Range: ";
		while (co < 2)
		{
			int key = _getch();
			if (movementInput(key)) {}
			else if (key == 32)
			{
				if (co == 0)
				{
					rs = currR;
					cs = currC;
					gotoRowCol(rows * cHeight + 10, 0);
					cout << "Range Start: (" << rs << ',' << cs << ')';
				}
				else if (co == 1)
				{
					re = currR;
					ce = currC;
					gotoRowCol(rows * cHeight + 11, 0);
					cout << "Range End: (" << re << ',' << ce << ')';
				}
				co++;
			}
		}

		if (re < rs)
			swap(re, rs);
		else if (ce < cs)
			swap(ce, cs);
	}

	void Save()
	{
		string filename;
		color(WHITE);
		gotoRowCol(rows * cHeight + 9, 0);
		cout << "Enter FileName: ";
		cin >> filename;


		ofstream wrt(filename);
		wrt << rows << " " << cols << " ";

		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				cell* itr = getCell(i, j);
				if (itr->data.empty())
					wrt << "-" << " ";
				else
					wrt << itr->data << " ";
				wrt << itr->clr << " " << itr->align << " ";
			}
		}

		gotoRowCol(rows * cHeight + 10, 0);
		cout << "SAVED"; _getch();
		clearPrompts();

		
	}
	void Load()
	{
		string filename;
		color(WHITE);
		gotoRowCol(rows * cHeight + 9, 0);
		cout << "Enter FileName: ";
		cin >> filename;

		ifstream rdr(filename);

		if (!rdr)
		{
			gotoRowCol(rows * cHeight + 9, 0);
			cout << "THE FILE WAS NOT FOUND!!"; _getch();
			clearPrompts();
			return;
		}

		int r, c;
		rdr >> r >> c;
		sheet* newSheet = new sheet(r, c);

		string data;
		char align;
		int clr;
		cell* itr = newSheet->head;

		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < c; j++)
			{
				itr = newSheet->getCell(i, j);

				rdr >> data;
				if (data == "-")
					data = "";
				rdr >> clr;
				rdr >> align;
				
				itr->data = data;
				itr->clr = clr;
				itr->align = align;

			}
			
		}

		*this = *newSheet;

		gotoRowCol(rows * cHeight + 9, 0);
		cout << "Loaded"; _getch();
		clearPrompts();



	}


	bool movementInput(int key)
	{
		if (key == 119) //w
		{
			if (curr->up != nullptr)
			{
				unHighlightCurr();
				curr = curr->up;
				currR--;
				highlightCurr();
			}
			return true;
		}
		else if (key == 115) //s
		{
			if (curr->dn != nullptr)
			{
				unHighlightCurr();
				curr = curr->dn;
				currR++;
				highlightCurr();
			}
			return true;
		}
		else if (key == 97) //a
		{
			if (curr->lt != nullptr)
			{
				unHighlightCurr();
				curr = curr->lt;
				currC--;
				highlightCurr();
			}
			return true;
		}
		else if (key == 100) //d
		{
			if (curr->rt != nullptr && currC < cols - 1)
			{
				unHighlightCurr();
				curr = curr->rt;
				currC++;
				highlightCurr();
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	void Input()
	{
		int key = _getch();

		if (movementInput(key)) {}
		else if (key == 13 /* enter */)
		{
			char inChar;
			string inString = "";
			
			color(WHITE);
			gotoRowCol(rows*cHeight+9, 0);
			cout << "Enter Data: ";

			while (true)
			{
				inChar = _getch();
				if (inChar == ' ' || inChar == 13)
					break;
				cout << inChar;
				inString += inChar;

				if (inString.size() >= 5)
					break;

			}

			
			clearPrompts();
			
			cell* inCell = getCell(currR, currC);
			inCell->data = "";
			printCellData(currR, currC);
			inCell->data = inString;
			printCellData(currR, currC);
		}
		else if (key == 2 /* ctrl + b */)
		{
			color(WHITE);
			clearPrompt(5);
			gotoRowCol(rows * cHeight + 13, 0);
			cout << "SUM";
			int ra, ca;
			getRange(ra,ca);
			
			clearPrompts();

			getCell(ra, ca)->data = sumCells();
			printCellData(ra, ca);
		}
		else if (key == 10 /* ctrl + j */)
		{
			color(WHITE);
			clearPrompt(5);
			gotoRowCol(rows * cHeight + 13, 0);
			cout << "COUNT CELLS";
			int ra, ca;
			getRange(ra, ca);
			
			clearPrompts();

			getCell(ra, ca)->data = countCells();
			printCellData(ra, ca);
		}
		else if (key == 1  /* ctrl + a */)
		{
			color(WHITE);
			clearPrompt(5);
			gotoRowCol(rows * cHeight + 13, 0);
			cout << "AVERAGE CELLS";
			int ra, ca;
			getRange(ra, ca);
			
			clearPrompts();

			getCell(ra, ca)->data = sumCells();
			printCellData(ra, ca);
		}
		else if (key == 8  /* ctrl + h */)
		{
			color(WHITE);
			clearPrompt(5);
			gotoRowCol(rows * cHeight + 13, 0);
			cout << "MAX";
			int ra, ca;
			getRange(ra, ca);
			
			clearPrompts();

			getCell(ra, ca)->data = getMax();
			printCellData(ra, ca);
		}
		else if (key == 14 /* ctrl + n */)
		{
			color(WHITE);
			clearPrompt(5);
			gotoRowCol(rows * cHeight + 13, 0);
			cout << "MIN";

			int ra, ca;
			getRange(ra, ca);
			
			clearPrompts();

			getCell(ra, ca)->data = getMin();
			printCellData(ra, ca);
		}
		else if (key == 3  /* ctrl + c */)
		{
			color(WHITE);
			clearPrompt(5);
			gotoRowCol(rows* cHeight + 13, 0);
			cout << "COPYING";
			getRange();

			gotoRowCol(rows * cHeight + 9, 0);
			cout << "Copied: ";
			clearPrompt(0);
			cut = false;
		}
		else if (key == 24 /* ctrl + x */)
		{

			color(WHITE);
			clearPrompt(5);
			gotoRowCol(rows* cHeight + 13, 0);
			cout << "SUM";
			getRange();

			gotoRowCol(rows * cHeight + 9, 0);
			cout << "Cut: ";
			clearPrompt(0);
			cut = true;
		}
		else if (key == 16 /* ctrl + p */)
		{
			Paste();
			clearPrompts();
			printSheet();
			highlightCurr();
		}
		else if (key == 4  /* ctrl + d */)
		{

			color(WHITE);
			clearPrompt(5);
			gotoRowCol(rows* cHeight + 13, 0);
			cout << "SELECT ATTRIBUTE(C for Color, A for Alignment)";

			key = _getch();
			if (key == 99)
			{
				color(WHITE);
				gotoRowCol(rows* cHeight + 9, 0);
				cout << "Enter Color Code: ";
				cin >> getCell(currR, currC)->clr;
				printCellData(currR, currC);
				clearPrompts();
			}
			else if (key == 97)
			{
				char ac;
				color(WHITE);
				gotoRowCol(rows* cHeight + 9, 0);
				cout << "Enter Alignment Code: ";
				ac = _getche();

				if(ac == 'c' || ac == 'l' || ac == 'r')
					getCell(currR, currC)->align = ac;

				printCellData(currR, currC);
				clearPrompts();

			}
		}
		else if (key == 9  /* ctrl + i */)
		{
			color(WHITE);
			clearPrompt(5);
			gotoRowCol(rows* cHeight + 13, 0);
			cout << "\"INSERT\"";
			key = _getch();
			if (key == 101 /*e*/)
			{
				InsertRowAbove(currR);
				curr = getCell(currR, currC);
			}
			else if (key == 116/*t*/)
			{
				InsertRowBelow(currR);
				curr = getCell(currR, currC);
			}
			else if (key == 120/*x*/)
			{
				InsertColLeft(currC);
				curr = getCell(currR, currC);
			}
			else if (key == 118/*v*/)
			{
				InsertColRight(currC);
				curr = getCell(currR, currC);
			}
			else if (key == 3  /* ctrl + c */)\
			{
				color(WHITE);
				clearPrompt(5);
				gotoRowCol(rows* cHeight + 13, 0);
				cout << "\"INSERT CELL\"";
				key = _getch();
				if (key == 114 /*r*/)
				{
					InsertCellRightShift(currR, currC);
					curr = getCell(currR, currC);
				}
				else if (key == 100/*d*/)
				{
					InsertCellDownShift(currR, currC);
					curr = getCell(currR, currC);
				}

			}

			system("cls");
			printSheet();
			highlightCurr();
		}
		else if (key == 18 /* ctrl + r */)
		{

			color(WHITE);
			clearPrompt(5);
			gotoRowCol(rows* cHeight + 13, 0);
			cout << "\"REMOVE\"";
			key = _getch();
			if (key == 114)
			{
				DeleteRow(currR);
				curr = getCell(currR, currC);
			}
			else if (key == 99)
			{
				DeleteColumn(currC);
				curr = getCell(currR, currC);
			}
			else if (key == 3  /* ctrl + c */)\
			{
				color(WHITE);
				clearPrompt(5);
				gotoRowCol(rows* cHeight + 13, 0);
				cout << "\"REMOVE CELL\"";
				key = _getch();
				if (key == 108 /*l*/)
				{
					DeleteCellLeftShift(currR, currC);
					curr = getCell(currR, currC);
				}
				else if (key == 117/*u*/)
				{
					DeleteCellUpShift(currR, currC);
					curr = getCell(currR, currC);
				}

			}

			system("cls");
			printSheet();
			highlightCurr();
		}
		else if (key == 5  /* ctrl + e */)
		{
			color(WHITE);
			clearPrompt(5);
			gotoRowCol(rows* cHeight + 13, 0);
			cout << "\"CLEAR\"";
			key = _getch();
			if (key == 114)
			{
				ClearRow(currR);
			}
			else if (key == 99)
			{
				ClearColumn(currC);
			}

			printSheet();
			highlightCurr();
		}
		else if (key == 19 /* ctrl + s */)
		{
			color(WHITE);
			clearPrompts();
			Save();
		}
		else if (key == 12 /* ctrl + l */)
		{
			color(WHITE);
			Load();
			printSheet();
		}

		clearPrompts();
		color(WHITE);

	}

	


};
