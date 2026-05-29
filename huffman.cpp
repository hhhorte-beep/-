#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Узел дерева Хаффмана
struct Node {
    char ch;
    int freq;
    Node *left, *right;

    Node(char character, int frequency) : ch(character), freq(frequency), left(nullptr), right(nullptr) {}
    Node(int frequency, Node* l, Node* r) : ch('\0'), freq(frequency), left(l), right(r) {}
};

// Компаратор для min-heap (приоритетная очередь)
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

// Подсчёт частот символов
unordered_map<char, int> buildFrequencyTable(const string& text) {
    unordered_map<char, int> freqTable;
    for (char c : text) {
        freqTable[c]++;
    }
    return freqTable;
}

// Построение дерева Хаффмана
Node* buildHuffmanTree(const unordered_map<char, int>& freqTable) {
    priority_queue<Node*, vector<Node*>, Compare> minHeap;

    for (const auto& pair : freqTable) {
        minHeap.push(new Node(pair.first, pair.second));
    }

    // Особый случай: один символ
    if (minHeap.size() == 1) {
        return minHeap.top();
    }

    while (minHeap.size() > 1) {
        Node* left = minHeap.top(); minHeap.pop();
        Node* right = minHeap.top(); minHeap.pop();

        Node* parent = new Node(left->freq + right->freq, left, right);
        minHeap.push(parent);
    }

    return minHeap.top();
}

// Рекурсивная генерация кодов
void buildCodes(Node* root, const string& prefix, unordered_map<char, string>& codes) {
    if (!root) return;

    // Лист
    if (root->ch != '\0') {
        // Для дерева из одного узла код "0"
        codes[root->ch] = prefix.empty() ? "0" : prefix;
        return;
    }

    buildCodes(root->left, prefix + "0", codes);
    buildCodes(root->right, prefix + "1", codes);
}

// Кодирование
string encode(const string& text, const unordered_map<char, string>& codes) {
    string result;
    for (char c : text) {
        result += codes.at(c);
    }
    return result;
}

// Декодирование
string decode(const string& encoded, Node* root) {
    if (encoded.empty() || !root) return "";

    // Особый случай: один символ
    if (!root->left && !root->right) {
        return string(encoded.length(), root->ch);
    }

    string result;
    Node* current = root;
    for (char bit : encoded) {
        current = (bit == '0') ? current->left : current->right;
        if (current->ch != '\0') {
            result.push_back(current->ch);
            current = root;
        }
    }
    return result;
}

// Коэффициент сжатия
double compressionRatio(const string& original, const string& encoded) {
    if (original.empty()) return 0.0;
    return 1.0 - static_cast<double>(encoded.length()) / (original.length() * 8);
}

// Полный цикл сжатия
tuple<string, Node*, unordered_map<char, string>, double> huffmanCompress(const string& text) {
    if (text.empty()) {
        return {"", nullptr, {}, 0.0};
    }

    auto freq = buildFrequencyTable(text);
    Node* tree = buildHuffmanTree(freq);
    unordered_map<char, string> codes;
    buildCodes(tree, "", codes);
    string encoded = encode(text, codes);
    double ratio = compressionRatio(text, encoded);

    return {encoded, tree, codes, ratio};
}

// Очистка памяти (обход дерева)
void deleteTree(Node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}