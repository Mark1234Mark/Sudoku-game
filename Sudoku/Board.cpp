#include "Board.h"
#include "Screen_1.h"

Board::Board(){}// ����������� ��� ���������

// ����
Board::Board(ResourceHolder* _rh, sf::RenderWindow& window): 
	rh(_rh), grid(_rh->grid_size, std::vector<int>(_rh->grid_size)), 
	squares(_rh->grid_size, std::vector<Square>(_rh->grid_size))
{
	reset_and_draw_load(window);
}

Board::~Board()
{
}

// ������� ������ �����������
void Board::reset_and_draw_load(sf::RenderWindow& window)
{
	std::atomic<bool> done(false); // �����, ��� ��������������� ��� ���������� ���������� ��������
	std::thread load(&Board::reset, this, &done);// ��������� ������, ���� ������ ������� reset

	while (!done)
	{
		// ��������� �������� ������� ������������
		rh->load_sprite.setTextureRect(sf::IntRect((rh->load_texture.getSize().x*current_tex) / rh->num_of_anims, 0, rh->load_texture.getSize().x / rh->num_of_anims, rh->load_texture.getSize().y));

		window.clear(rh->background_color); // �������� ����
		draw(window);// ³���������� ���
		for (auto b : rh->buttons) window.draw(b); // ³���������� ������
		for (auto t : rh->texts) window.draw(t); // ³���������� ������

		window.draw(rh->load_sprite); // ³���������� ������� ������������
		window.display();// ��������� ����

		current_tex++;// ���� ����� �������
		current_tex %= rh->num_of_anims; // ���������� ����� �������
		std::this_thread::sleep_for(std::chrono::milliseconds(30)); // ����� �� 30 ��������
	}
	window.display(); // ��������� ����
	load.join(); // ���������� ���������� ������ load
}

// �������� �����
void Board::reset(std::atomic<bool>* done)
{
	init_board(); // ����������� ����� �� ������
	generate(); // ��������� ������ ������
	update_squares(); // ��������� ��������
	if(done != nullptr) *done = true; // �������� ��������
}

// ��������� ���������� ����� �� ������������ ���� �������
void Board::init_board()
{
	board_img.create(rh->board_size, rh->board_size); // ��������� ������� �����
	for (int i = 0; i < rh->board_size; i++)
	{
		for (int j = 0; j < rh->board_size; j++)
		{
			// ���� ������ ����������� � �������� ��� ������ ����� - ������ ����
			if (i < 0 + rh->big_border_width || i > rh->board_size - rh->big_border_width
			 || j < 0 + rh->big_border_width || j > rh->board_size - rh->big_border_width
			 || (i > 300 - rh->big_border_width/2 && i < 300 + rh->big_border_width/2)
			 || (i > 600 - rh->big_border_width / 2 && i < 600 + rh->big_border_width / 2)
			 || (j > 300 - rh->big_border_width / 2 && j < 300 + rh->big_border_width / 2)
			 || (j > 600 - rh->big_border_width / 2 && j < 600 + rh->big_border_width / 2))
				board_img.setPixel(i, j, sf::Color(0x000000ff));
			else if (i % 100 == 0 || j % 100 == 0) // ���� ������ ������� �� 100 ��� ������ - ���� ����
			{
				board_img.setPixel(i, j, sf::Color(0xC2C5CCFF));
			}
			else board_img.setPixel(i, j, sf::Color(0xFFFFFFFF));  // � ������ ������� - ���� ����
		}
	}

	board_texture.loadFromImage(board_img); // ������������ ���������� 
	board_sprite.setTexture(board_texture); // ������������ ��������
	board_sprite.setPosition(rh->board_offset_x, rh->board_offset_y); // ������������ �������
}

