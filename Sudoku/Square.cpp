#include "Square.h"

// Клас Square

Square::Square(){} // конструктор без параметрів 

// Конструктор, що приймає параметри: позиція, правильне число та управління ресурсами
Square::Square(sf::Vector2f pos, int _correct_number, ResourceHolder* _rh): 
	correct_number(_correct_number), entered_number(_correct_number), rh(_rh), locked(true)
{
	setPosition(pos); // Встановлення позиції квадратної комірки
	setFillColor(rh->clear_color); // Встановлення кольору заповнення комірки
	setSize(sf::Vector2f(rh->square_pixel_size, rh->square_pixel_size)); // Встановлення розмірів комірки
	text.setFont(rh->number_font); // Встановлення шрифту для тексту в комірці
	text.setCharacterSize(20); // Встановлення розміру символів тексту
	text.setFillColor(rh->black_color); // Встановлення кольору тексту

	auto rect = getGlobalBounds(); // Отримання границі квадратної комірки
	auto textRect = text.getGlobalBounds(); // Отримання границі тексту

	text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f); // Встановлення точки центрування тексту
	// Встановлення позиції тексту в центрі комірки
	text.setPosition({ rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f }); 
}


Square::~Square() //деструктор 
{
}

// Оновлення поля
void Square::update(bool manual_mode, int game_mode)
{
	// Якщо введене число не дорівнює 0, встановити текст введеного числа
	if (entered_number != 0) text.setString(std::to_string(entered_number)); 
	else text.setString(""); // В іншому випадку очистити текст комірки

	//if (manual_mode && !game_mode)
	//{
	//	correct_number = entered_number; // Якщо режим ручного вводу, встановити правильне число рівним введеному числу
	//	text.setFillColor(rh->black_color); // Встановити колір тексту комірки на чорний
	//}
	//else
	//{
		// Якщо введене число співпадає з правильним числом і комірка не заблокована, встановити колір тексту комірки на синій
		if (entered_number == correct_number && !locked) text.setFillColor(rh->blue_color); 
		else if (entered_number != correct_number && !locked) text.setFillColor(rh->blue_color); 
	//}
}

void Square::draw(sf::RenderWindow& window)
{
	window.draw(*this); // Відображення квадратної комірки на заданому вікні
	window.draw(text); // Відображення тексту в комірці на заданому вікні
}