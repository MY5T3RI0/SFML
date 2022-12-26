#include <SFML/Graphics.hpp>
#include "view.h"
#include <iostream>
#include <sstream>
#include "mission.h"
#include "iostream"
#include "level.h"
#include <vector>
#include <list>

using namespace sf;

void menu(RenderWindow& window) {
	Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("images/111.png");
	menuTexture2.loadFromFile("images/222.png");
	menuTexture3.loadFromFile("images/333.png");
	aboutTexture.loadFromFile("images/about.png");
	menuBackground.loadFromFile("images/Penguins.jpg");
	Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), about(aboutTexture), menuBg(menuBackground);
	bool isMenu = 1;
	int menuNum = 0;
	menu1.setPosition(100, 30);
	menu2.setPosition(100, 90);
	menu3.setPosition(100, 150);
	menuBg.setPosition(345, 0);

	//////////////////////////////МЕНЮ///////////////////
	while (isMenu)
	{

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		if (Keyboard::isKeyPressed && menuNum != 2) {

			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				exit(1);
			}

		}

		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		menu3.setColor(Color::White);
		menuNum = 0;
		window.clear(Color(129, 181, 221));

		if (IntRect(100, 30, 300, 50).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Blue); menuNum = 1; }
		if (IntRect(100, 90, 300, 50).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Blue); menuNum = 2; }
		if (IntRect(100, 150, 300, 50).contains(Mouse::getPosition(window))) { menu3.setColor(Color::Blue); menuNum = 3; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) isMenu = false;//если нажали первую кнопку, то выходим из меню 
			if (menuNum == 2) { window.draw(about); window.display(); while (!Keyboard::isKeyPressed(Keyboard::Escape)); }
			if (menuNum == 3) { window.close(); isMenu = false; }

		}

		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu2);
		window.draw(menu3);

		window.display();
	}
	////////////////////////////////////////////////////
}

////////////////////////////////////Общий класс-родитель//////////////////////////
class Entity {

public:
	std::vector<Object> obj;//вектор объектов карты
	float dx, dy, x, y, speed, moveTimer, mass;
	int w, h, health;
	bool life, isMove, onGround;
	bool isRight = true;
	Texture texture;
	Sprite sprite;
	String name;

	Entity() {

	}

	Entity(Image& image, String Name, float X, float Y, int W, int H, float _mass) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0; mass = _mass;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}

	FloatRect getRect() {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
		return FloatRect(x, y, w, h);//эта ф-ция нужна для проверки столкновений 
	}
};

class LifeBar
{
public:
	Image image;
	Texture t;
	Sprite s;
	int max;
	RectangleShape bar;

	LifeBar()
	{
		image.loadFromFile("images/life.png");
		image.createMaskFromColor(Color(50, 96, 166));
		t.loadFromImage(image);
		s.setTexture(t);
		s.setTextureRect(IntRect(783, 2, 15, 84));
		s.setRotation(90);
		bar.setRotation(90);

		bar.setFillColor(Color(255, 255, 255));//черный прямоугольник накладывается сверху и появляется эффект отсутствия здоровья
		max = 100;
	}

	void update(int k)// k-текущее здоровье
	{
		if (k > 0)
			if (k < max)
				bar.setSize(Vector2f(10, (max - k) * 70 / max));//если не отрицательно и при этом меньше максимума, то устанавливаем новое значение (новый размер) для черного прямоугольника
	}

	void draw(RenderWindow& window, Entity entity)
	{

		s.setPosition(entity.x + entity.w/2 + 42, entity.y - 25);//позиция на экране
		bar.setPosition(entity.x + entity.w/2 + 38, entity.y - 21);

		window.draw(s);//сначала рисуем полоску здоровья
		window.draw(bar);//поверх неё уже черный прямоугольник, он как бы покрывает её
	}

};
////////////////////////////////////////////////////КЛАСС ИГРОКА////////////////////////

class Enemy :public Entity{
public:
	bool isMouseDown = false;

	Enemy(Image &image, String Name,Level &lvl, float X, float Y, int W, int H, int mass) :Entity(image, Name, X, Y, W, H, mass){
		obj = lvl.GetObjects("solid");//инициализируем.получаем нужные объекты для взаимодействия врага с картой
		if (name == "EasyEnemy"){
			sprite.setTextureRect(IntRect(0, 0, w, h));
			dx = 0.0;
		}
	}

