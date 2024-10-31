#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cassert>
#include <clocale>

using namespace std;

// размер блока и модуль гаммирования
const int BLOCK_SIZE = 16;
int MODULUS;

// функция для определения, каких символов в тексте больше — латинских или кириллических
char detectMajorityAlphabet(const string& text) {
    int latinCount = 0;
    int cyrillicCount = 0;

    for (unsigned char c : text) {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
            latinCount++;
        } else if (c >= 0xC0 && c <= 0xFF) { // кириллические символы в Windows-1251
            cyrillicCount++;
        }
    }
    return (latinCount > cyrillicCount) ? 'L' : 'C';
}

// Функция для создания гаммы на основе выбранного алфавита
string generateGamma(char alphabetType) {
    string gamma;

    if (alphabetType == 'L') {
        gamma = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    } else {
        gamma = "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЫЭЮЯ";
    }
    // Генерируем гамму длиной BLOCK_SIZE с использованием модуля
    string result;
    srand(static_cast<unsigned int>(time(nullptr))); // Инициализируем генератор случайных чисел
    for (int i = 0; i < BLOCK_SIZE; ++i) {
        result += gamma[(rand() % gamma.size()) % MODULUS]; // Применение модуля гаммирования
    }
    return result;
}

// функция для создания гаммы на основе времени (Ш4.2.4, Ш4.2.5, Ш4.2.6)
string generateTimeBasedGamma(int methodChoice) {
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    string gamma;

    switch (methodChoice) {
        case 1: // Ш4.2.4 (маркант от секунд)
            gamma = (ltm->tm_sec / 10 % 2 == 0) ? "ABCDEFGHIJKLMNOP" : "GHEFABCDOPMNIJKL";
            break;

        case 2: // Ш4.2.5 (маркант от дня месяца)
            gamma = (ltm->tm_mday % 2 == 0) ? "ABCDEFGHIJKLMNOP" : "GHEFABCDOPMNIJKL";
            break;

        case 3: // Ш4.2.6 (маркант от дня недели)
            switch (ltm->tm_wday) {
                case 0: case 6:
                    gamma = "GHEFCDABOPMNIJKL";
                    break;
                case 1: case 3: case 5:
                    gamma = "ABCDEFGHIJKLMNOP";
                    break;
                case 2: case 4:
                    gamma = "GHEFOPMNABCDIJKL";
                    break;
            }
            break;
    }

    while (gamma.size() < BLOCK_SIZE) {
        gamma += gamma;
    }
    gamma.resize(BLOCK_SIZE);

    return gamma;
}

// функция шифрования/дешифрования методом XOR
string xorBlock(const string& text, const string& gamma) {
    string result;
    for (size_t i = 0; i < text.size(); i++) {
        result += text[i] ^ gamma[i % BLOCK_SIZE];  // XOR с гаммой
    }
    return result;
}

// функция для вычисления контрольной суммы
unsigned char calculateChecksum(const string& text) {
    unsigned char checksum = 0;
    for (char c : text) {
        checksum ^= c;
    }
    return checksum;
}

// функция для удаления байта длины или контрольной суммы
string removeLastByte(const string& text) {
    return (text.size() > 1) ? text.substr(0, text.size() - 1) : text;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian"); // устанавливаем локаль для Windows-1251

    if (argc < 5) {
        cerr << "Неверное количество аргументов." << endl;
        return 1;
    }

    string inputFileName = argv[1];
    string gammaFileName = argv[2];
    char hasLengthByte = (argv[3][0] == 'y');
    char hasSummByte = (argv[4][0] == 'y');

    // Чтение файла с исходным текстом
    ifstream inputFile(inputFileName, ios::binary);
    if (!inputFile.is_open()) {
        cerr << "Не удалось открыть файл: " << inputFileName << endl;
        return 1;
    }
    string text((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    inputFile.close();

    // Чтение файла с гаммой
    ifstream gammaFile(gammaFileName, ios::binary);
    if (!gammaFile.is_open()) {
        cerr << "Не удалось открыть файл с гаммой: " << gammaFileName << endl;
        return 1;
    }
    string gamma;
    getline(gammaFile, gamma);
    gammaFile.close();

    if (gamma.size() != BLOCK_SIZE) {
        cerr << "Длина гаммы должна быть равна " << BLOCK_SIZE << endl;
        return 1;
    }

    // Дешифрование текста
    string resultText = xorBlock(text, gamma);

    // Удаление байта длины и контрольной суммы при необходимости
    if (hasLengthByte) {
        resultText = removeLastByte(resultText);
    }
    if (hasSummByte) {
        resultText = removeLastByte(resultText);
    }

    // Сохранение результата в файл
    ofstream outputFile(inputFileName, ios::binary | ios::trunc);
    outputFile << resultText;
    outputFile.close();

    cout << "Дешифрование завершено." << endl;
    return 0;
}
