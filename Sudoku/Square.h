#pragma once
#include <SFML/Graphics.hpp>
#include "ResourceHolder.h"

class Square : public sf::RectangleShape
{
public:
	Square();
	// Конструктор, який приймає позицію, правильне число та об'єкт ResourceHolder.
	Square(sf::Vector2f pos, int _correct_number, ResourceHolder* _rh);
	~Square();
	int correct_number;       // Правильне число
	bool locked;              // Прапорець, що вказує на блокування квадрату
	int entered_number;       // Введене число
	// Оновлює квадрат. Параметр manual_mode вказує на ручний режим, game_mode - на режим гри.
	void update(bool manual_mode, int game_mode);
	// Малює квадрат на вікні sf::RenderWindow.
	void draw(sf::RenderWindow& window);

private:
	sf::Text text; // Текст на квадраті
	ResourceHolder* rh; // Об'єкт ResourceHolder
};

