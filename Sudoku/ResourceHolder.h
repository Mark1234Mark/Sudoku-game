#pragma once
#include <SFML/Graphics.hpp>

// Клас для управління ресурсами
class ResourceHolder
{
public:
	ResourceHolder();
	~ResourceHolder();

	int game_mode; // Змінна для режиму гри
	// Константи	
	const int grid_size = 9;  // Розмір сітки
	const int square_pixel_size = 100;  // Розмір квадрату в пікселях
	const int small_border_width = 3;  // Товщина тонкої межі
	const int big_border_width = 6;  // Товщина товстої межі
	const int board_size = 900;  // Розмір дошки
	const int s_width = 1600;  // Ширина вікна
	const int s_height = 1000;  // Висота вікна
	const int board_offset_x = 100;  // Зміщення дошки по осі X
	const int board_offset_y = 100;  // Зміщення дошки по осі Y
	const int button_width = 400;  // Ширина кнопки
	const int button_height = 150;  // Висота кнопки
	const int button_outline_thickness = -5;  // Товщина контуру кнопки
	const int button_text_size = 50;  // Розмір тексту кнопки
	const int num_of_anims = 12;  // Кількість анімацій

	sf::Texture load_texture;  // Текстура для завантаження
	sf::Sprite load_sprite;  // Спрайт для завантаження
	sf::Image icon;  // Зображення іконки

	// Кольори
	sf::Color button_color;  // Колір кнопок
	sf::Color button_line_color;  // Колір контуру кнопок
	sf::Color background_color;  // Колір фону
	sf::Color chosen_square_color;  // Колір виділеного квадрата
	sf::Color red_color;  // Червоний колір
	sf::Color clear_color;  // Прозорий колір
	sf::Color black_color;  // Чорний колір
	sf::Color blue_color;  // Синій колір

	sf::Font bohemian_font;  // Шрифт Bohemian
	sf::Font number_font;  // Шрифт для чисел

	std::vector<sf::Text> texts;  // Список текстів
	std::vector<sf::RectangleShape> buttons;  // Список кнопок
};

