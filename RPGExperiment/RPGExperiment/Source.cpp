#include <conio.h>
#include <iostream>
#include <Windows.h>
#include <string>
#include <time.h>
using namespace std;
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_SPACE 32
#define KEY_ENTER 13
class tile {
public:
	bool collide;
	char ascii;
	void set_values(bool, char);
	//bool collision() { return collide; }
};
void tile::set_values(bool a, char b)
{
	collide = a;
	ascii = b;
}
class item {
public:
	string name;
	int value; //attack strength for weapon, amount restored for potion
	void set_values(int, string);
};
void item::set_values(int a, string b)
{
	value = a;
	name = b;
}
class actor {
public:
	string name;
	int health;
	int strength;
	short x;
	short y;
	void set_values(string, int, int, short, short);
};
void actor::set_values(string a, int b, int c, short d, short e)
{
	name = a;
	health = b;
	strength = c;
	x = d;
	y = e;
}
void bigFont()
{
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.FontFamily = 0;
	cfi.dwFontSize.X = 16;
	cfi.dwFontSize.Y = 16;
	cfi.FontFamily = FW_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, 750, 750, TRUE);
}
void title()
{
	std::cout << "ASCII Roleplaying Game\n\n";
	std::cout << "Instructions:\n\n";
	std::cout << "Use arrowkeys to move\n";
	std::cout << "If you have a key, you can open a door (";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0E);
	std::cout << "+";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
	std::cout << ") by pressing SPACE\n";
	std::cout << "You can also open chests (";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x06);
	std::cout << "C";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
	std::cout << ") by pressing SPACE\n";
	std::cout << "SPACE, when pressed, can also be used to 'wait' and monsters will move without you doing the same\n";
	std::cout << "If you have a potion (!), you can drink it by pressing ENTER. You will gain 75 HP. You cannot have more than 100 HP.\n";
	std::cout << "If you are adjacent to a monster ("; //B, 2, 5, 4
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0B);
	std::cout << "S";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
	std::cout << ", ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x02);
	std::cout << "Z";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
	std::cout << ", ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x05);
	std::cout << "L";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
	std::cout << ", ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x04);
	std::cout << "D";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
	std::cout << "), you can attack by trying to move into their tile\n";
	std::cout << "If your stuck try defeating an enemy or stepping on a (";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x09);
	std::cout << "?";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
	std::cout << ") tile\n\n\n\n";
	std::cout << "Finally, I spent about an hour on this wonderful random map generator by Wizards of the Coast trying to generate\n";
	std::cout << "just the right map to use as the basis for this games map: https://www.wizards.com/dnd/mapper/launcher.htm" << "\n";
	std::cout << "You can see the original randomly generated map at:\n";
	std::cout << "https://www.wizards.com/dnd/mapper/mapper.htm?6,6,75,4,6,0,0,0,4,8,0,1556820573726,1,1,,0,1,A,C,0,0,0,3,0,0" << "\n\n";
	std::cout << "Enjoy this program. Started this back in April/ early May, and some concepts were pretty difficult to learn.\n";
	std::cout << "I think I got the hang of them though.\n\n";
	std::cout << "-W. Alex Goddard 17/6/2019\n\n";
	std::system("Pause");
	std::system("cls");
}
int main()
{
	title();
	bigFont();
	//misc. variables
	int c = 0;
    int input;
	bool win = false;
	//coordinates
	//cursor coordinates
	short x = 23; //column
	short y = 1; //row
	//key number coordinates
	short aa = 23;
	short bb = 9;
	//player health coordinates
	short cc = 23;
	short dd = 5;
	//potion number coordinates
	short ee = 23;
	short ff = 11;
	//enemy health coordinates
	short gg = 23;
	short hh = 7;
	//sword name coordinates
	short ii = 23;
	short jj = 13;
	//sword damage coordinates
	short kk = 23;
	short ll = 15;
	COORD coord;
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	//items
	int numKeys = 0; //number of keys
	int numPotions = 0;
	item Potion, BronzeSword , IronSword, SteelSword;
	Potion.set_values(75, "Potion");
	//going to have these swords increase in <value> from 10 - 30
	BronzeSword.set_values(10, "Bronze Sword");
	IronSword.set_values(20, "Iron Sword");
	SteelSword.set_values(30, "Steel Sword");
	item Sword[1] = { BronzeSword };
	//tiles
	tile LockedDoor, UnlockedDoor, Floor, WallV, WallH, Hallway, Stairs, Chest, Player, Slime, Zombie, Lich, Dragon, Blackspace, PotTile, Mechanism;
	LockedDoor.set_values(true, '+'); //door locked until opened by key (Or if specific enemy is defeated)
	UnlockedDoor.set_values(false, '.'); //same as floor
	Mechanism.set_values(false, '?'); //if player steps on this, some walls may disappear revealing new path
	Floor.set_values(false, '.');
	WallV.set_values(true, '|'); //Vertical wall
	WallH.set_values(true, '-'); //Horizontal wall
	Hallway.set_values(false, '#');
	Stairs.set_values(false, '>'); //decoration tile
	Chest.set_values(true, 'C'); //can be opened for key
	PotTile.set_values(false, '!'); //can be stepped on by player to gain a potion, then vanishes
	Player.set_values(true, '@');
	Slime.set_values(true, 'S');
	Zombie.set_values(true, 'Z');
	Lich.set_values(true, 'L');
	Dragon.set_values(true, 'D');
	Blackspace.set_values(true, ' '); //empty space outside map
	//actors
	actor PC;
	PC.set_values("Player", 100, 10, 13, 27);
	//I sorted "enemies" by the order in which the player should encounter them
	actor slimea;
	slimea.set_values("Slime", 50, 4, 11, 19);
	actor slimeb;
	slimeb.set_values("Slime", 50, 4, 6, 18);
	actor slimec;
	slimec.set_values("Slime", 50, 4, 5, 25);
	actor zombiea;
	zombiea.set_values("Zombie", 75, 8, 3, 20);
	actor zombieb;
	zombieb.set_values("Zombie", 75, 8, 7, 31);
	actor zombiec;
	zombiec.set_values("Zombie", 75, 8, 8, 36);
	actor lich;
	lich.set_values("Lich", 100, 16, 12, 8);
	actor zombied;
	zombied.set_values("Zombie", 75, 8, 13, 3);
	actor zombiee;
	zombiee.set_values("Zombie", 75, 8, 14, 2);
	actor dragon;
	dragon.set_values("Dragon", 125, 24, 17, 33);
	//dungeon
	char map[40][20] = {
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '-', '-', '-', '-', '-', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ', ' ', '-', '-', '-', '-', '-', ' ', '|', '.', '.', 'Z', '|', ' ', ' ', ' ', ' ' }, //one of the zombies in room 12 will unlock the door to room 13 upon defeat
	{ ' ', ' ', ' ', ' ', ' ', '|', '.', '.', '!', '|', ' ', '|', '.', 'Z', '.', '|', '-', '-', '-', '-' },
	{ ' ', ' ', ' ', ' ', ' ', '|', 'C', '.', '.', '|', ' ', '|', '.', '.', '.', '+', '.', '.', '!', '|' }, //chest has key
	{ ' ', ' ', ' ', ' ', ' ', '-', '-', '-', '.', '-', '-', '-', '+', '-', '-', '|', '.', '.', 'C', '|' }, //chest in this row will contain steel sword
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '|', '.', '.', '.', '.', '.', '|', '?', '.', 'C', '|' }, //chest has (final) key
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '|', '.', '.', '.', '.', '.', '|', '-', '-', '-', '-' },
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '|', '.', '.', 'L', '.', '.', '|', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|', '.', '.', '.', '.', '.', '|', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '|', '.', '.', '.', '.', '.', '|', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '-', '-', '-', '+', '-', '-', '-', ' ', ' ', ' ', ' ' }, //having door here prevents lich from wandering out of its room
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', '-', '-', '-', '-', ' ', ' ', ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', '|', '.', 'C', '|', ' ', ' ', ' ', ' ', '-', '-', '-', '-', ' ', ' ', ' ', ' ', ' ', ' ' }, //chest has key
	{ ' ', ' ', '|', '.', '!', '|', ' ', ' ', ' ', ' ', '|', '.', '.', '|', ' ', ' ', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', '-', '+', '-', '|', '#', '#', '#', '#', '+', '.', '.', '|', ' ', ' ', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', '#', '#', '#', 'S', '-', '-', '-', '|', '.', '.', '|', '#', ' ', ' ', ' ', ' ', ' ' },
	{ '-', '-', '-', '-', '-', '-', '#', '.', '.', '.', '|', 'S', '.', '|', '#', ' ', ' ', ' ', ' ', ' ' },
	{ '|', '.', '+', 'Z', '.', '|', ' ', '|', '.', '.', '|', '-', '-', '|', '#', ' ', ' ', ' ', ' ', ' ' },
	{ '|', '?', '|', '.', '.', '|', ' ', '|', 'C', '.', '|', ' ', ' ', '|', '#', ' ', ' ', ' ', ' ', ' ' }, //chest has key
	{ '-', '-', '-', '+', '-', '-', ' ', '-', '-', '-', '-', ' ', ' ', '|', '#', ' ', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', '-', '-', '-', '-', '-', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', ' ', ' ', '|', '?', '.', '.', '|', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ', '#', 'S', '#', '#', '#', '#', '#', '+', '.', '.', '.', '|', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ', ' ', ' ', '-', '-', '-', '-', ' ', '|', '.', '.', '.', '|', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ', ' ', ' ', '|', '.', '.', '|', ' ', '|', '.', '@', '>', '|', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ', ' ', ' ', '|', '.', '.', '|', ' ', '-', '-', '-', '-', '-', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ', ' ', ' ', '|', '.', '.', '|', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ', ' ', ' ', '|', '.', '.', '|', ' ', ' ', ' ', '-', '-', '-', '-', '-', '-', ' ' },
	{ ' ', ' ', ' ', ' ', ' ', ' ', '|', 'Z', '.', '|', '#', '#', '#', '+', '.', '.', '.', '.', '|', ' ' }, //need a door here because I absolutely do not want a dragon randomly wandering out of its room!
	{ ' ', ' ', ' ', ' ', ' ', ' ', '|', '.', '.', '|', ' ', ' ', ' ', '|', '.', '.', '.', '.', '|', ' ' },
	{ ' ', ' ', ' ', ' ', ' ', ' ', '|', '.', '.', '|', ' ', ' ', ' ', '|', '.', '.', '.', 'D', '|', ' ' },
	{ '-', '-', '-', '-', '#', '#', '|', '.', '.', '|', ' ', ' ', ' ', '|', '.', '.', '.', '.', '|', ' ' },
	{ '|', '.', '.', '.', '#', ' ', '|', '.', '.', '|', '-', '-', '-', '|', '.', '.', '.', '.', '|', ' ' },
	{ '|', 'C', '!', '|', ' ', ' ', '|', '.', 'Z', '+', '.', '.', 'C', '|', '-', '-', '-', '-', '-', ' ' }, //both chests have key
	{ '-', '-', '-', '-', ' ', ' ', '|', '.', '.', '|', '.', '?', 'C', '|', ' ', ' ', ' ', ' ', ' ', ' ' }, //chest has iron sword
	{ ' ', ' ', ' ', ' ', ' ', ' ', '-', '-', '-', '-', '-', '-', '-', '-', ' ', ' ', ' ', ' ', ' ', ' ' },
	{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }
	};
	tile tilemap[40][20]; //this is for checking collision, tracking location of actors
	tile secondtilemap[40][20]; //this is for overwriting tiles being moved from
	//monster movements
	short savevalue; //saves a coordinate
	short savevalue2; //just for a quick test to fix a bug
	//slime a
	short sax = slimea.x; //slime a (x coord)
	short say = slimea.y; //slime a (y coord)
	int samovement; //slime a (random movement)
	//slime b
	short sbx = slimeb.x;
	short sby = slimeb.y;
	int sbmovement;
	//slime c
	short scx = slimec.x;
	short scy = slimec.y;
	int scmovement;
	//zombie a
	short zax = zombiea.x;
	short zay = zombiea.y;
	int zamovement;
	//zombie b
	short zbx = zombieb.x;
	short zby = zombieb.y;
	int zbmovement;
	//zombie c
	short zcx = zombiec.x;
	short zcy = zombiec.y;
	int zcmovement;
	//lich
	short lx = lich.x;
	short ly = lich.y;
	int lmovement;
	//zombie d
	short zdx = zombied.x;
	short zdy = zombied.y;
	int zdmovement;
	//zombie e
	short zex = zombiee.x;
	short zey = zombiee.y;
	int zemovement;
	//dragon
	short dx = dragon.x;
	short dy = dragon.y;
	int dmovement;
	//draw map
	for (int j = 0; j < 40; ++j) //prints each row
	{
		for (int k = 0; k < 20; ++k) //prints each character on a row
		{
			if (map[j][k] == '+')
			{
				SetConsoleTextAttribute(hStd, 0x0E);
				std::cout << map[j][k];
			}
			else if (map[j][k] == 'C')
			{
				SetConsoleTextAttribute(hStd, 0x06);
				std::cout << map[j][k];
			}
			else if (map[j][k] == '?')
			{
				SetConsoleTextAttribute(hStd, 0x09);
				std::cout << map[j][k];
			}
			else if (map[j][k] == '@')
			{
				SetConsoleTextAttribute(hStd, 0x03);
				std::cout << map[j][k];
			}
			else if (map[j][k] == 'D')
			{
				SetConsoleTextAttribute(hStd, 0x04);
				std::cout << map[j][k];
			}
			else if (map[j][k] == 'S')
			{
				SetConsoleTextAttribute(hStd, 0x0B);
				std::cout << map[j][k];
			}
			else if (map[j][k] == 'Z')
			{
				SetConsoleTextAttribute(hStd, 0x02);
				std::cout << map[j][k];
			}
			else if (map[j][k] == 'L')
			{
				SetConsoleTextAttribute(hStd, 0x05);
				std::cout << map[j][k];
			}
			else
			{
				SetConsoleTextAttribute(hStd, 0x07);
				std::cout << map[j][k];
			}
		}
		std::cout << endl;	
	}
	//fill tile map
	for (int j = 0; j < 40; ++j) 
	{
		for (int k = 0; k < 20; ++k)
		{
			if (map[j][k] == '+')
			{
				tilemap[j][k] = LockedDoor;
			}
			else if (map[j][k] == '?')
			{
				tilemap[j][k] = Mechanism;
			}
			else if (map[j][k] == '.')
			{
				tilemap[j][k] = Floor;
			}
			else if (map[j][k] == '|')
			{
				tilemap[j][k] = WallV;
			}
			else if (map[j][k] == '-')
			{
				tilemap[j][k] = WallH;
			}
			else if (map[j][k] == '#')
			{
				tilemap[j][k] = Hallway;
			}
			else if (map[j][k] == '>')
			{
				tilemap[j][k] = Stairs;
			}
			else if (map[j][k] == 'C')
			{
				tilemap[j][k] = Chest;
			}
			else if (map[j][k] == '!')
			{
				tilemap[j][k] = PotTile;
			}
			else if (map[j][k] == '@')
			{
				tilemap[j][k] = Player;
			}
			else if (map[j][k] == 'S')
			{
				
				tilemap[j][k] = Slime;
			}
			else if (map[j][k] == 'Z')
			{
				tilemap[j][k] = Zombie;
			}
			else if (map[j][k] == 'L')
			{
				tilemap[j][k] = Lich;
			}
			else if (map[j][k] == 'D')
			{
				tilemap[j][k] = Dragon;
			}
			else if (map[j][k] == ' ')
			{
				tilemap[j][k] = Blackspace;
			}
		}
	}
	//fill second tile map
	for (int j = 0; j < 40; ++j)
	{
		for (int k = 0; k < 20; ++k)
		{
			if (map[j][k] == '|')
			{
				secondtilemap[j][k] = WallV;
			}
			else if (map[j][k] == '-')
			{
				secondtilemap[j][k] = WallH;
			}
			else if (map[j][k] == '#')
			{
				secondtilemap[j][k] = Hallway;
			}
			else if (map[j][k] == '>')
			{
				secondtilemap[j][k] = Stairs;
			}
			else if (map[j][k] == '?')
			{
				secondtilemap[j][k] = Mechanism;
			}
			else if (map[j][k] == '!')
			{
				secondtilemap[j][k] = PotTile;
			}
			else if (map[j][k] == ' ')
			{
				secondtilemap[j][k] = Blackspace;
			}
			else if (j == 25 && k == 5)
			{
				secondtilemap[j][k] = Hallway;
			}
			else
			{
				secondtilemap[j][k] = Floor;
			}
		}
	}
	secondtilemap[sby][sbx] = Hallway;
	//prints all the information on the right side on the map
	SetConsoleTextAttribute(hStd, 0x07);
	coord = { (short)x, (short)y };
	SetConsoleCursorPosition(hStd, coord);
	std::cout << "-----------";
	++y;
	coord = { (short)x, (short)y };
	SetConsoleCursorPosition(hStd, coord);
	std::cout << "INFORMATION";
	++y;
	coord = { (short)x, (short)y };
	SetConsoleCursorPosition(hStd, coord);
	std::cout << "-----------";
	coord = { (short)cc, (short)dd };
	SetConsoleCursorPosition(hStd, coord);
	std::cout << "HP: " << PC.health;
	cc = 27;
	coord = { (short)gg, (short)hh };
	SetConsoleCursorPosition(hStd, coord);
	std::cout << "Enemy HP: -";
	gg = 33;
	coord = { (short)aa, (short)bb };
	SetConsoleCursorPosition(hStd, coord);
	std::cout << "Keys: " << numKeys;
	aa = 29;
	coord = { (short)ee, (short)ff };
	SetConsoleCursorPosition(hStd, coord);
	std::cout << "Potions: " << numPotions;
	ee = 32;
	coord = { (short)ii, (short)jj };
	SetConsoleCursorPosition(hStd, coord);
	std::cout << "Sword: " << Sword[0].name;
	ii = 30;
	coord = { (short)kk, (short)ll };
	SetConsoleCursorPosition(hStd, coord);
	PC.strength = PC.strength + Sword[0].value;
	std::cout << "Base Strength: " << PC.strength;
	kk = 38; 
	//Actions
	x = PC.x;
	y = PC.y;
	coord = { (short)x, (short)y };
	SetConsoleCursorPosition(hStd, coord);
	while (1)
	{
		c = 0;
		//rnadomizing attack (Have to make this somewhat unpredictable!)
		PC.strength = rand() % 10 + 5;
		PC.strength = PC.strength + Sword[0].value;
		slimea.strength = rand() % 5 + 3;
		slimeb.strength = rand() % 5 + 3;
		slimec.strength = rand() % 5 + 3;
		zombiea.strength = rand() % 10 + 5;
		zombieb.strength = rand() % 10 + 5;
		zombiec.strength = rand() % 10 + 5;
		zombied.strength = rand() % 10 + 5;
		zombiee.strength = rand() % 10 + 5;
		lich.strength = rand() % 15 + 8;
		dragon.strength = rand() % 20 + 10;
		//randomizing NPC movement
		int samovement = rand() % 4 + 1;
		int sbmovement = rand() % 4 + 1;
		int scmovement = rand() % 4 + 1;
		int zamovement = rand() % 4 + 1;
		int zbmovement = rand() % 4 + 1;
		int zcmovement = rand() % 4 + 1;
		int lmovement = rand() % 4 + 1;
		int zdmovement = rand() % 4 + 1;
		int zemovement = rand() % 4 + 1;
		int dmovement = rand() % 4 + 1;
		if (_kbhit())
		{
			input = _getch();
			c = input;
		}
		if (c == KEY_UP)
		{
			PC.y--;	
			if (tilemap[PC.y][PC.x].collide == true)
			{
				if (tilemap[PC.y][PC.x].ascii == 'S')
				{
					if ((PC.y == say && PC.x == sax) && (slimea.health > 0)) //slime a
					{
						slimea.health = slimea.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (slimea.health <= 0)
						{
							slimea.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)sax, (short)say };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[say][sax] = secondtilemap[say][sax];
							map[say][sax] = tilemap[say][sax].ascii;
							std::cout << map[say][sax];
							//defeating this slime will unlock a door
							x = 10;
							y = 17;
							coord = { (short)x, (short)y };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[y][x] = UnlockedDoor;
							map[y][x] = tilemap[y][x].ascii;
							std::cout << map[y][x];
							x = PC.x;
							y = PC.y;
						}
						else if (slimea.health < 10)
						{
							std::cout << slimea.health << " ";
						}
						else
						{
							std::cout << slimea.health;
						}
						PC.y++;
					}
					else if ((PC.y == sby && PC.x == sbx) && (slimeb.health > 0)) //slime b
					{
						slimeb.health = slimeb.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (slimeb.health <= 0)
						{
							slimeb.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)sax, (short)say };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[sby][sbx] = secondtilemap[sby][sbx];
							map[sby][sbx] = tilemap[sby][sbx].ascii;
							std::cout << map[sby][sbx];
							//defeating this slime will unlock a door
							x = 3;
							y = 17;
							coord = { (short)x, (short)y };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[y][x] = UnlockedDoor;
							map[y][x] = tilemap[y][x].ascii;
							std::cout << map[y][x];
							x = PC.x;
							y = PC.y;
						}
						else if (slimeb.health < 10)
						{
							std::cout << slimeb.health << " ";
						}
						else
						{
							std::cout << slimeb.health;
						}
						PC.y++;
					}
					else if ((PC.y == scy && PC.x == scx) && (slimec.health > 0)) //slime c
					{
						slimec.health = slimec.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (slimec.health <= 0)
						{
							slimec.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)scx, (short)scy };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[scy][scx] = secondtilemap[scy][scx];
							map[scy][scx] = tilemap[scy][scx].ascii;
							std::cout << map[scy][scx];
						}
						else if (slimec.health < 10)
						{
							std::cout << slimec.health << " ";
						}
						else
						{
							std::cout << slimec.health;
						}
						PC.y++;
					}
					else
					{
						PC.y++;
					}
				}
				else if (tilemap[PC.y][PC.x].ascii == 'Z')
				{
					if ((PC.y == zay && PC.x == zax) && (zombiea.health > 0)) //zombie a
					{
						zombiea.health = zombiea.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (zombiea.health <= 0)
						{
							zombiea.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)zax, (short)zay };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[zay][zax] = secondtilemap[zay][zax];
							map[zay][zax] = tilemap[zay][zax].ascii;
							std::cout << map[zay][zax];
							//defeating this zombie will give the player a key
							++numKeys;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)aa, (short)bb };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << numKeys;
						}
						else if (zombiea.health < 10)
						{
							std::cout << zombiea.health << " ";
						}
						else
						{
							std::cout << zombiea.health;
						}
						PC.y++;
					}
					else if ((PC.y == zby && PC.x == zbx) && (zombieb.health > 0)) //zombie b
					{
						zombieb.health = zombieb.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (zombieb.health <= 0)
						{
							zombieb.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)zbx, (short)zby };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[zby][zbx] = secondtilemap[zby][zbx];
							map[zby][zbx] = tilemap[zby][zbx].ascii;
							std::cout << map[zby][zbx];
						}
						else if (zombieb.health < 10)
						{
							std::cout << zombieb.health << " ";
						}
						else
						{
							std::cout << zombieb.health;
						}
						PC.y++;
					}
					else if ((PC.y == zcy && PC.x == zcx) && (zombiec.health > 0)) //zombie c
					{
						zombiec.health = zombiec.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (zombiec.health <= 0)
						{
							zombiec.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)zcx, (short)zcy };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[zcy][zcx] = secondtilemap[zcy][zcx];
							map[zcy][zcx] = tilemap[zcy][zcx].ascii;
							std::cout << map[zcy][zcx];
							//defeating this zombie will open a wall
							x = 6;
							y = 34;
							coord = { (short)x, (short)y };
							SetConsoleCursorPosition(hStd, coord);
							secondtilemap[y][x] = UnlockedDoor;
							tilemap[y][x] = secondtilemap[y][x];
							map[y][x] = tilemap[y][x].ascii;
							std::cout << map[y][x];
							x = PC.x;
							y = PC.y;
						}
						else if (zombiec.health < 10)
						{
							std::cout << zombiec.health << " ";
						}
						else
						{
							std::cout << zombiec.health;
						}
						PC.y++;
					}
					else if ((PC.y == zdy && PC.x == zdx) && (zombied.health > 0)) //zombie d
					{
						zombied.health = zombied.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (zombied.health <= 0)
						{
							zombied.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)zdx, (short)zdy };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[zdy][zdx] = secondtilemap[zdy][zdx];
							map[zdy][zdx] = tilemap[zdy][zdx].ascii;
							std::cout << map[zdy][zdx];
						}
						else if (zombied.health < 10)
						{
							std::cout << zombied.health << " ";
						}
						else
						{
							std::cout << zombied.health;
						}
						PC.y++;
					}
					else if ((PC.y == zey && PC.x == zex) && (zombiee.health > 0)) //zombie e
					{
						zombiee.health = zombiee.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (zombiee.health <= 0)
						{
							zombiee.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)zex, (short)zey };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[zey][zex] = secondtilemap[zey][zex];
							map[zey][zex] = tilemap[zey][zex].ascii;
							std::cout << map[zey][zex];
							//defeating this zombie will give the player a key
							++numKeys;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)aa, (short)bb };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << numKeys;
						}
						else if (zombiee.health < 10)
						{
							std::cout << zombiee.health << " ";
						}
						else
						{
							std::cout << zombiee.health;
						}
						PC.y++;
					}
					else
					{
						PC.y++;
					}
				}
				else if (tilemap[PC.y][PC.x].ascii == 'L') //lich
				{
					lich.health = lich.health - PC.strength;
					SetConsoleTextAttribute(hStd, 0x07);
					coord = { (short)gg, (short)hh };
					SetConsoleCursorPosition(hStd, coord);
					if (lich.health <= 0)
					{
						lich.health = 0;
						std::cout << "- ";
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)lx, (short)ly };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[ly][lx] = secondtilemap[ly][lx];
						map[ly][lx] = tilemap[ly][lx].ascii;
						std::cout << map[ly][lx];
						//defeating lich reveals hidden passage
						x = 9;
						y = 8;
						coord = { (short)x, (short)y };
						SetConsoleCursorPosition(hStd, coord);
						secondtilemap[y][x] = UnlockedDoor;
						tilemap[y][x] = secondtilemap[y][x];
						map[y][x] = tilemap[y][x].ascii;
						std::cout << map[y][x];
						x = PC.x;
						y = PC.y;
					}
					else if (lich.health < 10)
					{
						std::cout << lich.health << " ";
					}
					else
					{
						std::cout << lich.health;
					}
					PC.y++;
				}
				else if (tilemap[PC.y][PC.x].ascii == 'D')
				{
					dragon.health = dragon.health - PC.strength;
					SetConsoleTextAttribute(hStd, 0x07);
					coord = { (short)gg, (short)hh };
					SetConsoleCursorPosition(hStd, coord);
					if (dragon.health <= 0)
					{
						dragon.health = 0;
						std::cout << "- ";
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)dx, (short)dy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[dy][dx] = secondtilemap[dy][dx];
						map[dy][dx] = tilemap[dy][dx].ascii;
						std::cout << map[dy][dx];
						win = true;
						break;
					}
					else if (dragon.health < 10)
					{
						std::cout << dragon.health << " ";
					}
					else
					{
						std::cout << dragon.health;
					}
					PC.y++;
				}
				else
				{
					PC.y++;
				}
			}			
			else
			{
				PC.y++;			
				SetConsoleTextAttribute(hStd, 0x07);
				coord = { (short)PC.x, (short)PC.y };
				SetConsoleCursorPosition(hStd, coord);
				tilemap[PC.y][PC.x] = secondtilemap[PC.y][PC.x];
				map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
				std::cout << map[PC.y][PC.x];
				PC.y--;
				if (secondtilemap[PC.y][PC.x].ascii == '!')
				{
					++numPotions;
					secondtilemap[PC.y][PC.x] = Floor;
					SetConsoleTextAttribute(hStd, 0x07);
					coord = { (short)ee, (short)ff };
					SetConsoleCursorPosition(hStd, coord);
					std::cout << numPotions;
				}
				else if (secondtilemap[PC.y][PC.x].ascii == '?')
				{
					if (PC.x == 12 && PC.y == 24)
					{						
						secondtilemap[PC.y][PC.x] = Floor;
						x = 14;
						y = 23;
						secondtilemap[x][y] = Floor;
						tilemap[x][y] = secondtilemap[x][y];
						map[x][y] = tilemap[x][y].ascii;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)x, (short)y };
						SetConsoleCursorPosition(hStd, coord);
						savevalue = PC.x;
						savevalue2 = PC.y;
						PC.x = x;
						PC.y = y;
						secondtilemap[PC.y][PC.x] = tilemap[x][y];
						tilemap[PC.y][PC.x] = secondtilemap[PC.y][PC.x];
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						std::wcout << map[PC.y][PC.x];
						x = 13;
						y = 18;
						secondtilemap[x][y] = Floor;
						tilemap[x][y] = secondtilemap[x][y];
						map[x][y] = tilemap[x][y].ascii;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)x, (short)y };
						SetConsoleCursorPosition(hStd, coord);
						PC.x = x;
						PC.y = y;
						secondtilemap[PC.y][PC.x] = tilemap[x][y];
						tilemap[PC.y][PC.x] = secondtilemap[PC.y][PC.x];
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						std::wcout << map[PC.y][PC.x];
						PC.x = savevalue;
						PC.y = savevalue2;
					}
					else if (PC.x == 1 && PC.y == 21)
					{
						secondtilemap[PC.y][PC.x] = Floor;
						x = 7;
						y = 26;
						secondtilemap[x][y] = Floor;
						tilemap[x][y] = secondtilemap[x][y];
						map[x][y] = tilemap[x][y].ascii;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)x, (short)y };
						SetConsoleCursorPosition(hStd, coord);
						savevalue = PC.x;
						savevalue2 = PC.y;
						PC.x = x;
						PC.y = y;
						secondtilemap[PC.y][PC.x] = tilemap[x][y];
						tilemap[PC.y][PC.x] = secondtilemap[PC.y][PC.x];
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						std::wcout << map[PC.y][PC.x];
						PC.x = savevalue;
						PC.y = savevalue2;
					}
					else if (PC.x == 11 && PC.y == 37)
					{
						secondtilemap[PC.y][PC.x] = Floor;
						x = 12;
						y = 15;
						secondtilemap[x][y] = Floor;
						tilemap[x][y] = secondtilemap[x][y];
						map[x][y] = tilemap[x][y].ascii;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)x, (short)y };
						SetConsoleCursorPosition(hStd, coord);
						savevalue = PC.x;
						savevalue2 = PC.y;
						PC.x = x;
						PC.y = y;
						secondtilemap[PC.y][PC.x] = tilemap[x][y];
						tilemap[PC.y][PC.x] = secondtilemap[PC.y][PC.x];
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						std::wcout << map[PC.y][PC.x];
						PC.x = savevalue;
						PC.y = savevalue2;
					}
				}
				SetConsoleTextAttribute(hStd, 0x03);
				coord = { (short)PC.x, (short)PC.y };
				SetConsoleCursorPosition(hStd, coord);
				tilemap[PC.y][PC.x] = Player;
				map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
				std::cout << map[PC.y][PC.x];	
			}
		}
		else if (c == KEY_DOWN)
		{
			PC.y++;
			if (tilemap[PC.y][PC.x].collide == true)
			{
				if (tilemap[PC.y][PC.x].ascii == 'S')
				{
					if ((PC.y == say && PC.x == sax) && (slimea.health > 0)) //slime a
					{
						slimea.health = slimea.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (slimea.health <= 0)
						{
							slimea.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)sax, (short)say };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[say][sax] = secondtilemap[say][sax];
							map[say][sax] = tilemap[say][sax].ascii;
							std::cout << map[say][sax];
							//defeating this slime will unlock a door
							x = 10;
							y = 17;
							coord = { (short)x, (short)y };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[y][x] = UnlockedDoor;
							map[y][x] = tilemap[y][x].ascii;
							std::cout << map[y][x];
							x = PC.x;
							y = PC.y;
						}
						else if (slimea.health < 10)
						{
							std::cout << slimea.health << " ";
						}
						else
						{
							std::cout << slimea.health;
						}
						PC.y--;
					}
					else if ((PC.y == sby && PC.x == sbx) && (slimeb.health > 0)) //slime b
					{
						slimeb.health = slimeb.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (slimeb.health <= 0)
						{
							slimeb.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)sbx, (short)sby };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[sby][sbx] = secondtilemap[sby][sbx];
							map[sby][sbx] = tilemap[sby][sbx].ascii;
							std::cout << map[sby][sbx];
							//defeating this slime will unlock a door
							x = 3;
							y = 17;
							coord = { (short)x, (short)y };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[y][x] = UnlockedDoor;
							map[y][x] = tilemap[y][x].ascii;
							std::cout << map[y][x];
							x = PC.x;
							y = PC.y;
						}
						else if (slimeb.health < 10)
						{
							std::cout << slimeb.health << " ";
						}
						else
						{
							std::cout << slimeb.health;
						}
						PC.y--;
					}
					else if ((PC.y == scy && PC.x == scx) && (slimec.health > 0)) //slime c
					{
						slimec.health = slimec.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (slimec.health <= 0)
						{
							slimec.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)scx, (short)scy };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[scy][scx] = secondtilemap[scy][scx];
							map[scy][scx] = tilemap[scy][scx].ascii;
							std::cout << map[scy][scx];
						}
						else if (slimec.health < 10)
						{
							std::cout << slimec.health << " ";
						}
						else
						{
							std::cout << slimec.health;
						}
						PC.y--;
					}
					else
					{
						PC.y--;
					}
				}
				else if (tilemap[PC.y][PC.x].ascii == 'Z')
				{
					if ((PC.y == zay && PC.x == zax) && (zombiea.health > 0)) //zombie a
					{
						zombiea.health = zombiea.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (zombiea.health <= 0)
						{
							zombiea.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)zax, (short)zay };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[zay][zax] = secondtilemap[zay][zax];
							map[zay][zax] = tilemap[zay][zax].ascii;
							std::cout << map[zay][zax];
							//defeating this zombie will give the player a key
							++numKeys;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)aa, (short)bb };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << numKeys;
						}
						else if (zombiea.health < 10)
						{
							std::cout << zombiea.health << " ";
						}
						else
						{
							std::cout << zombiea.health;
						}
						PC.y--;
					}
					else if ((PC.y == zby && PC.x == zbx) && (zombieb.health > 0)) //zombie b
					{
						zombieb.health = zombieb.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (zombieb.health <= 0)
						{
							zombieb.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)zbx, (short)zby };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[zby][zbx] = secondtilemap[zby][zbx];
							map[zby][zbx] = tilemap[zby][zbx].ascii;
							std::cout << map[zby][zbx];
						}
						else if (zombieb.health < 10)
						{
							std::cout << zombieb.health << " ";
						}
						else
						{
							std::cout << zombieb.health;
						}
						PC.y--;
					}
					else if ((PC.y == zcy && PC.x == zcx) && (zombiec.health > 0)) //zombie c
					{
						zombiec.health = zombiec.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (zombiec.health <= 0)
						{
							zombiec.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)zcx, (short)zcy };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[zcy][zcx] = secondtilemap[zcy][zcx];
							map[zcy][zcx] = tilemap[zcy][zcx].ascii;
							std::cout << map[zcy][zcx];
							//defeating this zombie will open a wall
							x = 6;
							y = 34;
							coord = { (short)x, (short)y };
							SetConsoleCursorPosition(hStd, coord);
							secondtilemap[y][x] = UnlockedDoor;
							tilemap[y][x] = secondtilemap[y][x];
							map[y][x] = tilemap[y][x].ascii;
							std::cout << map[y][x];
							x = PC.x;
							y = PC.y;
						}
						else if (zombiec.health < 10)
						{
							std::cout << zombiec.health << " ";
						}
						else
						{
							std::cout << zombiec.health;
						}
						PC.y--;
					}
					else if ((PC.y == zdy && PC.x == zdx) && (zombied.health > 0)) //zombie d
					{
						zombied.health = zombied.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (zombied.health <= 0)
						{
							zombied.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)zdx, (short)zdy };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[zdy][zdx] = secondtilemap[zdy][zdx];
							map[zdy][zdx] = tilemap[zdy][zdx].ascii;
							std::cout << map[zdy][zdx];
						}
						else if (zombied.health < 10)
						{
							std::cout << zombied.health << " ";
						}
						else
						{
							std::cout << zombied.health;
						}
						PC.y--;
					}
					else if ((PC.y == zey && PC.x == zex) && (zombiee.health > 0)) //zombie e
					{
						zombiee.health = zombiee.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (zombiee.health <= 0)
						{
							zombiee.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)zex, (short)zey };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[zey][zex] = secondtilemap[zey][zex];
							map[zey][zex] = tilemap[zey][zex].ascii;
							std::cout << map[zey][zex];
							//defeating this zombie will give the player a key
							++numKeys;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)aa, (short)bb };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << numKeys;
						}
						else if (zombiee.health < 10)
						{
							std::cout << zombiee.health << " ";
						}
						else
						{
							std::cout << zombiee.health;
						}
						PC.y--;
					}
					else
					{
						PC.y--;
					}
				}
				else if (tilemap[PC.y][PC.x].ascii == 'L') //lich
				{
					lich.health = lich.health - PC.strength;
					SetConsoleTextAttribute(hStd, 0x07);
					coord = { (short)gg, (short)hh };
					SetConsoleCursorPosition(hStd, coord);
					if (lich.health <= 0)
					{
						lich.health = 0;
						std::cout << "- ";
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)lx, (short)ly };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[ly][lx] = secondtilemap[ly][lx];
						map[ly][lx] = tilemap[ly][lx].ascii;
						std::cout << map[ly][lx];
						//defeating lich reveals hidden passage
						x = 9;
						y = 8;
						coord = { (short)x, (short)y };
						SetConsoleCursorPosition(hStd, coord);
						secondtilemap[y][x] = UnlockedDoor;
						tilemap[y][x] = secondtilemap[y][x];
						map[y][x] = tilemap[y][x].ascii;
						std::cout << map[y][x];
						x = PC.x;
						y = PC.y;
					}
					else if (lich.health < 10)
					{
						std::cout << lich.health << " ";
					}
					else
					{
						std::cout << lich.health;
					}
					PC.y--;
				}
				else if (tilemap[PC.y][PC.x].ascii == 'D')
				{
					dragon.health = dragon.health - PC.strength;
					SetConsoleTextAttribute(hStd, 0x07);
					coord = { (short)gg, (short)hh };
					SetConsoleCursorPosition(hStd, coord);
					if (dragon.health <= 0)
					{
						dragon.health = 0;
						std::cout << "- ";
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)dx, (short)dy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[dy][dx] = secondtilemap[dy][dx];
						map[dy][dx] = tilemap[dy][dx].ascii;
						std::cout << map[dy][dx];
						win = true;
						break;
					}
					else if (dragon.health < 10)
					{
						std::cout << dragon.health << " ";
					}
					else
					{
						std::cout << dragon.health;
					}
					PC.y--;
				}
				else
				{
					PC.y--;
				}
			}
			else
			{
				PC.y--;
				SetConsoleTextAttribute(hStd, 0x07);
				coord = { (short)PC.x, (short)PC.y };
				SetConsoleCursorPosition(hStd, coord);
				tilemap[PC.y][PC.x] = secondtilemap[PC.y][PC.x];
				map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
				std::cout << map[PC.y][PC.x];
				PC.y++;
				if (secondtilemap[PC.y][PC.x].ascii == '!')
				{
					++numPotions;
					secondtilemap[PC.y][PC.x] = Floor;
					SetConsoleTextAttribute(hStd, 0x07);
					coord = { (short)ee, (short)ff };
					SetConsoleCursorPosition(hStd, coord);
					std::cout << numPotions;
				}
				else if (secondtilemap[PC.y][PC.x].ascii == '?')
				{
					if (PC.x == 12 && PC.y == 24)
					{
						secondtilemap[PC.y][PC.x] = Floor;
						x = 14;
						y = 23;
						secondtilemap[x][y] = Floor;
						tilemap[x][y] = secondtilemap[x][y];
						map[x][y] = tilemap[x][y].ascii;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)x, (short)y };
						SetConsoleCursorPosition(hStd, coord);
						savevalue = PC.x;
						savevalue2 = PC.y;
						PC.x = x;
						PC.y = y;
						secondtilemap[PC.y][PC.x] = tilemap[x][y];
						tilemap[PC.y][PC.x] = secondtilemap[PC.y][PC.x];
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						std::wcout << map[PC.y][PC.x];
						x = 13;
						y = 18;
						secondtilemap[x][y] = Floor;
						tilemap[x][y] = secondtilemap[x][y];
						map[x][y] = tilemap[x][y].ascii;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)x, (short)y };
						SetConsoleCursorPosition(hStd, coord);
						PC.x = x;
						PC.y = y;
						secondtilemap[PC.y][PC.x] = tilemap[x][y];
						tilemap[PC.y][PC.x] = secondtilemap[PC.y][PC.x];
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						std::wcout << map[PC.y][PC.x];
						PC.x = savevalue;
						PC.y = savevalue2;
					}
					else if (PC.x == 1 && PC.y == 21)
					{
						secondtilemap[PC.y][PC.x] = Floor;
						x = 7;
						y = 26;
						secondtilemap[x][y] = Floor;
						tilemap[x][y] = secondtilemap[x][y];
						map[x][y] = tilemap[x][y].ascii;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)x, (short)y };
						SetConsoleCursorPosition(hStd, coord);
						savevalue = PC.x;
						savevalue2 = PC.y;
						PC.x = x;
						PC.y = y;
						secondtilemap[PC.y][PC.x] = tilemap[x][y];
						tilemap[PC.y][PC.x] = secondtilemap[PC.y][PC.x];
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						std::wcout << map[PC.y][PC.x];
						PC.x = savevalue;
						PC.y = savevalue2;
					}
					else if (PC.x == 11 && PC.y == 37)
					{
						secondtilemap[PC.y][PC.x] = Floor;
						x = 12;
						y = 15;
						secondtilemap[x][y] = Floor;
						tilemap[x][y] = secondtilemap[x][y];
						map[x][y] = tilemap[x][y].ascii;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)x, (short)y };
						SetConsoleCursorPosition(hStd, coord);
						savevalue = PC.x;
						savevalue2 = PC.y;
						PC.x = x;
						PC.y = y;
						secondtilemap[PC.y][PC.x] = tilemap[x][y];
						tilemap[PC.y][PC.x] = secondtilemap[PC.y][PC.x];
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						std::wcout << map[PC.y][PC.x];
						PC.x = savevalue;
						PC.y = savevalue2;
					}
					else if (PC.x == 16 && PC.y == 6)
					{
						secondtilemap[PC.y][PC.x] = Floor;
						x = 9;
						y = 31;
						secondtilemap[x][y] = Floor;
						tilemap[x][y] = secondtilemap[x][y];
						map[x][y] = tilemap[x][y].ascii;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)x, (short)y };
						SetConsoleCursorPosition(hStd, coord);
						savevalue = PC.x;
						savevalue2 = PC.y;
						PC.x = x;
						PC.y = y;
						secondtilemap[PC.y][PC.x] = tilemap[x][y];
						tilemap[PC.y][PC.x] = secondtilemap[PC.y][PC.x];
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						std::wcout << map[PC.y][PC.x];
						PC.x = savevalue;
						PC.y = savevalue2;
					}
				}
				SetConsoleTextAttribute(hStd, 0x03);
				coord = { (short)PC.x, (short)PC.y };
				SetConsoleCursorPosition(hStd, coord);
				tilemap[PC.y][PC.x] = Player;
				map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
				std::cout << map[PC.y][PC.x];
			}
		}
		else if (c == KEY_LEFT)
		{
			PC.x--;
			if (tilemap[PC.y][PC.x].collide == true)
			{
				if (tilemap[PC.y][PC.x].ascii == 'S')
				{
					if ((PC.y == say && PC.x == sax) && (slimea.health > 0)) //slime a
					{
						slimea.health = slimea.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (slimea.health <= 0)
						{
							slimea.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)sax, (short)say };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[say][sax] = secondtilemap[say][sax];
							map[say][sax] = tilemap[say][sax].ascii;
							std::cout << map[say][sax];
							//defeating this slime will unlock a door
							x = 10;
							y = 17;
							coord = { (short)x, (short)y };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[y][x] = UnlockedDoor;
							map[y][x] = tilemap[y][x].ascii;
							std::cout << map[y][x];
							x = PC.x;
							y = PC.y;
						}
						else if (slimea.health < 10)
						{
							std::cout << slimea.health << " ";
						}
						else
						{		
							std::cout << slimea.health;
						}
						PC.x++;
					}
					else if ((PC.y == sby && PC.x == sbx) && (slimeb.health > 0)) //slime b
					{
						slimeb.health = slimeb.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (slimeb.health <= 0)
						{
							slimeb.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)sbx, (short)sby };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[sby][sbx] = secondtilemap[sby][sbx];
							map[sby][sbx] = tilemap[sby][sbx].ascii;
							std::cout << map[sby][sbx];
							//defeating this slime will unlock a door
							x = 3;
							y = 17;
							coord = { (short)x, (short)y };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[y][x] = UnlockedDoor;
							map[y][x] = tilemap[y][x].ascii;
							std::cout << map[y][x];
							x = PC.x;
							y = PC.y;
						}
						else if (slimeb.health < 10)
						{
							std::cout << slimeb.health << " ";
						}
						else
						{
							std::cout << slimeb.health;
						}
						PC.x++;
					}
					else if ((PC.y == scy && PC.x == scx) && (slimec.health > 0)) //slime c
					{
						slimec.health = slimec.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (slimec.health <= 0)
						{
							slimec.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)scx, (short)scy };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[scy][scx] = secondtilemap[scy][scx];
							map[scy][scx] = tilemap[scy][scx].ascii;
							std::cout << map[scy][scx];
						}
						else if (slimec.health < 10)
						{
							std::cout << slimec.health << " ";
						}
						else
						{
							std::cout << slimec.health;
						}
						PC.x++;
					}
					else
					{
						PC.x++;
					}
				}
				else if (tilemap[PC.y][PC.x].ascii == 'Z')
				{
					if ((PC.y == zay && PC.x == zax) && (zombiea.health > 0)) //zombie a
					{
						zombiea.health = zombiea.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (zombiea.health <= 0)
						{
							zombiea.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)zax, (short)zay };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[zay][zax] = secondtilemap[zay][zax];
							map[zay][zax] = tilemap[zay][zax].ascii;
							std::cout << map[zay][zax];
							//defeating this zombie will give the player a key
							++numKeys;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)aa, (short)bb };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << numKeys;
						}
						else if (zombiea.health < 10)
						{
							std::cout << zombiea.health << " ";
						}
						else
						{
							std::cout << zombiea.health;
						}
						PC.x++;
					}
					else if ((PC.y == zby && PC.x == zbx) && (zombieb.health > 0)) //zombie b
					{
						zombieb.health = zombieb.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (zombieb.health <= 0)
						{
							zombieb.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)zbx, (short)zby };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[zby][zbx] = secondtilemap[zby][zbx];
							map[zby][zbx] = tilemap[zby][zbx].ascii;
							std::cout << map[zby][zbx];
						}
						else if (zombieb.health < 10)
						{
							std::cout << zombieb.health << " ";
						}
						else
						{
							std::cout << zombieb.health;
						}
						PC.x++;
					}
					else if ((PC.y == zcy && PC.x == zcx) && (zombiec.health > 0)) //zombie c
					{
						zombiec.health = zombiec.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (zombiec.health <= 0)
						{
							zombiec.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)zcx, (short)zcy };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[zcy][zcx] = secondtilemap[zcy][zcx];
							map[zcy][zcx] = tilemap[zcy][zcx].ascii;
							std::cout << map[zcy][zcx];
							//defeating this zombie will open a wall
							x = 6;
							y = 34;
							coord = { (short)x, (short)y };
							SetConsoleCursorPosition(hStd, coord);
							secondtilemap[y][x] = UnlockedDoor;
							tilemap[y][x] = secondtilemap[y][x];
							map[y][x] = tilemap[y][x].ascii;
							std::cout << map[y][x];
							x = PC.x;
							y = PC.y;
						}
						else if (zombiec.health < 10)
						{
							std::cout << zombiec.health << " ";
						}
						else
						{
							std::cout << zombiec.health;
						}
						PC.x++;
					}
					else if ((PC.y == zdy && PC.x == zdx) && (zombied.health > 0)) //zombie d
					{
						zombied.health = zombied.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (zombied.health <= 0)
						{
							zombied.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)zdx, (short)zdy };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[zdy][zdx] = secondtilemap[zdy][zdx];
							map[zdy][zdx] = tilemap[zdy][zdx].ascii;
							std::cout << map[zdy][zdx];
						}
						else if (zombied.health < 10)
						{
							std::cout << zombied.health << " ";
						}
						else
						{
							std::cout << zombied.health;
						}
						PC.x++;
					}
					else if ((PC.y == zey && PC.x == zex) && (zombiee.health > 0)) //zombie e
					{
						zombiee.health = zombiee.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (zombiee.health <= 0)
						{
							zombiee.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)zex, (short)zey };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[zey][zex] = secondtilemap[zey][zex];
							map[zey][zex] = tilemap[zey][zex].ascii;
							std::cout << map[zey][zex];
							//defeating this zombie will give the player a key
							++numKeys;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)aa, (short)bb };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << numKeys;
						}
						else if (zombiee.health < 10)
						{
							std::cout << zombiee.health << " ";
						}
						else
						{
							std::cout << zombiee.health;
						}
						PC.x++;
					}
					else
					{
						PC.x++;
					}
				}
				else if (tilemap[PC.y][PC.x].ascii == 'L') //lich
				{
					lich.health = lich.health - PC.strength;
					SetConsoleTextAttribute(hStd, 0x07);
					coord = { (short)gg, (short)hh };
					SetConsoleCursorPosition(hStd, coord);
					if (lich.health <= 0)
					{
						lich.health = 0;
						std::cout << "- ";
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)lx, (short)ly };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[ly][lx] = secondtilemap[ly][lx];
						map[ly][lx] = tilemap[ly][lx].ascii;
						std::cout << map[ly][lx];
						//defeating lich reveals hidden passage
						x = 9;
						y = 8;
						coord = { (short)x, (short)y };
						SetConsoleCursorPosition(hStd, coord);
						secondtilemap[y][x] = UnlockedDoor;
						tilemap[y][x] = secondtilemap[y][x];
						map[y][x] = tilemap[y][x].ascii;
						std::cout << map[y][x];
						x = PC.x;
						y = PC.y;
					}
					else if (lich.health < 10)
					{
						std::cout << lich.health << " ";
					}
					else
					{
						std::cout << lich.health;
					}
					PC.x++;
				}
				else if (tilemap[PC.y][PC.x].ascii == 'D')
				{
					dragon.health = dragon.health - PC.strength;
					SetConsoleTextAttribute(hStd, 0x07);
					coord = { (short)gg, (short)hh };
					SetConsoleCursorPosition(hStd, coord);
					if (dragon.health <= 0)
					{
						dragon.health = 0;
						std::cout << "- ";
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)dx, (short)dy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[dy][dx] = secondtilemap[dy][dx];
						map[dy][dx] = tilemap[dy][dx].ascii;
						std::cout << map[dy][dx];
						win = true;
						break;
					}
					else if (dragon.health < 10)
					{
						std::cout << dragon.health << " ";
					}
					else
					{
						std::cout << dragon.health;
					}
					PC.x++;
				}
				else
				{
					PC.x++;
				}
			}
			else
			{
				PC.x++;
				SetConsoleTextAttribute(hStd, 0x07);
				coord = { (short)PC.x, (short)PC.y };
				SetConsoleCursorPosition(hStd, coord);
				tilemap[PC.y][PC.x] = secondtilemap[PC.y][PC.x];
				map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
				std::cout << map[PC.y][PC.x];
				PC.x--;
				if (secondtilemap[PC.y][PC.x].ascii == '!')
				{
					++numPotions;
					secondtilemap[PC.y][PC.x] = Floor;
					SetConsoleTextAttribute(hStd, 0x07);
					coord = { (short)ee, (short)ff };
					SetConsoleCursorPosition(hStd, coord);
					std::cout << numPotions;
				}
				else if (secondtilemap[PC.y][PC.x].ascii == '?')
				{
					if (PC.x == 12 && PC.y == 24)
					{
						secondtilemap[PC.y][PC.x] = Floor;
						x = 14;
						y = 23;
						secondtilemap[x][y] = Floor;
						tilemap[x][y] = secondtilemap[x][y];
						map[x][y] = tilemap[x][y].ascii;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)x, (short)y };
						SetConsoleCursorPosition(hStd, coord);
						savevalue = PC.x;
						savevalue2 = PC.y;
						PC.x = x;
						PC.y = y;
						secondtilemap[PC.y][PC.x] = tilemap[x][y];
						tilemap[PC.y][PC.x] = secondtilemap[PC.y][PC.x];
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						std::wcout << map[PC.y][PC.x];
						x = 13;
						y = 18;
						secondtilemap[x][y] = Floor;
						tilemap[x][y] = secondtilemap[x][y];
						map[x][y] = tilemap[x][y].ascii;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)x, (short)y };
						SetConsoleCursorPosition(hStd, coord);
						PC.x = x;
						PC.y = y;
						secondtilemap[PC.y][PC.x] = tilemap[x][y];
						tilemap[PC.y][PC.x] = secondtilemap[PC.y][PC.x];
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						std::wcout << map[PC.y][PC.x];
						PC.x = savevalue;
						PC.y = savevalue2;
					}
					else if (PC.x == 1 && PC.y == 21)
					{
						secondtilemap[PC.y][PC.x] = Floor;
						x = 7;
						y = 26;
						secondtilemap[x][y] = Floor;
						tilemap[x][y] = secondtilemap[x][y];
						map[x][y] = tilemap[x][y].ascii;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)x, (short)y };
						SetConsoleCursorPosition(hStd, coord);
						savevalue = PC.x;
						savevalue2 = PC.y;
						PC.x = x;
						PC.y = y;
						secondtilemap[PC.y][PC.x] = tilemap[x][y];
						tilemap[PC.y][PC.x] = secondtilemap[PC.y][PC.x];
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						std::wcout << map[PC.y][PC.x];
						PC.x = savevalue;
						PC.y = savevalue2;
					}
					else if (PC.x == 11 && PC.y == 37)
					{
						secondtilemap[PC.y][PC.x] = Floor;
						x = 12;
						y = 15;
						secondtilemap[x][y] = Floor;
						tilemap[x][y] = secondtilemap[x][y];
						map[x][y] = tilemap[x][y].ascii;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)x, (short)y };
						SetConsoleCursorPosition(hStd, coord);
						savevalue = PC.x;
						savevalue2 = PC.y;
						PC.x = x;
						PC.y = y;
						secondtilemap[PC.y][PC.x] = tilemap[x][y];
						tilemap[PC.y][PC.x] = secondtilemap[PC.y][PC.x];
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						std::wcout << map[PC.y][PC.x];
						PC.x = savevalue;
						PC.y = savevalue2;
					}
					else if (PC.x == 16 && PC.y == 6)
					{
						secondtilemap[PC.y][PC.x] = Floor;
						x = 9;
						y = 31;
						secondtilemap[x][y] = Floor;
						tilemap[x][y] = secondtilemap[x][y];
						map[x][y] = tilemap[x][y].ascii;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)x, (short)y };
						SetConsoleCursorPosition(hStd, coord);
						savevalue = PC.x;
						savevalue2 = PC.y;
						PC.x = x;
						PC.y = y;
						secondtilemap[PC.y][PC.x] = tilemap[x][y];
						tilemap[PC.y][PC.x] = secondtilemap[PC.y][PC.x];
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						std::wcout << map[PC.y][PC.x];
						PC.x = savevalue;
						PC.y = savevalue2;
					}
				}
				SetConsoleTextAttribute(hStd, 0x03);
				coord = { (short)PC.x, (short)PC.y };
				SetConsoleCursorPosition(hStd, coord);
				tilemap[PC.y][PC.x] = Player;
				map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
				std::cout << map[PC.y][PC.x];
			}
		}
		else if (c == KEY_RIGHT)
		{
			PC.x++;
			if (tilemap[PC.y][PC.x].collide == true)
			{
				if (tilemap[PC.y][PC.x].ascii == 'S')
				{
					if ((PC.y == say && PC.x == sax) && (slimea.health > 0)) //slime a
					{
						slimea.health = slimea.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (slimea.health <= 0)
						{
							slimea.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)sax, (short)say };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[say][sax] = secondtilemap[say][sax];
							map[say][sax] = tilemap[say][sax].ascii;
							std::cout << map[say][sax];
							//defeating this slime will unlock a door
							x = 10;
							y = 17;
							coord = { (short)x, (short)y };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[y][x] = UnlockedDoor;
							map[y][x] = tilemap[y][x].ascii;
							std::cout << map[y][x];
							x = PC.x;
							y = PC.y;
						}
						else if (slimea.health < 10)
						{
							std::cout << slimea.health << " ";
						}
						else
						{
							std::cout << slimea.health;
						}
						PC.x--;
					}
					else if ((PC.y == sby && PC.x == sbx) && (slimeb.health > 0)) //slime b
					{
						slimeb.health = slimeb.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (slimeb.health <= 0)
						{
							slimeb.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)sbx, (short)sby };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[sby][sbx] = secondtilemap[sby][sbx];
							map[sby][sbx] = tilemap[sby][sbx].ascii;
							std::cout << map[sby][sbx];
							//defeating this slime will unlock a door
							x = 3;
							y = 17;
							coord = { (short)x, (short)y };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[y][x] = UnlockedDoor;
							map[y][x] = tilemap[y][x].ascii;
							std::cout << map[y][x];
							x = PC.x;
							y = PC.y;
						}
						else if (slimeb.health < 10)
						{
							std::cout << slimeb.health << " ";
						}
						else
						{
							std::cout << slimeb.health;
						}
						PC.x--;
					}
					else if ((PC.y == scy && PC.x == scx) && (slimec.health > 0)) //slime c
					{
						slimec.health = slimec.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (slimec.health <= 0)
						{
							slimec.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)scx, (short)scy };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[scy][scx] = secondtilemap[scy][scx];
							map[scy][scx] = tilemap[scy][scx].ascii;
							std::cout << map[scy][scx];
						}
						else if (slimec.health < 10)
						{
							std::cout << slimec.health << " ";
						}
						else
						{
							std::cout << slimec.health;
						}
						PC.x--;
					}
					else
					{
						PC.x--;
					}
				}
				else if (tilemap[PC.y][PC.x].ascii == 'Z')
				{
					if ((PC.y == zay && PC.x == zax) && (zombiea.health > 0)) //zombie a
					{
						zombiea.health = zombiea.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (zombiea.health <= 0)
						{
							zombiea.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)zax, (short)zay };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[zay][zax] = secondtilemap[zay][zax];
							map[zay][zax] = tilemap[zay][zax].ascii;
							std::cout << map[zay][zax];
							//defeating this zombie will give the player a key
							++numKeys;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)aa, (short)bb };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << numKeys;
						}
						else if (zombiea.health < 10)
						{
							std::cout << zombiea.health << " ";
						}
						else
						{
							std::cout << zombiea.health;
						}
						PC.x--;
					}
					else if ((PC.y == zby && PC.x == zbx) && (zombieb.health > 0)) //zombie b
					{
						zombieb.health = zombieb.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (zombieb.health <= 0)
						{
							zombieb.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)zbx, (short)zby };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[zby][zbx] = secondtilemap[zby][zbx];
							map[zby][zbx] = tilemap[zby][zbx].ascii;
							std::cout << map[zby][zbx];
						}
						else if (zombieb.health < 10)
						{
							std::cout << zombieb.health << " ";
						}
						else
						{
							std::cout << zombieb.health;
						}
						PC.x--;
					}
					else if ((PC.y == zcy && PC.x == zcx) && (zombiec.health > 0)) //zombie c
					{
						zombiec.health = zombiec.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (zombiec.health <= 0)
						{
							zombiec.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)zcx, (short)zcy };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[zcy][zcx] = secondtilemap[zcy][zcx];
							map[zcy][zcx] = tilemap[zcy][zcx].ascii;
							std::cout << map[zcy][zcx];
							//defeating this zombie will open a wall
							x = 6;
							y = 34;
							coord = { (short)x, (short)y };
							SetConsoleCursorPosition(hStd, coord);
							secondtilemap[y][x] = UnlockedDoor;
							tilemap[y][x] = secondtilemap[y][x];
							map[y][x] = tilemap[y][x].ascii;
							std::cout << map[y][x];
							x = PC.x;
							y = PC.y;
						}
						else if (zombiec.health < 10)
						{
							std::cout << zombiec.health << " ";
						}
						else
						{
							std::cout << zombiec.health;
						}
						PC.x--;
					}
					else if ((PC.y == zdy && PC.x == zdx) && (zombied.health > 0)) //zombie d
					{
						zombied.health = zombied.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (zombied.health <= 0)
						{
							zombied.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)zdx, (short)zdy };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[zdy][zdx] = secondtilemap[zdy][zdx];
							map[zdy][zdx] = tilemap[zdy][zdx].ascii;
							std::cout << map[zdy][zdx];
						}
						else if (zombied.health < 10)
						{
							std::cout << zombied.health << " ";
						}
						else
						{
							std::cout << zombied.health;
						}
						PC.x--;
					}
					else if ((PC.y == zey && PC.x == zex) && (zombiee.health > 0)) //zombie e
					{
						zombiee.health = zombiee.health - PC.strength;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)gg, (short)hh };
						SetConsoleCursorPosition(hStd, coord);
						if (zombiee.health <= 0)
						{
							zombiee.health = 0;
							std::cout << "- ";
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)zex, (short)zey };
							SetConsoleCursorPosition(hStd, coord);
							tilemap[zey][zex] = secondtilemap[zey][zex];
							map[zey][zex] = tilemap[zey][zex].ascii;
							std::cout << map[zey][zex];
							//defeating this zombie will give the player a key
							++numKeys;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)aa, (short)bb };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << numKeys;
						}
						else if (zombiee.health < 10)
						{
							std::cout << zombiee.health << " ";
						}
						else
						{
							std::cout << zombiee.health;
						}
						PC.x--;
					}
					else
					{
						PC.x--;
					}
				}
				else if (tilemap[PC.y][PC.x].ascii == 'L') //lich
				{
					lich.health = lich.health - PC.strength;
					SetConsoleTextAttribute(hStd, 0x07);
					coord = { (short)gg, (short)hh };
					SetConsoleCursorPosition(hStd, coord);
					if (lich.health <= 0)
					{
						lich.health = 0;
						std::cout << "- ";
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)lx, (short)ly };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[ly][lx] = secondtilemap[ly][lx];
						map[ly][lx] = tilemap[ly][lx].ascii;
						std::cout << map[ly][lx];
						//defeating lich reveals hidden passage
						x = 9;
						y = 8;
						coord = { (short)x, (short)y };
						SetConsoleCursorPosition(hStd, coord);
						secondtilemap[y][x] = UnlockedDoor;
						tilemap[y][x] = secondtilemap[y][x];
						map[y][x] = tilemap[y][x].ascii;
						std::cout << map[y][x];
						x = PC.x;
						y = PC.y;
					}
					else if (lich.health < 10)
					{
						std::cout << lich.health << " ";
					}
					else
					{
						std::cout << lich.health;
					}
					PC.x--;
				}
				else if (tilemap[PC.y][PC.x].ascii == 'D')
				{
					dragon.health = dragon.health - PC.strength;
					SetConsoleTextAttribute(hStd, 0x07);
					coord = { (short)gg, (short)hh };
					SetConsoleCursorPosition(hStd, coord);
					if (dragon.health <= 0)
					{
						dragon.health = 0;
						std::cout << "- ";
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)dx, (short)dy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[dy][dx] = secondtilemap[dy][dx];
						map[dy][dx] = tilemap[dy][dx].ascii;
						std::cout << map[dy][dx];
						win = true;
						break;
					}
					else if (dragon.health < 10)
					{
						std::cout << dragon.health << " ";
					}
					else
					{
						std::cout << dragon.health;
					}
					PC.x--;
				}
				else
				{
					PC.x--;
				}
			}
			else
			{
				PC.x--;
				SetConsoleTextAttribute(hStd, 0x07);
				coord = { (short)PC.x, (short)PC.y };
				SetConsoleCursorPosition(hStd, coord);
				tilemap[PC.y][PC.x] = secondtilemap[PC.y][PC.x];
				map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
				std::cout << map[PC.y][PC.x];
				PC.x++;
				if (secondtilemap[PC.y][PC.x].ascii == '!')
				{
					++numPotions;
					secondtilemap[PC.y][PC.x] = Floor;
					SetConsoleTextAttribute(hStd, 0x07);
					coord = { (short)ee, (short)ff };
					SetConsoleCursorPosition(hStd, coord);
					std::cout << numPotions;
				}
				else if (secondtilemap[PC.y][PC.x].ascii == '?')
				{
					if (PC.x == 12 && PC.y == 24)
					{
						secondtilemap[PC.y][PC.x] = Floor;
						x = 14;
						y = 23;
						secondtilemap[x][y] = Floor;
						tilemap[x][y] = secondtilemap[x][y];
						map[x][y] = tilemap[x][y].ascii;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)x, (short)y };
						SetConsoleCursorPosition(hStd, coord);
						savevalue = PC.x;
						savevalue2 = PC.y;
						PC.x = x;
						PC.y = y;
						secondtilemap[PC.y][PC.x] = tilemap[x][y];
						tilemap[PC.y][PC.x] = secondtilemap[PC.y][PC.x];
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						std::wcout << map[PC.y][PC.x];
						x = 13;
						y = 18;
						secondtilemap[x][y] = Floor;
						tilemap[x][y] = secondtilemap[x][y];
						map[x][y] = tilemap[x][y].ascii;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)x, (short)y };
						SetConsoleCursorPosition(hStd, coord);
						PC.x = x;
						PC.y = y;
						secondtilemap[PC.y][PC.x] = tilemap[x][y];
						tilemap[PC.y][PC.x] = secondtilemap[PC.y][PC.x];
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						std::wcout << map[PC.y][PC.x];
						PC.x = savevalue;
						PC.y = savevalue2;
					}
					else if (PC.x == 1 && PC.y == 21)
					{
						secondtilemap[PC.y][PC.x] = Floor;
						x = 7;
						y = 26;
						secondtilemap[x][y] = Floor;
						tilemap[x][y] = secondtilemap[x][y];
						map[x][y] = tilemap[x][y].ascii;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)x, (short)y };
						SetConsoleCursorPosition(hStd, coord);
						savevalue = PC.x;
						savevalue2 = PC.y;
						PC.x = x;
						PC.y = y;
						secondtilemap[PC.y][PC.x] = tilemap[x][y];
						tilemap[PC.y][PC.x] = secondtilemap[PC.y][PC.x];
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						std::wcout << map[PC.y][PC.x];
						PC.x = savevalue;
						PC.y = savevalue2;
					}
					else if (PC.x == 11 && PC.y == 37)
					{
						secondtilemap[PC.y][PC.x] = Floor;
						x = 12;
						y = 15;
						secondtilemap[x][y] = Floor;
						tilemap[x][y] = secondtilemap[x][y];
						map[x][y] = tilemap[x][y].ascii;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)x, (short)y };
						SetConsoleCursorPosition(hStd, coord);
						savevalue = PC.x;
						savevalue2 = PC.y;
						PC.x = x;
						PC.y = y;
						secondtilemap[PC.y][PC.x] = tilemap[x][y];
						tilemap[PC.y][PC.x] = secondtilemap[PC.y][PC.x];
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						std::wcout << map[PC.y][PC.x];
						PC.x = savevalue;
						PC.y = savevalue2;
					}
				}
				SetConsoleTextAttribute(hStd, 0x03);
				coord = { (short)PC.x, (short)PC.y };
				SetConsoleCursorPosition(hStd, coord);
				tilemap[PC.y][PC.x] = Player;
				map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
				std::cout << map[PC.y][PC.x];
			}
		}
		else if (c == KEY_SPACE)
		{
			while (1)
			{
				PC.strength = 10;
				//checking up
				--PC.y;
				if ((tilemap[PC.y][PC.x].ascii == '+') && numKeys > 0)
				{
					--numKeys;
					SetConsoleTextAttribute(hStd, 0x07);
					coord = { (short)aa, (short)bb };
					SetConsoleCursorPosition(hStd, coord);
					std::cout << numKeys;
					tilemap[PC.y][PC.x] = UnlockedDoor;
					map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
					SetConsoleTextAttribute(hStd, 0x07);
					coord = { (short)PC.x, (short)PC.y };
					SetConsoleCursorPosition(hStd, coord);
					std::cout << map[PC.y][PC.x];
					PC.y++;
					break;
				}
				else if ((tilemap[PC.y][PC.x].ascii == '+') && numKeys == 0)
				{
					PC.y++;
					break;
				}
				else
				{
					++PC.y;
					--PC.x;
				}
				//checking left
				if ((tilemap[PC.y][PC.x].ascii == '+') && numKeys > 0)
				{
					--numKeys;
					SetConsoleTextAttribute(hStd, 0x07);
					coord = { (short)aa, (short)bb };
					SetConsoleCursorPosition(hStd, coord);
					std::cout << numKeys;
					tilemap[PC.y][PC.x] = UnlockedDoor;
					map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
					SetConsoleTextAttribute(hStd, 0x07);
					coord = { (short)PC.x, (short)PC.y };
					SetConsoleCursorPosition(hStd, coord);
					std::cout << map[PC.y][PC.x];
					PC.x++;
					break;
				}
				else if ((tilemap[PC.y][PC.x].ascii == '+') && numKeys == 0)
				{
					PC.x++;
					break;
				}
				else
				{
					++PC.x;
					++PC.y;
				}
				//checking down
				if ((tilemap[PC.y][PC.x].ascii == '+') && numKeys > 0)
				{
					--numKeys;
					SetConsoleTextAttribute(hStd, 0x07);
					coord = { (short)aa, (short)bb };
					SetConsoleCursorPosition(hStd, coord);
					std::cout << numKeys;
					tilemap[PC.y][PC.x] = UnlockedDoor;
					map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
					SetConsoleTextAttribute(hStd, 0x07);
					coord = { (short)PC.x, (short)PC.y };
					SetConsoleCursorPosition(hStd, coord);
					std::cout << map[PC.y][PC.x];
					PC.y--;
					break;
				}
				else if ((tilemap[PC.y][PC.x].ascii == '+') && numKeys == 0)
				{
					PC.y--;
					break;
				}
				else
				{
					--PC.y;
					++PC.x;
				}
				//checking right
				if ((tilemap[PC.y][PC.x].ascii == '+') && numKeys > 0)
				{
					--numKeys;
					SetConsoleTextAttribute(hStd, 0x07);
					coord = { (short)aa, (short)bb };
					SetConsoleCursorPosition(hStd, coord);
					std::cout << numKeys;
					tilemap[PC.y][PC.x] = UnlockedDoor;
					map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
					SetConsoleTextAttribute(hStd, 0x07);
					coord = { (short)PC.x, (short)PC.y };
					SetConsoleCursorPosition(hStd, coord);
					std::cout << map[PC.y][PC.x];
					PC.x--;
					break;
				}
				else if ((tilemap[PC.y][PC.x].ascii == '+') && numKeys == 0)
				{
					PC.x--;
					break;
				}
				else
				{
					--PC.x;
				}
				--PC.y;
				//chest up
				if (tilemap[PC.y][PC.x].ascii == 'C')
				{
					if (PC.x == 18 && PC.y == 5)
					{
						Sword[0] = SteelSword;
						coord = { (short)ii, (short)jj };
						SetConsoleCursorPosition(hStd, coord);
						SetConsoleTextAttribute(hStd, 0x07);
						std::cout << Sword[0].name << "   ";
						coord = { (short)kk, (short)ll };
						SetConsoleCursorPosition(hStd, coord);
						PC.strength = PC.strength + Sword[0].value;
						std::cout << PC.strength;
						tilemap[PC.y][PC.x] = Floor;
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						coord = { (short)PC.x, (short)PC.y };
						SetConsoleCursorPosition(hStd, coord);
						std::cout << map[PC.y][PC.x];
						++PC.y;
					}
					else
					{
						++numKeys;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)aa, (short)bb };
						SetConsoleCursorPosition(hStd, coord);
						std::cout << numKeys;
						tilemap[PC.y][PC.x] = Floor;
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)PC.x, (short)PC.y };
						SetConsoleCursorPosition(hStd, coord);
						std::cout << map[PC.y][PC.x];
						++PC.y;
					}
					break;
				}
				else
				{
					++PC.y;
					--PC.x;
				}
				//chest left
				if (tilemap[PC.y][PC.x].ascii == 'C')
				{
					++numKeys;
					SetConsoleTextAttribute(hStd, 0x07);
					coord = { (short)aa, (short)bb };
					SetConsoleCursorPosition(hStd, coord);
					std::cout << numKeys;
					tilemap[PC.y][PC.x] = Floor;
					map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
					SetConsoleTextAttribute(hStd, 0x07);
					coord = { (short)PC.x, (short)PC.y };
					SetConsoleCursorPosition(hStd, coord);
					std::cout << map[PC.y][PC.x];
					PC.x++;
					break;
				}
				else
				{
					++PC.x;
					++PC.y;
				}
				//chest down
				if (tilemap[PC.y][PC.x].ascii == 'C')
				{
					if (PC.x == 12 && PC.y == 37) //can only be interacted with from right & up
					{
						Sword[0] = IronSword;
						coord = { (short)ii, (short)jj };
						SetConsoleCursorPosition(hStd, coord);
						SetConsoleTextAttribute(hStd, 0x07);
						std::cout << Sword[0].name << "   ";
						coord = { (short)kk, (short)ll };
						SetConsoleCursorPosition(hStd, coord);
						PC.strength = PC.strength + Sword[0].value;
						std::cout << PC.strength;
						tilemap[PC.y][PC.x] = Floor;
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						coord = { (short)PC.x, (short)PC.y };
						SetConsoleCursorPosition(hStd, coord);
						std::cout << map[PC.y][PC.x];
						--PC.y;
					}
					else if (PC.x == 18 && PC.y == 5)
					{
						Sword[0] = SteelSword;
						coord = { (short)ii, (short)jj };
						SetConsoleCursorPosition(hStd, coord);
						SetConsoleTextAttribute(hStd, 0x07);
						std::cout << Sword[0].name << "   ";
						coord = { (short)kk, (short)ll };
						SetConsoleCursorPosition(hStd, coord);
						PC.strength = PC.strength + Sword[0].value;
						std::cout << PC.strength;
						tilemap[PC.y][PC.x] = Floor;
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						coord = { (short)PC.x, (short)PC.y };
						SetConsoleCursorPosition(hStd, coord);
						std::cout << map[PC.y][PC.x];
						--PC.y;
					}
					else
					{
						++numKeys;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)aa, (short)bb };
						SetConsoleCursorPosition(hStd, coord);
						std::cout << numKeys;
						tilemap[PC.y][PC.x] = Floor;
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)PC.x, (short)PC.y };
						SetConsoleCursorPosition(hStd, coord);
						std::cout << map[PC.y][PC.x];
						--PC.y;
					}
					break;
				}
				else
				{
					--PC.y;
					++PC.x;
				}
				//chest right
				if (tilemap[PC.y][PC.x].ascii == 'C')
				{
					if (PC.x == 12 && PC.y == 37) //can only be interacted with from right & up
					{
						Sword[0] = IronSword;
						coord = { (short)ii, (short)jj };
						SetConsoleCursorPosition(hStd, coord);
						SetConsoleTextAttribute(hStd, 0x07);
						std::cout << Sword[0].name << "   ";
						coord = { (short)kk, (short)ll };
						SetConsoleCursorPosition(hStd, coord);
						PC.strength = PC.strength + Sword[0].value;
						std::cout << PC.strength;
						tilemap[PC.y][PC.x] = Floor;
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						coord = { (short)PC.x, (short)PC.y };
						SetConsoleCursorPosition(hStd, coord);
						std::cout << map[PC.y][PC.x];
						--PC.x;
					}
					else if (PC.x == 18 && PC.y == 5)
					{
						Sword[0] = SteelSword;
						coord = { (short)ii, (short)jj };
						SetConsoleCursorPosition(hStd, coord);
						SetConsoleTextAttribute(hStd, 0x07);
						std::cout << Sword[0].name << "   ";
						coord = { (short)kk, (short)ll };
						SetConsoleCursorPosition(hStd, coord);
						PC.strength = PC.strength + Sword[0].value;
						std::cout << PC.strength;
						tilemap[PC.y][PC.x] = Floor;
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						coord = { (short)PC.x, (short)PC.y };
						SetConsoleCursorPosition(hStd, coord);
						std::cout << map[PC.y][PC.x];
						--PC.x;
					}
					else
					{
						++numKeys;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)aa, (short)bb };
						SetConsoleCursorPosition(hStd, coord);
						std::cout << numKeys;
						tilemap[PC.y][PC.x] = Floor;
						map[PC.y][PC.x] = tilemap[PC.y][PC.x].ascii;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)PC.x, (short)PC.y };
						SetConsoleCursorPosition(hStd, coord);
						std::cout << map[PC.y][PC.x];
						--PC.x;
					}
					break;
				}
				else
				{
					--PC.x;
				}
				break;
				//otherwise, this acts as a "wait" button, advancing other actors without the player changing position
			}
		}
		else if (c == KEY_ENTER) //this will be the key for consuming a potion
		{
			if (numPotions > 0)
			{
				--numPotions;
				SetConsoleTextAttribute(hStd, 0x07);
				coord = { (short)ee, (short)ff };
				SetConsoleCursorPosition(hStd, coord);
				std::cout << numPotions;
				PC.health = PC.health + Potion.value;
				if (PC.health > 100)
				{
					PC.health = 100;
				}
				if (PC.health < 100)
				{
					SetConsoleTextAttribute(hStd, 0x07);
					coord = { (short)cc, (short)dd };
					SetConsoleCursorPosition(hStd, coord);
					std::cout << PC.health << " ";
				}
				else
				{
					SetConsoleTextAttribute(hStd, 0x07);
					coord = { (short)cc, (short)dd };
					SetConsoleCursorPosition(hStd, coord);
					std::cout << PC.health;
				}
			}
		}
		else continue;
		if (slimea.health > 0)
		{
			//slime a movement
			//slime a chasing player - no escaping this slime!	
			if (sax == PC.x) //same column
			{			
				savevalue = say; //save current value of say
				if (say > PC.y) //if slime is lower than player
				{
					if (tilemap[say - 1][sax].ascii == '@')
					{
						PC.health = PC.health - slimea.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
					}
					else
					{
						for (int col = say - 1; col > PC.y; --col)
						{
							if (tilemap[col][sax].collide == true)
							{
								say = 0;
								break;
							}
						}
					}
					if (say == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)sax, (short)say };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[say][sax] = secondtilemap[say][sax];
						map[say][sax] = tilemap[say][sax].ascii;
						std::cout << map[say][sax];
						say--;
						SetConsoleTextAttribute(hStd, 0x0B);
						coord = { (short)sax, (short)say };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[say][sax] = Slime;
						map[say][sax] = tilemap[say][sax].ascii;
						std::cout << map[say][sax];
						continue;
					}
				}
				else if (say < PC.y) //if slime is higher than player
				{	
					if (tilemap[say + 1][sax].ascii == '@')
					{
						PC.health = PC.health - slimea.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = say + 1; col < PC.y; ++col)
						{
							if (tilemap[col][sax].collide == true)
							{
								say = 0;
								break;
							}
						}
					}
					if (say == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)sax, (short)say };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[say][sax] = secondtilemap[say][sax];
						map[say][sax] = tilemap[say][sax].ascii;
						std::cout << map[say][sax];
						say++;
						SetConsoleTextAttribute(hStd, 0x0B);
						coord = { (short)sax, (short)say };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[say][sax] = Slime;
						map[say][sax] = tilemap[say][sax].ascii;
						std::cout << map[say][sax];
						continue;
					}
				}
			}
			else if (say == PC.y) //same row
			{
				savevalue = sax;
				if (sax > PC.x) //if slime is to the right of player
				{
					if (tilemap[say][sax - 1].ascii == '@')
					{
						PC.health = PC.health - slimea.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = sax - 1; col > PC.x; --col)
						{
							if (tilemap[say][col].collide == true)
							{
								sax = 0;
								break;
							}
						}
					}
					if (sax == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)sax, (short)say };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[say][sax] = secondtilemap[say][sax];
						map[say][sax] = tilemap[say][sax].ascii;
						std::cout << map[say][sax];
						sax--;
						SetConsoleTextAttribute(hStd, 0x0B);
						coord = { (short)sax, (short)say };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[say][sax] = Slime;
						map[say][sax] = tilemap[say][sax].ascii;
						std::cout << map[say][sax];
						continue;
					}
				}
				else if (sax < PC.x) //if slime is to the left of player
				{
					if (tilemap[say][sax + 1].ascii == '@')
					{
						PC.health = PC.health - slimea.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = sax + 1; col < PC.x; ++col)
						{
							if (tilemap[say][col].collide == true)
							{
								sax = 0;
								break;
							}
						}
					}
					if (sax == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)sax, (short)say };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[say][sax] = secondtilemap[say][sax];
						map[say][sax] = tilemap[say][sax].ascii;
						std::cout << map[say][sax];
						sax++;
						SetConsoleTextAttribute(hStd, 0x0B);
						coord = { (short)sax, (short)say };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[say][sax] = Slime;
						map[say][sax] = tilemap[say][sax].ascii;
						std::cout << map[say][sax];
						continue;
					}
				}
			}
			if ((sax == 0 || say == 0) || (sax != PC.x && say != PC.y))
			{
				if (sax == 0)
				{
					sax = savevalue;
				}
				else if (say == 0)
				{
					say = savevalue;
				}
				//up
				if (samovement == 1)
				{
					say--;
					if (tilemap[say][sax].collide == true)
						say++;
					else
					{
						say++;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)sax, (short)say };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[say][sax] = secondtilemap[say][sax];
						map[say][sax] = tilemap[say][sax].ascii;
						std::cout << map[say][sax];
						say--;
						SetConsoleTextAttribute(hStd, 0x0B);
						coord = { (short)sax, (short)say };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[say][sax] = Slime;
						map[say][sax] = tilemap[say][sax].ascii;
						std::cout << map[say][sax];
					}
				}
				//down
				else if (samovement == 2)
				{
					say++;
					if (tilemap[say][sax].collide == true)
						say--;
					else
					{
						say--;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)sax, (short)say };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[say][sax] = secondtilemap[say][sax];
						map[say][sax] = tilemap[say][sax].ascii;
						std::cout << map[say][sax];
						say++;
						SetConsoleTextAttribute(hStd, 0x0B);
						coord = { (short)sax, (short)say };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[say][sax] = Slime;
						map[say][sax] = tilemap[say][sax].ascii;
						std::cout << map[say][sax];
					}
				}
				//left
				else if (samovement == 3)
				{
					sax--;
					if (tilemap[say][sax].collide == true)
						sax++;
					else
					{
						sax++;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)sax, (short)say };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[say][sax] = secondtilemap[say][sax];
						map[say][sax] = tilemap[say][sax].ascii;
						std::cout << map[say][sax];
						sax--;
						SetConsoleTextAttribute(hStd, 0x0B);
						coord = { (short)sax, (short)say };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[say][sax] = Slime;
						map[say][sax] = tilemap[say][sax].ascii;
						std::cout << map[say][sax];
					}
				}
				//right
				else if (samovement == 4)
				{
					sax++;
					if (tilemap[say][sax].collide == true)
						sax--;
					else
					{
						sax--;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)sax, (short)say };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[say][sax] = secondtilemap[say][sax];
						map[say][sax] = tilemap[say][sax].ascii;
						std::cout << map[say][sax];
						sax++;
						SetConsoleTextAttribute(hStd, 0x0B);
						coord = { (short)sax, (short)say };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[say][sax] = Slime;
						map[say][sax] = tilemap[say][sax].ascii;
						std::cout << map[say][sax];
					}
				}
			}
		}
		if (slimeb.health > 0)
		{
			//slime b movement
			//slime b chasing player - no escaping this slime!	
			if (sbx == PC.x) //same column
			{
				savevalue = sby; //save current value of sby
				if (sby > PC.y) //if slime is lower than player
				{
					if (tilemap[sby - 1][sbx].ascii == '@')
					{
						PC.health = PC.health - slimeb.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
					}
					else
					{
						for (int col = sby - 1; col > PC.y; --col)
						{
							if (tilemap[col][sbx].collide == true)
							{
								sby = 0;
								break;
							}
						}
					}
					if (sby == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)sbx, (short)sby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[sby][sbx] = secondtilemap[sby][sbx];
						map[sby][sbx] = tilemap[sby][sbx].ascii;
						std::cout << map[sby][sbx];
						sby--;
						SetConsoleTextAttribute(hStd, 0x0B);
						coord = { (short)sbx, (short)sby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[sby][sbx] = Slime;
						map[sby][sbx] = tilemap[sby][sbx].ascii;
						std::cout << map[sby][sbx];
						continue;
					}
				}
				else if (sby < PC.y) //if slime is higher than player
				{
					if (tilemap[sby + 1][sbx].ascii == '@')
					{
						PC.health = PC.health - slimeb.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = sby + 1; col < PC.y; ++col)
						{
							if (tilemap[col][sbx].collide == true)
							{
								sby = 0;
								break;
							}
						}
					}
					if (sby == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)sbx, (short)sby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[sby][sbx] = secondtilemap[sby][sbx];
						map[sby][sbx] = tilemap[sby][sbx].ascii;
						std::cout << map[sby][sbx];
						sby++;
						SetConsoleTextAttribute(hStd, 0x0B);
						coord = { (short)sbx, (short)sby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[sby][sbx] = Slime;
						map[sby][sbx] = tilemap[sby][sbx].ascii;
						std::cout << map[sby][sbx];
						continue;
					}
				}
			}
			else if (sby == PC.y) //same row
			{
				savevalue = sbx;
				if (sbx > PC.x) //if slime is to the right of player
				{
					if (tilemap[sby][sbx - 1].ascii == '@')
					{
						PC.health = PC.health - slimeb.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = sbx - 1; col > PC.x; --col)
						{
							if (tilemap[sby][col].collide == true)
							{
								sbx = 0;
								break;
							}
						}
					}
					if (sbx == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)sbx, (short)sby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[sby][sbx] = secondtilemap[sby][sbx];
						map[sby][sbx] = tilemap[sby][sbx].ascii;
						std::cout << map[sby][sbx];
						sbx--;
						SetConsoleTextAttribute(hStd, 0x0B);
						coord = { (short)sbx, (short)sby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[sby][sbx] = Slime;
						map[sby][sbx] = tilemap[sby][sbx].ascii;
						std::cout << map[sby][sbx];
						continue;
					}
				}
				else if (sbx < PC.x) //if slime is to the left of player
				{
					if (tilemap[sby][sbx + 1].ascii == '@')
					{
						PC.health = PC.health - slimeb.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = sbx + 1; col < PC.x; ++col)
						{
							if (tilemap[sby][col].collide == true)
							{
								sbx = 0;
								break;
							}
						}
					}
					if (sbx == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)sbx, (short)sby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[sby][sbx] = secondtilemap[sby][sbx];
						map[sby][sbx] = tilemap[sby][sbx].ascii;
						std::cout << map[sby][sbx];
						sbx++;
						SetConsoleTextAttribute(hStd, 0x0B);
						coord = { (short)sbx, (short)sby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[sby][sbx] = Slime;
						map[sby][sbx] = tilemap[sby][sbx].ascii;
						std::cout << map[sby][sbx];
						continue;
					}
				}
			}
			if ((sbx == 0 || sby == 0) || (sbx != PC.x && sby != PC.y))
			{
				if (sbx == 0)
				{
					sbx = savevalue;
				}
				else if (sby == 0)
				{
					sby = savevalue;
				}
				//up
				if (sbmovement == 1)
				{
					sby--;
					if (tilemap[sby][sbx].collide == true)
						sby++;
					else
					{
						sby++;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)sbx, (short)sby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[sby][sbx] = secondtilemap[sby][sbx];
						map[sby][sbx] = tilemap[sby][sbx].ascii;
						std::cout << map[sby][sbx];
						sby--;
						SetConsoleTextAttribute(hStd, 0x0B);
						coord = { (short)sbx, (short)sby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[sby][sbx] = Slime;
						map[sby][sbx] = tilemap[sby][sbx].ascii;
						std::cout << map[sby][sbx];
					}
				}
				//down
				else if (sbmovement == 2)
				{
					sby++;
					if (tilemap[sby][sbx].collide == true)
						sby--;
					else
					{
						sby--;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)sbx, (short)sby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[sby][sbx] = secondtilemap[sby][sbx];
						map[sby][sbx] = tilemap[sby][sbx].ascii;
						std::cout << map[sby][sbx];
						sby++;
						SetConsoleTextAttribute(hStd, 0x0B);
						coord = { (short)sbx, (short)sby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[sby][sbx] = Slime;
						map[sby][sbx] = tilemap[sby][sbx].ascii;
						std::cout << map[sby][sbx];
					}
				}
				//left
				else if (sbmovement == 3)
				{
					sbx--;
					if (tilemap[sby][sbx].collide == true)
						sbx++;
					else
					{
						sbx++;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)sbx, (short)sby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[sby][sbx] = secondtilemap[sby][sbx];
						map[sby][sbx] = tilemap[sby][sbx].ascii;
						std::cout << map[sby][sbx];
						sbx--;
						SetConsoleTextAttribute(hStd, 0x0B);
						coord = { (short)sbx, (short)sby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[sby][sbx] = Slime;
						map[sby][sbx] = tilemap[sby][sbx].ascii;
						std::cout << map[sby][sbx];
					}
				}
				//right
				else if (sbmovement == 4)
				{
					sbx++;
					if (tilemap[sby][sbx].collide == true)
						sbx--;
					else
					{
						sbx--;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)sbx, (short)sby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[sby][sbx] = secondtilemap[sby][sbx];
						map[sby][sbx] = tilemap[sby][sbx].ascii;
						std::cout << map[sby][sbx];
						sbx++;
						SetConsoleTextAttribute(hStd, 0x0B);
						coord = { (short)sbx, (short)sby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[sby][sbx] = Slime;
						map[sby][sbx] = tilemap[sby][sbx].ascii;
						std::cout << map[sby][sbx];
					}
				}
			}
		}
		if (slimec.health > 0)
		{
			//slime c movement
			//slime c chasing player - no escaping this slime!	
			if (scx == PC.x) //same column
			{
				savevalue = scy; //save current value of scy
				if (scy > PC.y) //if slime is lower than player
				{
					if (tilemap[scy - 1][scx].ascii == '@')
					{
						PC.health = PC.health - slimec.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
					}
					else
					{
						for (int col = scy - 1; col > PC.y; --col)
						{
							if (tilemap[col][scx].collide == true)
							{
								scy = 0;
								break;
							}
						}
					}
					if (scy == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)scx, (short)scy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[scy][scx] = secondtilemap[scy][scx];
						map[scy][scx] = tilemap[scy][scx].ascii;
						std::cout << map[scy][scx];
						scy--;
						SetConsoleTextAttribute(hStd, 0x0B);
						coord = { (short)scx, (short)scy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[scy][scx] = Slime;
						map[scy][scx] = tilemap[scy][scx].ascii;
						std::cout << map[scy][scx];
						continue;
					}
				}
				else if (scy < PC.y) //if slime is higher than player
				{
					if (tilemap[scy + 1][scx].ascii == '@')
					{
						PC.health = PC.health - slimec.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = scy + 1; col < PC.y; ++col)
						{
							if (tilemap[col][scx].collide == true)
							{
								scy = 0;
								break;
							}
						}
					}
					if (scy == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)scx, (short)scy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[scy][scx] = secondtilemap[scy][scx];
						map[scy][scx] = tilemap[scy][scx].ascii;
						std::cout << map[scy][scx];
						scy++;
						SetConsoleTextAttribute(hStd, 0x0B);
						coord = { (short)scx, (short)scy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[scy][scx] = Slime;
						map[scy][scx] = tilemap[scy][scx].ascii;
						std::cout << map[scy][scx];
						continue;
					}
				}
			}
			else if (scy == PC.y) //same row
			{
				savevalue = scx;
				if (scx > PC.x) //if slime is to the right of player
				{
					if (tilemap[scy][scx - 1].ascii == '@')
					{
						PC.health = PC.health - slimec.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = scx - 1; col > PC.x; --col)
						{
							if (tilemap[scy][col].collide == true)
							{
								scx = 0;
								break;
							}
						}
					}
					if (scx == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)scx, (short)scy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[scy][scx] = secondtilemap[scy][scx];
						map[scy][scx] = tilemap[scy][scx].ascii;
						std::cout << map[scy][scx];
						scx--;
						SetConsoleTextAttribute(hStd, 0x0B);
						coord = { (short)scx, (short)scy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[scy][scx] = Slime;
						map[scy][scx] = tilemap[scy][scx].ascii;
						std::cout << map[scy][scx];
						continue;
					}
				}
				else if (scx < PC.x) //if slime is to the left of player
				{
					if (tilemap[scy][scx + 1].ascii == '@')
					{
						PC.health = PC.health - slimec.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = scx + 1; col < PC.x; ++col)
						{
							if (tilemap[scy][col].collide == true)
							{
								scx = 0;
								break;
							}
						}
					}
					if (scx == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)scx, (short)scy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[scy][scx] = secondtilemap[scy][scx];
						map[scy][scx] = tilemap[scy][scx].ascii;
						std::cout << map[scy][scx];
						scx++;
						SetConsoleTextAttribute(hStd, 0x0B);
						coord = { (short)scx, (short)scy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[scy][scx] = Slime;
						map[scy][scx] = tilemap[scy][scx].ascii;
						std::cout << map[scy][scx];
						continue;
					}
				}
			}
			if ((scx == 0 || scy == 0) || (scx != PC.x && scy != PC.y))
			{
				if (scx == 0)
				{
					scx = savevalue;
				}
				else if (scy == 0)
				{
					scy = savevalue;
				}
				//up
				if (scmovement == 1)
				{
					scy--;
					if (tilemap[scy][scx].collide == true)
						scy++;
					else
					{
						scy++;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)scx, (short)scy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[scy][scx] = secondtilemap[scy][scx];
						map[scy][scx] = tilemap[scy][scx].ascii;
						std::cout << map[scy][scx];
						scy--;
						SetConsoleTextAttribute(hStd, 0x0B);
						coord = { (short)scx, (short)scy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[scy][scx] = Slime;
						map[scy][scx] = tilemap[scy][scx].ascii;
						std::cout << map[scy][scx];
					}
				}
				//down
				else if (scmovement == 2)
				{
					scy++;
					if (tilemap[scy][scx].collide == true)
						scy--;
					else
					{
						scy--;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)scx, (short)scy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[scy][scx] = secondtilemap[scy][scx];
						map[scy][scx] = tilemap[scy][scx].ascii;
						std::cout << map[scy][scx];
						scy++;
						SetConsoleTextAttribute(hStd, 0x0B);
						coord = { (short)scx, (short)scy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[scy][scx] = Slime;
						map[scy][scx] = tilemap[scy][scx].ascii;
						std::cout << map[scy][scx];
					}
				}
				//left
				else if (scmovement == 3)
				{
					scx--;
					if (tilemap[scy][scx].collide == true)
						scx++;
					else
					{
						scx++;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)scx, (short)scy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[scy][scx] = secondtilemap[scy][scx];
						map[scy][scx] = tilemap[scy][scx].ascii;
						std::cout << map[scy][scx];
						scx--;
						SetConsoleTextAttribute(hStd, 0x0B);
						coord = { (short)scx, (short)scy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[scy][scx] = Slime;
						map[scy][scx] = tilemap[scy][scx].ascii;
						std::cout << map[scy][scx];
					}
				}
				//right
				else if (scmovement == 4)
				{
					scx++;
					if (tilemap[scy][scx].collide == true)
						scx--;
					else
					{
						scx--;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)scx, (short)scy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[scy][scx] = secondtilemap[scy][scx];
						map[scy][scx] = tilemap[scy][scx].ascii;
						std::cout << map[scy][scx];
						scx++;
						SetConsoleTextAttribute(hStd, 0x0B);
						coord = { (short)scx, (short)scy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[scy][scx] = Slime;
						map[scy][scx] = tilemap[scy][scx].ascii;
						std::cout << map[scy][scx];
					}
				}
			}
		}
		if (zombiea.health > 0)
		{
			//Zombie a movement
			//Zombie a chasing player - no escaping this Zombie!	
			if (zax == PC.x) //same column
			{
				savevalue = zay; //save current value of zay
				if (zay > PC.y) //if Zombie is lower than player
				{
					if (tilemap[zay - 1][zax].ascii == '@')
					{
						PC.health = PC.health - zombiea.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
					}
					else
					{
						for (int col = zay - 1; col > PC.y; --col)
						{
							if (tilemap[col][zax].collide == true)
							{
								zay = 0;
								break;
							}
						}
					}
					if (zay == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zax, (short)zay };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zay][zax] = secondtilemap[zay][zax];
						map[zay][zax] = tilemap[zay][zax].ascii;
						std::cout << map[zay][zax];
						zay--;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zax, (short)zay };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zay][zax] = Zombie;
						map[zay][zax] = tilemap[zay][zax].ascii;
						std::cout << map[zay][zax];
						continue;
					}
				}
				else if (zay < PC.y) //if Zombie is higher than player
				{
					if (tilemap[zay + 1][zax].ascii == '@')
					{
						PC.health = PC.health - zombiea.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = zay + 1; col < PC.y; ++col)
						{
							if (tilemap[col][zax].collide == true)
							{
								zay = 0;
								break;
							}
						}
					}
					if (zay == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zax, (short)zay };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zay][zax] = secondtilemap[zay][zax];
						map[zay][zax] = tilemap[zay][zax].ascii;
						std::cout << map[zay][zax];
						zay++;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zax, (short)zay };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zay][zax] = Zombie;
						map[zay][zax] = tilemap[zay][zax].ascii;
						std::cout << map[zay][zax];
						continue;
					}
				}
			}
			else if (zay == PC.y) //same row
			{
				savevalue = zax;
				if (zax > PC.x) //if Zombie is to the right of player
				{
					if (tilemap[zay][zax - 1].ascii == '@')
					{
						PC.health = PC.health - zombiea.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = zax - 1; col > PC.x; --col)
						{
							if (tilemap[zay][col].collide == true)
							{
								zax = 0;
								break;
							}
						}
					}
					if (zax == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zax, (short)zay };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zay][zax] = secondtilemap[zay][zax];
						map[zay][zax] = tilemap[zay][zax].ascii;
						std::cout << map[zay][zax];
						zax--;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zax, (short)zay };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zay][zax] = Zombie;
						map[zay][zax] = tilemap[zay][zax].ascii;
						std::cout << map[zay][zax];
						continue;
					}
				}
				else if (zax < PC.x) //if Zombie is to the left of player
				{
					if (tilemap[zay][zax + 1].ascii == '@')
					{
						PC.health = PC.health - zombiea.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = zax + 1; col < PC.x; ++col)
						{
							if (tilemap[zay][col].collide == true)
							{
								zax = 0;
								break;
							}
						}
					}
					if (zax == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zax, (short)zay };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zay][zax] = secondtilemap[zay][zax];
						map[zay][zax] = tilemap[zay][zax].ascii;
						std::cout << map[zay][zax];
						zax++;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zax, (short)zay };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zay][zax] = Zombie;
						map[zay][zax] = tilemap[zay][zax].ascii;
						std::cout << map[zay][zax];
						continue;
					}
				}
			}
			if ((zax == 0 || zay == 0) || (zax != PC.x && zay != PC.y))
			{
				if (zax == 0)
				{
					zax = savevalue;
				}
				else if (zay == 0)
				{
					zay = savevalue;
				}
				//up
				if (zamovement == 1)
				{
					zay--;
					if (tilemap[zay][zax].collide == true)
						zay++;
					else
					{
						zay++;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zax, (short)zay };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zay][zax] = secondtilemap[zay][zax];
						map[zay][zax] = tilemap[zay][zax].ascii;
						std::cout << map[zay][zax];
						zay--;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zax, (short)zay };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zay][zax] = Zombie;
						map[zay][zax] = tilemap[zay][zax].ascii;
						std::cout << map[zay][zax];
					}
				}
				//down
				else if (zamovement == 2)
				{
					zay++;
					if (tilemap[zay][zax].collide == true)
						zay--;
					else
					{
						zay--;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zax, (short)zay };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zay][zax] = secondtilemap[zay][zax];
						map[zay][zax] = tilemap[zay][zax].ascii;
						std::cout << map[zay][zax];
						zay++;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zax, (short)zay };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zay][zax] = Zombie;
						map[zay][zax] = tilemap[zay][zax].ascii;
						std::cout << map[zay][zax];
					}
				}
				//left
				else if (zamovement == 3)
				{
					zax--;
					if (tilemap[zay][zax].collide == true)
						zax++;
					else
					{
						zax++;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zax, (short)zay };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zay][zax] = secondtilemap[zay][zax];
						map[zay][zax] = tilemap[zay][zax].ascii;
						std::cout << map[zay][zax];
						zax--;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zax, (short)zay };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zay][zax] = Zombie;
						map[zay][zax] = tilemap[zay][zax].ascii;
						std::cout << map[zay][zax];
					}
				}
				//right
				else if (zamovement == 4)
				{
					zax++;
					if (tilemap[zay][zax].collide == true)
						zax--;
					else
					{
						zax--;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zax, (short)zay };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zay][zax] = secondtilemap[zay][zax];
						map[zay][zax] = tilemap[zay][zax].ascii;
						std::cout << map[zay][zax];
						zax++;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zax, (short)zay };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zay][zax] = Zombie;
						map[zay][zax] = tilemap[zay][zax].ascii;
						std::cout << map[zay][zax];
					}
				}
			}
		}
		if (zombieb.health > 0)
		{
			//Zombie b movement
			//Zombie b chasing player - no escaping this Zombie!	
			if (zbx == PC.x) //same column
			{
				savevalue = zby; //save current value of zby
				if (zby > PC.y) //if Zombie is lower than player
				{
					if (tilemap[zby - 1][zbx].ascii == '@')
					{
						PC.health = PC.health - zombieb.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
					}
					else
					{
						for (int col = zby - 1; col > PC.y; --col)
						{
							if (tilemap[col][zbx].collide == true)
							{
								zby = 0;
								break;
							}
						}
					}
					if (zby == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zbx, (short)zby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zby][zbx] = secondtilemap[zby][zbx];
						map[zby][zbx] = tilemap[zby][zbx].ascii;
						std::cout << map[zby][zbx];
						zby--;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zbx, (short)zby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zby][zbx] = Zombie;
						map[zby][zbx] = tilemap[zby][zbx].ascii;
						std::cout << map[zby][zbx];
						continue;
					}
				}
				else if (zby < PC.y) //if Zombie is higher than player
				{
					if (tilemap[zby + 1][zbx].ascii == '@')
					{
						PC.health = PC.health - zombieb.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = zby + 1; col < PC.y; ++col)
						{
							if (tilemap[col][zbx].collide == true)
							{
								zby = 0;
								break;
							}
						}
					}
					if (zby == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zbx, (short)zby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zby][zbx] = secondtilemap[zby][zbx];
						map[zby][zbx] = tilemap[zby][zbx].ascii;
						std::cout << map[zby][zbx];
						zby++;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zbx, (short)zby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zby][zbx] = Zombie;
						map[zby][zbx] = tilemap[zby][zbx].ascii;
						std::cout << map[zby][zbx];
						continue;
					}
				}
			}
			else if (zby == PC.y) //same row
			{
				savevalue = zbx;
				if (zbx > PC.x) //if Zombie is to the right of player
				{
					if (tilemap[zby][zbx - 1].ascii == '@')
					{
						PC.health = PC.health - zombieb.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = zbx - 1; col > PC.x; --col)
						{
							if (tilemap[zby][col].collide == true)
							{
								zbx = 0;
								break;
							}
						}
					}
					if (zbx == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zbx, (short)zby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zby][zbx] = secondtilemap[zby][zbx];
						map[zby][zbx] = tilemap[zby][zbx].ascii;
						std::cout << map[zby][zbx];
						zbx--;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zbx, (short)zby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zby][zbx] = Zombie;
						map[zby][zbx] = tilemap[zby][zbx].ascii;
						std::cout << map[zby][zbx];
						continue;
					}
				}
				else if (zbx < PC.x) //if Zombie is to the left of player
				{
					if (tilemap[zby][zbx + 1].ascii == '@')
					{
						PC.health = PC.health - zombieb.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = zbx + 1; col < PC.x; ++col)
						{
							if (tilemap[zby][col].collide == true)
							{
								zbx = 0;
								break;
							}
						}
					}
					if (zbx == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zbx, (short)zby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zby][zbx] = secondtilemap[zby][zbx];
						map[zby][zbx] = tilemap[zby][zbx].ascii;
						std::cout << map[zby][zbx];
						zbx++;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zbx, (short)zby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zby][zbx] = Zombie;
						map[zby][zbx] = tilemap[zby][zbx].ascii;
						std::cout << map[zby][zbx];
						continue;
					}
				}
			}
			if ((zbx == 0 || zby == 0) || (zbx != PC.x && zby != PC.y))
			{
				if (zbx == 0)
				{
					zbx = savevalue;
				}
				else if (zby == 0)
				{
					zby = savevalue;
				}
				//up
				if (zbmovement == 1)
				{
					zby--;
					if (tilemap[zby][zbx].collide == true)
						zby++;
					else
					{
						zby++;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zbx, (short)zby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zby][zbx] = secondtilemap[zby][zbx];
						map[zby][zbx] = tilemap[zby][zbx].ascii;
						std::cout << map[zby][zbx];
						zby--;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zbx, (short)zby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zby][zbx] = Zombie;
						map[zby][zbx] = tilemap[zby][zbx].ascii;
						std::cout << map[zby][zbx];
					}
				}
				//down
				else if (zbmovement == 2)
				{
					zby++;
					if (tilemap[zby][zbx].collide == true)
						zby--;
					else
					{
						zby--;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zbx, (short)zby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zby][zbx] = secondtilemap[zby][zbx];
						map[zby][zbx] = tilemap[zby][zbx].ascii;
						std::cout << map[zby][zbx];
						zby++;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zbx, (short)zby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zby][zbx] = Zombie;
						map[zby][zbx] = tilemap[zby][zbx].ascii;
						std::cout << map[zby][zbx];
					}
				}
				//left
				else if (zbmovement == 3)
				{
					zbx--;
					if (tilemap[zby][zbx].collide == true)
						zbx++;
					else
					{
						zbx++;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zbx, (short)zby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zby][zbx] = secondtilemap[zby][zbx];
						map[zby][zbx] = tilemap[zby][zbx].ascii;
						std::cout << map[zby][zbx];
						zbx--;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zbx, (short)zby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zby][zbx] = Zombie;
						map[zby][zbx] = tilemap[zby][zbx].ascii;
						std::cout << map[zby][zbx];
					}
				}
				//right
				else if (zbmovement == 4)
				{
					zbx++;
					if (tilemap[zby][zbx].collide == true)
						zbx--;
					else
					{
						zbx--;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zbx, (short)zby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zby][zbx] = secondtilemap[zby][zbx];
						map[zby][zbx] = tilemap[zby][zbx].ascii;
						std::cout << map[zby][zbx];
						zbx++;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zbx, (short)zby };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zby][zbx] = Zombie;
						map[zby][zbx] = tilemap[zby][zbx].ascii;
						std::cout << map[zby][zbx];
					}
				}
			}
		}
		if (zombiec.health > 0)
		{
			//Zombie c movement
			//Zombie c chasing player - no escaping this Zombie!	
			if (zcx == PC.x) //same column
			{
				savevalue = zcy; //save current value of zcy
				if (zcy > PC.y) //if Zombie is lower than player
				{
					if (tilemap[zcy - 1][zcx].ascii == '@')
					{
						PC.health = PC.health - zombiec.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
					}
					else
					{
						for (int col = zcy - 1; col > PC.y; --col)
						{
							if (tilemap[col][zcx].collide == true)
							{
								zcy = 0;
								break;
							}
						}
					}
					if (zcy == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zcx, (short)zcy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zcy][zcx] = secondtilemap[zcy][zcx];
						map[zcy][zcx] = tilemap[zcy][zcx].ascii;
						std::cout << map[zcy][zcx];
						zcy--;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zcx, (short)zcy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zcy][zcx] = Zombie;
						map[zcy][zcx] = tilemap[zcy][zcx].ascii;
						std::cout << map[zcy][zcx];
						continue;
					}
				}
				else if (zcy < PC.y) //if Zombie is higher than player
				{
					if (tilemap[zcy + 1][zcx].ascii == '@')
					{
						PC.health = PC.health - zombiec.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = zcy + 1; col < PC.y; ++col)
						{
							if (tilemap[col][zcx].collide == true)
							{
								zcy = 0;
								break;
							}
						}
					}
					if (zcy == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zcx, (short)zcy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zcy][zcx] = secondtilemap[zcy][zcx];
						map[zcy][zcx] = tilemap[zcy][zcx].ascii;
						std::cout << map[zcy][zcx];
						zcy++;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zcx, (short)zcy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zcy][zcx] = Zombie;
						map[zcy][zcx] = tilemap[zcy][zcx].ascii;
						std::cout << map[zcy][zcx];
						continue;
					}
				}
			}
			else if (zcy == PC.y) //same row
			{
				savevalue = zcx;
				if (zcx > PC.x) //if Zombie is to the right of player
				{
					if (tilemap[zcy][zcx - 1].ascii == '@')
					{
						PC.health = PC.health - zombiec.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = zcx - 1; col > PC.x; --col)
						{
							if (tilemap[zcy][col].collide == true)
							{
								zcx = 0;
								break;
							}
						}
					}
					if (zcx == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zcx, (short)zcy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zcy][zcx] = secondtilemap[zcy][zcx];
						map[zcy][zcx] = tilemap[zcy][zcx].ascii;
						std::cout << map[zcy][zcx];
						zcx--;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zcx, (short)zcy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zcy][zcx] = Zombie;
						map[zcy][zcx] = tilemap[zcy][zcx].ascii;
						std::cout << map[zcy][zcx];
						continue;
					}
				}
				else if (zcx < PC.x) //if Zombie is to the left of player
				{
					if (tilemap[zcy][zcx + 1].ascii == '@')
					{
						PC.health = PC.health - zombiec.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = zcx + 1; col < PC.x; ++col)
						{
							if (tilemap[zcy][col].collide == true)
							{
								zcx = 0;
								break;
							}
						}
					}
					if (zcx == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zcx, (short)zcy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zcy][zcx] = secondtilemap[zcy][zcx];
						map[zcy][zcx] = tilemap[zcy][zcx].ascii;
						std::cout << map[zcy][zcx];
						zcx++;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zcx, (short)zcy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zcy][zcx] = Zombie;
						map[zcy][zcx] = tilemap[zcy][zcx].ascii;
						std::cout << map[zcy][zcx];
						continue;
					}
				}
			}
			if ((zcx == 0 || zcy == 0) || (zcx != PC.x && zcy != PC.y))
			{
				if (zcx == 0)
				{
					zcx = savevalue;
				}
				else if (zcy == 0)
				{
					zcy = savevalue;
				}
				//up
				if (zcmovement == 1)
				{
					zcy--;
					if (tilemap[zcy][zcx].collide == true)
						zcy++;
					else
					{
						zcy++;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zcx, (short)zcy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zcy][zcx] = secondtilemap[zcy][zcx];
						map[zcy][zcx] = tilemap[zcy][zcx].ascii;
						std::cout << map[zcy][zcx];
						zcy--;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zcx, (short)zcy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zcy][zcx] = Zombie;
						map[zcy][zcx] = tilemap[zcy][zcx].ascii;
						std::cout << map[zcy][zcx];
					}
				}
				//down
				else if (zcmovement == 2)
				{
					zcy++;
					if (tilemap[zcy][zcx].collide == true)
						zcy--;
					else
					{
						zcy--;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zcx, (short)zcy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zcy][zcx] = secondtilemap[zcy][zcx];
						map[zcy][zcx] = tilemap[zcy][zcx].ascii;
						std::cout << map[zcy][zcx];
						zcy++;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zcx, (short)zcy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zcy][zcx] = Zombie;
						map[zcy][zcx] = tilemap[zcy][zcx].ascii;
						std::cout << map[zcy][zcx];
					}
				}
				//left
				else if (zcmovement == 3)
				{
					zcx--;
					if (tilemap[zcy][zcx].collide == true)
						zcx++;
					else
					{
						zcx++;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zcx, (short)zcy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zcy][zcx] = secondtilemap[zcy][zcx];
						map[zcy][zcx] = tilemap[zcy][zcx].ascii;
						std::cout << map[zcy][zcx];
						zcx--;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zcx, (short)zcy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zcy][zcx] = Zombie;
						map[zcy][zcx] = tilemap[zcy][zcx].ascii;
						std::cout << map[zcy][zcx];
					}
				}
				//right
				else if (zcmovement == 4)
				{
					zcx++;
					if (tilemap[zcy][zcx].collide == true)
						zcx--;
					else
					{
						zcx--;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zcx, (short)zcy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zcy][zcx] = secondtilemap[zcy][zcx];
						map[zcy][zcx] = tilemap[zcy][zcx].ascii;
						std::cout << map[zcy][zcx];
						zcx++;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zcx, (short)zcy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zcy][zcx] = Zombie;
						map[zcy][zcx] = tilemap[zcy][zcx].ascii;
						std::cout << map[zcy][zcx];
					}
				}
			}
		}
		if (lich.health > 0)
		{
			//Lich a movement
			//Lich a chasing player - no escaping this Lich!	
			if (lx == PC.x) //same column
			{
				savevalue = ly; //save current value of ly
				if (ly > PC.y) //if Lich is lower than player
				{
					if (tilemap[ly - 1][lx].ascii == '@')
					{
						PC.health = PC.health - lich.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
					}
					else
					{
						for (int col = ly - 1; col > PC.y; --col)
						{
							if (tilemap[col][lx].collide == true)
							{
								ly = 0;
								break;
							}
						}
					}
					if (ly == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)lx, (short)ly };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[ly][lx] = secondtilemap[ly][lx];
						map[ly][lx] = tilemap[ly][lx].ascii;
						std::cout << map[ly][lx];
						ly--;
						SetConsoleTextAttribute(hStd, 0x05);
						coord = { (short)lx, (short)ly };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[ly][lx] = Lich;
						map[ly][lx] = tilemap[ly][lx].ascii;
						std::cout << map[ly][lx];
						continue;
					}
				}
				else if (ly < PC.y) //if Lich is higher than player
				{
					if (tilemap[ly + 1][lx].ascii == '@')
					{
						PC.health = PC.health - lich.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = ly + 1; col < PC.y; ++col)
						{
							if (tilemap[col][lx].collide == true)
							{
								ly = 0;
								break;
							}
						}
					}
					if (ly == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)lx, (short)ly };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[ly][lx] = secondtilemap[ly][lx];
						map[ly][lx] = tilemap[ly][lx].ascii;
						std::cout << map[ly][lx];
						ly++;
						SetConsoleTextAttribute(hStd, 0x05);
						coord = { (short)lx, (short)ly };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[ly][lx] = Lich;
						map[ly][lx] = tilemap[ly][lx].ascii;
						std::cout << map[ly][lx];
						continue;
					}
				}
			}
			else if (ly == PC.y) //same row
			{
				savevalue = lx;
				if (lx > PC.x) //if Lich is to the right of player
				{
					if (tilemap[ly][lx - 1].ascii == '@')
					{
						PC.health = PC.health - lich.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = lx - 1; col > PC.x; --col)
						{
							if (tilemap[ly][col].collide == true)
							{
								lx = 0;
								break;
							}
						}
					}
					if (lx == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)lx, (short)ly };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[ly][lx] = secondtilemap[ly][lx];
						map[ly][lx] = tilemap[ly][lx].ascii;
						std::cout << map[ly][lx];
						lx--;
						SetConsoleTextAttribute(hStd, 0x05);
						coord = { (short)lx, (short)ly };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[ly][lx] = Lich;
						map[ly][lx] = tilemap[ly][lx].ascii;
						std::cout << map[ly][lx];
						continue;
					}
				}
				else if (lx < PC.x) //if Lich is to the left of player
				{
					if (tilemap[ly][lx + 1].ascii == '@')
					{
						PC.health = PC.health - lich.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = lx + 1; col < PC.x; ++col)
						{
							if (tilemap[ly][col].collide == true)
							{
								lx = 0;
								break;
							}
						}
					}
					if (lx == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)lx, (short)ly };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[ly][lx] = secondtilemap[ly][lx];
						map[ly][lx] = tilemap[ly][lx].ascii;
						std::cout << map[ly][lx];
						lx++;
						SetConsoleTextAttribute(hStd, 0x05);
						coord = { (short)lx, (short)ly };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[ly][lx] = Lich;
						map[ly][lx] = tilemap[ly][lx].ascii;
						std::cout << map[ly][lx];
						continue;
					}
				}
			}
			if ((lx == 0 || ly == 0) || (lx != PC.x && ly != PC.y))
			{
				if (lx == 0)
				{
					lx = savevalue;
				}
				else if (ly == 0)
				{
					ly = savevalue;
				}
				//up
				if (lmovement == 1)
				{
					ly--;
					if (tilemap[ly][lx].collide == true)
						ly++;
					else
					{
						ly++;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)lx, (short)ly };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[ly][lx] = secondtilemap[ly][lx];
						map[ly][lx] = tilemap[ly][lx].ascii;
						std::cout << map[ly][lx];
						ly--;
						SetConsoleTextAttribute(hStd, 0x05);
						coord = { (short)lx, (short)ly };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[ly][lx] = Lich;
						map[ly][lx] = tilemap[ly][lx].ascii;
						std::cout << map[ly][lx];
					}
				}
				//down
				else if (lmovement == 2)
				{
					ly++;
					if (tilemap[ly][lx].collide == true)
						ly--;
					else
					{
						ly--;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)lx, (short)ly };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[ly][lx] = secondtilemap[ly][lx];
						map[ly][lx] = tilemap[ly][lx].ascii;
						std::cout << map[ly][lx];
						ly++;
						SetConsoleTextAttribute(hStd, 0x05);
						coord = { (short)lx, (short)ly };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[ly][lx] = Lich;
						map[ly][lx] = tilemap[ly][lx].ascii;
						std::cout << map[ly][lx];
					}
				}
				//left
				else if (lmovement == 3)
				{
					lx--;
					if (tilemap[ly][lx].collide == true)
						lx++;
					else
					{
						lx++;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)lx, (short)ly };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[ly][lx] = secondtilemap[ly][lx];
						map[ly][lx] = tilemap[ly][lx].ascii;
						std::cout << map[ly][lx];
						lx--;
						SetConsoleTextAttribute(hStd, 0x05);
						coord = { (short)lx, (short)ly };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[ly][lx] = Lich;
						map[ly][lx] = tilemap[ly][lx].ascii;
						std::cout << map[ly][lx];
					}
				}
				//right
				else if (lmovement == 4)
				{
					lx++;
					if (tilemap[ly][lx].collide == true)
						lx--;
					else
					{
						lx--;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)lx, (short)ly };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[ly][lx] = secondtilemap[ly][lx];
						map[ly][lx] = tilemap[ly][lx].ascii;
						std::cout << map[ly][lx];
						lx++;
						SetConsoleTextAttribute(hStd, 0x05);
						coord = { (short)lx, (short)ly };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[ly][lx] = Lich;
						map[ly][lx] = tilemap[ly][lx].ascii;
						std::cout << map[ly][lx];
					}
				}
			}
		}
		if (zombied.health > 0)
		{
			//Zombie d movement
			//Zombie d chasing player - no escaping this Zombie!	
			if (zdx == PC.x) //same column
			{
				savevalue = zdy; //save current value of zdy
				if (zdy > PC.y) //if Zombie is lower than player
				{
					if (tilemap[zdy - 1][zdx].ascii == '@')
					{
						PC.health = PC.health - zombied.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
					}
					else
					{
						for (int col = zdy - 1; col > PC.y; --col)
						{
							if (tilemap[col][zdx].collide == true)
							{
								zdy = 0;
								break;
							}
						}
					}
					if (zdy == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zdx, (short)zdy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zdy][zdx] = secondtilemap[zdy][zdx];
						map[zdy][zdx] = tilemap[zdy][zdx].ascii;
						std::cout << map[zdy][zdx];
						zdy--;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zdx, (short)zdy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zdy][zdx] = Zombie;
						map[zdy][zdx] = tilemap[zdy][zdx].ascii;
						std::cout << map[zdy][zdx];
						continue;
					}
				}
				else if (zdy < PC.y) //if Zombie is higher than player
				{
					if (tilemap[zdy + 1][zdx].ascii == '@')
					{
						PC.health = PC.health - zombied.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = zdy + 1; col < PC.y; ++col)
						{
							if (tilemap[col][zdx].collide == true)
							{
								zdy = 0;
								break;
							}
						}
					}
					if (zdy == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zdx, (short)zdy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zdy][zdx] = secondtilemap[zdy][zdx];
						map[zdy][zdx] = tilemap[zdy][zdx].ascii;
						std::cout << map[zdy][zdx];
						zdy++;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zdx, (short)zdy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zdy][zdx] = Zombie;
						map[zdy][zdx] = tilemap[zdy][zdx].ascii;
						std::cout << map[zdy][zdx];
						continue;
					}
				}
			}
			else if (zdy == PC.y) //same row
			{
				savevalue = zdx;
				if (zdx > PC.x) //if Zombie is to the right of player
				{
					if (tilemap[zdy][zdx - 1].ascii == '@')
					{
						PC.health = PC.health - zombied.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = zdx - 1; col > PC.x; --col)
						{
							if (tilemap[zdy][col].collide == true)
							{
								zdx = 0;
								break;
							}
						}
					}
					if (zdx == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zdx, (short)zdy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zdy][zdx] = secondtilemap[zdy][zdx];
						map[zdy][zdx] = tilemap[zdy][zdx].ascii;
						std::cout << map[zdy][zdx];
						zdx--;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zdx, (short)zdy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zdy][zdx] = Zombie;
						map[zdy][zdx] = tilemap[zdy][zdx].ascii;
						std::cout << map[zdy][zdx];
						continue;
					}
				}
				else if (zdx < PC.x) //if Zombie is to the left of player
				{
					if (tilemap[zdy][zdx + 1].ascii == '@')
					{
						PC.health = PC.health - zombied.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = zdx + 1; col < PC.x; ++col)
						{
							if (tilemap[zdy][col].collide == true)
							{
								zdx = 0;
								break;
							}
						}
					}
					if (zdx == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zdx, (short)zdy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zdy][zdx] = secondtilemap[zdy][zdx];
						map[zdy][zdx] = tilemap[zdy][zdx].ascii;
						std::cout << map[zdy][zdx];
						zdx++;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zdx, (short)zdy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zdy][zdx] = Zombie;
						map[zdy][zdx] = tilemap[zdy][zdx].ascii;
						std::cout << map[zdy][zdx];
						continue;
					}
				}
			}
			if ((zdx == 0 || zdy == 0) || (zdx != PC.x && zdy != PC.y))
			{
				if (zdx == 0)
				{
					zdx = savevalue;
				}
				else if (zdy == 0)
				{
					zdy = savevalue;
				}
				//up
				if (zdmovement == 1)
				{
					zdy--;
					if (tilemap[zdy][zdx].collide == true)
						zdy++;
					else
					{
						zdy++;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zdx, (short)zdy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zdy][zdx] = secondtilemap[zdy][zdx];
						map[zdy][zdx] = tilemap[zdy][zdx].ascii;
						std::cout << map[zdy][zdx];
						zdy--;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zdx, (short)zdy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zdy][zdx] = Zombie;
						map[zdy][zdx] = tilemap[zdy][zdx].ascii;
						std::cout << map[zdy][zdx];
					}
				}
				//down
				else if (zdmovement == 2)
				{
					zdy++;
					if (tilemap[zdy][zdx].collide == true)
						zdy--;
					else
					{
						zdy--;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zdx, (short)zdy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zdy][zdx] = secondtilemap[zdy][zdx];
						map[zdy][zdx] = tilemap[zdy][zdx].ascii;
						std::cout << map[zdy][zdx];
						zdy++;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zdx, (short)zdy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zdy][zdx] = Zombie;
						map[zdy][zdx] = tilemap[zdy][zdx].ascii;
						std::cout << map[zdy][zdx];
					}
				}
				//left
				else if (zdmovement == 3)
				{
					zdx--;
					if (tilemap[zdy][zdx].collide == true)
						zdx++;
					else
					{
						zdx++;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zdx, (short)zdy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zdy][zdx] = secondtilemap[zdy][zdx];
						map[zdy][zdx] = tilemap[zdy][zdx].ascii;
						std::cout << map[zdy][zdx];
						zdx--;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zdx, (short)zdy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zdy][zdx] = Zombie;
						map[zdy][zdx] = tilemap[zdy][zdx].ascii;
						std::cout << map[zdy][zdx];
					}
				}
				//right
				else if (zdmovement == 4)
				{
					zdx++;
					if (tilemap[zdy][zdx].collide == true)
						zdx--;
					else
					{
						zdx--;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zdx, (short)zdy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zdy][zdx] = secondtilemap[zdy][zdx];
						map[zdy][zdx] = tilemap[zdy][zdx].ascii;
						std::cout << map[zdy][zdx];
						zdx++;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zdx, (short)zdy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zdy][zdx] = Zombie;
						map[zdy][zdx] = tilemap[zdy][zdx].ascii;
						std::cout << map[zdy][zdx];
					}
				}
			}
		}
		if (zombiee.health > 0)
		{
			//Zombie e movement
			//Zombie e chasing player - no escaping this Zombie!	
			if (zex == PC.x) //same column
			{
				savevalue = zey; //save current value of zey
				if (zey > PC.y) //if Zombie is lower than player
				{
					if (tilemap[zey - 1][zex].ascii == '@')
					{
						PC.health = PC.health - zombiee.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
					}
					else
					{
						for (int col = zey - 1; col > PC.y; --col)
						{
							if (tilemap[col][zex].collide == true)
							{
								zey = 0;
								break;
							}
						}
					}
					if (zey == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zex, (short)zey };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zey][zex] = secondtilemap[zey][zex];
						map[zey][zex] = tilemap[zey][zex].ascii;
						std::cout << map[zey][zex];
						zey--;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zex, (short)zey };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zey][zex] = Zombie;
						map[zey][zex] = tilemap[zey][zex].ascii;
						std::cout << map[zey][zex];
						continue;
					}
				}
				else if (zey < PC.y) //if Zombie is higher than player
				{
					if (tilemap[zey + 1][zex].ascii == '@')
					{
						PC.health = PC.health - zombiee.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = zey + 1; col < PC.y; ++col)
						{
							if (tilemap[col][zex].collide == true)
							{
								zey = 0;
								break;
							}
						}
					}
					if (zey == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zex, (short)zey };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zey][zex] = secondtilemap[zey][zex];
						map[zey][zex] = tilemap[zey][zex].ascii;
						std::cout << map[zey][zex];
						zey++;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zex, (short)zey };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zey][zex] = Zombie;
						map[zey][zex] = tilemap[zey][zex].ascii;
						std::cout << map[zey][zex];
						continue;
					}
				}
			}
			else if (zey == PC.y) //same row
			{
				savevalue = zex;
				if (zex > PC.x) //if Zombie is to the right of player
				{
					if (tilemap[zey][zex - 1].ascii == '@')
					{
						PC.health = PC.health - zombiee.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = zex - 1; col > PC.x; --col)
						{
							if (tilemap[zey][col].collide == true)
							{
								zex = 0;
								break;
							}
						}
					}
					if (zex == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zex, (short)zey };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zey][zex] = secondtilemap[zey][zex];
						map[zey][zex] = tilemap[zey][zex].ascii;
						std::cout << map[zey][zex];
						zex--;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zex, (short)zey };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zey][zex] = Zombie;
						map[zey][zex] = tilemap[zey][zex].ascii;
						std::cout << map[zey][zex];
						continue;
					}
				}
				else if (zex < PC.x) //if Zombie is to the left of player
				{
					if (tilemap[zey][zex + 1].ascii == '@')
					{
						PC.health = PC.health - zombiee.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = zex + 1; col < PC.x; ++col)
						{
							if (tilemap[zey][col].collide == true)
							{
								zex = 0;
								break;
							}
						}
					}
					if (zex == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zex, (short)zey };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zey][zex] = secondtilemap[zey][zex];
						map[zey][zex] = tilemap[zey][zex].ascii;
						std::cout << map[zey][zex];
						zex++;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zex, (short)zey };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zey][zex] = Zombie;
						map[zey][zex] = tilemap[zey][zex].ascii;
						std::cout << map[zey][zex];
						continue;
					}
				}
			}
			if ((zex == 0 || zey == 0) || (zex != PC.x && zey != PC.y))
			{
				if (zex == 0)
				{
					zex = savevalue;
				}
				else if (zey == 0)
				{
					zey = savevalue;
				}
				//up
				if (zemovement == 1)
				{
					zey--;
					if (tilemap[zey][zex].collide == true)
						zey++;
					else
					{
						zey++;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zex, (short)zey };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zey][zex] = secondtilemap[zey][zex];
						map[zey][zex] = tilemap[zey][zex].ascii;
						std::cout << map[zey][zex];
						zey--;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zex, (short)zey };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zey][zex] = Zombie;
						map[zey][zex] = tilemap[zey][zex].ascii;
						std::cout << map[zey][zex];
					}
				}
				//down
				else if (zemovement == 2)
				{
					zey++;
					if (tilemap[zey][zex].collide == true)
						zey--;
					else
					{
						zey--;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zex, (short)zey };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zey][zex] = secondtilemap[zey][zex];
						map[zey][zex] = tilemap[zey][zex].ascii;
						std::cout << map[zey][zex];
						zey++;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zex, (short)zey };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zey][zex] = Zombie;
						map[zey][zex] = tilemap[zey][zex].ascii;
						std::cout << map[zey][zex];
					}
				}
				//left
				else if (zemovement == 3)
				{
					zex--;
					if (tilemap[zey][zex].collide == true)
						zex++;
					else
					{
						zex++;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zex, (short)zey };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zey][zex] = secondtilemap[zey][zex];
						map[zey][zex] = tilemap[zey][zex].ascii;
						std::cout << map[zey][zex];
						zex--;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zex, (short)zey };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zey][zex] = Zombie;
						map[zey][zex] = tilemap[zey][zex].ascii;
						std::cout << map[zey][zex];
					}
				}
				//right
				else if (zemovement == 4)
				{
					zex++;
					if (tilemap[zey][zex].collide == true)
						zex--;
					else
					{
						zex--;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)zex, (short)zey };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zey][zex] = secondtilemap[zey][zex];
						map[zey][zex] = tilemap[zey][zex].ascii;
						std::cout << map[zey][zex];
						zex++;
						SetConsoleTextAttribute(hStd, 0x02);
						coord = { (short)zex, (short)zey };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[zey][zex] = Zombie;
						map[zey][zex] = tilemap[zey][zex].ascii;
						std::cout << map[zey][zex];
					}
				}
			}
		}
		if (dragon.health > 0)
		{
			//Dragon movement
			//Dragon chasing player - no escaping this Dragon!	
			if (dx == PC.x) //same column
			{
				savevalue = dy; //save current value of dy
				if (dy > PC.y) //if Dragon is lower than player
				{
					if (tilemap[dy - 1][dx].ascii == '@')
					{
						PC.health = PC.health - dragon.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
					}
					else
					{
						for (int col = dy - 1; col > PC.y; --col)
						{
							if (tilemap[col][dx].collide == true)
							{
								dy = 0;
								break;
							}
						}
					}
					if (dy == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)dx, (short)dy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[dy][dx] = secondtilemap[dy][dx];
						map[dy][dx] = tilemap[dy][dx].ascii;
						std::cout << map[dy][dx];
						dy--;
						SetConsoleTextAttribute(hStd, 0x04);
						coord = { (short)dx, (short)dy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[dy][dx] = Dragon;
						map[dy][dx] = tilemap[dy][dx].ascii;
						std::cout << map[dy][dx];
						continue;
					}
				}
				else if (dy < PC.y) //if dragon is higher than player
				{
					if (tilemap[dy + 1][dx].ascii == '@')
					{
						PC.health = PC.health - dragon.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = dy + 1; col < PC.y; ++col)
						{
							if (tilemap[col][dx].collide == true)
							{
								dy = 0;
								break;
							}
						}
					}
					if (dy == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)dx, (short)dy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[dy][dx] = secondtilemap[dy][dx];
						map[dy][dx] = tilemap[dy][dx].ascii;
						std::cout << map[dy][dx];
						dy++;
						SetConsoleTextAttribute(hStd, 0x04);
						coord = { (short)dx, (short)dy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[dy][dx] = Dragon;
						map[dy][dx] = tilemap[dy][dx].ascii;
						std::cout << map[dy][dx];
						continue;
					}
				}
			}
			else if (dy == PC.y) //same row
			{
				savevalue = dx;
				if (dx > PC.x) //if dragon is to the right of player
				{
					if (tilemap[dy][dx - 1].ascii == '@')
					{
						PC.health = PC.health - dragon.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = dx - 1; col > PC.x; --col)
						{
							if (tilemap[dy][col].collide == true)
							{
								dx = 0;
								break;
							}
						}
					}
					if (dx == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)dx, (short)dy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[dy][dx] = secondtilemap[dy][dx];
						map[dy][dx] = tilemap[dy][dx].ascii;
						std::cout << map[dy][dx];
						dx--;
						SetConsoleTextAttribute(hStd, 0x04);
						coord = { (short)dx, (short)dy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[dy][dx] = Dragon;
						map[dy][dx] = tilemap[dy][dx].ascii;
						std::cout << map[dy][dx];
						continue;
					}
				}
				else if (dx < PC.x) //if dragon is to the left of player
				{
					if (tilemap[dy][dx + 1].ascii == '@')
					{
						PC.health = PC.health - dragon.strength;
						if (PC.health <= 0)
						{
							PC.health = 0;
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							break;
						}
						else if (PC.health < 10)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << "  ";
							continue;
						}
						else if (PC.health < 100)
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health << " ";
							continue;
						}
						else
						{
							SetConsoleTextAttribute(hStd, 0x07);
							coord = { (short)cc, (short)dd };
							SetConsoleCursorPosition(hStd, coord);
							std::cout << PC.health;
							continue;
						}
						continue;
					}
					else
					{
						for (int col = dx + 1; col < PC.x; ++col)
						{
							if (tilemap[dy][col].collide == true)
							{
								dx = 0;
								break;
							}
						}
					}
					if (dx == savevalue)
					{
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)dx, (short)dy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[dy][dx] = secondtilemap[dy][dx];
						map[dy][dx] = tilemap[dy][dx].ascii;
						std::cout << map[dy][dx];
						dx++;
						SetConsoleTextAttribute(hStd, 0x04);
						coord = { (short)dx, (short)dy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[dy][dx] = Dragon;
						map[dy][dx] = tilemap[dy][dx].ascii;
						std::cout << map[dy][dx];
						continue;
					}
				}
			}
			if ((dx == 0 || dy == 0) || (dx != PC.x && dy != PC.y))
			{
				if (dx == 0)
				{
					dx = savevalue;
				}
				else if (dy == 0)
				{
					dy = savevalue;
				}
				//up
				if (dmovement == 1)
				{
					dy--;
					if (tilemap[dy][dx].collide == true)
						dy++;
					else
					{
						dy++;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)dx, (short)dy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[dy][dx] = secondtilemap[dy][dx];
						map[dy][dx] = tilemap[dy][dx].ascii;
						std::cout << map[dy][dx];
						dy--;
						SetConsoleTextAttribute(hStd, 0x04);
						coord = { (short)dx, (short)dy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[dy][dx] = Dragon;
						map[dy][dx] = tilemap[dy][dx].ascii;
						std::cout << map[dy][dx];
					}
				}
				//down
				else if (dmovement == 2)
				{
					dy++;
					if (tilemap[dy][dx].collide == true)
						dy--;
					else
					{
						dy--;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)dx, (short)dy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[dy][dx] = secondtilemap[dy][dx];
						map[dy][dx] = tilemap[dy][dx].ascii;
						std::cout << map[dy][dx];
						dy++;
						SetConsoleTextAttribute(hStd, 0x04);
						coord = { (short)dx, (short)dy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[dy][dx] = Dragon;
						map[dy][dx] = tilemap[dy][dx].ascii;
						std::cout << map[dy][dx];
					}
				}
				//left
				else if (dmovement == 3)
				{
					dx--;
					if (tilemap[dy][dx].collide == true)
						dx++;
					else
					{
						dx++;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)dx, (short)dy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[dy][dx] = secondtilemap[dy][dx];
						map[dy][dx] = tilemap[dy][dx].ascii;
						std::cout << map[dy][dx];
						dx--;
						SetConsoleTextAttribute(hStd, 0x04);
						coord = { (short)dx, (short)dy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[dy][dx] = Dragon;
						map[dy][dx] = tilemap[dy][dx].ascii;
						std::cout << map[dy][dx];
					}
				}
				//right
				else if (dmovement == 4)
				{
					dx++;
					if (tilemap[dy][dx].collide == true)
						dx--;
					else
					{
						dx--;
						SetConsoleTextAttribute(hStd, 0x07);
						coord = { (short)dx, (short)dy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[dy][dx] = secondtilemap[dy][dx];
						map[dy][dx] = tilemap[dy][dx].ascii;
						std::cout << map[dy][dx];
						dx++;
						SetConsoleTextAttribute(hStd, 0x04);
						coord = { (short)dx, (short)dy };
						SetConsoleCursorPosition(hStd, coord);
						tilemap[dy][dx] = Dragon;
						map[dy][dx] = tilemap[dy][dx].ascii;
						std::cout << map[dy][dx];
					}
				}
			}
		}
		else
		{
			if (tilemap[PC.y + 1][PC.x].collide == false && tilemap[PC.y - 1][PC.x].collide == false && tilemap[PC.y][PC.x + 1].collide == false && tilemap[PC.y][PC.x - 1].collide == false)
			{
				SetConsoleTextAttribute(hStd, 0x07);
				coord = { (short)gg, (short)hh };
				SetConsoleCursorPosition(hStd, coord);
				std::cout << "- ";
			}
		}
	}
	x = 0;
	y = 45;
	coord = { (short)x, (short)y };
	SetConsoleCursorPosition(hStd, coord);
	SetConsoleTextAttribute(hStd, 0x07);
	std::system("Pause");
	std::system("cls");
	if (win == true)
	{
		std::cout << "VICTORY!\n\n";
		std::system("Pause");
	}
	else
	{
		std::cout << "GAME OVER\n";
		std::system("Pause");
	}
	return 0;
}