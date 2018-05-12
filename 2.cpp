#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>

using namespace std;

// Второе приложение отсылает третьему коэффициенты b1, b2, … bn,
int main(){ 
	int n; 

	// Создание канала
	mkfifo("3_2", S_IFIFO | 0666); 
	mkfifo("2_3", S_IFIFO | 0666);
	
	// Чтение
	int fd32 = open("3_2", O_RDONLY);
	read(fd32, &n, 4);
	close(fd32);

	// Вывод
	cout << "n: " << n << endl;

	// Ввод 
	int arr[n];
	cout << "Enter " << n << " elements\n";
	for (int i = 0; i < n; i++) {
		cin >> arr[i];
	}

	// Запись
	int fd23 = open("2_3", O_WRONLY);
	write(fd23, &arr, 4*n);
	close(fd23);

	unlink("3_2");
	unlink("2_3");
	return 0;
}