	Enemy() {};

	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = 0; i<obj.size(); i++)//проходимся по объектам
		if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
		{
			//if (obj[i].name == "solid"){//если встретили препятствие (объект с именем solid)
				if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; onGround = true;}
				if (Dy<0)	{ y = obj[i].rect.top + obj[i].rect.height;  dy = 0; }
				if (Dx>0)	{ x = obj[i].rect.left - w;  dx = -0.1; sprite.scale(-1, 1); }
				if (Dx<0)	{ x = obj[i].rect.left + obj[i].rect.width; dx = 0.1; sprite.scale(-1, 1); }
			//}
		}
	}

	void update(float time)
	{
		if (name == "EasyEnemy"){
			checkCollisionWithMap(dx, 0);
			x += dx*time;
			checkCollisionWithMap(0, dy);
			y += dy * time;
			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0){ life = false; }
			if (!isMouseDown)
			{
				dy = dy + mass * 3. / 100000 * time;
			}
		}
	}
};

class Player : public Entity {
public:
	enum { left, right, up, down, jump, stay, attack } state;
	int playerScore;
	bool isAttack = false;
	bool isMouseDown = false;
	Enemy enemy;

	Player(Image& image, String Name, Level& lev, float X, float Y, int W, int H, float mass, Enemy _enemy) :Entity(image, Name, X, Y, W, H, mass) {
		playerScore = 0; state = stay; obj = lev.GetAllObjects(); enemy = _enemy;//инициализируем.получаем все объекты для взаимодействия персонажа с картой
		if (name == "Player1") {
			sprite.setTextureRect(IntRect(1, 1, w, h));
		}
	}

	void control() {
		if (Keyboard::isKeyPressed) {
			if (Keyboard::isKeyPressed(Keyboard::A)) {
				state = left; speed = 0.3;
				isRight = false;
			}
			if (Keyboard::isKeyPressed(Keyboard::D)) {
				state = right; speed = 0.3;
				isRight = true;
			}

			if ((Keyboard::isKeyPressed(Keyboard::W)) && (onGround)) {
				state = jump; dy = -0.6; onGround = false;
			}

			if (Keyboard::isKeyPressed(Keyboard::S)) {
				state = down;
			}

			if (Keyboard::isKeyPressed(Keyboard::E)) {
				state = attack;
				isAttack = true;
			}

		}
	}



	void checkCollisionWithMap(float Dx, float Dy)
	{

		for (int i = 0; i < obj.size(); i++)//проходимся по объектам
			if (getRect().intersects(obj[i].rect))//проверяем пересечение игрока с объектом
			{
				if (obj[i].name == "solid")//если встретили препятствие
				{
					if (Dy > 0) { y = obj[i].rect.top - h;  dy = 0; onGround = true; state = stay; dx = 0; }
					if (Dy < 0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx > 0) { x = obj[i].rect.left - w; }
					if (Dx < 0) { x = obj[i].rect.left + obj[i].rect.width; }
				}
			}

		FloatRect enemyRect = enemy.getRect();
		enemyRect.top -= 3;
		enemyRect.top = (int)enemyRect.top;
		if (getRect().intersects(enemyRect))//проверяем пересечение игрока с объектом
		{
			if (Dy > 0) { y = enemyRect.top - h;  dy = 0; onGround = true; state = stay; dx = 0; }
			if (Dy < 0) { y = enemyRect.top + enemyRect.height ; dy = 0; }
			if (Dx > 0) { x = enemyRect.left - w; }
			if (Dx < 0) { x = enemyRect.left + enemyRect.width; }
		}
	}

