#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

class Node {
    public:
        int data;
        unsigned char byte;
        Node *left;
        Node *right;

        Node (int d, unsigned char b, Node *l, Node *r) {
            data = d;
            byte = b;
            left = l;
            right = r;
        }
};

map< string, unsigned char > unhuff;
unsigned char buffer[10000000];

void generate_tree() ;

void error(string msg) 
{
    cout << msg << endl;
    exit(1);
}

int main()
{
    generate_tree();

    /*for (map< string, unsigned char > :: iterator it = unhuff.begin(); it != unhuff.end(); it++) {
        cout << it -> second << " " << it -> first << endl;
    }*/


    return 0;
}

void generate_tree() 
{
    FILE *fp = fopen("tree.txt", "r");
    if (fp == NULL) {
        error("Error opening tree file");
    }

    while (1) {
        int inp;
        unsigned char y;
        char arr[300];
        fscanf(fp, "%d %s", &inp, arr);
        string str (arr);
        y = inp;
        if (y == 0 && str == "0") {
            break;
        }
        unhuff[str] = y;
    }
    fclose(fp);
}




