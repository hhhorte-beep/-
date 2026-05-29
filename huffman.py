import heapq
from collections import Counter

class HuffmanNode:
    """Узел дерева Хаффмана."""
    def __init__(self, char, freq):
        self.char = char          # символ (для листьев), для внутренних узлов None
        self.freq = freq
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.freq < other.freq

def build_frequency_table(text):
    """Подсчёт частот символов."""
    return Counter(text)

def build_huffman_tree(freq_table):
    """Построение дерева Хаффмана."""
    if not freq_table:
        return None

    heap = [HuffmanNode(ch, f) for ch, f in freq_table.items()]
    heapq.heapify(heap)

    # Особый случай: только один символ
    if len(heap) == 1:
        return heap[0]

    while len(heap) > 1:
        left = heapq.heappop(heap)
        right = heapq.heappop(heap)
        merged = HuffmanNode(None, left.freq + right.freq)
        merged.left = left
        merged.right = right
        heapq.heappush(heap, merged)

    return heap[0]

def build_codes(node, prefix="", codes=None):
    """Рекурсивный обход дерева для генерации кодов."""
    if codes is None:
        codes = {}
    if node is None:
        return codes
    # Лист
    if node.char is not None:
        # Если дерево состоит из одного узла, кодом будет "0"
        codes[node.char] = prefix if prefix else "0"
    else:
        build_codes(node.left, prefix + "0", codes)
        build_codes(node.right, prefix + "1", codes)
    return codes

def encode(text, codes):
    """Кодирование строки в битовую последовательность (строка из '0'/'1')."""
    if not text:
        return ""
    return "".join(codes[ch] for ch in text)

def decode(encoded, root):
    """Декодирование битовой последовательности."""
    if not encoded or root is None:
        return ""
    # Особый случай: дерево из одного листа (все символы одинаковы)
    if root.left is None and root.right is None:
        # Каждый бит игнорируется, возвращаем символ столько раз, сколько битов
        return root.char * len(encoded)

    result = []
    node = root
    for bit in encoded:
        node = node.left if bit == "0" else node.right
        if node.char is not None:   # достигли листа
            result.append(node.char)
            node = root
    return "".join(result)

def compression_ratio(original, encoded):
    """Коэффициент сжатия."""
    if not original:
        return 0.0
    return 1 - len(encoded) / (len(original) * 8)

def huffman_compress(text):
    """Полный цикл сжатия: возвращает (закодированная_строка, дерево, коды, коэффициент)."""
    if not text:
        return "", None, {}, 0.0

    freq = build_frequency_table(text)
    tree = build_huffman_tree(freq)
    codes = build_codes(tree)
    encoded = encode(text, codes)
    ratio = compression_ratio(text, encoded)
    return encoded, tree, codes, ratio

