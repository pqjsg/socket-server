#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <string>
#include <WS2tcpip.h>
#include <iostream>
#include <ctime>
#include <filesystem>
#include <vector>
#include <fstream>
namespace fs = std::filesystem;
using namespace std;

#pragma warning(disable: 4996)

vector<string> getDrives()
{
	vector<string> drives;

	DWORD mask = GetLogicalDrives(); // Получаем битовую маску дисков, которые у нас есть
	for (char drive = 'A'; drive <= 'Z'; drive++) // Проходим по всем возможным дискам
	{
		if (mask & 1) // Если диск существует, то добавляем его
		{
			string rootPath;
			rootPath += drive;
			rootPath += ":\\";
			drives.push_back(rootPath);
		}
		mask >>= 1; // Сдвигаем битовую маску на один бит вправо
	}
	return drives;
}

vector<string> getDirectoryFiles(const fs::path& dir)
{
	vector<string> files;
	for (auto& p : fs::directory_iterator(dir))
	{
		files.push_back(p.path().filename().string());
	}
	return files;
}

int main()
{
	setlocale(LC_ALL, "rus");
	
	// Инициализируем последнюю библиотеку winsock
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 2);
	if (WSAStartup(DLLVersion, &wsaData) != 0)
	{
		cerr << "Ошибка инициализации winsock." << endl;
		return -1;
	}

	// Создаем сокет
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Невозможно создать сокет." << endl;
		return -1;
	}

	// Привязываем порт и ip адрес к сокету
	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	bind(listening, (SOCKADDR*)&addr, sizeof(addr));
	
	// Говорим winsock, что сокет создан для прослушивания
	listen(listening, SOMAXCONN);

	// Цикл для того, чтобы при отключении искались новые клиенты
	while (true)
	{
		// Принимаем одно соединение с сокетом
		SOCKET connection;
		connection = accept(listening, (SOCKADDR*)&addr, &sizeofaddr);

		if (connection == 0)
		{
			cerr << "Не удалось подключиться." << endl;
		}
		else // Если соединено успешно
		{
			time_t conn = time(0);
			cout << "Клиент с адреса " << inet_ntoa(addr.sin_addr) << " подсоединился " << ctime(&conn);

			// Выводим диски
			string drives;
			for (string drive : getDrives())
				drives += drive + "\r\n";
			send(connection, drives.c_str(), drives.size(), NULL); // Отправляем диски к клиенту

			// Цикл для получения сообщений от клиента
			while (true)
			{
				char msg[4096];
				ZeroMemory(msg, 4096); // Инициализируем пустые ячейки в символьном массиве для корректного отображения

				int bytesRecieved = recv(connection, msg, sizeof(msg), NULL); // Получаем сообщение от клиента
				if (bytesRecieved == SOCKET_ERROR)
				{
					cerr << "Ошибка в получении сообщения от клиента." << endl;
					break;
				}
				if (bytesRecieved == 0) // Если получено 0 байт, значит клиент отключился
					break;
				
				time_t rec = time(0);
				cout << "Сервер получил " << ctime(&rec) << msg << endl;

				fs::path path = (string)msg;
				if (fs::exists(path)) // Если директория существует
				{
					if (path.extension() != ".txt") // Если это не файл
					{
						string files;
						for (string file : getDirectoryFiles(fs::path((string)msg)))
							files += file + "\r\n";

						send(connection, files.c_str(), files.size(), NULL); // Отправляем файлы к клиенту
					}
					else
					{
						ifstream f(path, ios::in | ios::binary); // Открываем поток с файлом
						const auto sz = fs::file_size(path); // СОхраняем размер файла
						string result(sz, '\0'); // Создаем пустой буффер
						f.read(result.data(), sz); // Копируем содержимое
						f.close(); // Закрываем поток

						send(connection, result.c_str(), result.size(), NULL); // Отправляем файлы к клиенту
					}
				}
				else
				{
					string error = "Ошибка: Данного каталога/файла не сущетсвует.\r\n";
					send(connection, error.c_str(), error.size(), NULL); // Отправляем ответ к клиенту
				}
			}
			time_t disconn = time(0);
			cout << "Клиент отключился " << ctime(&disconn);
		}
		shutdown(connection, SD_SEND); // Выключаем соединение
		closesocket(connection); // Закрываем сокет
	}
	closesocket(listening); // Закрываем сокет прослушивания
	WSACleanup(); // Очищаем данные о winsock
}