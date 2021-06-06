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
		// �������������� ��������� ���������� winsock
		WSAData wsaData;
		WORD DLLVersion = MAKEWORD(2, 2);
		if (WSAStartup(DLLVersion, &wsaData) != 0)
			throw gcnew System::Exception("������ ������������� ����������.\n");

		// ����������� ���� � ip ����� � ������
		SOCKADDR_IN addr;
		int sizeofaddr = sizeof(addr);
		addr.sin_addr.s_addr = inet_addr(ip);
		addr.sin_port = htons(1111);
		addr.sin_family = AF_INET;

		// �������������� � �������
		connection = socket(AF_INET, SOCK_STREAM, NULL);
		if (connect(connection, (SOCKADDR*)&addr, sizeof(addr)) != 0)
		{
			throw gcnew System::Exception("�� ������� ������������ � �������.\n");
		}
	}

	~Client()
	{
		// ��������� ��� ������ � ������ ������ � winsock
		shutdown(connection, SD_SEND);
		closesocket(connection);
		WSACleanup();
	}

	string getAnswer()
	{
		char msg[4096];
		ZeroMemory(msg, 4096); // �������������� ������ ������ � ���������� ������� ��� ����������� �����������

		int bytesRecieved = recv(connection, msg, sizeof(msg), NULL); // �������� ��������� �� �������
		if (bytesRecieved <= 0)
			throw gcnew System::Exception("������ � ��������� ������ �� �������.\n");

		return (string)msg;
	}

	string sendMessage(string path)
	{
		send(connection, path.c_str(), path.size(), NULL); // ���������� ���� �� �����/��������
		return getAnswer();
	}
};

