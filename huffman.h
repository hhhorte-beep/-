#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <cstring>
#include <cstdlib>

struct HuffmanNode {
    char character;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;
    
    HuffmanNode(char ch, int freq);
    HuffmanNode(int freq, HuffmanNode* l, HuffmanNode* r);
    ~HuffmanNode();
    
    bool isLeaf() const;
};

class DynamicString {
private:
    char* data;
    int len;
    int capacity;
    
    void resize(int newCapacity);
    
public:
    DynamicString();
    DynamicString(const char* str);
    DynamicString(const DynamicString& other);
    ~DynamicString();
    
    DynamicString& operator=(const DynamicString& other);
    
    void append(char ch);
    void append(const char* str);
    void append(const DynamicString& other);
    
    const char* c_str() const;
    int length() const;
    char at(int index) const;
    void clear();
    DynamicString operator+(const DynamicString& other) const;
};

class PriorityQueue {
private:
    HuffmanNode** heap;
    int heapSize;
    int capacity;
    
    void resize();
    void heapifyUp(int index);
    void heapifyDown(int index);
    void swap(int i, int j);
    
public:
    PriorityQueue();
    ~PriorityQueue();
    
    void push(HuffmanNode* node);
    HuffmanNode* pop();
    HuffmanNode* top() const;
    int size() const;
    bool empty() const;
};

class HuffmanCoder {
private:
    HuffmanNode* root;
    char** codes;
    int freqTable[256];
    
    void buildFrequencyTable(const char* text);
    HuffmanNode* buildTree();
    void generateCodes(HuffmanNode* node, DynamicString prefix);
    void deleteTree(HuffmanNode* node);
    
public:
    HuffmanCoder();
    ~HuffmanCoder();
    
    DynamicString compress(const char* text);
    DynamicString decompress(const DynamicString& encoded, HuffmanNode* tree);
    
    int getFrequency(char ch) const;
    const char* getCode(char ch) const;
    HuffmanNode* getTree() const;
    double getCompressionRatio(const char* original, const DynamicString& encoded) const;
    
    struct CompressionResult {
        DynamicString encoded;
        HuffmanNode* tree;
        double ratio;
    };
    
    CompressionResult huffmanCompress(const char* text);
};

#endif