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
// ���� Board ����������� ������ ����� ��� ���.
class Board
{
public:
	Board();// ����������� �� �������������.
	// �����������, ���� ������ ��'��� ResourceHolder � ���� sf::RenderWindow
	Board(ResourceHolder* _rh, sf::RenderWindow& window); 
	~Board();// ����������
	// ����� ���� �����, �� ����������� ����� �� ������ ��� ����������.
	void reset(std::atomic<bool>* done = nullptr);
	// ����� ����� �� ���� sf::RenderWindow.
	void draw(sf::RenderWindow& window);
	void remove_numbers();// ������� ����� � �����
	void update_squares();// ������� �������� �� �����
	// �������� ��� ���� �� ���� sf::RenderWindow �� �������� pos
	bool handle_click(sf::RenderWindow& window, sf::Vector2i pos);
	void handle_input(int num);// �������� �������� �����



private:
	void fill_grid(); // �������� ���� �������
	int intRand(const int & min, const int & max);// ������� ��������� ���� ����� � �������� �������� [min, max]
	void create_squares();// ������� �������� �� �����
	void init_board();// ��������� �����
	void generate();// ������ ����� �� �����
	// ������� ��������� ����� ��� ������ ������� (�����, ��������)
	int get_valid_number(int row, int col);
	// ��������, �� � ����'���� ����� �����
	bool upward_sol(std::vector<std::vector<int>>* g); 
	bool downward_sol(std::vector<std::vector<int>>* g);// ��� ����, ��� ����
	// ��������, �� � ����� ���������� ��� ������ ������� (�����, ��������) � ������-���� g.
	bool check_valid_number(int num, int row, int col, std::vector<std::vector<int>>* g);
	// ��������, �� � ����� ���������� ��� ������ ������� (�����, ��������) � ����������� ������-���� g.
	bool check_valid_number(int num, int row, int col, std::vector<std::vector<int>> g);
	// ��������, �� ���� ��� �������-���� v1 � v2.
	bool equal_sol(std::vector<std::vector<int>>* v1, std::vector<std::vector<int>>* v2);
	bool has_empty(std::vector<std::vector<int>>* g); // ��������, �� � ������� ������� �� �����
	// �������� ����������� �������� �����
	bool check_valid_inputs();
	// ����'����
	bool solve();

	Square* clicked_square;// �������, �� ���� ���� ������� �����
	// ������� ����� � ������������ (�����, ��������)
	std::queue<std::pair<int, int>> get_cord_queue();
	std::vector<std::vector<int>> grid;// ѳ��� ����� �� �����
	std::vector <std::vector<Square>> squares;// ������ � ���������� �� �����
	
	ResourceHolder* rh;// ��'��� ResourceHolder
	sf::Image board_img;// ���������� �����
	sf::Texture board_texture;// �������� �����
	sf::Sprite board_sprite;
	std::vector<sf::Text> numbers;// ������ � ���������� �������

	bool manual_mode = false; // ���������, �� ����� �� ������ ����� ���
	// ����� ���� ����� � ����� ����� ������������ �� ���� sf::RenderWindow
	void reset_and_draw_load(sf::RenderWindow& window);
	int current_tex = 0; // ������� ��������
	int game_mode=1; // ����� ��� �� �������������
};

