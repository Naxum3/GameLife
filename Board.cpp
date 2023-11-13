#include "Board.h"

Board::Board()
{
	parse_rules("#R B3/S23");
	for (int i = 0; i < 9; i++) {
		if (forbirth[i] == 1)
			count_b++;
		if (forsurvival[i] == 1)
			count_s++;
	}
	width = 118;
	height = 27;
	number_iteration = 0;
	field = new int* [width];
	for (int i = 0; i < width; i++) {
		field[i] = new int[height];
	}
	str_div = get_str();
}

Board::Board(int widthn, int heightn, std::string rul)
{
	parse_rules(rul);
	for (int i = 0; i < 9; i++) {
		if (forbirth[i] == 1)
			count_b++;
		if (forsurvival[i] == 1)
			count_s++;
	}
	width = abs(widthn);
	number_iteration = 0;
	height = abs(heightn);
	field = new int* [width];
	for (int i = 0; i < width; i++) {
		field[i] = new int[height];
	}
	str_div = get_str();
}

void Board::parse_rules(std::string rul)
{
	int i = 4;
	while (rul[i] != '/') {
		forbirth[rul[i]-'0'] = 1;
		i++;
	}
	i += 2;
	while (rul[i] != '\0') {
		forsurvival[rul[i]-'0'] = 1;
		i++;
	}
}

void Board::fill_glider()
{
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			field[i][j] = 0;
		}
	}
	field[0][2] = 1;
	field[1][0] = 1;
	field[1][2] = 1;
	field[2][1] = 1;
	field[2][2] = 1;
}

void Board::fill_from_file(std::list<std::pair<int, int>> life_list)
{
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			field[i][j] = 0;
		}
	}
	for (auto& elem : life_list) {
		field[abs(elem.first)][abs(elem.second)] = 1;
	}
}

void Board::show_board()
{
	unsigned char life = '#';
	unsigned char dead = ' ';
	std::cout << str_div << std::endl;
	for (int i = 0; i < height; i++) {
		std::cout << "|";
		for (int j = 0; j < width; j++) {
			if (field[j][i]) {
				std::cout << life;
			}
			else {
				std::cout << dead;
			}
		}
		std::cout << "|";
		std::cout << std::endl;
	}
	std::cout << str_div << std::endl;
}

void Board::copy(Board dst)
{
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			dst.field[j][i] = field[j][i];
		}
	}
	dst.number_iteration = number_iteration;
}

void Board::step()
{
	Board prev;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			prev.field[j][i] = field[j][i];
		}
	}
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int count_neib = 0;
			count_neib += prev.field[get_x(i - 1)][get_y(j - 1)];
			count_neib += prev.field[get_x(i)][get_y(j - 1)];
			count_neib += prev.field[get_x(i + 1)][get_y(j - 1)];
			count_neib += prev.field[get_x(i - 1)][get_y(j)];
			count_neib += prev.field[get_x(i + 1)][get_y(j)];
			count_neib += prev.field[get_x(i - 1)][get_y(j + 1)];
			count_neib += prev.field[get_x(i)][get_y(j + 1)];
			count_neib += prev.field[get_x(i + 1)][get_y(j + 1)];

			for (int k = 0; k < count_b; k++) {
				for (int l = 0; l < 9; l++) {
					if (forbirth[l] == 1) {
						if (prev.field[i][j] == 0 && count_neib == l) {
							field[i][j] = 1;
						}
						else {
							field[i][j] = 0;
						}
					}
				}
			}
			for (int k = 0; k < count_s; k++) {
				for (int l = 0; l < 9; l++) {
					if (prev.field[i][j] == 1 && forsurvival[l] == 0 && count_neib == l) {
						field[i][j] = 0;
					}
					else if (prev.field[i][j] == 1 && forsurvival[l] == 1 && count_neib == l) {
						field[i][j] = 1;
						break;
					}
				}
			}
		}
	}
	number_iteration++;
}

void Board::save_universe(std::string outpath, std::string name, std::string rules)
{
	std::ofstream out(outpath);
	out << "Life 1.06" << '\n' << name << '\n' << rules << '\n' << width << " " << height;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (field[i][j] == 1) {
				out << '\n' << i << " " << j;
			}
		}
	}
}

std::string Board::get_str()
{
	std::string str;
	for (int j = 0; j < width + 2; j++) {
		if (j == 0 || j == width + 1) {
			str.append("+");
		}
		else {
			str.append("-");
		}
	}
	return str;
}

int Board::get_x(int i)
{
	return (width + i) % width;
}

int Board::get_y(int i)
{
	return (height + i) % height;
}

int Board::get_width()
{
	return width;
}

int Board::get_height()
{
	return height;
}

void Board::print_rules()
{
	std::cout << "birth" << std::endl;
	for (int i = 0; i < 9; i++) {
		std::cout << i << ": " << forbirth[i] << std::endl;
	}
	std::cout << "survival" << std::endl;
	for (int i = 0; i < 9; i++) {
		std::cout << i << ": " << forsurvival[i] << std::endl;
	}
}

int Board::get_number_loops()
{
	return number_iteration;
}
