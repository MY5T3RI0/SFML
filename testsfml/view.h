#include <SFML/Graphics.hpp>
using namespace sf;

sf::View view;

void setPlayerCoordinateForView(float x, float y) { 
	float tempX = x; float tempY = y;

	if (x < 600) tempX = 600;
	if (x > 1000) tempX = 1000;
	if (y < 400) tempY = 400;//������� �������
	if (y > 465) tempY = 465;//������ �������.��� ����� �����

	view.setCenter(tempX, tempY); 
}