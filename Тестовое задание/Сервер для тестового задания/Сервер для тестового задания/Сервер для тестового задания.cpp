
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <filesystem>
#include <fstream>

#pragma warning(disable: 4996)
using namespace std;
using namespace std::filesystem;

enum Pacet {
	Login_and_parol,
	authorization_login,
	authorization_parol,
	authorization_name,
	authorization_fourname,
	authorization_surname,
	authorization_dateBirth
};

int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	ofstream fout;
	ifstream fin;

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	WSAStartup(DLLVersion, &wsaData);


	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(4879);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	SOCKET newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

	if (newConnection == 0) {
		cout << "Error #2\n";
	}
	else {
		cout << "Client Connected!\n";
	}

	Pacet type_packet;
	string agress = "C:/shot/Тестовое_задание";//адрес для формирования нового клиента
	int i_agress = 0;
	string R_login, R_parol;

	while(1) {
		recv(newConnection, (char*)&type_packet, sizeof(Pacet), NULL);

		switch (type_packet)
		{
		case Login_and_parol: {
			cout << "Иницилизация пакета: Login_and_parol" << endl;
			int len_messanger;//Размер отправленного сообщения клиентом
			recv(newConnection, (char*)&len_messanger, sizeof(len_messanger), NULL);

			//Динамическое выделение памяти под сообщение
			char* messanger = new char[len_messanger + 1];
			messanger[len_messanger] = '\0';

			recv(newConnection, messanger, len_messanger, NULL);//Прием сообщение который отправил клиент
			string palogin = messanger;

			int len_agress = agress.size();
			do {
				agress.erase(len_agress, agress.length() - len_agress); //Приведение адреса к виду ("C:/shot/Тестовое_задание")
				i_agress++;
				agress = agress + "/user(" + to_string(i_agress) + ").txt";
			} while (ifstream(agress));
			agress.erase(len_agress, agress.length() - len_agress); //Приведение адреса к виду ("C:/shot/Тестовое_задание")

			for (int j = 0; j < i_agress; j++) {
				agress += "/user(" + to_string(j) + ").txt";
				fin.open(agress, ios_base::in);
				string paLogin_txt;
				fin >> paLogin_txt;
				if (palogin == paLogin_txt) {
					Pacet type = Login_and_parol;
					string yes_exit = "Entrance is allowed!";
					int len_yes_exit = yes_exit.size();
					send(newConnection, (char*)&type, sizeof(Pacet), NULL);
					send(newConnection, (char*)&len_yes_exit, sizeof(int), NULL);
					send(newConnection, yes_exit.c_str(), len_yes_exit, NULL);
					delete[] messanger;
					break;
				}
				agress.erase(len_agress, agress.length() - len_agress); //Приведение адреса к виду ("C:/shot/Тестовое_задание")
			}
			break;
		}case(authorization_login): {
			cout << "Иницилизация пакета: authorization_login" << endl;
			int len_messanger;//Размер отправленного сообщения клиентом
			recv(newConnection, (char*)&len_messanger, sizeof(len_messanger), NULL);

			//Динамическое выделение памяти под сообщение
			char* messanger = new char[len_messanger + 1];
			messanger[len_messanger] = '\0';

			recv(newConnection, messanger, len_messanger, NULL);//Прием сообщение который отправил клиент
			R_login = messanger ;
			break;
		}
		case(authorization_parol): {
			cout << "Иницилизация пакета: authorization_parol" << endl;
			int len_messanger;//Размер отправленного сообщения клиентом
			recv(newConnection, (char*)&len_messanger, sizeof(len_messanger), NULL);

			//Динамическое выделение памяти под сообщение
			char* messanger = new char[len_messanger + 1];
			messanger[len_messanger] = '\0';

			recv(newConnection, messanger, len_messanger, NULL);//Прием сообщение который отправил клиент
			R_parol = messanger;

			string R_paLogin = R_login + "[" + R_parol + "]";

			path ag = current_path(); // Получение путя исполнительного файла
			ag = ag.root_path(); //Получение корневой папки
			ag /= "shot";
			if (!exists(status(ag))) { create_directories(ag); }

			ag /= "Тестовое_задание";
			if (!exists(status(ag))) { create_directories(ag); }
			string agress = ag.generic_string();

			create_directories(agress.c_str());
			int len_agress = agress.size();
			do {
				agress.erase(len_agress, agress.length() - len_agress);
				i_agress++;
				agress = agress + "/user(" + to_string(i_agress) + ").txt";
			} while (ifstream(agress));

			fout.open(agress, ios_base::app);
			fout << R_paLogin << endl;
			cout <<"Данные для файла: "<< R_paLogin << endl;
			cout << "Адресс файла: " << agress << endl;
			fout.close();
			break;
		}
		default: {
			//Если придет не изветный нам пакет об этом выведиться сообщение
			cout << "Not initialization package" << type_packet << endl;
			break;
		}
		}
	}

	system("pause");
	return 0;
}
