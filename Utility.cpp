#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <windows.h>

using namespace std;

string string_for_code; // Строка для кодирования
string string_for_decode; // Строка для декодирования

string get_new_index(); // Получение нового гнезда
string file_name; // Имя файла
string compression(string s); // Сжатие строки
string decode(string s); // Декодирование

map<string, int> dictionary; // Словарь
map<string, string> table_code; // Таблица для кодирования
map<string, string> table_decode; // Таблица для декодирования

vector<int> transitions; // Вектор переходов на новую строку

void dictionary_create(string s); // Создание словаря
void table_create(); // Создание таблицы кодирования и декодирования
void code_string_operation(); // Операция "Кодирование"
void decode_string_operation(); // Операция "Декодирование"
void panic(); // Выход
void menu_init(); // Инициализация меню
void merge(); // Укрепление подряд идущих гнезд

int get_medium(); // Вычисление медианы частот

ifstream in; // Поток ввода
ofstream out; // Поток вывода

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // Дескриптор устройства вывода

int last_i_digraph;
int last_j_digraph;

bool error;

int get_medium() {
	int sum = 0;

	map<string, int>::iterator it = dictionary.begin();

	for(it; it != dictionary.end(); it++) {
		sum += (*it).second;
	}

	return sum / dictionary.size();
}

void dictionary_create(string s) {
	string match = "";
	string last_match = "";

	int count = 0;
	int last_count = 0;
	int i = 0;
	int por = 2;
	int med = 0;

	bool flag = 0;

	while(true) {
		if(!flag) {
			i = 0;
		} else {
			flag = 0;
		}

		match = s[i];
		i++;

		while(i < s.size()) {
			map<string, int>::iterator it = dictionary.lower_bound(match + s[i]);

			if(it == dictionary.end()) {
				break;
			}

			if((*it).first.size() < (match.size()) || ((*it).first.size() > (match.size() + 1) && (*it).first.substr(match.size() + 1) != (match + s[i]))) {
				break;
			}

			match += s[i];
			i++;
		}

		if(match.size() != 1) {
			count = ++dictionary[match];
		} else {
			match = s[i];
			dictionary[match] = 1;
		}

		if(count >= por && last_count >= por) {
			i += match.size() - 1;
			flag = 1;

			last_match = match;
			last_count = count;
		} else {
			dictionary[last_match + match]++;

			s = s.substr(last_match.size() + match.size() , s.size() - (last_match.size() + match.size()));

			last_match = "";
			last_count = 0;
		}

		med = get_medium();

		if(dictionary.size() >= 40) {
			map<string, int> newdictionary;
			map<string, int>::iterator it = dictionary.begin();

			for(it; it != dictionary.end(); it++) {
				if((*it).second >= med) {
					newdictionary[(*it).first] = (*it).second;
				} else {
					size_t pos = s.find((*it).first);

					if (pos == string::npos) {
						last_count = 0;
						last_match = "";
					}
				}
			}

			dictionary = newdictionary;
		}

		if(s == "") {
			break;
		}
	}

	return;
}

void table_create() {
	if(dictionary.size() == 0) return;

	map<string, int>::iterator it;

	it = dictionary.begin();

	for(int i = '!'; i <= '~' && it != dictionary.end(); i++) {
		last_i_digraph = i;

		for(int j = '!'; j <= '~' && it != dictionary.end(); j++) {
			if(i == j) {
				continue;
			}

			if(dictionary[(*it).first] > 0) {
				string index = "";
				index += (char)i;
				index += (char)j;

				table_code[(*it).first] = index;
				table_decode[index] = (*it).first;

				it++;
			} else {
				it++;
				i--;
			}

			last_j_digraph = j;
		}
	}
	return;
}

string compression(string s) {
	string temp;
	string result = "";

	for(int l = 0; l < s.size(); l++) {
		temp = "";
		temp += s[l];

		string temp2 = temp;
		for(int j = l + 1; j < s.size(); j++) {
			temp2 += s[j];

			if(dictionary[temp2] > 0) {
				temp = temp2;
			} else {
				continue;
			}
		}

		result += table_code[temp];
		l += temp.size() - 1;
	}
	return result;
}

void code_string_operation() {
	fflush(stdin);
	fflush(stdout);

	cout << "Enter file name" << endl;

	getline(cin, file_name);

	file_name += ".txt";

	in.open(file_name);
	out.open("output.txt");

	if(!in.is_open()) {
		SetConsoleTextAttribute(hConsole, 77);
		cout << "An error has occurred!" << endl;

		SetConsoleTextAttribute(hConsole, 15);

		error = true;

		return;
	}

	string temp_input;
	while(getline(in, temp_input)) {
		string_for_code += temp_input;
		transitions.push_back(string_for_code.size() - 1);
	}

	if(string_for_code == "") {
		return;
	}

	dictionary_create(string_for_code);
	table_create();

	string result = compression(string_for_code);
	string_for_decode = result;
	merge();

	cout << "Result of compression: " << string_for_decode << endl;
	cout << "Source string length:" << string_for_code.size() << " & Result string length: " << string_for_decode.size() << endl;

	out << "Encoded text:";
	out << string_for_decode;

	cout << endl << "Line break indexes in source text:";
	out << endl << "Line break indexes in source text:";

	for(int i = 0; i < transitions.size(); i++) {
		cout << transitions[i] << " ";
		out << transitions[i] << " ";
	}

	cout << endl;

	in.close();
	out.close();
}

