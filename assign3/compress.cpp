#include <bits/stdc++.h>
#include <unistd.h>
#define MAX_SIZE 10000000
using namespace std;

class Node {
    public:
        int data;
        unsigned char byte;
        Node *left;
        Node *right;

        Node(int d, unsigned char b, Node *l, Node *r) {
            data = d;
            byte = b;
            left = l;
            right = r;
        }
};

typedef pair<int, Node*> pqpair;

void assign(Node *currNode, string currStr);
Node *create_tree(priority_queue< pqpair, vector<pqpair>, greater<pqpair> > pq);
void write_tree();
int fillBuffer(char filename[]);
void fillOutputBuffer(size_t bytes_read);
void writeOutput(char filename[]);
void write_tree();

unsigned char buffer[MAX_SIZE];
unsigned char output_buffer[MAX_SIZE];
int output_buffer_size = 0;
map< unsigned char, string > huff;

void error(string msg)
{
    cout << msg << endl;
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        error("Usage: compress inputfile outputfile");
    }

    size_t bytes_read = fillBuffer(argv[1]);

    //cout << bytes_read << endl;

    map< unsigned char, int > freq;
    for (int i = 0; i < bytes_read; i++) {
        freq[buffer[i]]++;
    }

    /*for(int i = 0; i < 256; i++) {
      if (freq[i] != 0) {
      unsigned char x = i;
      cout << x << " " << freq[x] << endl;
      }
      }*/

    priority_queue< pqpair, vector<pqpair>, greater<pqpair> > pq;

    for (int i = 0; i < 256; i++) {
        if (freq[i] != 0) {
            char x = i;
            Node *newNode = new Node(freq[x], x, NULL, NULL);
            pq.push(make_pair(freq[x], newNode));
        }
    }

    Node *tree = create_tree(pq);

    assign(tree, "");

    fillOutputBuffer(bytes_read);

    writeOutput(argv[2]);

    write_tree();

    return 0;
}

int fillBuffer(char filename[])
{
    FILE *input = fopen(filename, "rb");
    if (input == NULL) {
        error("Error opening file");
    }

    size_t bytes_read;
    bytes_read = fread(buffer, sizeof(unsigned char), sizeof(buffer), input);
    if (bytes_read == 0) {
        error("Error reading file");
    }
    fclose(input);
    return bytes_read;
}

void assign(Node *currNode, string currStr)
{
    if (currNode == NULL) {
        return;
    }
    if (currNode -> left == NULL && currNode -> right == NULL) {
        huff[currNode -> byte] = currStr;
        //cout << currNode -> byte << " " << currStr << endl;
        return;
    }
    assign(currNode -> left, currStr + "0");
    assign(currNode -> right, currStr + "1");
}

Node *create_tree(priority_queue< pqpair, vector<pqpair>, greater<pqpair> > pq)
{
    while (pq.size() > 1) {
        int freq1 = pq.top().first;
        Node* min1 = pq.top().second;
        pq.pop();

        int freq2 = pq.top().first;
        Node* min2 = pq.top().second;
        pq.pop();

        int sum = freq1 + freq2;
        pq.push(make_pair(sum, new Node(sum, 0, min1, min2)));
    }
    return pq.top().second;
}

void fillOutputBuffer(size_t bytes_read)
{
    output_buffer[0] = 0;
    int count = 0;

    for (int i = 0; i < bytes_read; i++) {
        string temp = huff[buffer[i]];
        for (int j = 0; j < temp.size(); j++) {
            output_buffer[output_buffer_size] |= temp[j] - '0' << (8 - count - 1);
            count++;
            if (count == 8) {
                output_buffer_size++;
                output_buffer[output_buffer_size] = 0;
                count = 0;
            }
        }
    }
    if (count != 0) {
        output_buffer_size++;
    }
}

void writeOutput(char filename[])
{
    FILE *out = fopen(filename, "wb");
    if (out == NULL) {
        error("Error writing to file");
    }
    fwrite(output_buffer, sizeof(unsigned char), output_buffer_size, out);
    fclose(out);
}

void write_tree()
{
    FILE *out = fopen("tree.txt", "w");
    for (int i = 0; i < 256; i++) {
        unsigned char x = i;
        if (huff.find(x) != huff.end()) {
            fprintf(out, "%d %s\n", i, huff[x].c_str());
        }
    }
    fprintf(out, "%x %s\n", 0, "0");
    fclose(out);
}
