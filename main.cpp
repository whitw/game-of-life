#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>


#ifdef _WIN32
	#include <Windows.h>
#endif
#ifdef __linux__
	#include <ncurses.h>
#endif

using namespace std;


#ifdef _WIN32
#define MAPH 29
#define MAPW 60
#endif

#define ALIVE 1
#define DEAD 0
#define MAXFRAME 30

#ifdef _WIN32
void gotoxy(int y, int x) {
	COORD pos = { y,x };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
#endif
#ifdef __linux__
void gotoxy(int y, int x){
	move(y, x);
}
#endif

#ifdef _WIN32
#define out cout
#endif

#ifdef __linux__
struct ostr{
};
void operator<<(struct ostr o, string t){
	printw(t.c_str());
}
struct ostr out;
#endif


class Pos {
public:
	int y, x;
	const Pos operator+(const Pos& p) const {
		return { y + p.y, x + p.x };
	}
};

Pos d[8] = {
	{1, -1},
	{1, 0},
	{1, 1},
	{0, -1},
	{0, 1},
	{-1, -1},
	{-1, 0},
	{-1, 1}
};

int pattern[2][9] = {
	{
	0,0,0,
	1,0,0,
	0,0,0,
	},
	{
	0,0,1,
	1,0,0,
	0,0,0,
	}
};



int main() {

	#ifdef __linux__
		initscr();
		int MAPH, MAPW;
		getmaxyx(stdscr,MAPH,MAPW);
	#endif

	vector<vector<int>> map(MAPH, vector<int>(MAPW, 0));
	/*int N;
	cin >> N;
	for (int i = 0; i < N; i++) {
		int y, x;
		cin >> y >> x;
		if (y < 0 || x < 0 || y >= MAPH || x >= MAPW) continue;
		map[y][x] = ALIVE;
	}*/
	map[12][30] = ALIVE;
	map[13][29] = ALIVE;
	map[14][29] = ALIVE;
	map[14][30] = ALIVE;
	map[14][31] = ALIVE;


	while (true) {
		//buffer image
		vector<string> v(MAPH);
		for (int i = 0; i < MAPH; i++) {
			for (int j = 0; j < MAPW; j++) {
				if (map[i][j] == ALIVE) {
					v[i] += "##";
				}
				else {
					v[i] += "  ";
				}
			}
		}

		//draw image
		for (int i = 0; i < MAPH - 1; i++) {
			gotoxy(0, i);
			out << v[i];
		}
		out << v[MAPH - 1];

		vector<vector<int>> mapcpy = map;
		//update part
		time_t prev = clock();
		for (int i = 0; i < MAPH; i++) {
			for (int j = 0; j < MAPW; j++) {
				int aliveNear = 0;
				int aliveMe = map[i][j];
				for (int k = 0; k < 8; k++) {
					Pos next = Pos{ i, j } +d[k];
					if (next.y < 0 || next.x < 0 || next.y >= MAPH || next.x >= MAPW) continue;
					if (map[next.y][next.x]) aliveNear++;
				}
				mapcpy[i][j] = pattern[aliveMe][aliveNear];
			}
		}
		map = mapcpy;

		//frame controller
		while (true) {
			time_t ellapsed = clock() - prev;
			if (CLOCKS_PER_SEC / (unsigned long)MAXFRAME < (unsigned long)ellapsed)
				break;
		}
	}
#ifdef __linux__
	endwin();
#endif
	return 0;
}