	void update(float time)
	{
		control();
		switch (state)
		{
		case right:dx = speed;
			break;
		case left:dx = -speed;
			break;
		case up:
			break;
		case down: dx = 0;
			break;
		case stay:
			break;
		case jump:

			break;
		case attack:
			if (isRight)
			{
				sprite.setTextureRect(IntRect({ 0, 174 }, { 55, 64 }));
			}
			else
			{
				sprite.setTextureRect(IntRect({ 55, 174 }, { -55, 64 }));
			}
			state = stay;
			dx = 0;
			break;
		}

		if (dy < 0 && !onGround) {
			if (isRight)
			{
				sprite.setTextureRect(IntRect({ 0, 58 * 2 }, { 53, 56 }));
			}
			else
			{
				sprite.setTextureRect(IntRect({ 53, 58 * 2 }, { -53, 56 }));
			}
		}
		if (dy > 0 && !onGround) {
			if (isRight)
			{
				sprite.setTextureRect(IntRect({ 58 * 3, 58 * 2 }, { 53, 56 }));
			}
			else
			{
				sprite.setTextureRect(IntRect({ 53 + 58 * 3, 58 * 2 }, { -53, 56 }));
			}
		}

		x += dx * time;
		checkCollisionWithMap(dx, 0);
		y += dy * time;
		checkCollisionWithMap(0, dy);
		sprite.setPosition(x + w / 2, y + h / 2);

		if (health <= 0) { life = false; }
		if (!isMove) { speed = 0; }
		setPlayerCoordinateForView(x, y);
		if (life) { setPlayerCoordinateForView(x, y); }
		if (!isMouseDown)
		{
			dy = dy + mass * 3. / 100000 * time;
		}
	}
};

