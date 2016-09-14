#include <algorithm>
#include <iostream>
#include <cstdio>
#include <queue>
#include <string>
#include <map>
#include <unistd.h>
#define MAX_SIZE 10000000
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
unsigned char buffer[MAX_SIZE];
unsigned char output_buffer[MAX_SIZE];
int output_buffer_size = 0;

int generate_tree(char filename[]);
int fillBuffer(FILE *fp);
void write_output(char output_file[]);
void fillOutputBuffer(size_t bytes_read);

void error(string msg) 
{
    cout << msg << endl;
    exit(1);
}

int main(int argc, char *argv[])
{

    /*for (map< string, unsigned char > :: iterator it = unhuff.begin(); it != unhuff.end(); it++) {
        cout << it -> second << " " << it -> first << endl;
    }*/
    if (argc != 3) {
        error("Usage: uncompress inputfile outputfile");
    }

    size_t bytes_read = generate_tree(argv[1]);

    fillOutputBuffer(bytes_read);

    write_output(argv[2]);
    return 0;
}

int generate_tree(char filename[]) 
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        error("Error opening file");
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
    size_t bytes_read = fillBuffer(fp);
    fclose(fp);
    return bytes_read;
}

int fillBuffer(FILE *fp)
{
    size_t bytes_read;
    bytes_read = fread(buffer, sizeof(unsigned char), sizeof(buffer), fp);
    if (bytes_read == 0) {
        error("Error reading file");
    }
    return bytes_read;
}

void fillOutputBuffer(size_t bytes_read)
{
    string currstr = "";
    for (int i = 0; i < bytes_read; i++) {
        unsigned char curr_byte = buffer[i];
        for (int pos = 7; pos >= 0; pos--) {
            unsigned char res = buffer[i] & (1 << pos);
            if (res == 0) {
                currstr += "0";
            } else {
                currstr += "1";
            }
            if (unhuff.find(currstr) != unhuff.end()) {
                //cout << unhuff[currstr] << endl;
                output_buffer[output_buffer_size] = unhuff[currstr];
                output_buffer_size++;
                currstr = "";
            }
        }
    }
}

void write_output(char output_file[])
{
    FILE *out = fopen(output_file, "wb");
    if (out == NULL) {
        error("Error opening output file");
    }
    size_t bytes_written = fwrite(output_buffer, sizeof(unsigned char), output_buffer_size, out);
    if (bytes_written == 0) {
        error("Error writing to output file");
    }
    fclose(out);
}

