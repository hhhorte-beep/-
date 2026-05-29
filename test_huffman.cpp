// simple_huffman.cpp - Упрощенная реализация в одном файле
#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

// Структура узла дерева
struct Node {
    char ch;
    int freq;
    Node *left, *right;
    
    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
    Node(int f, Node* l, Node* r) : ch('\0'), freq(f), left(l), right(r) {}
};

// Простая очередь с приоритетом (максимум 256 узлов)
class SimpleQueue {
private:
    Node* arr[256];
    int size;
    
public:
    SimpleQueue() : size(0) {}
    
    void push(Node* node) {
        arr[size++] = node;
        // Сортировка по частоте (пузырьком для простоты)
        for (int i = size - 1; i > 0; i--) {
            if (arr[i]->freq < arr[i-1]->freq) {
                swap(arr[i], arr[i-1]);
            } else break;
        }
    }
    
    Node* pop() {
        if (size == 0) return nullptr;
        Node* result = arr[0];
        for (int i = 0; i < size - 1; i++) {
            arr[i] = arr[i+1];
        }
        size--;
        return result;
    }
    
    int getSize() { return size; }
};

// Подсчет частот
void countFrequencies(const char* text, int freq[256]) {
    for (int i = 0; i < 256; i++) freq[i] = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        freq[(unsigned char)text[i]]++;
    }
}

// Построение дерева Хаффмана
Node* buildTree(int freq[256]) {
    SimpleQueue q;
    
    // Добавляем все символы с ненулевой частотой
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            q.push(new Node((char)i, freq[i]));
        }
    }
    
    if (q.getSize() == 0) return nullptr;
    if (q.getSize() == 1) return q.pop();
    
    while (q.getSize() > 1) {
        Node* left = q.pop();
        Node* right = q.pop();
        Node* parent = new Node(left->freq + right->freq, left, right);
        q.push(parent);
    }
    
    return q.pop();
}

// Генерация кодов
void generateCodes(Node* root, char* prefix, int depth, char* codes[256]) {
    if (!root) return;
    
    if (!root->left && !root->right) {
        // Лист - сохраняем код
        codes[(unsigned char)root->ch] = new char[depth + 1];
        for (int i = 0; i < depth; i++) {
            codes[(unsigned char)root->ch][i] = prefix[i];
        }
        codes[(unsigned char)root->ch][depth] = '\0';
        return;
    }
    
    if (root->left) {
        prefix[depth] = '0';
        generateCodes(root->left, prefix, depth + 1, codes);
    }
    if (root->right) {
        prefix[depth] = '1';
        generateCodes(root->right, prefix, depth + 1, codes);
    }
}

// Кодирование
string encode(const char* text, char* codes[256]) {
    string result;
    for (int i = 0; text[i] != '\0'; i++) {
        result += codes[(unsigned char)text[i]];
    }
    return result;
}

// Декодирование
string decode(const string& encoded, Node* root) {
    if (encoded.empty() || !root) return "";
    
    string result;
    Node* current = root;
    
    // Если дерево из одного узла
    if (!root->left && !root->right) {
        return string(encoded.length(), root->ch);
    }
    
    for (char bit : encoded) {
        if (bit == '0') current = current->left;
        else current = current->right;
        
        if (!current->left && !current->right) {
            result += current->ch;
            current = root;
        }
    }
    
    return result;
}

// Очистка дерева
void deleteTree(Node* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    cout << "========================================" << endl;
    cout << "   АЛГОРИТМ ХАФФМАНА - ТЕСТИРОВАНИЕ" << endl;
    cout << "========================================" << endl;
    
    // Тестовые строки
    const char* tests[] = {
        "abracadabra",
        "aaaaaabbbbccdd",
        "hello huffman",
        "abcdef",
        "The quick brown fox jumps over the lazy dog"
    };
    int numTests = 5;
    
    for (int t = 0; t < numTests; t++) {
        const char* text = tests[t];
        cout << "\n--- Тест " << (t+1) << ": \"" << text << "\"" << endl;
        
        int freq[256];
        char* codes[256] = {nullptr};
        char prefix[256];
        
        // 1. Подсчет частот
        countFrequencies(text, freq);
        
        // Вывод частот
        cout << "Частоты символов:" << endl;
        bool hasFreq = false;
        for (int i = 32; i < 128; i++) { // Только печатные символы
            if (freq[i] > 0) {
                cout << "  '" << (char)i << "': " << freq[i] << endl;
                hasFreq = true;
            }
        }
        if (!hasFreq) cout << "  (нет печатных символов)" << endl;
        
        // 2. Построение дерева
        Node* root = buildTree(freq);
        
        // 3. Генерация кодов
        generateCodes(root, prefix, 0, codes);
        
        // Вывод кодов
        cout << "Коды Хаффмана:" << endl;
        for (int i = 32; i < 128; i++) {
            if (codes[i]) {
                cout << "  '" << (char)i << "': " << codes[i] << endl;
            }
        }
        
        // 4. Кодирование
        string encoded = encode(text, codes);
        int originalBits = strlen(text) * 8;
        int compressedBits = encoded.length();
        double ratio = 1.0 - (double)compressedBits / originalBits;
        
        cout << "Исходный текст: \"" << text << "\"" << endl;
        cout << "Сжатый текст (биты): " << encoded << endl;
        cout << "Длина: " << strlen(text) << " символов = " << originalBits << " бит" << endl;
        cout << "Сжато: " << compressedBits << " бит" << endl;
        cout << "Коэффициент сжатия: " << ratio * 100 << "%" << endl;
        
        // 5. Декодирование
        string decoded = decode(encoded, root);
        cout << "Декодировано: \"" << decoded << "\"" << endl;
        
        // 6. Проверка
        if (decoded == text) {
            cout << "✓ Декодирование успешно!" << endl;
        } else {
            cout << "✗ ОШИБКА декодирования!" << endl;
        }
        
        // Очистка
        deleteTree(root);
        for (int i = 0; i < 256; i++) {
            delete[] codes[i];
        }
    }
    
    cout << "\n========================================" << endl;
    cout << "             ТЕСТИРОВАНИЕ ЗАВЕРШЕНО" << endl;
    cout << "========================================" << endl;
    
    return 0;
}