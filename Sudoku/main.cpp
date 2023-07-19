#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "Screen_1.h"
#include <ctime>
#include "ResourceHolder.h"


int main()
{
	// ��������� ��'���� ResourceHolder ��� ����������� ������� ���
	ResourceHolder* rh = new ResourceHolder(); 
	// ��������� ���� ��� ������� 1600x1200 ������ � ������ "Sudoku"
	sf::RenderWindow window(sf::VideoMode(1600, 1200), "Sudoku"); 
	// ������������ ������ ���� � ������� �� ���������� ������ � ��'���� ResourceHolder
	window.setIcon(rh->icon.getSize().x, rh->icon.getSize().y, rh->icon.getPixelsPtr()); 
	std::vector<cScreen*> Screens; // ������ ��� ��������� ��'���� ������
	int screen = 0;
	// ��������� ������� ��'���� ������ (Screen_1) � ��������� ���� �� �������
	cScreen* s0 = new Screen_1(rh, window);
	Screens.push_back(s0);
	while (screen <= 0)
	{
		// ������ ��������� ������ � ��������� ������ ������� ������
		screen = Screens[screen]->Run(window); 
		if (screen == 0) break; // ���� ������ ������� 0, ����� � ����� � ��������� ���
		if (screen == 2)
		{
			delete Screens[0]; // ��������� ������� ������ � ���'��
			cScreen* s0 = new Screen_1(rh, window); // ��������� ������ ��'���� ������� ������
			Screens.push_back(s0); // ��������� ������ ������ �� �������
			screen = 0; // ������������ ������� ������ �� ������ �����
		}
	}
	return 0; 
}