//��������� ������ ������
void Board::generate()
{
		for (int i = 0; i < grid.size(); i++)
		{
			for (int j = 0; j < grid[i].size(); j++)
			{
				grid[i][j] = 0;  // ��������� ��� ����� � ���� ������ ����
			}
		}

		fill_grid(); // ���������� ���� ����������� �������
		create_squares(); // ��������� ��������
		remove_numbers(); // ��������� �����

		// ��������� ������� � ��������� ���� ���������
		update_squares();
}


//���������� ���� ����������� �������
void Board::fill_grid()
{
	std::vector<std::vector<int>> temp_grid; // ��������� ����
	for (int i = 0; i < grid.size(); i++)
	{
		for (int j = 0; j < grid[i].size(); j++)
		{
			while (grid[i][j] == 0)  // ���� ������ ������� (=0)
			{
				grid[i][j] = get_valid_number(i, j); // ���������� ����������� �������
				temp_grid = grid;
				if (!upward_sol(&temp_grid)) grid[i][j] = 0; // ��������, �� �� ���� � ���������� �������
			}
		}
	}
}

//������� ��������� ����� ��� ���������� ������ ����
int Board::get_valid_number(int row, int col)
{
	std::vector<int> nums{ 1,2,3,4,5,6,7,8,9 }; // ������ � ����������� �������
	for (int i = nums.size()-1; i >= 0; i--)
	{
		// ���� ���� ����� ���� - ���� �����������
		if (!check_valid_number(nums[i], row, col, &grid)) nums.erase(nums.begin() + i);
	}
	if (nums.size() == 0) return 0; // ����������� 0, ���� ������ �������
	return nums[intRand(0, nums.size()-1)]; // � ������ ������� ����������� ��������� ����� � �������
}

// ��������, �� � ����� ���������� ��� ������� (��� ������������� ���������)
bool Board::check_valid_number(int num, int row, int col, std::vector<std::vector<int>>* g)
{
	// �������� ����� � ��������
	for (int i = 0; i < rh->grid_size; i++)
	{
		// ���� ����� ������� � ����� ������ - ��������� false
		if (((*g)[row][i] == num) || ((*g)[i][col] == num)) return false;	
	}
	// �������� ����� 3�3
	int r = row - row % 3;
	int c = col - col % 3;
	// ���� ���-������ ������� �� ��������, ���� �����, ��� �� ������ ��������,
	//� ���� �� ����������� � ������� ������� - ��������� false.
	for (int i = r; i < r + 3; i++)
	{
		for (int j = c; j < c + 3; j++)
		{
			if ((*g)[i][j] == num && i != row && j != col) return false;
		}
	}
	return true;
}

// �� ����, �� � ����, ��� ��������������� ��� ������� �����
bool Board::check_valid_number(int num, int row, int col, std::vector<std::vector<int>> g)
{

	for (int i = 0; i < rh->grid_size; i++)
	{
		if ((g[row][i] == num && i != col) || (g[i][col] == num && i != row)) return false;
	}
	int r = row - row % 3;
	int c = col - col % 3;
	for (int i = r; i < r + 3; i++)
	{
		for (int j = c; j < c + 3; j++)
		{
			if (g[i][j] == num && i != row && j != col) return false;
		}
	}
	return true;
}

// �������� ����'����, ������ ������ �� 1 �� 9
bool Board::upward_sol(std::vector<std::vector<int>>* g)
{
	if (!has_empty(g)) return true; // ���� ����� ���� ������� ������� - ����'���� �������

	// ����������� ������ �������
	for (int i = 0; i < g->size(); i++)
	{
		for (int j = 0; j < (*g)[i].size(); j++)
		{
			if ((*g)[i][j] == 0)
			{
				for (int n = 1; n <= 9; n++)
				{
					if (check_valid_number(n, i, j, g)) // �������� ����������� �����
					{
						(*g)[i][j] = n;
						if (upward_sol(g)) return true;
						else
						{
							(*g)[i][j] = 0;
						}
					}

				}
				return false;
			}
		}
	}
	return false; // ���� ��������� �� ������� ������ ���������� ����'���� - ��������� false
}

