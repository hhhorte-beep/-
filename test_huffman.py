
import unittest
from huffman import huffman_compress, decode, build_frequency_table

class TestHuffman(unittest.TestCase):
    
    def test_frequency_table(self):
        print("\n--- Тест 1: Подсчёт частот ---")
        freq = build_frequency_table("abracadabra")
        print(f"Частоты для 'abracadabra': {freq}")
        self.assertEqual(freq["a"], 5)
        self.assertEqual(freq["b"], 2)
        print("✅ Тест пройден!")

    def test_empty_string(self):
        print("\n--- Тест 2: Пустая строка ---")
        encoded, tree, codes, ratio = huffman_compress("")
        print(f"Закодировано: '{encoded}'")
        print(f"Дерево: {tree}")
        print(f"Коды: {codes}")
        print(f"Коэффициент сжатия: {ratio}")
        self.assertEqual(encoded, "")
        self.assertEqual(ratio, 0.0)
        print("✅ Тест пройден!")

    def test_single_char_repeated(self):
        print("\n--- Тест 3: Один повторяющийся символ ---")
        text = "aaaa"
        encoded, tree, codes, ratio = huffman_compress(text)
        print(f"Исходный текст: '{text}'")
        print(f"Закодировано: '{encoded}'")
        print(f"Коды: {codes}")
        print(f"Коэффициент сжатия: {ratio:.2%}")
        decoded = decode(encoded, tree)
        print(f"Декодировано: '{decoded}'")
        self.assertEqual(codes['a'], "0")
        self.assertEqual(decoded, text)
        print("✅ Тест пройден!")

    def test_abracadabra(self):
        print("\n--- Тест 4: 'abracadabra' ---")
        text = "abracadabra"
        encoded, tree, codes, ratio = huffman_compress(text)
        print(f"Исходный текст: '{text}' (длина: {len(text)} символов)")
        print(f"Размер в ASCII: {len(text) * 8} бит")
        print(f"Размер после сжатия: {len(encoded)} бит")
        print(f"Коэффициент сжатия: {ratio:.2%}")
        print(f"Коды символов: {codes}")
        decoded = decode(encoded, tree)
        print(f"Декодировано: '{decoded}'")
        self.assertEqual(decoded, text)
        print("✅ Тест пройден!")

    def test_hello_huffman(self):
        print("\n--- Тест 5: 'hello huffman' ---")
        text = "hello huffman"
        encoded, tree, codes, ratio = huffman_compress(text)
        print(f"Исходный текст: '{text}'")
        print(f"Коды символов: {codes}")
        print(f"Закодировано: '{encoded}'")
        print(f"Коэффициент сжатия: {ratio:.2%}")
        decoded = decode(encoded, tree)
        print(f"Декодировано: '{decoded}'")
        self.assertEqual(decoded, text)
        print("✅ Тест пройден!")

    def test_all_unique_chars(self):
        print("\n--- Тест 6: Все символы уникальны ---")
        text = "abcdefgh"
        encoded, tree, codes, ratio = huffman_compress(text)
        print(f"Исходный текст: '{text}'")
        print(f"Количество уникальных символов: {len(codes)}")
        print(f"Коды: {codes}")
        print(f"Средняя длина кода: {len(encoded)/len(text):.2f} бит/символ")
        print(f"Коэффициент сжатия: {ratio:.2%}")
        decoded = decode(encoded, tree)
        print(f"Декодировано: '{decoded}'")
        self.assertEqual(decoded, text)
        print("✅ Тест пройден!")

    def test_long_text(self):
        print("\n--- Тест 7: Длинный текст ---")
        text = "a" * 100 + "b" * 50 + "c" * 30 + "d" * 20 + "e" * 10
        original_bits = len(text) * 8
        encoded, tree, codes, ratio = huffman_compress(text)
        print(f"Длина текста: {len(text)} символов")
        print(f"Исходный размер: {original_bits} бит")
        print(f"Сжатый размер: {len(encoded)} бит")
        print(f"Экономия: {original_bits - len(encoded)} бит")
        print(f"Коэффициент сжатия: {ratio:.2%}")
        print(f"Количество уникальных кодов: {len(codes)}")
        decoded = decode(encoded, tree)
        self.assertEqual(decoded, text)
        print("✅ Тест пройден!")

if __name__ == "__main__":
    print("\n" + "="*60)
    print(" ЗАПУСК ТЕСТОВ АЛГОРИТМА ХАФФМАНА")
    print("="*60)
    unittest.main()
