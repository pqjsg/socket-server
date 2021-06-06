#pragma once
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <string>
#include <WS2tcpip.h>

using namespace std;
class Client
{
private:
	SOCKET connection;
public:
	Client(const char* ip)
	{
		// Инициализируем последнюю библиотеку winsock
		WSAData wsaData;
		WORD DLLVersion = MAKEWORD(2, 2);
		if (WSAStartup(DLLVersion, &wsaData) != 0)
			throw gcnew System::Exception("Ошибка инициализации библиотеки.\n");

		// Привязываем порт и ip адрес к сокету
		SOCKADDR_IN addr;
		int sizeofaddr = sizeof(addr);
		addr.sin_addr.s_addr = inet_addr(ip);
		addr.sin_port = htons(1111);
		addr.sin_family = AF_INET;

		// Подсоединяемся к серверу
		connection = socket(AF_INET, SOCK_STREAM, NULL);
		if (connect(connection, (SOCKADDR*)&addr, sizeof(addr)) != 0)
		{
			throw gcnew System::Exception("Не удалось подключиться к серверу.\n");
		}
	}

	~Client()
	{
		// Отключаем все сокеты и чистим данные о winsock
		shutdown(connection, SD_SEND);
		closesocket(connection);
		WSACleanup();
	}

	string getAnswer()
	{
		char msg[4096];
		ZeroMemory(msg, 4096); // Инициализируем пустые ячейки в символьном массиве для корректного отображения

		int bytesRecieved = recv(connection, msg, sizeof(msg), NULL); // Получаем сообщение от сервера
		if (bytesRecieved <= 0)
			throw gcnew System::Exception("Ошибка в получении ответа от сервера.\n");

		return (string)msg;
	}

	string sendMessage(string path)
	{
		send(connection, path.c_str(), path.size(), NULL); // Отправляем путь до файла/каталога
		return getAnswer();
	}
};