// �� ����, �� � ����, ��� �������� ���������� � ���������� ������� �� 9 �� 1
bool Board::downward_sol(std::vector<std::vector<int>>* g)
{
	if (!has_empty(g)) return true;
	for (int i = 0; i < g->size(); i++)
	{
		for (int j = 0; j < g->size(); j++)
		{
			if ((*g)[i][j] == 0)
			{
				for (int n = 9; n >= 0; n--)
				{
					if (check_valid_number(n, i, j, g))
					{
						(*g)[i][j] = n;
						if (downward_sol(g)) return true;
						else
						{
							(*g)[i][j] = 0;
						}
					}

				}
				return false;
			}
		}
	}
	return true;
}

// ��������, �� � ���� �������
bool Board::has_empty(std::vector<std::vector<int>>* g)
{
	for (int i = 0; i < g->size(); i++)
	{
		for (int j = 0; j < g->at(i).size(); j++)
		{
			if ((*g)[i][j] == 0) return true;
		}
	}
	return false;
}

// ��������, �� � ���� ������
bool Board::equal_sol(std::vector<std::vector<int>>* v1, std::vector<std::vector<int>>* v2)
{
	for (int i = 0; i < v1->size(); i++)
	{
		for (int j = 0; j < v1->size(); j++)
		{
			if ((*v1)[i][j] != (*v2)[i][j]) return false;
		}
	}
	return true;
}

// ��������� ����� �� ������ ���������� ����� � ��������, �� �� ����'���� ����� ������
void Board::remove_numbers()
{
	int delCells= 0;

	if (game_mode == 1)
	{
		delCells= 35; // ʳ������ ������, �� ���������� ���� ��������� � ����� 1
	}
	else if (game_mode == 2)
	{
		delCells= 20; // ʳ������ ������, �� ���������� ���� ��������� � ����� 2
	}
	else if (game_mode == 3)
	{
		delCells= 15; // ʳ������ ������, �� ���������� ���� ��������� � ����� 3
	}

	int filledCount = 0;
	std::vector<std::pair<int, int>> filledCells;

	// ���������� ������� ��������� ��� ���������� �������
	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			if (grid[i][j] != 0)
			{
				filledCells.push_back(std::make_pair(i, j));
			}
		}
	}

	// ������������ ������� ���������� �����
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(filledCells.begin(), filledCells.end(), gen);

	// ��������� ����� � ���������� �������, ���� �� ������� �������� ������
	for (int i = delCells; i < filledCells.size(); ++i)
	{
		int x = filledCells[i].first;
		int y = filledCells[i].second;
		grid[x][y] = 0;
		squares[x][y].locked = false;
	}
}


// ������� ����� ��������� ��� ��������� ����� � ������� ������
std::queue<std::pair<int, int>> Board::get_cord_queue()
{
	//���������� ��� ��������� � �����
	std::deque<std::pair<int, int>> queue;
	for (int i = 0; i < grid.size(); i++)
	{
		for (int j = 0; j < grid[i].size(); j++)
		{
			std::pair <int, int> cord(i, j);
			queue.push_back(cord);
		}
	}
	// ������������ �������� � ����, ��������� �������� ����� � ���������� ��
	std::random_shuffle(queue.begin(), queue.end());
	std::queue<std::pair<int, int>> q(queue);	
	return q;
}

// ��������� �� ����������� �������� 
void Board::create_squares()
{
	for (int i = 0; i < grid.size(); i++)
	{
		for (int j = 0; j < grid[i].size(); j++)
		{
			// ��������� ��'���� Square ��� ������� ������
			Square s(sf::Vector2f(rh->board_offset_x * (i + 1), rh->board_offset_y * (j + 1)), grid[i][j], rh);
			squares[i][j] = s;// ��������� ��'���� Square ��� ������� ������ 
		}
	}
}