string decode(string s) {
	string source = "";

	for(int i = 0; i + 1 < s.size(); i++) {
		string index = "";

		index += s[i];
		index += s[i + 1];

		source += table_decode[index];
	}

	return source;
}

void decode_string_operation() {
	fflush(stdin);
	fflush(stdout);

	if(error) {
		return;
	}

	string result = decode(string_for_decode);

	SetConsoleTextAttribute(hConsole, 77);
	cout << endl << "CHECK" << endl;

	SetConsoleTextAttribute(hConsole, 15);
	cout << "Result of decoding:" << endl;

	int pos_endl = 0;
	for(int i = 0; i < result.size(); i++) {
		cout << result[i];

		if(i == transitions[pos_endl]) {
			cout << endl;
			pos_endl++;
		}
	}
}

string get_new_index() {
	if(last_j_digraph < 127) {
		last_j_digraph++;
	} else {
		last_i_digraph++;
	}

	string new_index = "";
	new_index += (char)last_i_digraph;
	new_index += (char)last_j_digraph;

	return new_index;
}

void merge() {
	string initial_string = string_for_decode;

	vector<string> temp;

	string for_pb = "";
	for_pb += initial_string[0];
	for_pb += initial_string[1];

	temp.push_back(for_pb);

	int k = 1;
	for(int i = 2; i + 1 < initial_string.size(); i += 2) {
		for_pb = "";

		for_pb += initial_string[i];
		for_pb += initial_string[i + 1];

		if(temp[temp.size() - 1] == (for_pb)) {
			k++;
		} else {
			if(k > 1) {
				string new_item = "";

				for(int j = 0; j < k; j++) {
					new_item += temp[temp.size() - 1];
				}

				string new_index = get_new_index();

				table_code[decode(new_item)] = new_index;
				table_decode[new_index] = decode(new_item);

				temp.pop_back();
				temp.push_back(new_index);

			}
			k = 1;

			temp.push_back(for_pb);
		}
	}

	string result_of_merge = "";

	for(int i = 0; i + 1 < temp.size(); i += 2) {
		string new_item = "";

		new_item += temp[i];
		new_item += temp[i + 1];


		string new_index = get_new_index();

		table_code[decode(new_item)] = new_index;
		table_decode[new_index] = decode(new_item);

		result_of_merge += new_index;
	}

	cout << endl << "Code table: " << endl;
	out << endl << "Code table: " << endl;

	map<string, string>:: iterator itt = table_decode.begin();
	for(itt; itt != table_decode.end(); itt++) {
		cout << (*itt).first << " -> " << (*itt).second << endl;
		out << (*itt).first << " -> " << (*itt).second << endl;
	}
	cout << endl;

	string_for_decode = result_of_merge;
}

void decode_selected() {
	table_code.clear();
	table_decode.clear();
	transitions.clear();

	cout << "Enter the length of the dictionary" << endl;

	int dictionary_length;
	cin >> dictionary_length;

	cout << "Enter the dictionary in the format: 'string' -> 'digraph' " << endl;

	string s1_input, s2_input;
	for(int i = 0; i < dictionary_length; i++) {
		cin >> s1_input >> s2_input;

		table_code[s1_input] = s2_input;
		table_decode[s2_input] = s1_input;
	}

	cout << "Enter the encoded string" << endl;
	cin >> string_for_decode;

	cout << "Enter size and transitions array" << endl;

	int transitions_size;
	cin >> transitions_size;

	int temp_ind;
	for(int i = 0; i < transitions.size(); i++) {
		cin >> temp_ind;

		transitions.push_back(temp_ind);
	}

	string result = decode(string_for_decode);

	SetConsoleTextAttribute(hConsole, 40);
	cout << "Source string: " << endl;

	SetConsoleTextAttribute(hConsole, 15);

	int pos_endl = 0;
	for(int i = 0; i < result.size(); i++) {
		cout << result[i];

		if(transitions.size() > 0 && i == transitions[pos_endl]) {
			cout << endl;
			pos_endl++;
		}
	}
}

void panic() {
	cout << endl << "Bye!" << endl;
	exit(0);
}

void menu_init() {
	SetConsoleTextAttribute(hConsole, 15);

	cout << endl << endl;
	cout << "What do u want to do?" << endl;
	cout << "1. I want CODE string" << endl;
	cout << "2. I want DECODE string" << endl;
	cout << "3. I want EXIT" << endl;

	int operation;
	while(1) {
		cin >> operation;

		switch(operation) {
			case 1:
				code_string_operation();
				decode_string_operation();
				break;
			case 2:
				decode_selected();
				break;
			case 3:
				panic();
				break;
		}
		menu_init();
	}
}

int main() {
	setlocale(LC_ALL, "Russian");

	SetConsoleTextAttribute(hConsole, 40);
	cout << " String encoding utility " << endl << endl << endl << endl;

	menu_init();
}
