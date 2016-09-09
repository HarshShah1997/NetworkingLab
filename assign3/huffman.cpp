#include <bits/stdc++.h>
#include <unistd.h>
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

unsigned char buffer[1000000];
map< unsigned char, string > huff;

void error(string msg)
{
    cout << msg << endl;
    exit(1);
}

int main()
{
    FILE *fp = fopen("a.out", "rb");
    if (fp == NULL) {
        error("Error opening file");
    }

    size_t bytes_read;
    bytes_read = fread(buffer, sizeof(unsigned char), sizeof(buffer), fp);
    if (bytes_read == 0) {
        error("Error reading file");
    }
    fclose(fp);
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

    //cout << pq.top().first << endl;
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

    //cout << (pq.top().second -> right) -> data << endl;
    assign(pq.top().second, "");

    //FILE *out = fopen("output", "wb");

    for (int i = 0; i < bytes_read; i++) {
        cout << huff[buffer[i]];
    }
    cout << endl;



    return 0;
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

