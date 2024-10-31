
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
            if (ltm->tm_sec / 10 % 2 == 0) {
                gamma = "ABCDEFGHIJKLMNOP"; // Чётные 10 секунд
            } else {
                gamma = "GHEFABCDOPMNIJKL"; // Нечётные 10 секунд
            }
            break;

        case 2: // Ш4.2.5 (маркант от дня месяца)
            if (ltm->tm_mday % 2 == 0) {
                gamma = "ABCDEFGHIJKLMNOP"; // Чётное число месяца
            } else {
                gamma = "GHEFABCDOPMNIJKL"; // Нечётное число месяца
            }
            break;

        case 3: // Ш4.2.6 (маркант от дня недели)
            switch (ltm->tm_wday) {
                case 0: // Воскресенье
                case 6: // Суббота
                    gamma = "GHEFCDABOPMNIJKL"; // Сб, Вс
                    break;
                case 1: // Понедельник
                case 3: // Среда
                case 5: // Пятница
                    gamma = "ABCDEFGHIJKLMNOP"; // Пн, Ср, Пт
                    break;
                case 2: // Вторник
                case 4: // Четверг
                    gamma = "GHEFOPMNABCDIJKL"; // Вт, Чт
                    break;
            }
            break;
    }

    // обеспечиваем длину гаммы равной BLOCK_SIZE
    while (gamma.size() < BLOCK_SIZE) {
        gamma += gamma; // повторяем гамму, чтобы достичь нужной длины
    }
    gamma.resize(BLOCK_SIZE); // обрезаем до нужной длины

    return gamma;
}


// функция шифрования/дешифрования методом XOR
string xorBlock(const string& text, const string& gamma, int MODULUS) {
    string result;
    for (size_t i = 0; i < text.size(); i++) {
            if (MODULUS == 69){
                result += text[i] ^ gamma[i % BLOCK_SIZE % MODULUS];  // XOR с гаммой, модуль используется для циклического доступа к гамме
            }
            else {
                 result += text[i] ^ gamma[i % BLOCK_SIZE];  // `BLOCK_SIZE` — это длина гаммы
            }
    }
    return result;
}


// функция для вычисления контрольной суммы
unsigned char calculateChecksum(const string& text) {
    unsigned char checksum = 0;
    for (char c : text) {
        checksum ^= c; // XOR для вычисления контрольной суммы
    }
    return checksum;
}


int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian"); // устанавливаем локаль для Windows-1251
    // Проверяем количество аргументов
        string inputFileName = argv[1]; // Имя входного файла
        MODULUS = stoi(argv[2]); // Модуль
        int methodChoice = stoi(argv[3]); // Выбор метода
        string gamma;


        ifstream inputFile(inputFileName, ios::binary);
        if (!inputFile.is_open()) {
            return 1;
        }

        // читаем содержимое файла
        string text((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
        inputFile.close();

        string resultText;

        // Генерация гаммы на основе метода
        if (methodChoice == 1 || methodChoice == 5 || methodChoice == 6) {
            char majorityAlphabet = detectMajorityAlphabet(text);
            gamma = generateGamma(majorityAlphabet); // Генерация гаммы
        } else if (methodChoice == 2 || methodChoice == 3 || methodChoice == 4) {
            gamma = generateTimeBasedGamma(methodChoice - 1); // Генерация гаммы на основе времени с вычитанием 1
        } else if (methodChoice == 7 || methodChoice == 8 || methodChoice == 9) {
            gamma = generateTimeBasedGamma(methodChoice - 6); // Генерация гаммы на основе времени с вычитанием 6
        }

        resultText = xorBlock(text, gamma, MODULUS);

        // Добавление байта длины или контрольной суммы, если это требуется
        if (methodChoice == 5 || methodChoice == 7 || methodChoice == 8 || methodChoice == 9) {
            unsigned char lengthByte = static_cast<unsigned char>(text.size());
            resultText += lengthByte; // Добавляем байт длины

        } else if (methodChoice == 6) {
            unsigned char checksumByte = calculateChecksum(text);
            resultText += checksumByte; // Добавляем байт контрольной суммы
        }

        // Сохраняем гамму в файл для последующего дешифрования
        ofstream gammaFile("gamma.txt", ios::binary);
        gammaFile << gamma;
        gammaFile.close();

         ofstream outputFile(inputFileName, ios::binary | ios::trunc);
        outputFile << resultText;
        outputFile.close();
        return 0;
}






