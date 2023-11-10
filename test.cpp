#include <iostream>
#include <random> 
#include <ctime>
#include <fstream>
using namespace std;
// размеры массива
int const n = 4;
int const m = 4;
ofstream fout("coordinations.xyz"); // создаём объект класса ofstream для записи и связываем его с файлом cppstudio.txt

// счетчик кадров
unsigned short int count = 0;

float Degree_of_surface_coverage = 0;

int main()
{
    unsigned seed = static_cast<unsigned>(time(nullptr)); // семя от времени
    mt19937 rng(seed); // сам генератор
    // диапозон возможных координат
    uniform_int_distribution<int> distribution_i(0, n-1);
    uniform_int_distribution<int> distribution_j(0, m-1);
    uniform_int_distribution<int> move_id(0, 3);
    uniform_int_distribution<int> cyl_bessel_j_id(1, 100);

// Создание массива N х M, заполненного нулями
    int array[n][m];
    for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                array[i][j] = 0;
                cout << array[i][j] << " ";
            }
            cout << endl;
        }

    // //пробный массив
    // int array[3][3] = {
    //     {1,0,0},
    //     {0,0,0},
    //     {0,0,0}
    // };
    

    for(auto __ = 0; __ < 100; __++)
    {
    // 1 МК шаг
        for (auto _ = 0; _ < n*m; _++) 
        {
                // генерация координат
                short int i = distribution_i(rng); 
                short int j = distribution_j(rng);

                short int move = move_id(rng);
                short int cyl_bessel_j = cyl_bessel_j_id(rng);
                // cout << endl << "случайные индексы " << i << " " << j << endl;

                if (cyl_bessel_j % 50 != 0)
                {
                    // ход наверх, проверка
                    if (array[(i+n-1)%n][j] == 0 && array[i][j] == 1 && move == 0)
                    {
                        // cout << endl<< "Вывод новой позиции по вертикали " << (i+n-1)%m << endl;
                        array[i][j] = 0;
                        array[(i+n-1)%n][j] = 1;
                        // cout << endl << endl;
                        // for(auto k = 0; k < n; k++) cout << array[k][j] << " "; // вывод измененной столбца
                        // cout << endl << "Новый столбец имеет вид: ";
                    }
                    // ход вправо, проверка
                    else if (array[i][(j+m+1)%m] == 0 && array[i][j] == 1 && move == 1)
                    {
                        // cout << endl<< "Вывод новой позиции по горизонтали " << (j+m+1)%m << " " << endl;
                        array[i][j] = 0;
                        array[i][(j+m+1)%m] = 1;
                        // cout << endl << "Новая строка имеет вид: ";
                        // for(auto k = 0; k < m; k++) cout << array[i][k] << " "; // вывод измененной строки
                        // cout << endl << endl;
                        // cout << endl << endl;
                    }
                    // ход вниз, проверка
                    else if (array[(i+n+1)%n][j] == 0 && array[i][j] == 1 && move == 2)
                    {
                        // cout << endl<< "Вывод новой позиции по вертикали " << (i+n-1)%m << " " << endl;
                        array[i][j] = 0;
                        array[(i+n+1)%n][j] = 1;
                        // cout << endl << "Новый столбец имеет вид: ";
                        // for(auto k = 0; k < n; k++) cout << array[k][j] << " "; // вывод измененной столбца
                        // cout << endl << endl;
                        // cout << endl;

                    }
                    // ход влево, проверка
                    else if (array[i][(j+m-1)%m] == 0 && array[i][j] == 1 && move == 3)
                    {
                        // cout << endl<< "Вывод новой позиции по горизонтали " << (j+m-1)%m << " " << endl;
                        array[i][j] = 0;
                        array[i][(j+m-1)%m] = 1;
                        // cout << endl << "Новая строка имеет вид: ";
                        // for(auto k = 0; k < m; k++) cout << array[i][k] << " "; // вывод измененной строки
                        // cout << endl << endl;
                        // cout << endl << endl;
                    }
                }
                else{
                    // cyl_bessel_j
                        // адсорбция
                        if (array[i][j] == 0)
                            {
                            array[i][j] = 1;
                            }
                        // дисорбция
                        else{
                            array[i][j] = 0;
                        }
                    }
        fout << n*m << endl;
        fout << ++count << endl;
        for (auto y = 0; y < n; y++)
        {
            for (auto x = 0; x < m; x++)
            {
                if (array[y][x] == 1 )
                {
                    fout << "C" << " " << x << " " << y << " 0" << endl;
                }
                
                else
                {
                    fout << "H" << " " << x << " " << y << " 0" << endl;
                }
            }
        
        }
        
        
        

        // вывод массива после каждого хода
        cout << count << " - номер кадра" << endl;
        for (auto ii = 0; ii < n; ii++) 
        {
            for (auto jj = 0; jj < m; jj++) 
            {
                if (array[ii][jj] == 1 )
                {
                    cout << "C" << " " << ii << " " << jj << " 0" << endl;
                }
                
                else
                {
                    cout << "H" << " " << ii << " " << jj << " 0" << endl;
                }
            }

        }

        }
    // // вывод массива после 1 МК шага
    // cout << ++count << " - номер кадра" << endl;
    // for (auto i = 0; i < n; i++) 
    // {
    //     for (auto j = 0; j < m; j++) 
    //     {
    //         cout << array[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;
    }

    

    cout << "конечная поверхность" << endl;
    for (auto i = 0; i < n; i++) 
    {
        for (auto j = 0; j < m; j++) 
        {
            if (array[i][j] == 1)
            {
                Degree_of_surface_coverage++;
            }
            cout << array[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;

//  запись в файл
    // for (auto i = 0; i < 3; i++)
    // {
    //     for (auto j = 0; j < 3; j++)
    //     {
    //         fout << array[i][j] << " "; // запись строки в файл
    //     }
        
    //     fout << endl; // запись строки в файл
    // }
    
    

    fout.close(); // закрываем файл
    cout << "Степень покрытия молекулами поверхности " << n << "х" << m << ": " << Degree_of_surface_coverage/(n*m) << endl;
    return 0;
}