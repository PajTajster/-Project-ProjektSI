
#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <fstream>

using namespace std;

typedef unsigned int uint;

// Struktura punktu
typedef struct coord
{
	int x;
	int y;
}Coordinate;

// Do porównywania koordynatów
bool operator==(const Coordinate &c1, const Coordinate &c2)
{
	return (c1.x == c2.x) && (c1.y == c2.y);
}

class Maze
{
private:
	Coordinate start;	// Koordynaty startu
	Coordinate end;		// Koordynaty końca
	string* maze;		// Tablica na labirynt
	uint mazeSize;		// Określenie jak "długi" jest labirynt
	bool isMazePresent;	// Czy wczytany jest labirynt

	// Szukanie algorytmem BFS - Breadth First Search
	bool searchBFS()
	{
		queue<Coordinate> BFSqueue;
		int i, j;

		// Koordynaty aktualnie rozpatrywanego wierzchołka
		Coordinate actualVertex;
		// Koordynaty sąsiada do rozpatrzenia
		Coordinate nextVertex;

		// Zaczynamy od startu
		BFSqueue.push(start);

		// Rozpatrujemy ścieżkę dopóki jest jakaś droga
		while (!BFSqueue.empty())
		{
			// Najpierw pobieramy wiersz (i zdejmujemy z kolejki)
			actualVertex = BFSqueue.front();
			BFSqueue.pop();

			// Jeśli dany wierzchołek jest wyjściem, to koniec pętli
			if (actualVertex == end)
				break;

			for (i = -1; i <= 1; ++i)
			{
				for (j = -1; j <= 1; ++j)
				{
					if ((i != j) && (!i || !j))
					{
						if (maze[actualVertex.x + i][actualVertex.y + j] == ' ')
						{
							// Informacja o kieruku przybycia algorytmu

							if (i == -1)
								maze[actualVertex.x + i][actualVertex.y + j] = 'd'; // dół
							else if (i == 1)
								maze[actualVertex.x + i][actualVertex.y + j] = 'u'; // góra
							else if (j == -1)
								maze[actualVertex.x + i][actualVertex.y + j] = 'r'; // prawo
							else
								maze[actualVertex.x + i][actualVertex.y + j] = 'l'; // lewo

							nextVertex.x = (actualVertex.x + i);
							nextVertex.y = (actualVertex.y + j);

							// Następnie rozpatrywanego sąsiada umieszcza się w kolejce
							BFSqueue.push(nextVertex);
						}
					}
				}
			}


		}

		if (maze[end.x][end.y] == ' ')
			return false;

		return true;
	}

public:

	// Tworzenie labiryntu
	int createMaze(string fileName)
	{
		ifstream file;
		string fileBuf;
		
		file.open(fileName + ".txt");
		if (!file.is_open())
			return -1;
		
		
		while (file.good())
		{
			getline(file, fileBuf);
			++mazeSize;
		}

		if (!(fileBuf == "END"))
			return -1;

		maze = new string[--mazeSize];

		file.clear();
		file.seekg(0, ios::beg);
		
		//Wypełnianie tablicy i poszukiwanie startu i końca
		for (uint i = 0; i < mazeSize; ++i)
		{
			getline(file, maze[i]);
			
			for (uint j = 0; j<maze[i].length();++j)
			{
				// Znalezienie startu
				if (maze[i][j] == 'S')
				{
					start.x = i;
					start.y = j;
				}
				// Znalezienie końca
				else if (maze[i][j] == 'E')
				{
					end.x = i;
					end.y = j;
					// Zastępujemy znak końca znakiem ścieżki
					//by łatwiej było dla algorytmu
					maze[i][j] = ' ';
				}
			}
		}

		file.close();

		// Błąd tworzenia labiryntu
		if (start.x == -1 || start.y == -1
			|| end.x == -1 || end.y == -1
			|| mazeSize == 0)
			return -1;

		isMazePresent = true;
		return 0;
	}

