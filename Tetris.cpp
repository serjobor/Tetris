
#include <SFML/Graphics.hpp>
#include <time.h>

#include <sstream>
#include <iostream>

#pragma warning(disable : 4996)

using namespace sf;

const int M = 20;
const int N = 10;

int field[M][N] = { 0 };
int w = 34;

struct Point
{
	int x, y;
}a[4], b[4];

int figures[7][4] = {
1,3,5,7,
2,4,5,7,
3,5,4,6,
3,5,4,7,
2,3,5,7,
3,5,7,6,
2,3,4,5,
};

void menu(RenderWindow& window) 
{
	Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground1, menuBackground2;

	aboutTexture.loadFromFile("./Assets/image/information.png");
	menuBackground1.loadFromFile("./Assets/image/background.png");
	menuBackground2.loadFromFile("./Assets/image/background.png");

	Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), about(aboutTexture), menuBg1(menuBackground1), menuBg2(menuBackground2);
	bool isMenu = 1;
	int menuNum = 0;

	Font font2;
	font2.loadFromFile("./Assets/font/font2.ttf");

	Text text1("New Game", font2, 40);
	text1.setColor(Color::White);
	text1.setPosition(234, 210);

	Text text2("Info", font2, 40);
	text2.setColor(Color::White);
	text2.setPosition(234, 280);

	Text text3("Exit", font2, 40);
	text3.setColor(Color::White);
	text3.setPosition(234, 345);

	menuBg1.setPosition(0, 0);
	menuBg2.setPosition(340, 0);

	//МЕНЮ
	while (isMenu)
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		text1.setColor(Color::White);
		text2.setColor(Color::White);
		text3.setColor(Color::White);

		menuNum = 0;
		window.clear(Color(50, 50, 50));

		if (IntRect(234, 210, 300, 50).contains(Mouse::getPosition(window))) { text1.setColor(Color(200, 200, 30)); menuNum = 1; }
		if (IntRect(234, 280, 300, 50).contains(Mouse::getPosition(window))) { text2.setColor(Color(200, 200, 30)); menuNum = 2; }
		if (IntRect(234, 345, 300, 50).contains(Mouse::getPosition(window))) { text3.setColor(Color(200, 200, 30)); menuNum = 3; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) isMenu = false;//Новая игра -> закрывается меню, запускается игра 
			if (menuNum == 2) { window.draw(about); window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); } // окно с информацией, Esc-выход обратно в меню
			if (menuNum == 3) { window.close(); isMenu = false; } // выход из игры

		}

		window.draw(menuBg1);
		window.draw(menuBg2);
		window.draw(text1);
		window.draw(text2);
		window.draw(text3);
		window.display();
	}	
}

bool check() 
{
	for (int i = 0; i < 4; i++)

		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)

			return 0;

		else if (field[a[i].y][a[i].x])

			return 0;

	return 1;
}

int main()
{
	srand(time(0));

	RenderWindow window(VideoMode((M * w) - w, M * w), "Tetris!"); // игровое окно

	menu(window);

	Font font;
	font.loadFromFile("./Assets/font/font.ttf");
	Text text("", font, 40);
	text.setColor(Color::White);
	int Points = 0; // очки за игру


	Texture background;
	background.loadFromFile("./Assets/image/background.png");
	Sprite bg;
	bg.setTexture(background);
	bg.setPosition(0,0);

	Texture background2;
	background2.loadFromFile("./Assets/image/background2.png");
	Sprite bg2;
	bg2.setTexture(background2);
	bg2.setPosition(340, 0);


	Texture t;
	t.loadFromFile("./Assets/image/tiles.png");
	Sprite tiles(t);

	int dx = 0, colorNum = 1;
	bool rotate = false;
	float timer = 0, delay = 0.3;

	Clock clock;
	bool ad = true;

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();

		clock.restart();

		timer += time;

		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::KeyPressed)

				if (event.key.code == Keyboard::Up)
					rotate = true;

				else if (event.key.code == Keyboard::Right)
					dx = 1;

				else if (event.key.code == Keyboard::Left)
					dx = -1;
		}

		if (Keyboard::isKeyPressed(Keyboard::Down))
			delay = 0.05;

		for (int i = 0; i < 4; i++) 
		{
			b[i] = a[i];
			a[i].x += dx;
		}

		if (!check()) 
		{
			for (int i = 0; i < 4; i++)
				a[i] = b[i];
		}

		if (rotate) 
		{
			Point p = a[1];

			for (int i = 0; i < 4; i++) 
			{
				int x = a[i].y - p.y;
				int y = a[i].x - p.x;

				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}

			if (!check()) 
			{
				for (int i = 0; i < 4; i++)
					a[i] = b[i];
			}
		}

		if (timer > delay) 
		{
			for (int i = 0; i < 4; i++) 
			{
				b[i] = a[i];
				a[i].y += 1;
			}

			if (!check()) 
			{
				for (int i = 0; i < 4; i++)
					field[b[i].y][b[i].x] = colorNum;

				colorNum = 1 + rand() % 7;

				int n = rand() % 7;

				for (int i = 0; i < 4; i++) 
				{
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
				}
			}
			timer = 0;
		}

		if (ad) 
		{
			int n = rand() % 7;

			if (a[0].x == 0)

				for (int i = 0; i < 4; i++) 
				{
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
				}

			ad = false;
		}

		int k = M - 1;   // удаление при заполнении линии
		for (int i = M - 1; i > 0; i--)
		{
			int count = 0;

			for (int j = 0; j < N; j++) 
			{
				if (field[i][j])

					count++;
				field[k][j] = field[i][j];
			}

			if (count < N)// удаление при заполнении линии, Points++;
				k--;

			//Счетчик очков
			if (count == N) 
			{
				Points++;
			}
		}

		dx = 0;
		rotate = false;
		delay = 0.3; // счетчик скорости

		window.clear(Color(50,50,50));		

		//Фон
		window.draw(bg);
		window.draw(bg2);

		// заработанные очки
		std::ostringstream playerPointsString;
		playerPointsString << Points;
		text.setString("Points: " + playerPointsString.str());
		text.setPosition(408,136);
		window.draw(text);

		for (int i = 0; i < M; i++)

			for (int j = 0; j < N; j++) 
			{
				if (field[i][j] == 0)
					continue;

				tiles.setTextureRect(IntRect(field[i][j] * w, 0, w, w));
				tiles.setPosition(j * w, i * w);
				window.draw(tiles);
			}

		for (int i = 0; i < 4; i++) 
		{
			tiles.setTextureRect(IntRect(colorNum * w, 0, w, w));
			tiles.setPosition(a[i].x * w, a[i].y * w);
			window.draw(tiles);
		}
		window.display();
	}
	return 0;
}