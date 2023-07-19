#include "Screen_1.h"

//Цикл гри

// Приймає покажчик на ResourceHolder та посилання на вікно
Screen_1::Screen_1(ResourceHolder* _rh, sf::RenderWindow& window) : rh(_rh), board(_rh, window)
{
}
// деструктор
Screen_1::~Screen_1()
{
}

// Відповідає за обробку події кліку миші
void Screen_1::handle_mouseclick(sf::RenderWindow& window , sf::Vector2i m_pos)
{
	if (board.handle_click(window, m_pos)) return;
}

// Головний екран, відмальовування та обробка дій
int Screen_1::Run(sf::RenderWindow& window)
{
    while (window.isOpen()) // змінено умову виходу із цикла
    {
        sf::Vector2i m_pos = sf::Mouse::getPosition(window); // позиція мишки
        sf::Event event;

        while (window.pollEvent(event)) // події
        {
            
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left) // змінено подію кнопки миші
                {
                    handle_mouseclick(window, m_pos);
                }
            }
            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::Numpad1:// подія для цифри 1
                case sf::Keyboard::Num1:   // з вводом з клавіатури та Numpad
                    board.handle_input(1); // заповнює цифрою 1
                    break;
                case sf::Keyboard::Numpad2: // подія для цифри 2
                case sf::Keyboard::Num2:    // з вводом з клавіатури та Numpad
                    board.handle_input(2); // заповнює цифрою 2
                    break;
                case sf::Keyboard::Numpad3:
                case sf::Keyboard::Num3: 
                    board.handle_input(3);
                    break;
                case sf::Keyboard::Numpad4:
                case sf::Keyboard::Num4: 
                    board.handle_input(4);
                    break;
                case sf::Keyboard::Numpad5:
                case sf::Keyboard::Num5: 
                    board.handle_input(5);
                    break;
                case sf::Keyboard::Numpad6:
                case sf::Keyboard::Num6: 
                    board.handle_input(6);
                    break;
                case sf::Keyboard::Numpad7:
                case sf::Keyboard::Num7:
                    board.handle_input(7);
                    break;
                case sf::Keyboard::Numpad8:
                case sf::Keyboard::Num8: 
                    board.handle_input(8);
                    break;
                case sf::Keyboard::Numpad9:
                case sf::Keyboard::Num9: 
                    board.handle_input(9);
                    break;
                case sf::Keyboard::Numpad0:
                case sf::Keyboard::Num0: 
                    board.handle_input(0);
                    break;
                case sf::Keyboard::Escape: // подія для кнопки ESC
                    window.close();         // закриває вікно програми
                    return 0;
                }
            }
        }
        board.update_squares(); // Оновлення 

        window.clear(rh->background_color); //очищення вікна
        board.draw(window); // Відображення поля
        for (auto b : rh->buttons) window.draw(b); // Відображення всіх кнопок в циклі
        for (auto t : rh->texts) window.draw(t); // текстові елементи
        window.display(); // Відображення всіх відмальованих елементів
    }
    return 1;
}
