#pragma once
#include <SFML/Graphics.hpp>
#include "ResourceHolder.h"

class Square : public sf::RectangleShape
{
public:
	Square();
	// �����������, ���� ������ �������, ��������� ����� �� ��'��� ResourceHolder.
	Square(sf::Vector2f pos, int _correct_number, ResourceHolder* _rh);
	~Square();
	int correct_number;       // ��������� �����
	bool locked;              // ���������, �� ����� �� ���������� ��������
	int entered_number;       // ������� �����
	// ������� �������. �������� manual_mode ����� �� ������ �����, game_mode - �� ����� ���.
	void update(bool manual_mode, int game_mode);
	// ����� ������� �� ��� sf::RenderWindow.
	void draw(sf::RenderWindow& window);

private:
	sf::Text text; // ����� �� �������
	ResourceHolder* rh; // ��'��� ResourceHolder
};

