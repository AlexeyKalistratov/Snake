// Snake.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <ctime>    //для srand(time)
#include <iostream> //для cout, rand() и system()
#include <list>     
#include <windows.h>//для функций управления консолью
using namespace std;
const int width = 50;   //размеры экрана и некоторые 
const int height = 25;  //прочие глобальные константы
const char* DOT = "*";
const char* SPACE = " ";
const COORD run_up = { 0, -1 };  //вектора направлений перемещения
const COORD run_down = { 0,  1 };
const COORD run_left = { -1, 0 };
const COORD run_right = { 1, 0 };
const DWORD DELAY_TIME = 166;    //аргумент для Sleep()
//используемые типы
typedef COORD cell;   //тип точек на экране.
typedef list<cell>    tSnake;//тип змейка - это список точек.
//глобальные переменные
tSnake snake;        //главный перс - змейка
HANDLE hStdout;
COORD dir = run_right;  //направление её движения     
cell apple;          //яблоко
COORD pos = { 0,0 };
void put_cell(cell place, const char* symbol) {//вывод символа в заданные координаты
    DWORD written;
    SetConsoleCursorPosition(hStdout, place);
    WriteConsole(hStdout, symbol, 1, &written, NULL);
}
void init_window() {
    system("mode con cols=50 lines=25");
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
}
void init_snake() {  //Змейка - список точек. snake.back это хвост змеи
    for (pos.X = pos.Y = 0; pos.X < 3; pos.X++)//snake.front это её голова
        snake.push_front(pos);//в начале змейка заполнена тремя точками(0,0) (1,0) (2,0)
}
void redraw() {//вывести змейку целиком на экран (обычно она выводится не вся)
    for (tSnake::iterator it = snake.begin(); it != snake.end(); it++)
        put_cell(*it, DOT);
}
void resetApple() {//установка нового яблока в случайные координаты
    bool run = true;
    while (run) {
        run = false;
        apple.X = rand() % (width - 3) + 1;
        apple.Y = rand() % (height - 3) + 1;
        for (tSnake::iterator it = snake.begin(); it != snake.end() && (!run); it++)
            run = (it->X == apple.X && it->Y == apple.Y);//если яблоко упало на змейку,
    }                                              //цикл while запустится снова.
}
bool step_and_die() {
    bool hit, eat;
    pos = snake.back();
    snake.pop_back();//подвинуть хвост
    if (snake.back().X != pos.X || snake.back().Y != pos.Y) put_cell(pos, SPACE);//стереть хвост с экрана, если в нём одна клетка
    pos = snake.front();
    pos.X += dir.X;
    pos.Y += dir.Y;//подвинуть голову
    if (pos.X == -1) pos.X = width - 1;//проверить, проходит ли голова сквозь стену
    if (pos.X == width) pos.X = 0;
    if (pos.Y == -1) pos.Y = height - 1;
    if (pos.Y == height) pos.Y = 0;
    hit = eat = false;//начать цикл проверки столкновений
    for (tSnake::iterator it = snake.begin(); it != snake.end() && (!hit) && (!eat); it++) {
        hit = (it->X == pos.X && it->Y == pos.Y);  //столкновение с самим собой
        eat = (it->X == apple.X && it->Y == apple.Y);//столкновение с яблоком
    }
    snake.push_front(pos);//добавить змейке двинутую бошку
    put_cell(pos, DOT);//нарисовать эту бошку
    if (eat) {//поедание яблока, если попал в него
        resetApple();
        snake.push_front(pos);
    }
    return hit;
}
void finish() {
    pos.X = width / 2 - 5;
    pos.Y = height / 2;
    put_cell(pos, SPACE);
    cout << "GAME OVER!";
    system("pause");
}
void HandleKeyboard() {//обработка клавиатуры
    SHORT keys[4];//сначала проверяем нажатие всех 4х стрелок
    int i, key;   //и запоминаем их состояние в массив keys
    for (i = 0, key = VK_LEFT; i < 4; i++, key++)
        keys[i] = GetAsyncKeyState(key);
    //затем соответственно стрелкам меняем направление змейки
    if (dir.Y == 0) {//если змейка движется горизонтально, то на вправо-влево не реагирууем
        if (keys[VK_UP - VK_LEFT]) dir = run_up;
        if (keys[VK_DOWN - VK_LEFT]) dir = run_down;
    }
    else {//и соответственно наоборот, если змейка вертикально, то на вверх-низ не реагирууем
        if (keys[VK_LEFT - VK_LEFT]) dir = run_left;
        if (keys[VK_RIGHT - VK_LEFT]) dir = run_right;
    }
}
int main() {//точка входа.
    srand(time(NULL));
    init_window();//инициализация
    init_snake();
    resetApple();
    redraw();
    const char* anim[] = { DOT, SPACE, DOT };//"кадры анимации яблока" 
    while (1) {
        for (int j = 0; j < 3; j++) {//мигнуть анимацией яблока три кадра
            put_cell(apple, anim[j & 1]);
            Sleep(DELAY_TIME);
        }

        HandleKeyboard();//считать клавиши
        if (step_and_die() || GetAsyncKeyState(VK_ESCAPE))break;//step_and_die() - главная функция
    }
    finish();
    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
