#include "Board.h"
#include "Screen_1.h"

Board::Board(){}// конструктор без параметрів

// Окно
Board::Board(ResourceHolder* _rh, sf::RenderWindow& window): 
	rh(_rh), grid(_rh->grid_size, std::vector<int>(_rh->grid_size)), 
	squares(_rh->grid_size, std::vector<Square>(_rh->grid_size))
{
	reset_and_draw_load(window);
}

Board::~Board()
{
}

// Головна кнопка перезапуску
void Board::reset_and_draw_load(sf::RenderWindow& window)
{
	std::atomic<bool> done(false); // змінна, яка використовується для позначення завершення операції
	std::thread load(&Board::reset, this, &done);// Створення потоку, який виконує функцію reset

	while (!done)
	{
		// Оновлення текстури анімації завантаження
		rh->load_sprite.setTextureRect(sf::IntRect((rh->load_texture.getSize().x*current_tex) / rh->num_of_anims, 0, rh->load_texture.getSize().x / rh->num_of_anims, rh->load_texture.getSize().y));

		window.clear(rh->background_color); // Очищення вікна
		draw(window);// Відображення гри
		for (auto b : rh->buttons) window.draw(b); // Відображення кнопок
		for (auto t : rh->texts) window.draw(t); // Відображення текстів

		window.draw(rh->load_sprite); // Відображення анімації завантаження
		window.display();// Оновлення вікна

		current_tex++;// Зміна кадру анімації
		current_tex %= rh->num_of_anims; // Зациклення кадру анімації
		std::this_thread::sleep_for(std::chrono::milliseconds(30)); // Пауза на 30 мілісекунд
	}
	window.display(); // Оновлення вікна
	load.join(); // Очікування завершення потоку load
}

// Скидання дошки
void Board::reset(std::atomic<bool>* done)
{
	init_board(); // Ініціалізація дошки та комірок
	generate(); // Генерація нового судоку
	update_squares(); // Оновлення квадратів
	if(done != nullptr) *done = true; // Скидання операції
}

// Малювання зображення дошки та встановлення його позиції
void Board::init_board()
{
	board_img.create(rh->board_size, rh->board_size); // Створення великой рамки
	for (int i = 0; i < rh->board_size; i++)
	{
		for (int j = 0; j < rh->board_size; j++)
		{
			// Якщо піксель знаходиться в границях для великої рамки - чорний колір
			if (i < 0 + rh->big_border_width || i > rh->board_size - rh->big_border_width
			 || j < 0 + rh->big_border_width || j > rh->board_size - rh->big_border_width
			 || (i > 300 - rh->big_border_width/2 && i < 300 + rh->big_border_width/2)
			 || (i > 600 - rh->big_border_width / 2 && i < 600 + rh->big_border_width / 2)
			 || (j > 300 - rh->big_border_width / 2 && j < 300 + rh->big_border_width / 2)
			 || (j > 600 - rh->big_border_width / 2 && j < 600 + rh->big_border_width / 2))
				board_img.setPixel(i, j, sf::Color(0x000000ff));
			else if (i % 100 == 0 || j % 100 == 0) // Якщо піксель ділиться на 100 без остачі - сірий колір
			{
				board_img.setPixel(i, j, sf::Color(0xC2C5CCFF));
			}
			else board_img.setPixel(i, j, sf::Color(0xFFFFFFFF));  // В іншому випадку - білий колір
		}
	}

	board_texture.loadFromImage(board_img); // Завантаження зображення 
	board_sprite.setTexture(board_texture); // Завантаження текстури
	board_sprite.setPosition(rh->board_offset_x, rh->board_offset_y); // Встановлення позиції
}

//Генерація нового судоку
void Board::generate()
{
		for (int i = 0; i < grid.size(); i++)
		{
			for (int j = 0; j < grid[i].size(); j++)
			{
				grid[i][j] = 0;  // Установка всіх чисел в сітці рівними нулю
			}
		}

		fill_grid(); // Заповнення сітки правильними числами
		create_squares(); // Створення квадратів
		remove_numbers(); // Видалення чисел

		// Оновлення значень в квадратах після генерації
		update_squares();
}


