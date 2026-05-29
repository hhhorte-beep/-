#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;

struct Node {
    char ch;
    int freq;
    Node *left, *right;
    
    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
    Node(int f, Node* l, Node* r) : ch('\0'), freq(f), left(l), right(r) {}
};

class MinHeap {
private:
    Node** arr;
    int size;
    int capacity;
    
    void resize() {
        capacity = capacity == 0 ? 4 : capacity * 2;
        Node** newArr = new Node*[capacity];
        for (int i = 0; i < size; i++) newArr[i] = arr[i];
        delete[] arr;
        arr = newArr;
    }
    
    void heapifyUp(int idx) {
        while (idx > 0) {
            int parent = (idx - 1) / 2;
            if (arr[parent]->freq <= arr[idx]->freq) break;
            swap(arr[parent], arr[idx]);
            idx = parent;
        }
    }
    
    void heapifyDown(int idx) {
        while (true) {
            int left = 2 * idx + 1;
            int right = 2 * idx + 2;
            int smallest = idx;
            
            if (left < size && arr[left]->freq < arr[smallest]->freq) smallest = left;
            if (right < size && arr[right]->freq < arr[smallest]->freq) smallest = right;
            
            if (smallest == idx) break;
            swap(arr[idx], arr[smallest]);
            idx = smallest;
        }
    }
    
public:
    MinHeap() : arr(nullptr), size(0), capacity(0) {}
    
    ~MinHeap() { delete[] arr; }
    
    void push(Node* node) {
        if (size >= capacity) resize();
        arr[size++] = node;
        heapifyUp(size - 1);
    }
    
    Node* pop() {
        if (size == 0) return nullptr;
        Node* result = arr[0];
        arr[0] = arr[--size];
        heapifyDown(0);
        return result;
    }
    
    int getSize() { return size; }
};

class Huffman {
private:
    Node* root;
    char* codes[256];
    int freq[256];
    
    void countFreq(const char* text) {
        for (int i = 0; i < 256; i++) freq[i] = 0;
        for (int i = 0; text[i]; i++) freq[(unsigned char)text[i]]++;
    }
    
    Node* buildTree() {
        MinHeap heap;
        
        for (int i = 0; i < 256; i++) {
            if (freq[i] > 0) {
                heap.push(new Node((char)i, freq[i]));
            }
        }
        
        if (heap.getSize() == 0) return nullptr;
        if (heap.getSize() == 1) return heap.pop();
        
        while (heap.getSize() > 1) {
            Node* left = heap.pop();
            Node* right = heap.pop();
            Node* parent = new Node(left->freq + right->freq, left, right);
            heap.push(parent);
        }
        
        return heap.pop();
    }
    
    void genCodes(Node* node, char* prefix, int depth) {
        if (!node) return;
        
        if (!node->left && !node->right) {
            codes[(unsigned char)node->ch] = new char[depth + 1];
            for (int i = 0; i < depth; i++) {
                codes[(unsigned char)node->ch][i] = prefix[i];
            }
            codes[(unsigned char)node->ch][depth] = '\0';
            return;
        }
        
        if (node->left) {
            prefix[depth] = '0';
            genCodes(node->left, prefix, depth + 1);
        }
        if (node->right) {
            prefix[depth] = '1';
            genCodes(node->right, prefix, depth + 1);
        }
    }
    
    void deleteTree(Node* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
    
public:
    Huffman() : root(nullptr) {
        for (int i = 0; i < 256; i++) codes[i] = nullptr;
    }
    
    ~Huffman() {
        deleteTree(root);
        for (int i = 0; i < 256; i++) delete[] codes[i];
    }
    
    string compress(const char* text) {
        if (!text || !text[0]) return "";
        
        countFreq(text);
        root = buildTree();
        
        for (int i = 0; i < 256; i++) {
            delete[] codes[i];
            codes[i] = nullptr;
        }
        
        char prefix[256];
        genCodes(root, prefix, 0);
        
        string result;
        for (int i = 0; text[i]; i++) {
            result += codes[(unsigned char)text[i]];
        }
        return result;
    }
    
    string decompress(const string& encoded) {
        if (encoded.empty() || !root) return "";
        
        string result;
        Node* current = root;
        
        if (!root->left && !root->right) {
            return string(encoded.length(), root->ch);
        }
        
        for (char bit : encoded) {
            current = (bit == '0') ? current->left : current->right;
            if (!current->left && !current->right) {
                result += current->ch;
                current = root;
            }
        }
        
        return result;
    }
    
    double getRatio(const char* original, const string& encoded) {
        if (!original || !original[0]) return 0.0;
        int origBits = strlen(original) * 8;
        return 1.0 - (double)encoded.length() / origBits;
    }
    
    const char* getCode(char ch) const {
        return codes[(unsigned char)ch];
    }
};

double entropy(const char* text) {
    int freq[256] = {0};
    int len = 0;
    for (int i = 0; text[i]; i++) {
        freq[(unsigned char)text[i]]++;
        len++;
    }
    if (len == 0) return 0.0;
    
    double e = 0.0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            double p = (double)freq[i] / len;
            e -= p * log2(p);
        }
    }
    return e;
}

int main() {
    cout << "========================================" << endl;
    cout << "   АЛГОРИТМ ХАФФМАНА - ТЕСТИРОВАНИЕ" << endl;
    cout << "========================================" << endl;
    
    const char* tests[] = {
        "abracadabra",
        "aaaaaabbbbccdd",
        "hello huffman",
        "abcdef",
        "The quick brown fox jumps over the lazy dog"
    };
    
    for (int t = 0; t < 5; t++) {
        Huffman huff;
        const char* text = tests[t];
        
        cout << "\n--- Тест " << (t+1) << ": \"" << text << "\"" << endl;
        
        string encoded = huff.compress(text);
        string decoded = huff.decompress(encoded);
        double ratio = huff.getRatio(text, encoded);
        double ent = entropy(text);
        
        // Подсчет средней длины
        double avgLen = 0;
        int len = strlen(text);
        for (int i = 0; i < len; i++) {
            const char* code = huff.getCode(text[i]);
            if (code) avgLen += strlen(code);
        }
        avgLen /= len;
        
        cout << "Исходный размер: " << len * 8 << " бит" << endl;
        cout << "Сжатый размер: " << encoded.length() << " бит" << endl;
        cout << "Коэффициент сжатия: " << ratio * 100 << "%" << endl;
        cout << "Энтропия: " << ent << " бит/символ" << endl;
        cout << "Ср. длина Хаффмана: " << avgLen << " бит/символ" << endl;
        cout << "Декодирование: " << (decoded == text ? "УСПЕШНО" : "ОШИБКА") << endl;
        
        // Вывод кодов
        cout << "Коды символов:" << endl;
        bool printed[256] = {false};
        for (int i = 0; i < len; i++) {
            unsigned char uc = (unsigned char)text[i];
            if (!printed[uc]) {
                printed[uc] = true;
                cout << "  '" << text[i] << "': " << huff.getCode(text[i]) << endl;
            }
        }
    }
    
    cout << "\n========================================" << endl;
    cout << "         ТЕСТИРОВАНИЕ ЗАВЕРШЕНО" << endl;
    cout << "========================================" << endl;
    
    return 0;
}
