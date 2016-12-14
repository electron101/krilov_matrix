/* Метод Крылова (нахождение собственного значения матрицы)
 *
 */
#include <iostream>
#include <stdio.h>
#include <math.h>

//Объявим прототипы процедур и функций
void dispArr (float **Arr, int i, int j, char t[7]);
float** arrMult (float **Arr, float **ArrO, int m, int n, int q);
float** treugArr (float **Arr, int n);
float* gSolve (float **Arr, int n);
float halfDiv(float *P, float eps);
float* urDiv (float *P, float a, int n);
float urTest4 (float a, float b, float *P);

//Функция перемножения матриц
float** arrMult (float **Arr, float **ArrO, int m, int n, int q) 
{
    float **MultArr;
  
    //Динамически создаем матрицу - результат произведения
    MultArr = new float*[m];
    for (int i=0; i<m; i++) 
    { 
        MultArr[i] = new float[q]; 
    };
    
    //Вычисляем произведение матриц
    for (int i=0; i<m; i++)
        for (int j=0; j<q; j++)
        {
            MultArr[i][j] = 0;
            for (int k=0; k<n; k++)
                MultArr[i][j] += Arr[i][k]*ArrO[k][j];
        }

    return MultArr;
}

//Приведение матрицы к треугольному виду
float** treugArr (float **Arr, int n) 
{
    //Приводим матрицу A к треугольному виду
    for (int i=0; i<n; i++)
    {
        //Обнуляем необходимые элементы
        for (int j=i+1; j<n; j++)
        {
            float koeff = Arr[j][i]/Arr[i][i]; //Коэффициент, на который будем домножать строку
            for (int k=0; k<n+1; k++)
                Arr[j][k]=Arr[j][k]-Arr[i][k]*koeff;
        }
    }
    return Arr;
}

//Нахождение переменных в матрице, приведенной к треугольному виду
float* gSolve (float **Arr, int n) 
{
    float *X;
    X = new float[n];
    //Расчет неизвестных:
    X[3] = Arr[3][4]/Arr[3][3];
    X[2] = (Arr[2][4] - Arr[2][3]*X[3])/Arr[2][2];
    X[1] = (Arr[1][4] - Arr[1][2]*X[2] - Arr[1][3]*X[3])/Arr[1][1];
    X[0] = (Arr[0][4] - Arr[0][1]*X[1] - Arr[0][2]*X[2] - Arr[0][3]*X[3])/Arr[0][0];
    
    return X;
}

//Метод половинного деления
float halfDiv(float *P, float eps) 
{
    float a=0, b=0, prec;
    
    do {
        a--; b++;
    } while (urTest4(a, b, P) > 0);
    std::cout << "Исходный отрезок для нахождения корней: ";
    std::cout << "[" << a << "," << b << "]\n";
    do
    {
        prec = b - a;
        float cent = (a+b)/2;
        if (urTest4(a, cent, P) < 0) b=cent;
        else a=cent;
    } while (prec>eps);
    
    return a;
}

//Деление уравнения
float* urDiv (float *P, float a, int n) 
{
    P[n]=0;
    for (int i=n-1; i>=0; i--)
    {
        if (i+1 == n)
            P[i]=-P[i]+a;
        else
            P[i]=-P[i]+P[i+1]*a;
    }
    for (int i=0; i<n; i++)
        if (i+1 == n)
            P[i] = -1;
        else
            P[i] = -P[i+1];
 
    return P; 
}

//Проверка знаков на концах отрезка
float urTest4 (float a, float b, float *P) 
{
    float r1 = P[4]*pow(a, 4) - P[3]*pow(a, 3) - P[2]*pow(a,2) - P[1]*a - P[0];
    float r2 = P[4]*pow(b, 4) - P[3]*pow(b, 3) - P[2]*pow(b,2) - P[1]*b - P[0];
    
    return r1*r2;
}
 
void dispArr (float **Arr, int si, int sj, char t[7]) //Процедура отображения матрицы
{
    std::cout << "------------------------------ \n";
    for (int i=0; i<si; i++)
    {
        for (int j=0; j<sj; j++)
            printf(t, Arr[i][j]);
        std::cout << "\n";
    }
    std::cout << "------------------------------ \n";
    
}
 
