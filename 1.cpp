#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>

using namespace std;

// Первое приложение отсылает третьему коэффициенты а1, а2, …an
int main(){ 
	int n; 

	// Создание канала
	mkfifo("3_1", S_IFIFO | 0666); 
	mkfifo("1_3", S_IFIFO | 0666);
	
	// Чтение
	int fd31 = open("3_1", O_RDONLY);
	read(fd31, &n, 4);
	close(fd31);

	// Вывод
	cout << "n: " << n << endl;

	// Ввод 
	int arr[n];
	cout << "Enter " << n << " elements\n";
	for (int i = 0; i < n; i++) {
		cin >> arr[i];
	}

	// Запись
	int fd13 = open("1_3", O_WRONLY);
	write(fd13, &arr, 4*n);
	close(fd13);

	unlink("3_1");
	unlink("1_3");
	return 0;
}