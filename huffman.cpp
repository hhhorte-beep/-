#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

// Huffman Tree Node
struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;

    Node(char character, int frequency) {
        ch = character;
        freq = frequency;
        left = nullptr;
        right = nullptr;
    }

    Node(int frequency, Node* l, Node* r) {
        ch = '\0';
        freq = frequency;
        left = l;
        right = r;
    }
};

// Comparator for Min Heap
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

// Build Frequency Table
unordered_map<char, int> buildFrequencyTable(const string& text) {
    unordered_map<char, int> freqTable;

    for (char c : text) {
        freqTable[c]++;
    }

    return freqTable;
}

// Build Huffman Tree
Node* buildHuffmanTree(const unordered_map<char, int>& freqTable) {
    priority_queue<Node*, vector<Node*>, Compare> minHeap;

    for (auto it = freqTable.begin(); it != freqTable.end(); ++it) {
        minHeap.push(new Node(it->first, it->second));
    }

    if (minHeap.empty())
        return nullptr;

    if (minHeap.size() == 1)
        return minHeap.top();

    while (minHeap.size() > 1) {
        Node* left = minHeap.top();
        minHeap.pop();

        Node* right = minHeap.top();
        minHeap.pop();

        Node* parent =
            new Node(left->freq + right->freq, left, right);

        minHeap.push(parent);
    }

    return minHeap.top();
}

// Generate Huffman Codes
void buildCodes(
    Node* root,
    string code,
    unordered_map<char, string>& codes) {

    if (!root)
        return;

    if (!root->left && !root->right) {
        if (code.empty())
            code = "0";

        codes[root->ch] = code;
        return;
    }

    buildCodes(root->left, code + "0", codes);
    buildCodes(root->right, code + "1", codes);
}

// Encode Text
string encode(
    const string& text,
    const unordered_map<char, string>& codes) {

    string encoded;

    for (char c : text) {
        encoded += codes.at(c);
    }

    return encoded;
}

// Decode Text
string decode(const string& encoded, Node* root) {

    if (!root)
        return "";

    if (!root->left && !root->right) {
        return string(encoded.size(), root->ch);
    }

    string result;
    Node* current = root;

    for (char bit : encoded) {

        if (bit == '0')
            current = current->left;
        else
            current = current->right;

        if (!current->left && !current->right) {
            result += current->ch;
            current = root;
        }
    }

    return result;
}

// Compression Ratio
double compressionRatio(
    const string& original,
    const string& encoded) {

    if (original.empty())
        return 0.0;

    double originalBits =
        static_cast<double>(original.size()) * 8.0;

    double compressedBits =
        static_cast<double>(encoded.size());

    return (originalBits - compressedBits) / originalBits;
}

// Full Compression Process
tuple<string, Node*, unordered_map<char, string>, double>
huffmanCompress(const string& text) {

    auto freqTable = buildFrequencyTable(text);

    Node* tree = buildHuffmanTree(freqTable);

    unordered_map<char, string> codes;

    buildCodes(tree, "", codes);

    string encoded = encode(text, codes);

    double ratio =
        compressionRatio(text, encoded);

    return make_tuple(
        encoded,
        tree,
        codes,
        ratio
    );
}

// Delete Tree
void deleteTree(Node* root) {

    if (!root)
        return;

    deleteTree(root->left);
    deleteTree(root->right);

    delete root;
}

// Main Function
int main() {

    string text;

    cout << "Enter text: ";
    getline(cin, text);

    if (text.empty()) {
        cout << "Empty string!" << endl;
        return 0;
    }

    auto result = huffmanCompress(text);

    string encoded = get<0>(result);
    Node* tree = get<1>(result);
    unordered_map<char, string> codes = get<2>(result);
    double ratio = get<3>(result);

    cout << "\n===== HUFFMAN CODES =====\n";

    for (auto it = codes.begin(); it != codes.end(); ++it) {

        if (it->first == ' ')
            cout << "' ' -> ";
        else
            cout << it->first << " -> ";

        cout << it->second << endl;
    }

    cout << "\n===== RESULT =====\n";

    cout << "Original text:\n";
    cout << text << endl;

    cout << "\nEncoded string:\n";
    cout << encoded << endl;

    cout << "\nDecoded string:\n";
    cout << decode(encoded, tree) << endl;

    cout << "\nOriginal size: "
        << text.size() * 8
        << " bits" << endl;

    cout << "Compressed size: "
        << encoded.size()
        << " bits" << endl;

    cout << "Compression ratio: "
        << ratio * 100
        << "%" << endl;

    deleteTree(tree);

    return 0;
}
