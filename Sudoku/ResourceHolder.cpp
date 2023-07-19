#include "ResourceHolder.h"

ResourceHolder::ResourceHolder() :
    // Встановлення кольору фону в шістнадцятковому форматі ARGB
    background_color(0x536878FF), chosen_square_color(0x708090AA),
    red_color(0xFF0000FF), clear_color(0xFFFFFF00), black_color(0x000000FF), blue_color(0x0000FFFF),
    button_color(0xf99820FF), button_line_color(0x000000FF)
{
        // Загрузка шрифтів
    number_font.loadFromFile("fonts/SummitLight-Display.ttf"); 
    bohemian_font.loadFromFile("fonts/Bohemian_Typewriter.ttf");

    // Ініціалізація заголовка
    sf::Text headline("Sudoku", bohemian_font, 100); // Створення тексту "Sudoku", з заданим шрифтом та розміром
    headline.setFillColor(sf::Color::Black); // Встановлення кольору тексту на чорний
    headline.setStyle(sf::Text::Bold); // Встановлення жирного стилю тексту
    sf::FloatRect textRect = headline.getLocalBounds(); // Отримання обмежуючого прямокутника тексту
    // Встановлення центру тексту як його початку
    headline.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);
    // Встановлення позиції тексту
    headline.setPosition(sf::Vector2f((s_width - (s_width - board_size - board_offset_x) / 2), 150));
    texts.push_back(headline);// Додавання тексту до вектору текстів

    // Ініціалізація кнопки "Generate New""
    sf::RectangleShape new_button(sf::Vector2f(button_width, button_height));
    new_button.setOrigin(button_width / 2, button_height / 2);
    new_button.setFillColor(button_color);
    new_button.setOutlineColor(button_line_color);
    new_button.setOutlineThickness(button_outline_thickness);
    new_button.setPosition(sf::Vector2f(s_width - (s_width - board_size - board_offset_x) / 2, 400));
    buttons.push_back(new_button);// Додаємо кнопку в список 
    sf::Text new_game_text("Generate New", bohemian_font, button_text_size);
    new_game_text.setFillColor(black_color);
    sf::FloatRect new_rectBounds = new_button.getGlobalBounds();
    sf::FloatRect new_textBounds = new_game_text.getGlobalBounds();
    new_game_text.setPosition(
        new_rectBounds.left + (new_rectBounds.width / 2) - (new_textBounds.width / 2),
        new_rectBounds.top + (new_rectBounds.height / 2) - (new_textBounds.height / 2));
    texts.push_back(new_game_text);// Додаємо текст кнопки в список текстів

    // Ініціалізація кнопки "Solve"
    sf::RectangleShape solve_button(sf::Vector2f(button_width, button_height));
    solve_button.setOrigin(button_width / 2, button_height / 2);
    solve_button.setFillColor(button_color);
    solve_button.setOutlineColor(button_line_color);
    solve_button.setOutlineThickness(button_outline_thickness);
    solve_button.setPosition(sf::Vector2f(s_width - (s_width - board_size - board_offset_x) / 2, 650));
    buttons.push_back(solve_button);
    sf::Text solve_text("Solve", bohemian_font, button_text_size);
    solve_text.setFillColor(black_color);// Додаємо кнопку в список
    sf::FloatRect solve_rectBounds = solve_button.getGlobalBounds();
    sf::FloatRect solve_textBounds = solve_text.getGlobalBounds();
    solve_text.setPosition(
        solve_rectBounds.left + (solve_rectBounds.width / 2) - (solve_textBounds.width / 2),
        solve_rectBounds.top + (solve_rectBounds.height / 2) - (solve_textBounds.height / 2));
    texts.push_back(solve_text); // Додаємо текст кнопки в список текстів

    // Розміри для менших кнопок
    float small_button_width = button_width / 2;
    float small_button_height = button_height / 2;

    // Створення першої кнопки для вибору складності
    sf::RectangleShape button1(sf::Vector2f(small_button_width, small_button_height));
    button1.setOrigin(small_button_width / 2, small_button_height / 2);
    button1.setFillColor(button_color);
    button1.setOutlineColor(button_line_color);
    button1.setOutlineThickness(button_outline_thickness);
    button1.setPosition(sf::Vector2f(s_width - (s_width - board_size - board_offset_x) / 2 - small_button_width, 900));
    buttons.push_back(button1);
    sf::Text text1("1", bohemian_font, button_text_size);
    text1.setFillColor(black_color);
    sf::FloatRect rectBounds1 = button1.getGlobalBounds();
    sf::FloatRect textBounds1 = text1.getGlobalBounds();
    text1.setPosition(
        rectBounds1.left + (rectBounds1.width / 2) - (textBounds1.width / 2),
        rectBounds1.top + (rectBounds1.height / 2) - (textBounds1.height / 2));
    texts.push_back(text1);

    // Створення другої кнопки для вибору складності
    sf::RectangleShape button2(sf::Vector2f(small_button_width, small_button_height));
    button2.setOrigin(small_button_width / 2, small_button_height / 2);
    button2.setFillColor(button_color);
    button2.setOutlineColor(button_line_color);
    button2.setOutlineThickness(button_outline_thickness);
    button2.setPosition(sf::Vector2f(s_width - (s_width - board_size - board_offset_x) / 2, 900));
    buttons.push_back(button2);
    sf::Text text2("2", bohemian_font, button_text_size);
    text2.setFillColor(black_color);
    sf::FloatRect rectBounds2 = button2.getGlobalBounds();
    sf::FloatRect textBounds2 = text2.getGlobalBounds();
    text2.setPosition(
        rectBounds2.left + (rectBounds2.width / 2) - (textBounds2.width / 2),
        rectBounds2.top + (rectBounds2.height / 2) - (textBounds2.height / 2));
    texts.push_back(text2);

    // Створення третої кнопки для вибору складності
    sf::RectangleShape button3(sf::Vector2f(small_button_width, small_button_height));
    button3.setOrigin(small_button_width / 2, small_button_height / 2);
    button3.setFillColor(button_color);
    button3.setOutlineColor(button_line_color);
    button3.setOutlineThickness(button_outline_thickness);
    button3.setPosition(sf::Vector2f(s_width - (s_width - board_size - board_offset_x) / 2 + small_button_width, 900));
    buttons.push_back(button3);
    sf::Text text3("3", bohemian_font, button_text_size);
    text3.setFillColor(black_color);
    sf::FloatRect rectBounds3 = button3.getGlobalBounds();
    sf::FloatRect textBounds3 = text3.getGlobalBounds();
    text3.setPosition(
        rectBounds3.left + (rectBounds3.width / 2) - (textBounds3.width / 2),
        rectBounds3.top + (rectBounds3.height / 2) - (textBounds3.height / 2));
    texts.push_back(text3);

    // Завантаження текстур та картинок
    load_texture.loadFromFile("img/load_sheet.png");
    load_sprite.setTexture(load_texture);
    load_sprite.setTextureRect(sf::IntRect(0, 0, load_texture.getSize().x / num_of_anims, load_texture.getSize().y));
    load_sprite.setOrigin(load_sprite.getGlobalBounds().width / 2, load_sprite.getGlobalBounds().height / 2);
    load_sprite.setPosition(board_offset_x + board_size / 2, board_offset_y + board_size / 2);

    icon.loadFromFile("img/icon.png");
}

ResourceHolder::~ResourceHolder()
{
}
