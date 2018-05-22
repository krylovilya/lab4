#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>

using namespace std;

int main(){ 
	int n, n2; 

	// Создание канала
	mkfifo("1_2", S_IFIFO | 0666); 
	mkfifo("2_1", S_IFIFO | 0666);

	start:

	// Чтение
	int fd21 = open("2_1", O_RDONLY);
	read(fd21, &n2, 4);
	close(fd21);

	cout << "n: ";
	cin >> n;

	//Запись
	int fd12 = open("1_2", O_WRONLY);
	write(fd12, &n, 4);
	close(fd12);
	
	if (n2 == n) {cout << endl << "exit"; goto end; }

	goto start;		
	end:

	unlink("2_1");
	unlink("1_2");
	return 0;
}