int main (int argc, const char * argv[])
{
    FILE *file;
    float kf, eps, a;
    float **A, **B, **C, **mC, **c0, *P, *lambda, **tmpArr;
    int ni, nj, i, j;
  
    //Открываем файл для чтения исходных данных
    file = fopen("/home/grom/.krilov_conf", "r");
  
    //Считываем из файла размерность массивов Ni x Nj
    fscanf(file, "%d", &ni); 
    fscanf(file, "%d", &nj);

    //Создаем динамические массивы
    A = new float*[ni]; 
    B = new float*[ni]; 
    C = new float*[ni];
 
    for (int i=0; i<ni; i++) 
    { 
        A[i] = new float[nj]; 
        B[i] = new float[nj]; 
        C[i] = new float[nj];
    };
    
    //Считываем массив B:
    for (i=0; i<ni; i++)
        for (j=0; j<nj; j++)
            fscanf(file, "%f ", &B[i][j]);
    
    //Считываем массив C:
    for (i=0; i<ni; i++)
        for (j=0; j<nj; j++)
            fscanf(file, "%f ", &C[i][j]);
  
    //Считываем число k:
    fscanf(file, "%f", &kf);

    //Считываем желаемую точность
    fscanf(file, "%f", &eps);
    fclose(file);

    //Формируем массив A, с которым будем работать (A = B + kC):
    for (i=0; i<ni; i++)
        for (j=0; j<nj; j++)
            A[i][j] = B[i][j] + kf*C[i][j];

    std::cout << "Исходная матрица A:\n";
    dispArr(A, ni, nj, "%5.2f ");

    //Блок нахождения значений C(0), C(1), C(2), C(3), C(4)
    //Создадим массив который будет хранить эти значения
    mC = new float*[ni];
    for (int i=0; i<ni; i++) 
        mC[i] = new float[ni+1];
 
    //В этом массиве за C(0) примем (1, 1, 1, 1)
    for (int i=0; i<ni; i++) 
        mC[i][0] = 1;

    //Создадим массив c0, который будет хранить текущее значение C(1), C(2), C(3), C(4)
    c0 = new float*[ni];
    for (int i=0; i<ni; i++) 
        c0[i] = new float[1];

    //Перейдем к нахождению C(1), C(2), C(3), C(4)
    for (int i=0; i<ni; i++)
    {
        //Заполним массив c0 текущим значением - C(i)
        for (int j=0; j<ni; j++)
            c0[j][0]=mC[j][i];
        //Для нахождения C(i+1) умножим массив A на с0
        tmpArr = arrMult(A, c0, ni, nj, 1);
        //Занесем полученные данные в массив mC
        for (int j=0; j<ni; j++)
            mC[j][i+1]=tmpArr[j][0];
    }
    std::cout << "   C(0)    C(1)    C(2)    C(3)   C(4) \n";
    dispArr(mC, 4, 5, "%7.3f ");

    //Запишем систему уравнений для нахождения P1, P2, P3, P4    
    treugArr(mC, 4);
    P = new float[ni+1];
    P = gSolve(mC, 5);
    P[4] = 1;

    //Запишем получившееся уравнение с О»
    std::cout << "Характеристическое уравнение: \n";
    std::cout << P[4] << " О»^4 - (" << P[3] <<") О»^3 - (" << P[2] << ") О»^2 - (" << P[1] << ") О» - (" << P[0] <<") = 0 \n";
    lambda = new float[ni];

    //Ищем значения О» методом половинного деления     
    for (int k=4; k>0; k--)
    {
        //Будем решать полученное уравнение методом половинного деления    
        a = halfDiv(P, eps);
        lambda[k-1] = a;
        std::cout << "О»[" << ni+1-k << "]: ";
        printf("%7.3f \n", a);
        //Разделим уравнение на полученный корень, чтобы получить кубическое
        P = urDiv(P, a, k);
        //Запишем результат деления
        if (k-1 > 0) { std::cout << "Уравнение " << k-1 << " степени: \n";
            std::cout << P[4] << " О»^4 - (" << P[3] <<") О»^3 - (" << P[2] << ") О»^2 - (" << P[1] << ") О» - (" << P[0] <<") = 0 \n"; }
    }

    //Аккуратно выведем полученные значения О»
    std::cout << "------------------------------ \n";
    std::cout << "Полученные собственные значения: \n";
    for (int i=ni; i>0; i--)
    {
        std::cout << "О»[" << ni-i+1 << "]: " << lambda[i-1] << ";\n";
    }
    std::cout << "------------------------------ \n";
    return 0;
}
