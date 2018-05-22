#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>

using namespace std;

int main(){ 
	int n, n1; 

	// Создание канала
	mkfifo("1_2", S_IFIFO | 0666); 
	mkfifo("2_1", S_IFIFO | 0666);
	
	start:

	cout << "n: ";
        cin >> n;

	//Запись
	int fd21 = open("2_1", O_WRONLY);
	write(fd21, &n, 4);

	// Чтение
	int fd12 = open("1_2", O_RDONLY);
	read(fd12, &n1, 4);
	close(fd12);

	if (n == n1) {cout << endl << "exit"; goto end; }

	goto start;
	end:

	unlink("2_1");
	unlink("1_2");
	return 0;
}