//Заповнення сітки правильними числами
void Board::fill_grid()
{
	std::vector<std::vector<int>> temp_grid; // Тимчасова сітка
	for (int i = 0; i < grid.size(); i++)
	{
		for (int j = 0; j < grid[i].size(); j++)
		{
			while (grid[i][j] == 0)  // Якщо комірка порожня (=0)
			{
				grid[i][j] = get_valid_number(i, j); // Заповнення правильними числами
				temp_grid = grid;
				if (!upward_sol(&temp_grid)) grid[i][j] = 0; // Перевірка, чи ця сітка є правильним рішенням
			}
		}
	}
}

//повертає правильне число для заповнення комірки сітки
int Board::get_valid_number(int row, int col)
{
	std::vector<int> nums{ 1,2,3,4,5,6,7,8,9 }; // Вектор з правильними числами
	for (int i = nums.size()-1; i >= 0; i--)
	{
		// Якщо таке число існує - воно видаляється
		if (!check_valid_number(nums[i], row, col, &grid)) nums.erase(nums.begin() + i);
	}
	if (nums.size() == 0) return 0; // Повертається 0, якщо комірка порожня
	return nums[intRand(0, nums.size()-1)]; // В іншому випадку повертається випадкове число з вектора
}

// Перевірка, чи є число правильним при вставці (при автоматичному заповненні)
bool Board::check_valid_number(int num, int row, int col, std::vector<std::vector<int>>* g)
{
	// Перевірка рядків і стовпців
	for (int i = 0; i < rh->grid_size; i++)
	{
		// Якщо число співпадає з іншим числом - повертаємо false
		if (((*g)[row][i] == num) || ((*g)[i][col] == num)) return false;	
	}
	// Перевірка блоку 3х3
	int r = row - row % 3;
	int c = col - col % 3;
	// Якщо яка-небудь клітинка має значення, рівне числу, яке ми хочемо вставити,
	//і вона не знаходиться в поточній позиції - повертаємо false.
	for (int i = r; i < r + 3; i++)
	{
		for (int j = c; j < c + 3; j++)
		{
			if ((*g)[i][j] == num && i != row && j != col) return false;
		}
	}
	return true;
}

// Те саме, що і вище, але використовується для ручного вводу
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

