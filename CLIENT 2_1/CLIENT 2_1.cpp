#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <fstream>

#pragma warning(disable: 4996)

SOCKET Connection;

void ClientHandler() {
	char msg[256];
	//FILE* inFile2 = fopen("C:\\Users\\Gomerich\\source\\repos\\LABA 6 Server\\x64\\Debug\\textClone.txt", "w+");
	while (true) {
		recv(Connection, msg, sizeof(msg), NULL);
		std::cout << msg << std::endl;
		std::ofstream out;
		out.open("C:\\Users\\Gomerich\\Desktop\\pul\\TEXTCLONE.txt");
		if (out.is_open())
		{
			out << msg << std::endl;
		}
	}
}

int main(int argc, char* argv[]) {
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1); //версия библиотеки
	if (WSAStartup(DLLVersion, &wsaData) != 0) {  //загружаем библиотеку
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;  //структура для хранения адреса 
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) { //подключаемся к серверу
		std::cout << "Error: failed connect to server.\n";
		return 1;
	}
	std::cout << "Connected!\n";

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);

	char msg1[256];
	while (true) {
		std::cin.getline(msg1, sizeof(msg1));
		send(Connection, msg1, sizeof(msg1), NULL);
		Sleep(10);
	}

	system("pause");
	return 0;
}