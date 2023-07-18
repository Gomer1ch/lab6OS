#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <string>

#pragma warning(disable: 4996)

SOCKET Connections[100];
int Counter = 0;

void ClientHandler(int index) {
	char msg[256];
	char FileSend[256];
	while (true) {
		recv(Connections[index], msg, sizeof(msg), NULL);
		if (msg[0] == 'Y')
		{
			for (int i = 0; i < Counter; i++) 
			{
				if (i == index) {
					std::string line;

					std::ifstream in("C:\\Users\\Gomerich\\Desktop\\pul\\TEXT.txt"); // окрываем файл для чтения
					if (in.is_open())
					{
						while (getline(in, line))
						{
							std::cout << line << std::endl;
						}							
						strcpy(FileSend, line.c_str());
							send(Connections[i], FileSend, sizeof(FileSend), NULL);
					}
					in.close();
				}
			}
		}
	}
}

int main(int argc, char* argv[]) {
	WSAData wsaData; //версия библиотеки
	WORD DLLVersion = MAKEWORD(2, 1);  //загружаем библиотеку
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;  //структура для хранения адреса
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); //привязываем адрес и порт к сокету
	listen(sListen, SOMAXCONN); //прослушивание порта в ожидании соединения

	SOCKET newConnection;
	for (int i = 0; i < 100; i++) {
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr); //принимаем подключение

		if (newConnection == 0) {
			std::cout << "Error #2\n";
		}
		else {
			std::cout << "Client Connected!\n";
			char msg[256] = "Send? Press Y";
			send(newConnection, msg, sizeof(msg), NULL);

			Connections[i] = newConnection;
			Counter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
		}
	}
	system("pause");
	return 0;
}