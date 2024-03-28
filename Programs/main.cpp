#include<iostream>
#include<string>
#include<ctime>
#include<vector>
#include<array>
#include<iomanip>
#include<fstream>
#include<sstream>

class User
{
private:
	std::string _name{};
	std::string _password{};
	bool _logged{};

public:
	//Metoda sprawdza czy zewn�trzne nazwa i has�o s� zgodne z wewn�trzn� nazwem i has�em u�ytkownika
	bool logIn(std::string name, std::string password) {
		if ((_name == name) && (_password == password)) {
			_logged = 1;
			return 1;
		}
		return 0;
	}

	User() {
		_name = "";
		_password = "";
		_logged = 0;
	}

	User(std::string name, std::string password) {
		_name = name;
		_password = password;
		_logged = 0;
	}


	//Sekcja metod dost�powych
	std::string& Name() { return _name; }
	const std::string& Name() const { return _name; }

	std::string& Password() { return _password; }
	const std::string& Password() const { return _password; }

	bool Logged() { return _logged; }
	const bool Logged() const { return _logged; }

};

class UserContainer 
{
private:	
	std::vector<User> _userList{};

public:
	//Metoda podejmuje pr�b� dodania u�ytkownika do bazy u�ytkownik�w i informuje o rezultacie
	bool addingNewUser(std::string name, std::string password) {
		if (name == "" || password == ""){							//Nazwa ani has�o nie mo�e pozosta� polem pustym
			std::cout << "Name nor password can be left empty" << std::endl;
			system("pause");
			return 0;
		}
			
		if (isNameTaken(name)) {									//Sprawdzenie czy dana nazwa jest ju� zaj�ta
			std::cout << "Name is already taken" << std::endl;
			system("pause");
			return 0;
		}

		User newUser(name, password);								//Stworzenie noweg u�ytkownika

		_userList.push_back(newUser);								//Dodanie nowego u�ytkownika do bazy u�ytkownik�w

		std::ofstream fileOut("accList.txt", std::ios_base::out | std::ios_base::app);
		fileOut << std::setw(16) << std::left << newUser.Name() << "\t" << std::setw(16) << std::left << newUser.Password() << "\n";
		fileOut.close();

		std::cout << "Account created!" << std::endl;

		system("pause");
		return 1;
	}

	//Sprawdza czy podany u�ytkownik znajduje si� w bazie u�ytkownik�w i zwraca u�ytkownika znalezionego lub pustego
	User doesUserExist(std::string name, std::string password) {
		for (User user : _userList) {								//Przej�cie po wszystkich u�ytkownikach w bazie
			if (user.logIn(name, password)) {						//Sprawdzenie dopasowania nazwy i has�a(Do zmiany bo to s�aby spos�b)
				return user;										//Je�li znaleziono zwr�� u�ytkownika z zmienionym statusem na zalagowanego
			}
		}

		User empty;													//Je�li nie znaleziono dopasowania nazwy i has�a nast�puje stworzneie pustego u�ytkownika
		std::cout << "Name or password incorrect!" << std::setw(64) << std::endl;
		system("pause");
		return empty;												//Zwr�cenie pustego u�ytkownika z statusem niezalogowanego co powoduje brak zmian w sekcji menu
	}

	//Sprawdza czy podana nazwa u�ytkownika jest ju� zaj�ta
	bool isNameTaken(std::string name) {
		for (User user : _userList) {
			if (user.Name() == name) {								//Je�li nazwa istnieje zwr�� true
				return 1;
			}
		}
		return 0;													//Je�li nie znaleziono takiej nazwy zwr�c false
	}

	void vectorInitialization(){
		std::ifstream fileIn("accList.txt");
		std::string line{};

		while (getline(fileIn, line)) {
			std::string username;
			std::string password;

			std::stringstream ss(line);

			ss >> username;
			ss >> password;

			User loadedUser(username, password);

			_userList.push_back(loadedUser);
		}

		fileIn.close();
	}
};

//Start menu
int menu1() {
	int choice{ 0 };
	std::cout << std::setw(64);
	std::cout << "--- SDMS ---" << std::setw(64) << std::endl;
	std::cout << "-----------" << std::setw(64) << std::endl;
	std::cout << "1. Log in  " << std::setw(64) << std::endl;
	std::cout << "2. Make acc" << std::setw(64) << std::endl;
	std::cout << "3. Exit    " << std::setw(64) << std::endl;
	std::cout << "-----------" << std::setw(64) << std::endl;
	std::cin >> choice;
	return choice;
}

//Sekcja menu odpowiedzialna za logowanie si� na istniej�ce konto
User logInMenu(UserContainer& allAcc) {
	std::string name{};
	std::string password{};

	std::cout << "--- SDMS ---" << std::setw(64) << std::endl;
	std::cout << "-----------" << std::setw(64) << std::endl;
	std::cout << "Name: " << std::setw(64);
	std::cin >> name;
	std::cout << "Password: " << std::setw(64);
	std::cin >> password;

	return allAcc.doesUserExist(name, password);
}

//Sekcja menu odpowiedzialna za tworzenie nowego konta
void createAccMenu(UserContainer& allAcc) {
	std::string name{};
	std::string password{};

	std::cout << "--- SDMS ---" << std::setw(64) << std::endl;
	std::cout << "-----------" << std::setw(64) << std::endl;
	std::cout << "Name: " << std::setw(64);
	std::cin >> name;
	std::cout << "Password: " << std::setw(64);
	std::cin >> password;

	allAcc.addingNewUser(name, password);

}

int main() {

	UserContainer allAcc;										//Dane wszystkich u�ytkownik�w. Docelowo inicjalizowane z pliku po ka�dorazowym odpaleniem programu
	User mainAcc;												//Konto sesji. Jest to szufladka na aktualnie zalogowanego u�ytkownika kt�ry znajduje si� w allAcc

	allAcc.vectorInitialization();

	// Sekcja menu. Trwa tak d�ugo, a� u�ytkownik si� nie zaloguje lub nie wyjdzie z programu
	while(mainAcc.Logged() == 0)
	{
		int choice = menu1();

		system("CLS");

		if (choice == 1) mainAcc = logInMenu(allAcc);			//Zalogowanie si�
		else if (choice == 2) createAccMenu(allAcc);			//Rejestracja
		else return 1;											//Wyj�cie z programu

		system("CLS");
	}

	//Sekcja sesji. Trwa tak d�ugo, a� u�ytkownik nie wyloguje si� lub nie wyjdzie z programu
	while (mainAcc.Logged() == 1) {

	}

	return 0;
}