// ��������� ������� � ��'����� Square �� ����� ����
void Board::update_squares()
{
	// ������ ����
	for (int i = 0; i < grid.size(); i++)
	{
		for(int j = 0; j < grid[i].size(); j++)
		{
			// ��������� ������� ������
			squares[i][j].entered_number = grid[i][j];
			squares[i][j].update(manual_mode,game_mode); // �������� "��������" ��� manual_mode
		}
	}
}
// ��������� ����� �� �����
void Board::draw(sf::RenderWindow& window)
{
	window.draw(board_sprite); // ��������� �����
	for (auto i : squares) // ������ ������� ��'����
	{
		for (auto j : i) j.draw(window); // ������ ������� ��� ���������
	}
}

// ������� ����� � ������
void Board::handle_input(int num)
{
	// �������� ��������� �������, ���� ������ ����������� ��� �� ������
	if (clicked_square == nullptr || clicked_square->locked)
		return;
	
	for (int i = 0; i < squares.size(); i++)
	{
		for (int j = 0; j < squares[i].size(); j++)
		{
			// ���� ������ ������ ������� ��������� ��'���� Square, ���������� �������� � ����
			if (clicked_square == &squares[i][j])
				grid[i][j] = num;
		}

	}
	update_squares(); // ��������� �������
}

// ������� ���� ����� �� ������� � ������� ����� � ��������
bool Board::handle_click(sf::RenderWindow& window, sf::Vector2i pos)
{
	// ���� ��� ������ ������, ������� �� ������� � ������� �������� �� ������ ������
	if (clicked_square != nullptr)
	{
		clicked_square->setFillColor(rh->clear_color);
		clicked_square = nullptr;
	}
	for(int i = 0; i < squares.size(); i++)
		for(int j = 0; j < squares[i].size(); j++)
		{
			// ��������, �� ������ ��� ����� � ��� ��������� ��'���� Square
			if (squares[i][j].getGlobalBounds().contains(pos.x, pos.y))
			{
				clicked_square = &squares[i][j];
				clicked_square->setFillColor(rh->chosen_square_color);
				return true;
			}
		}


	// ������ ��� ������ � ������ buttons
	for (int i = 0; i < rh->buttons.size(); i++)
	{
		// ��������, �� ������ ��� ����� � ��� ������� ������
		if (rh->buttons[i].getGlobalBounds().contains(pos.x, pos.y))
		{
			// ������� ���� �� ����� ������� ������
			switch (i)
			{
			case 0:
			{
				// ������ ����� (Manual mode)
				manual_mode = 0;
				reset_and_draw_load(window);
				return true;				
				break;
			}
			case 1:
			{
				return solve(); // ����������� ����� (Solve)
				break;
			}
			case 2:
			{
				// ����� Reset
				manual_mode = true;
				reset(nullptr);
				break;
			}
			case 3:
			{
				game_mode = 1; // ������������ ������ ���, � ��������� �� ��������� ������
				break;
			}
			case 4:
			{
				game_mode = 2;
				break;
			}
			case 5:
			{
				game_mode = 3;
				break;
			}
			}
		}
	}

	return false; // false - ���� ���������� �� ���� ������ ���������
}

// ��������� ������� ������ "Solve", ���������� �� ������� handle_click.
bool Board::solve()
{

		for (int i = 0; i < squares.size(); i++)
		{
			for (int j = 0; j < squares[i].size(); j++)
			{
				squares[i][j].entered_number = squares[i][j].correct_number;
				grid[i][j] = squares[i][j].correct_number;
			}
		}
		update_squares();
		return true;

}

// �������� ����������� �������� ������� ����� ���������� ������� ������
bool Board::check_valid_inputs()
{
	for (int i = 0; i < grid.size(); i++)
	{
		for (int j = 0; j < grid[i].size(); j++)
		{
			if (!check_valid_number(grid[i][j], i, j, grid))
			{
				return false;
			}
				
		}
	}
	return true;
}

//�������� ���������
int Board::intRand(const int& min, const int& max) {
	static thread_local std::mt19937* generator = nullptr;
	if (!generator) generator = new std::mt19937(clock());
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(*generator);
}
