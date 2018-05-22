#include <iostream>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <cmath>

/*
// http://webistore.ru/administrirovaniye-unix/nastrojka-lokalnoj-seti-mezhdu-kompyuterom-i-virtualnoj-mashinoj-virtualbox/
Вариант 13
а) Найти сумму всех седловых точек матрицы А. Будем считать, элемент седловой точкой,
	если он является наименьшим в своей строке и наибольшим в своем столбце.
	Входные данные: целое положительное число n, массив чисел А размерности nxn.
	1. Разработать алгоритм решения задания а, с учетом разделения вычислений между несколькими процессами.
	Для обмена информацией между процессами использовать неименованные каналы.
	2. Реализовать алгоритм решения задания а и протестировать на нескольких примерах.
б) Третье приложение отсылает натуральное число n первому и второму приложению.
	Первое приложение отсылает третьему коэффициенты а1, а2, …an,
	второе приложение отсылает третьему коэффициенты b1, b2, … bn,
	третье приложение вычисляет (а1-b1)*(а1-b1)+(а2-b2)*(а2-b2)+…+(аn-bn)*(аn-bn).
	3. Разработать алгоритм решения задания б, разделив вычисления между несколькими приложениями.
	Для обмена информацией между приложениями использовать именованные каналы.
	4. Реализовать алгоритм решения задания б и протестировать на нескольких примера
*/

using namespace std;

// Задача а

// Функция вывода массива на экран
void outArr(int n, int **arr) {
	cout << "Matrix:\n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << arr[i][j] << "\t";
		}
		cout << endl;
	}
}

// Минимум в строке
bool IsMin(int m, int v, int **arr, int n) {
	int min = arr[m][v];
	for (int i = 0; i < n; i++)
		if (min >= arr[m][i] && i != v)
			return false;
	return true;
}

// Максимум в столбце
bool IsMax(int m, int v, int **arr, int n) {
	int max = arr[m][v];
	for (int i = 0; i < n; i++)
		if (max <= arr[i][v] && i != m)
			return false;
	return true;
}

void one() {
	int n;
	cout << "Enter n\n";
	cin >> n;

	// Инициализация матрицы
	int **arr = new int *[n];
	for (int i_ = 0; i_ < n; i_++) arr[i_] = new int[n];

	// Ввод элементов матрицы
	cout << "Enter " << n*n << " matrix elements\n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> arr[i][j];
		}
	}

	outArr(n, arr);

	// Количество точек - data[0] и их сумма - data[1]
	int data[2] = { 0, 0 }; 

	int fd[2];

	// Проверка на доступность канала
	if (pipe(fd) < 0) {
		fprintf(stdout, "\nError") ; 
		return; 
	}

	// Программный код для дочернего процесса
	if (fork()==0) {
		// Вычисление
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (IsMin(i, j, arr, n) && IsMax(i, j, arr, n)) {
					data[0]++;
					data[1] += arr[i][j];
					cout << "Point " << data[0] << ": " << i + 1 << ' ' << j + 1 << " Value: " << arr[i][j] << endl;
				}
			}
		}

		// Запись
		write(fd[1], &data, 8);
		//return;
		exit(0);
	}	

	// Чтение
	read(fd[0], &data, 8);

	// Вывод
	if (data[0] == 0)
		cout << "Points not found\n";
	else cout << "Sum: " << data[1] << endl;

	close(fd[0]);
	close(fd[1]);
	exit(0);
}

// Задача б
void two() { // Третье приложение
	int n;
	int arrA[n], arrB[n];
	cout << "Enter n\n";
	cin >> n;
			

	// Запись
	int fd31 = open("3_1", O_WRONLY);
	int fd32 = open("3_2", O_WRONLY);
	write(fd31, &n, 4);
	write(fd32, &n, 4);


	// Чтение
	int fd13 = open("1_3", O_RDONLY);
	int fd23 = open("2_3", O_RDONLY);
	read(fd13, &arrA, 4*n);
	read(fd23, &arrB, 4*n);

	// Вывод 
	cout << "Received values from the first app: \n";
	for (int i = 0; i < n; i++) {
		cout << arrA[i] << " ";
	} 
	cout << "\nReceived values from the second app: \n";
	for (int i = 0; i < n; i++) {
		cout << arrB[i] << " ";
	} cout << endl;

	close(fd31); 
	close(fd13); 
	close(fd32); 
	close(fd23); 

	// Вычисление (а1-b1)*(а1-b1)+(а2-b2)*(а2-b2)+…+(аn-bn)*(аn-bn)
	int sum = 0;
	for (int i = 0; i < n; i++) {
		sum += pow (arrA[i]-arrB[i], 2);
	} 
	cout << "The result of the formula (а1-b1)*(а1-b1)+(а2-b2)*(а2-b2)+…+(аn-bn)*(аn-bn):\n" << sum << endl;

	exit(0);
}

typedef void(*menu)();
menu menuGroup[2] = { one, two };

int main(int argc, char **argv, char **envp) {
	int wait_;
	int positionMenu;
	while (true) {
		cout << "\nSelect action:\n 1 - Complete task \"a\"\n 2 - Complete task \"b\"\n 3 - Exit\n";
		cin >> positionMenu;
		if (positionMenu > 0 && positionMenu < 3) {
			if (fork() == 0) {
				menuGroup[positionMenu - 1]();
			}
			else
				wait(&wait_);
		}
		else if (positionMenu == 3) exit(0);
	}
	return 0;
}
