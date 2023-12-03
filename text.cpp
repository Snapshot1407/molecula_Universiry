#include <iostream>
#include <random> 
#include <ctime>
#include <cmath>
#include <fstream>
using namespace std;

const int m = 3;
const int n = 3;


ofstream fout("thetta.txt"); // создаём объект класса ofstream для записи и связываем его с файлом cppstudio.txt
ofstream rout("c.txt"); // создаём объект класса ofstream для записи и связываем его с файлом cppstudio.txt

unsigned seed = static_cast<unsigned>(time(nullptr)); // семя от времени
mt19937 rng(seed); // сам генератор
// диапозон возможных координат
uniform_int_distribution<int> distribution_i(0, n-1); // движение по строка
uniform_int_distribution<int> distribution_j(0, m-1); // движение по столбцам
uniform_int_distribution<int> move_id(0, 3); // куда пойдет молекула, атом, частица
uniform_int_distribution<int> cyl_bessel_j_id(1, 100); // адсорбция,дисобрция или диффузия

double const ε = 10000; // joule
double const R = 8140; // Химический потенциал
double const T = 273;

unsigned short int count = 0;

vector< double > thetts;

double thetta = 0;
double starting;
double finish;


double energy(int intmass[n][m], int i, int j)
{
    return ε*(intmass[(i + n - 1) % n][j] + intmass[i][(j + m - 1) % m] + intmass[(i + n + 1) % n][j] + intmass[i][(j + m + 1) % m]);

}
// проверка на отмену действаия
bool Metropolis(int intmas[n][m], int processing, double μ, int i, int j, double starting = 0, double finish = 0)
{
    double ΔH;
    switch (processing)
    {
    case 0:
        ΔH = energy(intmas, i,  j) - μ;
        break;
    case 1:
        ΔH = μ - energy(intmas, i,  j);
        break;
    
    default:
        ΔH = finish - starting;
        break;
    }



    if ( ΔH > 0)
    {
        uniform_int_distribution<int> s(0,1);
        if (s(rng) > exp(-ΔH/(R*T)))
        {
            return true;
        }
    }
    return false;
}


double Thetta(int intmas[n][m])
{
    thetta = 0;
    for (auto i = 0; i < n; i++) 
    {
        for (auto j = 0; j < m; j++) 
        {
            if (intmas[i][j] == 1)
            {
                thetta++;
            }
        }
    }
    return thetta / (n*m);
}

void printconsole(int intmas[n][m])
{
   for(int i=0; i<n; i++)  
   {
     for(int j=0; j<m; j++) 
       {
         cout <<  intmas[i][j] <<  " ";
       }
    cout << endl;
   } 

}

void printfile(int intmas[n][m])
{
    rout << n*m << endl;
        rout << ++count << endl;
        for (auto y = 0; y < n; y++)
        {
            for (auto x = 0; x < m; x++)
            {
                if (intmas[y][x] == 1 )
                {
                    rout << "1" << " " << x << " " << y << " 0" << endl;
                }
                
                else
                {
                    rout << "0" << " " << x << " " << y << " 0" << endl;
                }
            }
        
        }
}

void FileThetta(vector<double> ivec)
{
    double sum = 0;
    for(auto i=0;i<ivec.size();++i) sum+=ivec[i];
    fout << (double)sum/ivec.size() << endl;
}

int main()
{
    
    
     //пробный массив

    int array[3][3] = {
        {0,0,1},
        {0,0,0},
        {1,1,0}
    };



    for(double μ = -1; μ < 100; μ++)
    {
        // исправить в 1 цикл
        for(auto __ = 0; __ < 2000000*n*m; __++)
        {

            // генерация координат
            short int i = distribution_i(rng); 
            short int j = distribution_j(rng);

            short int cyl_bessel_j = cyl_bessel_j_id(rng);
            // // cout << endl << "случайные индексы " << i << " " << j << endl;
            if (cyl_bessel_j % 50 != 0)
            {

                starting = energy(array, i, j);

                // short int move = move_id(rng);
                short int move = 0;
                switch (move)
                {
                case 0:
                    // ход наверх, проверка
                    if (array[(i + n - 1) % n][j] == 0 && array[i][j] == 1) 
                    {
                        array[i][j] = 0;
                        array[( i + n - 1 ) % n][j] = 1;
                        finish = energy(array, i, j);
                        if (Metropolis(array, 3, μ, i, j, starting, finish))
                        {
                            array[i][j] = 1;
                            array[( i + n - 1 ) % n][j] = 0;
                        }
                    }
                    break;
                case 1:
                    // ход вправо, проверка
                    if (array[i][(j+m+1)%m] == 0 && array[i][j] == 1 && move == 1)
                    {
                        array[i][j] = 0;
                        array[i][( j + m + 1 ) % m] = 1;
                        if (Metropolis(array, 3, μ, i, j, starting, finish))
                        {
                            array[i][j] = 1;
                            array[i][( j + m + 1 ) % m] = 0;
                        }
                    }
                    break;
                case 2:
                    // ход вниз, проверка
                    if (array[(i+n+1)%n][j] == 0 && array[i][j] == 1 && move == 2)
                    {
                        array[i][j] = 0;
                        array[( i + n + 1 ) % n ][j] = 1;
                        if (Metropolis(array, 3, μ, i, j, starting, finish))
                        {
                            array[i][j] = 1;
                            array[( i + n + 1 ) % n ][j] = 0;
                        }
                    }
                    break;
                case 3:
                    // ход влево, проверка
                    if (array[i][(j+m-1)%m] == 0 && array[i][j] == 1 && move == 3)
                    {
                        array[i][j] = 0;
                        array[i][( j + m - 1 ) % m ] = 1;
                        if (Metropolis(array, 3, μ, i, j, starting, finish))
                        {
                            array[i][j] = 1;
                            array[i][(j + m - 1 ) % m ] = 0;
                        }

                    }
                    break;
                }
            }
            else
            {
            // cyl_bessel_j
                // адсорбция
                if (array[i][j] == 0)
                {
                    array[i][j] = 1;
                    if (Metropolis(array, 1, μ, i, j))
                    {
                        array[i][j] = 0; //молекулу не удалось адсорбировать
                    }
                }
                // дисорбция
                else
                {
                    array[i][j] = 0;
                    if (Metropolis(array, 0, μ, i, j))
                    {
                        array[i][j] = 1; //молекулу не удалось дeсорбировать
                    }
                }
            } // cyl_bessel_j
            if(__ - 1000000*n*m > 0)
            {   
                 thetts.push_back(Thetta(array));
            } 

        }
        fout << μ << endl;
        FileThetta(thetts);
        thetts.clear();
        printfile(array);

        
    }

    cout << "конечная поверхность" << endl;
    
    printconsole(array);
    printfile(array);
    cout << "Степень покрытия молекулами поверхности " << n << "х" << m << ": " << Thetta(array) << endl;
    
    
    fout.close(); // закрываем файл
    rout.close(); // закрываем файл
    return 0;
}

