#include <bits/stdc++.h>
using namespace std;

string find_remainder(string data, string divisor);
void generate_crc();
void check_crc();
string check_remainder(string data, string divisor);

int main() 
{
    while (1) {
        cout << "> ";
        string choice;
        cin >> choice;
        if (choice == "generate") {
            generate_crc();
        } else if (choice == "check") {
            check_crc();
        } else if (choice == "exit") {
            break;
        }
    }
    return 0;
}

void generate_crc()
{
    cout << "Data: ";
    string data;
    cin >> data;

    string divisor = "1011";

    string remain = find_remainder(data, divisor);
    cout << "Remainder: " << remain << endl;
    cout << "CRC: " << data + remain << endl;
}

void check_crc() 
{
    cout << "CRC: ";
    string crc;
    cin >> crc;

    string divisor = "1011";

    string remain = check_remainder(crc, divisor);
    cout << "Remainder: " << remain << endl;
}

string find_remainder(string data, string divisor)
{   
    string result = data;

    for (int i = 0; i < divisor.length() - 1; i++) {
        result += "0";
    }

    for (int p2 = 0; p2 < result.length() - divisor.length() + 1; p2++) {
        if (result[p2] == '0') {
            continue;
        }
        for (int j = 0; j < divisor.length(); j++) {
            result[p2+j] = '0' + ((result[p2+j] - '0') ^ (divisor[j] - '0'));
        }
    }
    string remain;
    for (int i = result.length() - divisor.length() + 1; i < result.length(); i++) {
        remain += result[i];
    }
    return remain;
}

string check_remainder(string data, string divisor)
{
    string result = data;
    for (int p2 = 0; p2 < data.length() - divisor.length() + 1; p2++) {
        if (result[p2] == '0') {
            continue;
        }
        for (int j = 0; j < divisor.length(); j++) {
            result[p2+j] = '0' + ((result[p2+j] - '0') ^ (divisor[j] - '0'));
        }
    }
    return result;
}

