#include "Screen_1.h"

//���� ���

// ������ �������� �� ResourceHolder �� ��������� �� ����
Screen_1::Screen_1(ResourceHolder* _rh, sf::RenderWindow& window) : rh(_rh), board(_rh, window)
{
}
// ����������
Screen_1::~Screen_1()
{
}

// ³������ �� ������� ��䳿 ���� ����
void Screen_1::handle_mouseclick(sf::RenderWindow& window , sf::Vector2i m_pos)
{
	if (board.handle_click(window, m_pos)) return;
}

// �������� �����, �������������� �� ������� ��
int Screen_1::Run(sf::RenderWindow& window)
{
    while (window.isOpen()) // ������ ����� ������ �� �����
    {
        sf::Vector2i m_pos = sf::Mouse::getPosition(window); // ������� �����
        sf::Event event;

        while (window.pollEvent(event)) // ��䳿
        {
            
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left) // ������ ���� ������ ����
                {
                    handle_mouseclick(window, m_pos);
                }
            }
            if (event.type == sf::Event::KeyPressed)
            {
                switch (event.key.code)
                {
                case sf::Keyboard::Numpad1:// ���� ��� ����� 1
                case sf::Keyboard::Num1:   // � ������ � ��������� �� Numpad
                    board.handle_input(1); // �������� ������ 1
                    break;
                case sf::Keyboard::Numpad2: // ���� ��� ����� 2
                case sf::Keyboard::Num2:    // � ������ � ��������� �� Numpad
                    board.handle_input(2); // �������� ������ 2
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
                case sf::Keyboard::Escape: // ���� ��� ������ ESC
                    window.close();         // ������� ���� ��������
                    return 0;
                }
            }
        }
        board.update_squares(); // ��������� 

        window.clear(rh->background_color); //�������� ����
        board.draw(window); // ³���������� ����
        for (auto b : rh->buttons) window.draw(b); // ³���������� ��� ������ � ����
        for (auto t : rh->texts) window.draw(t); // ������� ��������
        window.display(); // ³���������� ��� ������������ ��������
    }
    return 1;
}