int main()
{
	RenderWindow window(VideoMode(1200, 800), "Thanks, Idelia!");
	menu(window);
	view.reset(FloatRect(0, 0, 1200, 800));

	Level lvl;//создали экземпляр класса уровень
	lvl.LoadFromFile("map.tmx");//загрузили в него карту, внутри класса с помощью методов он ее обработает.

	Image heroImage;
	heroImage.loadFromFile("images/heroSprite.png");
	heroImage.createMaskFromColor(Color(0, 128, 0));
	heroImage.createMaskFromColor(Color(255, 255, 255));

	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/wall.png");
	easyEnemyImage.createMaskFromColor(Color(255, 255, 255));

	Image sunImage;
	sunImage.loadFromFile("images/sun.png");
	Texture sunTexture;
	sunTexture.loadFromImage(sunImage);
	Sprite sunSprite;
	sunSprite.setTexture(sunTexture);
	sunSprite.setTextureRect(IntRect({ 0, 0 }, { 290, 290 }));
	sunSprite.setPosition({500, 300});

	Object player=lvl.GetObject("player");//объект игрока на нашей карте.задаем координаты игроку в начале при помощи него
	Object easyEnemyObject = lvl.GetObject("easyEnemy");//объект легкого врага на нашей карте.задаем координаты игроку в начале при помощи него
	
	Enemy easyEnemy(easyEnemyImage, "EasyEnemy", lvl, easyEnemyObject.rect.left, easyEnemyObject.rect.top, 32, 128, 50);//передаем координаты прямоугольника easyEnemy из карты в координаты нашего врага
	Player p(heroImage, "Player1", lvl, player.rect.left, player.rect.top, player.rect.width, player.rect.height, 50, easyEnemy);//передаем координаты прямоугольника player из карты в координаты нашего игрока

	bool isMouseDown = false;
	bool isPlayer = false;
	bool isEnemy = false;
	float dx, dy;
	float CurrentFrame = 0;
	Clock clock;
	LifeBar lifeBarEnemy;
	LifeBar lifeBarPlayer;

	while (window.isOpen())
	{
		p.enemy = easyEnemy;
		float time = (float)clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
		CurrentFrame += 0.013f * time;
		
		Vector2i pixelPos = Mouse::getPosition(window);
		Vector2f pos = window.mapPixelToCoords(pixelPos);
		Event event;

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			view.reset(FloatRect(0, 0, 1200, 800));
			window.setView(view);
			menu(window);
		}

		p.sprite.setColor(Color::White); // и даем ему прежний цвет
		easyEnemy.sprite.setColor(Color::White);  // и даем ему прежний цвет
		if (isMouseDown && isPlayer)
		{										  // если можем двигать
			p.sprite.setColor(Color::Green); // красим спрайт в зеленый
			p.x = pos.x - dx;				  // двигаем спрайт по Х
			p.y = pos.y - dy;				  // двигаем по Y
												  // p.sprite.setPosition(pos.x - dX, pos.y - dY);//можно и так написать,если у вас нету х и у в классе игрока
		}
		else if (isMouseDown && isEnemy)
		{
			easyEnemy.sprite.setColor(Color::Green); // красим спрайт в зеленый
			easyEnemy.x = pos.x - dx;				 // двигаем спрайт по Х
			easyEnemy.y = pos.y - dy;				 // двигаем по Y
												 // p.sprite.setPosition(pos.x - dX, pos.y - dY);//можно и так написать,если у вас нету х и у в классе игрока
			for (size_t i = 0; i < p.obj.size(); i++)
			{
				if(p.obj[i].name == "easyEnemy")
				{
					p.obj[i].rect.left = pos.x - dx;
					p.obj[i].rect.top = pos.y - dy;
				}
			}
		}

		if (CurrentFrame >= 6)
			CurrentFrame = 0;

		if (p.isRight)
			p.sprite.setTextureRect(IntRect({ int(CurrentFrame) * 58, 0 }, { 42, 56 }));
		else
			p.sprite.setTextureRect(IntRect({ 42 + int(CurrentFrame) * 58, 0 }, {-42, 56 }));

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();	

			if (event.type == Event::MouseButtonPressed)
				if ((enum sf::Mouse::Button)event.key.code == Mouse::Left)		  // а именно левая
					if (p.sprite.getGlobalBounds().contains({pos.x, pos.y })) // и при этом координата курсора попадает в спрайт
					{
						isPlayer = true;
						dx = pos.x - p.sprite.getPosition().x; // делаем разность между позицией курсора и спрайта.для корректировки нажатия
						dy = pos.y - p.sprite.getPosition().y; // тоже самое по игреку
						isMouseDown = true;							// можем двигать спрайт
						p.isMouseDown = true;
					}
					else if (easyEnemy.sprite.getGlobalBounds().contains({ pos.x, pos.y }))
					{
						isEnemy = true;
						dx = pos.x - easyEnemy.sprite.getPosition().x; // делаем разность между позицией курсора и спрайта.для корректировки нажатия
						dy = pos.y - easyEnemy.sprite.getPosition().y; // тоже самое по игреку
						isMouseDown = true;						   // можем двигать спрайт
						easyEnemy.isMouseDown = true;
					}

			if (event.type == Event::MouseButtonReleased) // если отпустили клавишу
				if ((enum sf::Mouse::Button)event.key.code == Mouse::Left)
				{
					isMouseDown = false; // то не можем двигать спрайт
					isPlayer = false;
					isEnemy = false;
					p.isMouseDown = false;
					easyEnemy.isMouseDown = false;
				} // а именно левую

			if (event.type == Event::Closed)
				window.close();
		}

		if (Keyboard::isKeyPressed) {
			if (Keyboard::isKeyPressed(Keyboard::A)) {
				p.sprite.setTextureRect(IntRect({ int(CurrentFrame) * 58 + 53, 58 }, { -53, 56 }));
			}
			if (Keyboard::isKeyPressed(Keyboard::D)) {
				p.sprite.setTextureRect(IntRect({ int(CurrentFrame) * 58, 58 }, { 53, 56 }));
			}
		}

		lifeBarEnemy.update(easyEnemy.health);
		lifeBarPlayer.update(p.health);

		p.update(time);
		FloatRect attackRange = p.getRect();
		if (p.isRight)
			attackRange.width += 5;
		else
			attackRange.left -= 5;

		if (p.isAttack && attackRange.intersects(easyEnemy.getRect()))
		{
			easyEnemy.health -= 1;

			if (easyEnemy.health <= 0)
				easyEnemy.life = false;
		}
		
		p.isAttack = false;

		easyEnemy.update(time);
		window.setView(view);
		window.clear(Color(77,83,140));
		lvl.Draw(window);//рисуем новую карту

		if (easyEnemy.life)
		{
			window.draw(easyEnemy.sprite);
			lifeBarEnemy.draw(window, easyEnemy);
		}	
		if (p.life)
		{
			window.draw(p.sprite);
			lifeBarPlayer.draw(window, p);
		}
		float sunX;
		if (p.x - 145 < 500) sunX = 500;
		else if (p.x - 145 > 850) sunX = 850;
		else sunX = p.x - 145;
		sunSprite.setPosition({ sunX, 100 });
		window.draw(sunSprite);
		window.display();
	}
	return 0;
}
