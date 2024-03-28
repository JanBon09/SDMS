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
	//Metoda sprawdza czy zewnêtrzne nazwa i has³o s¹ zgodne z wewnêtrzn¹ nazwem i has³em u¿ytkownika
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


	//Sekcja metod dostêpowych
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
	//Metoda podejmuje próbê dodania u¿ytkownika do bazy u¿ytkowników i informuje o rezultacie
	bool addingNewUser(std::string name, std::string password) {
		if (name == "" || password == ""){							//Nazwa ani has³o nie mo¿e pozostaæ polem pustym
			std::cout << "Name nor password can be left empty" << std::endl;
			system("pause");
			return 0;
		}
			
		if (isNameTaken(name)) {									//Sprawdzenie czy dana nazwa jest ju¿ zajêta
			std::cout << "Name is already taken" << std::endl;
			system("pause");
			return 0;
		}

		if (!passwordSignRequirmentsCheck(password)) {				//Sprawdzenie has³a pod k¹tem narzucony wymagañ: 1 cyfra, 1 du¿a litera i 1 znak specjalny
			std::cout << "Password must contain: 1 uppercase letter, 1 number and 1 special character" << std::endl;
			system("pause");
			return 0;
		}

		if (!passwordLengthRequirmentsCheck(password)) {			//Sprawdzenie has³a pod k¹tem iloœci znaków
			std::cout << "Password must be at least 8 characters long" << std::endl;
			system("pause");
			return 0;
		}

		User newUser(name, password);								//Stworzenie noweg u¿ytkownika

		_userList.push_back(newUser);								//Dodanie nowego u¿ytkownika do bazy u¿ytkowników

		std::ofstream fileOut("accList.txt", std::ios_base::out | std::ios_base::app);
		fileOut << std::setw(16) << std::left << newUser.Name() << "\t" << std::setw(16) << std::left << newUser.Password() << "\n";
		fileOut.close();

		std::cout << "Account created!" << std::endl;

		system("pause");
		return 1;
	}

	//Sprawdza czy podany u¿ytkownik znajduje siê w bazie u¿ytkowników i zwraca u¿ytkownika znalezionego lub pustego
	User doesUserExist(std::string name, std::string password) {
		for (User user : _userList) {								//Przejœcie po wszystkich u¿ytkownikach w bazie
			if (user.logIn(name, password)) {						//Sprawdzenie dopasowania nazwy i has³a(Do zmiany bo to s³aby sposób)
				return user;										//Jeœli znaleziono zwróæ u¿ytkownika z zmienionym statusem na zalagowanego
			}
		}

		User empty;													//Jeœli nie znaleziono dopasowania nazwy i has³a nastêpuje stworzneie pustego u¿ytkownika
		std::cout << "Name or password incorrect!" << std::setw(64) << std::endl;
		system("pause");
		return empty;												//Zwrócenie pustego u¿ytkownika z statusem niezalogowanego co powoduje brak zmian w sekcji menu
	}

	//Sprawdza czy podana nazwa u¿ytkownika jest ju¿ zajêta
	bool isNameTaken(std::string name) {
		for (User user : _userList) {
			if (user.Name() == name) {								//Jeœli nazwa istnieje zwróæ true
				return 1;
			}
		}
		return 0;													//Jeœli nie znaleziono takiej nazwy zwróc false
	}

	//Sprawdza czy has³o zawiera 1 du¿¹ litere, 1 znak specjalny i 1 cyfre
	bool passwordSignRequirmentsCheck(std::string password) {
		bool upperCase{ false };
		bool number{ false };
		bool specialCharacter{ false };

		for (char letter : password)
		{
			//Sprawdzenie odbywa siê przez zakresy wartoœci znaków ASCII
			int asciiCode = (int)letter;
			if ((asciiCode > 32 && asciiCode < 48) || (asciiCode > 57 && asciiCode < 65) || (asciiCode > 90 && asciiCode < 97)) {
				specialCharacter = true;
			}
			else if (asciiCode > 63 && asciiCode < 91) {
				upperCase = true;
			}
			else if (asciiCode > 47 && asciiCode < 58) {
				number = true;
			}
		}

		//Wszystkie warunki musz¹ byæ spe³nione
		if (upperCase && number && specialCharacter) {
			return true;
		}

		return false;
	}
	//Sprawdza czy has³o zawiera conajmniej 8 znaków
	bool passwordLengthRequirmentsCheck(std::string password) {
		if (password.length() > 7) {
			return true;
		}
		return false;
	}

	//Metoda wpisuj¹ca do pamiêci loginy i has³a w celu weryfikacji danych przy logowaniu
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
int startMenu() {
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

//Sekcja menu odpowiedzialna za logowanie siê na istniej¹ce konto
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

	UserContainer allAcc;										//Dane wszystkich u¿ytkowników. Docelowo inicjalizowane z pliku po ka¿dorazowym odpaleniem programu
	User mainAcc;												//Konto sesji. Jest to szufladka na aktualnie zalogowanego u¿ytkownika który znajduje siê w allAcc

	allAcc.vectorInitialization();

	// Sekcja menu. Trwa tak d³ugo, a¿ u¿ytkownik siê nie zaloguje lub nie wyjdzie z programu
	while(mainAcc.Logged() == 0){
		int choice = startMenu();

		system("CLS");

		switch(choice) {
			case 1:
				mainAcc = logInMenu(allAcc);
				break;

			case 2:
				createAccMenu(allAcc);
				break;

			default:
				return 1;
		}

		system("CLS");
	}

	std::string command;

	//Sekcja sesji. Trwa tak d³ugo, a¿ u¿ytkownik nie wyloguje siê lub nie wyjdzie z programu
	while (mainAcc.Logged() == 1) {
		
	}

	return 0;
}