// Перевірка розв'язку, прямий перебір від 1 до 9
bool Board::upward_sol(std::vector<std::vector<int>>* g)
{
	if (!has_empty(g)) return true; // Якщо більше немає порожніх клітинок - розв'язок успішний

	// Рекурсивний виклик функції
	for (int i = 0; i < g->size(); i++)
	{
		for (int j = 0; j < (*g)[i].size(); j++)
		{
			if ((*g)[i][j] == 0)
			{
				for (int n = 1; n <= 9; n++)
				{
					if (check_valid_number(n, i, j, g)) // Перевірка правильності числа
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
	return false; // Якщо перебором не вдалося знайти правильний розв'язок - повертаємо false
}

// Те саме, що і вище, але перевірка відбувається в зворотному порядку від 9 до 1
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

// Перевірка, чи є поле порожнім
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

// Перевірка, чи є поля рівними
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

// Видалення чисел по одному випадковим чином і перевірка, чи має розв'язок більше одного
void Board::remove_numbers()
{
	int delCells= 0;

	if (game_mode == 1)
	{
		delCells= 35; // Кількість комірок, які залишаться після видалення у режимі 1
	}
	else if (game_mode == 2)
	{
		delCells= 20; // Кількість комірок, які залишаться після видалення у режимі 2
	}
	else if (game_mode == 3)
	{
		delCells= 15; // Кількість комірок, які залишаться після видалення у режимі 3
	}

	int filledCount = 0;
	std::vector<std::pair<int, int>> filledCells;

	// Заповнення вектору координат всіх заповнених клітинок
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

	// Перемішування вектору випадковим чином
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(filledCells.begin(), filledCells.end(), gen);

	// Видалення чисел з заповнених клітинок, якщо їх кількість перевищує задану
	for (int i = delCells; i < filledCells.size(); ++i)
	{
		int x = filledCells[i].first;
		int y = filledCells[i].second;
		grid[x][y] = 0;
		squares[x][y].locked = false;
	}
}


// Повертає чергу координат для видалення чисел з повного судоку
std::queue<std::pair<int, int>> Board::get_cord_queue()
{
	//Заповнення всіх координат у чергу
	std::deque<std::pair<int, int>> queue;
	for (int i = 0; i < grid.size(); i++)
	{
		for (int j = 0; j < grid[i].size(); j++)
		{
			std::pair <int, int> cord(i, j);
			queue.push_back(cord);
		}
	}
	// Перемішування елементів у черзі, створення звичайної черги і повернення її
	std::random_shuffle(queue.begin(), queue.end());
	std::queue<std::pair<int, int>> q(queue);	
	return q;
}

// Створення та ініціалізація квадратів 
void Board::create_squares()
{
	for (int i = 0; i < grid.size(); i++)
	{
		for (int j = 0; j < grid[i].size(); j++)
		{
			// Створення об'єкта Square для поточної комірки
			Square s(sf::Vector2f(rh->board_offset_x * (i + 1), rh->board_offset_y * (j + 1)), grid[i][j], rh);
			squares[i][j] = s;// Створення об'єкта Square для поточної комірки 
		}
	}
}

// Оновлення значень в об'єктах Square на основі сітки
void Board::update_squares()
{
	// Перебір сітки
	for (int i = 0; i < grid.size(); i++)
	{
		for(int j = 0; j < grid[i].size(); j++)
		{
			// Присвоєння значень комірці
			squares[i][j].entered_number = grid[i][j];
			squares[i][j].update(manual_mode,game_mode); // передача "прапорця" для manual_mode
		}
	}
}
// Малювання дошки та чисел
void Board::draw(sf::RenderWindow& window)
{
	window.draw(board_sprite); // Малювання дошки
	for (auto i : squares) // Перебір кожного об'єкта
	{
		for (auto j : i) j.draw(window); // Виклик функції для малювання
	}
}

// Обробка вводу в комірку
void Board::handle_input(int num)
{
	// Перериває виконання функції, якщо комірка заблокована або не обрана
	if (clicked_square == nullptr || clicked_square->locked)
		return;
	
	for (int i = 0; i < squares.size(); i++)
	{
		for (int j = 0; j < squares[i].size(); j++)
		{
			// Якщо обрана комірка відповідає поточному об'єкту Square, встановити значення в сітці
			if (clicked_square == &squares[i][j])
				grid[i][j] = num;
		}

	}
	update_squares(); // оновлення значень
}

// Обробка кліку мишею та взаємодія з ігровим полем і кнопками
bool Board::handle_click(sf::RenderWindow& window, sf::Vector2i pos)
{
	// Якщо вже обрана комірка, скинути її кольору і скинути покажчик на обрану комірку
	if (clicked_square != nullptr)
	{
		clicked_square->setFillColor(rh->clear_color);
		clicked_square = nullptr;
	}
	for(int i = 0; i < squares.size(); i++)
		for(int j = 0; j < squares[i].size(); j++)
		{
			// Перевірка, чи попадає клік мишею у межі поточного об'єкта Square
			if (squares[i][j].getGlobalBounds().contains(pos.x, pos.y))
			{
				clicked_square = &squares[i][j];
				clicked_square->setFillColor(rh->chosen_square_color);
				return true;
			}
		}


	// Перебір усіх кнопок у векторі buttons
	for (int i = 0; i < rh->buttons.size(); i++)
	{
		// Перевірка, чи попадає клік мишею у межі поточної кнопки
		if (rh->buttons[i].getGlobalBounds().contains(pos.x, pos.y))
		{
			// Обробка кліку на основі індексу кнопки
			switch (i)
			{
			case 0:
			{
				// ручний режим (Manual mode)
				manual_mode = 0;
				reset_and_draw_load(window);
				return true;				
				break;
			}
			case 1:
			{
				return solve(); // Автоматичне рішенн (Solve)
				break;
			}
			case 2:
			{
				// копка Reset
				manual_mode = true;
				reset(nullptr);
				break;
			}
			case 3:
			{
				game_mode = 1; // встановлення режиму гри, в залежності від натиснутої кнопки
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

	return false; // false - якщо натискання не було успішно оброблено
}

// Обработка нажатия кнопки "Solve", вызывается из функции handle_click.
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

// Перевірка правильності введених значень перед примусовим пошуком рішення
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

//Рандомна генерація
int Board::intRand(const int& min, const int& max) {
	static thread_local std::mt19937* generator = nullptr;
	if (!generator) generator = new std::mt19937(clock());
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(*generator);
}
