#include "Ruller.h"

Ruller::Ruller()
{
	iterations = 0;
	mode = 1;
	inpath = "";
	outpath = "";
	rules = "";
	name = "";
    width = 118;
    height = 27;
}

int Ruller::read_argv(int argc, char* argv[])
{
    if (argc <= 2) {
        if (argc == 2) {
            inpath = argv[1];
            mode = 0;
            return 0; //from file
        }
        else {
            if (argc == 1) {
                mode = 1;
                return 1; //online (without anything)
            }
            else {
                std::cout << "Invalid arguments (Needed input file first)" << std::endl;
                mode = -1;
                return -1;
            }
        }

    }
    else {
        inpath = argv[1];
        int i = 2;
        while (argv[i] != nullptr) {
            if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--iterations") == 0) {
                i++;
                iterations = std::stoi(argv[i]);
            }
            if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
                i++;
                outpath = argv[i];
                if (outpath.empty()) {
                    std::cout << "Invalid arguments (Argument after -o must be string)" << std::endl;
                    mode = -1;
                    return -1; //error
                }
            }
            i++;
        }
        mode = 2;
        return 2; //offline
    }
}

std::list<std::pair<int, int>> Ruller::read_file()
{
    std::ifstream input(inpath);
    std::string format;
    int x, y;
    std::list<std::pair<int, int>> life_list;
    if (input.is_open())
    {
        std::getline(input, format);
        if (format == "#Life 1.06") {
            std::getline(input, name);
            std::getline(input, rules);
            input >> width >> height;
            while (input >> x >> y) {
                life_list.push_back(std::make_pair(x, y));
            }
        }
        else {
            std::cout << "Invalid format of universe" << std::endl;
        }
    }
    input.close();
    return life_list;
}

void Ruller::start_online_game(Board* board)
{
    bool flag = false;
    while (!flag) {
        board->show_board();
        board->step();
        char key = 0;
        if (_kbhit()) {
            key = _getch();
            if (key == '`') {
                flag = true;
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        system("cls");
    }
}

void Ruller::start_offline_game(Board* board)
{
    for (unsigned int i = 0; i < iterations; i++) {
        board->step();
    }
}

void Ruller::start_game_from_file(Board* board)
{
    bool flag = false;
    while (!flag) {
        board->show_board();
        board->step();
        char key = 0;
        if (_kbhit()) {
            key = _getch();
            if (key == '`') {
                flag = true;
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        system("cls");
    }
}

void Ruller::start()
{
    switch (mode)
    {
    case 0: {
        life_list = read_file();
        Board board(width, height, rules);
        board.fill_from_file(life_list);
        start_game_from_file(&board);
        func_reader(&board);
        break;
    }
    case 1: {
        name = "#N Glider";
        rules = "#R B3/S23";
        Board board;
        board.fill_glider();
        start_online_game(&board);
        func_reader(&board);
        break;
    }
    case 2: {
        life_list = read_file();
        Board board(width, height, rules);
        board.fill_from_file(life_list);
        start_offline_game(&board);
        board.save_universe(outpath, name, rules);
        break;
    }
    case -1:
        break;
    default:
        break;
    }
}

void Ruller::func_reader(Board* board)
{
    bool flag = false;
    while (!flag) {
        std::cout << "Input comand:\n";
        std::string comand;
        std::getline(std::cin, comand);
        if (comand == "help" || comand == "?") {
            std::cout << '\n';
            std::cout << "List of comands:" << '\n';
            std::cout << "help - print list of comands and it means" << '\n';
            std::cout << "dump <file_name> - save universe in format Life 1.06" << '\n';
            std::cout << "tick <n=1> è t <n=1> - compute status of universe after n loops" << '\n';
            std::cout << "  and print the result's universe with it name, rules and loop number" << '\n';
            std::cout << "exit - quit the game" << '\n';
            std::cout << '\n';
        }
        else if (comand == "dump") {
            std::cout << "Input output file name" << '\n';
            std::string filename;
            std::getline(std::cin, filename);
            board->save_universe(filename, name, rules);
        }
        else if (comand.substr(0, 5) == "dump ") {
            std::string filename = comand.substr(5);
            board->save_universe(filename, name, rules);
        }
        else if (comand == "exit") {
            flag = true;
        }
        else if (comand == "t" || comand == "tick") {
            board->step();
            std::cout << name << '\n' << rules << '\n' << board->get_number_loops() << '\n';
            board->show_board();
        }
        else if (comand.substr(0, 2) == "t ") {
            std::string temp = comand.substr(2);
            std::stringstream ss(temp);
            int loops;
            ss >> loops;
            for (unsigned int i = 0; i < loops; i++) {
                board->step();
            }
            std::cout << name << '\n' << rules << '\n' << board->get_number_loops() << '\n';
            board->show_board();
        }
        else if (comand.substr(0, 5) == "tick ") {
            std::string temp = comand.substr(5);
            std::stringstream ss(temp);
            int loops;
            ss >> loops;
            for (unsigned int i = 0; i < loops; i++) {
                board->step();
            }
            std::cout << name << '\n' << rules << '\n' << board->get_number_loops() << '\n';
            board->show_board();
        }
        else {
            std::cout << "Incorrect comand. Input \"help\" for getting list of commands" << '\n';
        }
    }
}
