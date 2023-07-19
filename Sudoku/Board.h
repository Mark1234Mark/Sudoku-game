#pragma once
#include <vector>
#include <queue>
#include <algorithm>
#include <utility>
#include <SFML/Graphics.hpp>
#include "Square.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <random>

class Screen_1;
// Клас Board представляє ігрову дошку для гри.
class Board
{
public:
	Board();// Конструктор за замовчуванням.
	// Конструктор, який приймає об'єкт ResourceHolder і вікно sf::RenderWindow
	Board(ResourceHolder* _rh, sf::RenderWindow& window); 
	~Board();// Деструктор
	// Скидає стан дошки, за необхідності очікує на сигнал про завершення.
	void reset(std::atomic<bool>* done = nullptr);
	// Малює дошку на вікні sf::RenderWindow.
	void draw(sf::RenderWindow& window);
	void remove_numbers();// Видаляє числа з дошки
	void update_squares();// Оновлює квадрати на дошці
	// Обробляє клік миші на вікні sf::RenderWindow за позицією pos
	bool handle_click(sf::RenderWindow& window, sf::Vector2i pos);
	void handle_input(int num);// Обробляє введення числа



private:
	void fill_grid(); // Заповнює сітку числами
	int intRand(const int & min, const int & max);// Повертає випадкове ціле число у заданому діапазоні [min, max]
	void create_squares();// Створює квадрати на дошці
	void init_board();// Ініціалізує дошку
	void generate();// Генерує числа на дошці
	// Повертає допустиме число для заданої позиції (рядок, стовпець)
	int get_valid_number(int row, int col);
	// Перевіряє, чи є розв'язок дошки вгору
	bool upward_sol(std::vector<std::vector<int>>* g); 
	bool downward_sol(std::vector<std::vector<int>>* g);// теж саме, але вниз
	// Перевіряє, чи є число допустимим для заданої позиції (рядок, стовпець) у векторі-сітці g.
	bool check_valid_number(int num, int row, int col, std::vector<std::vector<int>>* g);
	// Перевіряє, чи є число допустимим для заданої позиції (рядок, стовпець) в двовимірному векторі-сітці g.
	bool check_valid_number(int num, int row, int col, std::vector<std::vector<int>> g);
	// Перевіряє, чи рівні два вектори-сітки v1 і v2.
	bool equal_sol(std::vector<std::vector<int>>* v1, std::vector<std::vector<int>>* v2);
	bool has_empty(std::vector<std::vector<int>>* g); // Перевіряє, чи є порожня позиція на дошці
	// Перевіряє допустимість введених чисел
	bool check_valid_inputs();
	// Розв'язок
	bool solve();

	Square* clicked_square;// Квадрат, на який було клікнуто мишею
	// Повертає чергу з координатами (рядок, стовпець)
	std::queue<std::pair<int, int>> get_cord_queue();
	std::vector<std::vector<int>> grid;// Сітка чисел на дошці
	std::vector <std::vector<Square>> squares;// Вектор з квадратами на дошці
	
	ResourceHolder* rh;// Об'єкт ResourceHolder
	sf::Image board_img;// Зображення дошки
	sf::Texture board_texture;// Текстура дошки
	sf::Sprite board_sprite;
	std::vector<sf::Text> numbers;// Вектор з текстовими числами

	bool manual_mode = false; // Прапорець, що вказує на ручний режим гри
	// Скидає стан дошки і малює екран завантаження на вікні sf::RenderWindow
	void reset_and_draw_load(sf::RenderWindow& window);
	int current_tex = 0; // Поточна текстура
	int game_mode=1; // Режим гри за замовчуванням
};

