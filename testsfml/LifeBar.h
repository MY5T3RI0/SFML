#include <SFML/Graphics.hpp>
#include "Entity.h"

using namespace sf;

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

		bar.setFillColor(Color(0, 0, 0));//������ ������������� ������������� ������ � ���������� ������ ���������� ��������
		max = 100;
	}

	void update(int k)// k-������� ��������

	{
		if (k > 0)
			if (k < max)
				bar.setSize(Vector2f(10, (max - k) * 70 / max));//���� �� ������������ � ��� ���� ������ ���������, �� ������������� ����� �������� (����� ������) ��� ������� ��������������
	}

	void draw(RenderWindow& window, Entity entity)
	{

		s.setPosition(entity.x - 5, entity.y + 10);//������� �� ������
		bar.setPosition(entity.x - 1, entity.y + 14);

		window.draw(s);//������� ������ ������� ��������
		window.draw(bar);//������ �� ��� ������ �������������, �� ��� �� ��������� �
	}

};