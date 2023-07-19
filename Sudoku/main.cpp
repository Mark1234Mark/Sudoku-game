#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "Screen_1.h"
#include <ctime>
#include "ResourceHolder.h"


int main()
{
	// Створення об'єкта ResourceHolder для утримування ресурсів гри
	ResourceHolder* rh = new ResourceHolder(); 
	// Створення вікна гри розміром 1600x1200 пікселів з назвою "Sudoku"
	sf::RenderWindow window(sf::VideoMode(1600, 1200), "Sudoku"); 
	// Встановлення іконки вікна з розміром та піксельними даними з об'єкта ResourceHolder
	window.setIcon(rh->icon.getSize().x, rh->icon.getSize().y, rh->icon.getPixelsPtr()); 
	std::vector<cScreen*> Screens; // Вектор для зберігання об'єктів екранів
	int screen = 0;
	// Створення першого об'єкта екрана (Screen_1) і додавання його до вектору
	cScreen* s0 = new Screen_1(rh, window);
	Screens.push_back(s0);
	while (screen <= 0)
	{
		// Запуск поточного екрана і отримання нового індексу екрана
		screen = Screens[screen]->Run(window); 
		if (screen == 0) break; // Якщо індекс дорівнює 0, вийти з циклу і завершити гру
		if (screen == 2)
		{
			delete Screens[0]; // Видалення першого екрана з пам'яті
			cScreen* s0 = new Screen_1(rh, window); // Створення нового об'єкта першого екрана
			Screens.push_back(s0); // Додавання нового екрана до вектору
			screen = 0; // Встановлення індексу екрана на перший екран
		}
	}
	return 0; 
}

