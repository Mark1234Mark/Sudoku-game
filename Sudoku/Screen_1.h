#pragma once
#include "cScreen.h"
#include "Board.h"

class Screen_1 : public cScreen
{
public:
	Screen_1(ResourceHolder* _rh, sf::RenderWindow& window);
	~Screen_1();

	Board board;        // Об'єкт дошки
	ResourceHolder* rh; // Об'єкт ResourceHolder	
	int Run(sf::RenderWindow& window);// Запускає екран.
	// Обробляє клік миші на вікні sf::RenderWindow за позицією m_pos.
	void handle_mouseclick(sf::RenderWindow& window, sf::Vector2i m_pos);
};

