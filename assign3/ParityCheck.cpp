#include <bits/stdc++.h>
using namespace std;

void generateParity();
void checkParity();

int main() 
{
    while (1) {
        cout << "> ";
        string choice;
        cin >> choice;
        if (choice == "generate") {
            generateParity();
        } else if (choice == "check") {
            checkParity();
        } else if (choice == "exit") {
            break;
        }
    }
    return 0;
}

int countSetBits(string data)
{
    int count = 0;
    for (int i = 0; i < data.length(); i++) {
    	if (data[i] == '1') {
    		count++;
        }
    }
    return count;
}

void generateParity()
{
    cout << "Data : ";
    string data;
    cin >> data;
    int setBits = countSetBits(data);
    string setBit;
    if (setBits % 2 == 0) {
    	setBit = "0";
    } else {
    	setBit = "1";
    }
    string generatedString = data + setBit;
    cout << generatedString << endl;
}
void checkParity() 
{
    cout << "GeneratedString : ";
    string data;
    cin >> data;
    int setBits = countSetBits(data);
    string setBit;
    if (setBits % 2 == 0) {
    	cout << "No Error Detected" << endl;
    } else {
    	cout << "Error Detected" << endl;
    }
}
