#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>

#pragma warning(disable: 4996)

using namespace std;

enum Pacet {
	Login_and_parol, 
	authorization_login,
	authorization_parol
};

int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	WSAStartup(DLLVersion, &wsaData);


	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(4879);
	addr.sin_family = AF_INET;

	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);
	connect(Connection, (SOCKADDR*)&addr, sizeof(addr));

	cout << "Регистрация - R" << endl;
	cout << "Авторизация - A" << endl;
	char AR; cin >> AR;
	if (AR == 'R') {
		cout << "Придумайте логин: ";
		string R_login;
		getline(cin, R_login);
		getline(cin, R_login);
		string R_parol1, R_parol2;
		int lenR_login = R_login.size();
		do {
			cout << "Придумайте пароль: ";
			getline(cin, R_parol1);
			cout << "Повторите пароль: ";
			getline(cin, R_parol2);
		} while (R_parol1 != R_parol2);
		int lenR_parol = R_parol1.size();

		Pacet type = authorization_login;
		send(Connection, (char*)&type, sizeof(Pacet), NULL);
		send(Connection, (char*)&lenR_login, sizeof(int), NULL);
		send(Connection, R_login.c_str(), lenR_login, NULL);

		type = authorization_parol;
		send(Connection, (char*)&type, sizeof(Pacet), NULL);
		send(Connection, (char*)&lenR_parol, sizeof(int), NULL);
		send(Connection, R_parol1.c_str(), lenR_parol, NULL);

	}
	else if(AR == 'A'){

		string parol, login;
		cout << "Введите логин: ";
		cin >> login;
		cout << "Введите пароль: ";
		cin >> parol;

		string palogin = login + "[" + parol + "]";
		int len_palogin = palogin.size();

		Pacet type = Login_and_parol;
		send(Connection, (char*)&type, sizeof(Pacet), NULL);
		send(Connection, (char*)&len_palogin, sizeof(int), NULL);
		send(Connection, palogin.c_str(), len_palogin, NULL);

	}

	Pacet type_packet;//Переменная хранит тип пакета
	recv(Connection, (char*)&type_packet, sizeof(Pacet), NULL);
	
	switch (type_packet) {
	case Login_and_parol: {
		int len;
		recv(Connection, (char*)&len, sizeof(int), NULL);
		char* msg = new char[len + 1];
		msg[len] = '\0';
		recv(Connection, msg, len, NULL);
		cout << msg << endl;
		delete[] msg;
		break;
	}
	default:
	{
		//Если придет не изветный нам пакет об этом выведиться сообщение
		cout << "Not initialization package" << endl;
		break;
	}
	}

	system("pause");
	return 0;
}