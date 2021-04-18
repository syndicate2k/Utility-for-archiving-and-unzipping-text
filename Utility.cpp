#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstring>

using namespace std;

/* 	
	To do list:
	1) Implement bor (2 algorithm)
	2) Speed up the merge
	3) Simplify functions
	4) Russian localization	
	5) Reading from file
*/

string string_for_code;
string string_for_decode;

map<string, int> dictionary;
map<string, char> table_code;
map<char, string> table_decode;

const int NMAX = 100;
const int K = 36; 

void init_bor();
void add_string(const string &s);
void merge_cells();
void dictionary_create(string s);
void table_create();
void code_string_operation();
void decode_string_operation();
void panic();
void menu_init();

string compression(string s);
string decode(string s);

int go(int v, char c);

struct vertex {
	int next[K];
	bool leaf;
	int p;
	char pch;
	int link;
	int go[K];
};

vertex t[NMAX+1];
int sz;

void init_bor() {
	t[0].p = -1;
	t[0].link = -1;
	
	memset (t[0].next, 255, sizeof t[0].next);
	memset (t[0].go, 255, sizeof t[0].go);
	
	sz = 1;
	
	return;
}

void add_string (const string &s) {
	int v = 0;
	for (int i = 0; i < s.length(); i++) {
		char c = s[i] - 'a';
		if (t[v].next[c] == -1) {
			memset (t[sz].next, 255, sizeof t[sz].next);
			memset (t[sz].go, 255, sizeof t[sz].go);
			
			t[sz].link = -1;
			t[sz].p = v;
			t[sz].pch = c;
			t[v].next[c] = sz++;
		}
		v = t[v].next[c];
	}
	t[v].leaf = true;
	
	return;
}

int get_link (int v) {
	if (t[v].link == -1)
		if (v == 0 || t[v].p == 0)
			t[v].link = 0;
		else
			t[v].link = go (get_link (t[v].p), t[v].pch);
			
	return t[v].link;
}

int go (int v, char c) {
	if (t[v].go[c] == -1)
		if (t[v].next[c] != -1)
			t[v].go[c] = t[v].next[c];
		else{
			if(v == 0) t[v].go[c] = 0;
			else t[v].go[c] = go(get_link(v), c);
		}
		
	return t[v].go[c];
}

void merge_cells() {
	map<string, int>::iterator i, j;

	i = dictionary.begin();
	j = dictionary.begin();
	j++;

	if(j == dictionary.end() || i == dictionary.end()) return;

	string s1, s2;
	for(i; i != dictionary.end(); i++) {
		s1 = (*i).first;
		
		if(s1 == "") continue;
		for(j; j != dictionary.end(); j++) {
			s2 = (*j).first;
			
			if(s2 == "") continue;
			if(string_for_code.find(s1 + s2) != -1){
				/*
				map<string, int>::iterator ti = i, tj = j;
				dictionary.erase(ti);
				dictionary.erase(tj);
				*/
				
				dictionary[s1 + s2]++;
				
				/*
				i = dictionary.begin();
				j = i;
				j++;
				*/
			}else if(string_for_code.find(s2 + s1) != -1){
				/*
				map<string, int>::iterator ti = i, tj = j;
				dictionary.erase(ti);
				dictionary.erase(tj);
				*/
				
				dictionary[s2 + s1]++;
				
				/*
				i = dictionary.begin();
				j = i;
				j++;
				*/
			}
		}
	}

	return;
}

void dictionary_create(string s) {
	string temp;

	for(int l = 0; l < s.size(); l++) {
		temp = "";
		temp += s[l];

		if(dictionary[temp] > 0) {
			string temp2 = "";
			
			for(int j = l + 1; j < s.size(); j++) {
				temp2 = "";
				temp2 += s[j];
				
				if(dictionary[temp2] > 0) {
					temp += s[j];
					dictionary[temp + temp2]++;
				} else {
					break;
				}
			}

			dictionary[temp]++;
			l += temp.size() - 1;
		} else {
			dictionary[temp]++;
			continue;
		}
	}
	
	merge_cells();

	/*
	map<string, int>::iterator it = dictionary.begin();
	cout << endl;
	for(it; it != dictionary.end(); it++) {
		cout << (*it).first << " " << (*it).second << endl;
	}
	*/
	
	return;
}

void table_create() {
	if(dictionary.size() == 0) return;

	map<string, int>::iterator it;

	it = dictionary.begin();

	cout << endl << "Code table: " << endl;
	
	for(int i = 'a'; i <= ('a' + dictionary.size()) && it != dictionary.end(); i++) {
		table_code[(*it).first] = (char)i;
		table_decode[(char)i] = (*it).first;

		cout << (*it).first << " -> " << table_code[(*it).first] << endl;
		it++;
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
	cout << "Input your string" << endl;

	fflush(stdin);
	fflush(stdout);

	getline(cin, string_for_code);

	dictionary_create(string_for_code);
	table_create();

	string result = compression(string_for_code);
	cout << "Result of compression: " << result << endl;
	cout << "Source string length:" << string_for_code.size() << " & Result string length: " << result.size() << endl;

}

string decode(string s) {
	string source = "";

	for(int i = 0; i < s.size(); i++) {
		source += table_decode[s[i]];
	}

	return source;
}

void decode_string_operation() {

	cout << "Input your string for decode" << endl;
	fflush(stdin);
	fflush(stdout);

	getline(cin, string_for_decode);

	string result = decode(string_for_decode);
	cout << "Result of decoding: " << result << endl;

}

void panic() {
	cout << endl << "Bye!" << endl;
	exit(0);
}

void menu_init() {
	cout << "[ String code/decode utility ] " << endl;
	cout << endl << endl << endl;

	cout << "What do u want to do?" << endl;
	cout << "1. I want CODE string" << endl;
	cout << "2. I want DECODE string (NEED FIX)" << endl;
	cout << "3. I want EXIT" << endl << endl << endl;

	int operation;
	while(1) {
		cin >> operation;

		switch(operation) {
			case 1:
				code_string_operation();
				decode_string_operation(); // used for test
			case 2:
				decode_string_operation(); // don't use 2 operation!
			case 3:
				panic();
		}
	}
}

int main() {
	// setlocale(LC_ALL, "Russian");
	
	menu_init();
}