	// Wyświetl labirynt ze ścieżką (jeżeli taka istnieje)
	int writePath()
	{
		if (!searchBFS())
		{
			return -1;
		}

		int x = end.x, y = end.y;
		char c;

		// Kropkujemy znalezioną ścieżkę
		for (; (x != start.x) || (y != start.y);)
		{
			c = maze[x][y];
			maze[x][y] = '.';
			switch (c)
			{
			case 'd':
				++x;
				break;
			case 'u':
				--x;
				break;
			case 'r':
				++y;
				break;
			case 'l':
				--y;
				break;
			}
		}

		// Przywracamy końcowi jego znak
		maze[end.x][end.y] = 'E';

		string s;
		// Wypisanie labiryntu
		for (uint i = 0; i < mazeSize; ++i)
		{
			s = maze[i];
			for (uint j = 0; j < s.length(); ++j)
			{
				// Usuwanie informacji kierunków dla algorytmu
				switch (s[j])
				{
				case 'd':
				case 'u':
				case 'r':
				case 'l':
					s[j] = ' ';
				}
			}

			cout << s << endl;
		}
		cout << endl;

		return 0;
	}

	

	// Czy załadowano labirynt
	//true - jeżeli tak
	//false - jeżeli nie
	bool isMazeLoaded()
	{
		return isMazePresent;
	}

	Maze()
	{
		start.x = -1;
		start.y = -1;
		end.x = -1;
		end.y = -1;
		mazeSize = 0;
		isMazePresent = false;
	}
	~Maze()
	{
		delete[] maze;
	}
};

void toContinue()
{
	system("pause");
	system("cls");
}

// Menu Główne
void MainMenu()
{
	Maze* m = new Maze();
	string info = "", fileName = "";
	char option;
	bool isMazeSearched = false;
	for (;;)
	{
		// Informacja o stanie załadowania labiryntu
		switch (m->isMazeLoaded())
		{
		case true:
			info = "(Maze loaded - filename: " + fileName + ".txt)";
			break;
		case false:
			info = "(Maze is not loaded!!)";
			break;
		}
		// Menu
		cout << "\tMaze searching program\n\n\
			\n1. Load maze\n2. Search the maze " << info \
			<< endl << "3. Exit\nChoose option: ";
		cin >> option;
		switch (option)
		{
			// Wczytywanie labiryntu
		case '1':
			cout << "Please enter the filename[without extension]: ";
			cin >> fileName;
			delete m;
			m = new Maze();
			if (m->createMaze(fileName))
			{
				cout << "Error creating maze." << endl;
				toContinue();
				break;
			}
			cout << "Maze loaded successfully." << endl;
			isMazeSearched = false;
			toContinue();
			break;
			// Przeszukiwanie labiryntu
		case '2':
			if (!m->isMazeLoaded())
			{
				cout << "MAZE IS NOT LOADED, RETURNING\n";
				toContinue();
				break;
			}
			if (isMazeSearched)
			{
				delete m;
				m = new Maze();
				m->createMaze(fileName);
				isMazeSearched = false;
			}
			if (m->writePath())
			{
				cout << "No way was found.";
				toContinue();
				break;
			}
			isMazeSearched = true;
			toContinue();
			break;
			// Wyjście
		case '3':
			delete m;
			return;
		default:
			cout << "WRONG OPTION, RETURNING\n";
			toContinue();
			break;
		}
	}

	delete m;
	return;
}

int main()
{
	Maze m;
	MainMenu();
	if (m.createMaze("test2"))
	{
		cout << "Error creating labyrinth :(" << endl;
		return -1;
	}


	cout << "\nProgram executed correctly, exiting.\n\n" \
		<< "Creators: \nRomanowicz Mateusz\nPatryk Wojtach\n\n"<< endl;

	toContinue();

	return 0;
}