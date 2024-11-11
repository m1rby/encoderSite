#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cassert>
#include <thread>
#include <chrono>

using namespace std;
using namespace chrono;
using namespace this_thread;

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

// Генерирование случайной гаммы на основе преимущественного алфавита
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

// функция для создания гаммы на основе времени
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
string xorBlock(const string& text, const string& gamma) {
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

// функция для вычисления контрольной суммы, не зависящей от времени
unsigned char calculateChecksum(const string& text) {
    unsigned char checksum = 0;
    for (char c : text) {
        checksum ^= c; // XOR для вычисления контрольной суммы
    }
    return checksum;
}

// функция для вычисления контрольной суммы, зависящей от времени
unsigned char calculateTimeDependentChecksum(const string& text) {
    unsigned char checksum = 0;
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);

    for (char c : text) {
        checksum ^= c; // XOR для базовой контрольной суммы
    }

    // Добавляем значение текущей секунды для зависимости от времени
    checksum ^= static_cast<unsigned char>(ltm->tm_sec); // или tm_min, tm_hour для других временных параметров

    return checksum;
}

// функция для удаления байта длины или контрольной суммы
string removeLastByte(const string& text) {
    return (text.size() > 1) ? text.substr(0, text.size() - 1) : text;
}

void showHelp() {
    wcout << L"Использование:\n"
         << L"  encoder.exe [команда] [аргументы]\n\n"
         << L"Команды:\n"
         << L"  help                   - Показать это справочное сообщение со всеми доступными командами.\n"
         << L"  list                   - Показать список всех доступных методов шифрования.\n"
         << L"  encrypt <inputFile> <MODULUS> <methodChoice>\n"
         << L"                        - Зашифровать файл с использованием указанного модуля и метода шифрования.\n"
         << L"                          Аргументы:\n"
         << L"                            <inputFile>     - Путь к файлу для шифрования.\n"
         << L"                            <MODULUS>       - Модуль для гаммы (2 или 69).\n"
         << L"                            <methodChoice>  - Метод шифрования (например, '1', '2').\n"
         << L"\n"
         << L"  decrypt <inputFile> <gammaFile> <lengthByte> <checksumByte>\n"
         << L"                        - Расшифровать файл с использованием указанного файла гаммы.\n"
         << L"                          Аргументы:\n"
         << L"                            <inputFile>     - Путь к файлу для расшифрования.\n"
         << L"                            <gammaFile>     - Путь к файлу гаммы, использованному при шифровании.\n"
         << L"                            <lengthByte>    - Укажите 1, если байт длины присутствует; в противном случае - 0.\n"
         << L"                            <checksumByte>  - Укажите 1, если байт контрольной суммы присутствует; в противном случае - 0.\n"
         << L"\n"
         << L"Примеры:\n"
         << L"  Шифрование файла:\n"
         << L"    encoder.exe encrypt input.txt 2 1\n"
         << L"      - Шифрует файл 'input.txt' с модулем 2, используя метод '1 - 4.1.1 и 4.1.2 Блочный шифр гаммирования'.\n\n"
         << L"  Дешифрование файла:\n"
         << L"    encoder.exe decrypt input.txt gamma.txt 0 0\n"
         << L"      - Расшифровывает 'input.txt' с использованием 'gamma.txt' без байта длины и без байта контрольной суммы.\n\n";
}

void showListMethods() {
    wcout << L"Методы шифрования: " << endl;
    wcout << L"1 - 4.1.1 и 4.1.2 Блочный шифр гаммирования" << endl;
    wcout << L"2 - 4.2.1 и 4.2.4 Блочный шифр гаммирования с маркантом от секунд" << endl;
    wcout << L"3 - 4.2.2 и 4.2.5 Блочный шифр гаммирования с маркантом от числа месяца" << endl;
    wcout << L"4 - 4.2.3 и 4.2.6 Блочный шифр гаммирования с маркантом от дня недели" << endl;

    wcout << L"5 - 4.3.1 и 4.3.2 Блочный шифр гаммирования с полем длины открытого текста" << endl;
    wcout << L"6 - 4.4.1 и 4.4.2 Блочный шифр гаммирования с контрольной суммой" << endl;

    wcout << L"7 - 4.5.1 и 4.5.4 Блочный шифр гаммирования с маркантом от секунд и полем длины открытого текста" << endl;
    wcout << L"8 - 4.5.2 и 4.5.5 Блочный шифр гаммирования с маркантом от числа месяца и полем длины открытого текста" << endl;
    wcout << L"9 - 4.5.3 и 4.5.6 Блочный шифр гаммирования с маркантом от дня недели и полем длины открытого текста" << endl;

    wcout << L"10 - 4.6.1.1 и 4.6.1.4 Блочный шифр гаммирования с маркантом от секунд и контрольной суммы, не зависящей от времени" << endl;
    wcout << L"11 - 4.6.1.2 и 4.6.1.5 Блочный шифр гаммирования с маркантом от числа месяца и контрольной суммы, не зависящей от времени" << endl;
    wcout << L"12 - 4.6.1.3 и 4.6.1.6 Блочный шифр гаммирования с маркантом от дня недели и контрольной суммы, не зависящей от времени" << endl;

    wcout << L"13 - 4.6.2.1 и 4.6.2.4 Блочный шифр гаммирования с маркантом от секунд и контрольной суммы, зависящей от времени" << endl;
    wcout << L"14 - 4.6.2.2 и 4.6.2.5 Блочный шифр гаммирования с маркантом от числа месяца и контрольной суммы, зависящей от времени" << endl;
    wcout << L"15 - 4.6.2.3 и 4.6.2.6 Блочный шифр гаммирования с маркантом от дня недели и контрольной суммы, зависящей от времени" << endl;

    wcout << L"16 - 4.7.1 и 4.7.2 Блочный шифр гаммирования с полем длины открытого текста и контрольной суммой" << endl;

    wcout << L"17 - 4.8.1.1 и 4.8.1.4 Блочный шифр гаммирования с маркантом от секунд, полем длины открытого текста и контрольной суммой, не зависящей от времени" << endl;
    wcout << L"18 - 4.8.1.2 и 4.8.1.5 Блочный шифр гаммирования с маркантом от числа месяца, полем длины открытого текста и контрольной суммой, не зависящей от времени" << endl;
    wcout << L"19 - 4.8.1.3 и 4.8.1.6 Блочный шифр гаммирования с маркантом от дня недели, полем длины открытого текста и контрольной суммой, не зависящей от времени" << endl;

    wcout << L"20 - 4.8.2.1 и 4.8.2.4 Блочный шифр гаммирования с маркантом от секунд, полем длины открытого текста и контрольной суммой, зависящей от времени" << endl;
    wcout << L"21 - 4.8.2.2 и 4.8.2.5 Блочный шифр гаммирования с маркантом от числа месяца, полем длины открытого текста и контрольной суммой, зависящей от времени" << endl;
    wcout << L"22 - 4.8.2.3 и 4.8.2.6 Блочный шифр гаммирования с маркантом от дня недели, полем длины открытого текста и контрольной суммой, зависящей от времени" << endl;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");

    wcout << L"Encoder.exe" << endl;

    int methodChoice, hasLengthByte, hasSumByte;;
    string inputFileName, gamma, resultText, gammaFileName;

    string command = argv[1];

    if (argc < 2) {
        wcout << L"Используйте команду 'help' для просмотра подробной справочной информации." << endl;
        showHelp();
        return 8;
    }

    if (command == "help") {
        showHelp();
    } else if (command == "list") {
        showListMethods();
    } else if (command == "encrypt") {

        if(argc != 5) {
            wcout << L"Недопустимое количество аргументов." << endl;
            wcout << L"Проверьте правильность выполнения команды: <encrypt> <inputFile> <MODULUS> <methodChoice>" << endl;
            wcout << L"Используйте команду 'help' для просмотра подробной справочной информации." << endl;
            return 1;
        }

        inputFileName = argv[2];
        MODULUS = stoi(argv[3]);
        methodChoice = stoi(argv[4]);

        // Открытие файла
        ifstream inputFile(inputFileName, ios::binary);
        if (!inputFile.is_open()) {
            wcout << L"Ошибка при открытии файла." << endl;
            return 2;
        }

        // Чтение содержимого
        string text((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
        inputFile.close();

        // Генерация гаммы на основе метода
        if (methodChoice == 1 || methodChoice == 5 || methodChoice == 6 || methodChoice == 16) {
            char majorityAlphabet = detectMajorityAlphabet(text);
            gamma = generateGamma(majorityAlphabet); // Генерация гаммы
        } else if (methodChoice == 2 || methodChoice == 3 || methodChoice == 4) {
            gamma = generateTimeBasedGamma(methodChoice - 1); // Генерация гаммы на основе времени с вычитанием 1
        } else if (methodChoice == 7 || methodChoice == 8 || methodChoice == 9) {
            gamma = generateTimeBasedGamma(methodChoice - 6); // Генерация гаммы на основе времени с вычитанием 6
        } else if (methodChoice == 10 || methodChoice == 11 || methodChoice == 12) {
            gamma = generateTimeBasedGamma(methodChoice - 9); // Генерация гаммы на основе времени с вычитанием 9
        } else if (methodChoice == 13 || methodChoice == 14 || methodChoice == 15) {
            gamma = generateTimeBasedGamma(methodChoice - 12); // Генерация гаммы на основе времени с вычитанием 12
        } else if (methodChoice == 17 || methodChoice == 18 || methodChoice == 19) {
            gamma = generateTimeBasedGamma(methodChoice - 16); // Генерация гаммы на основе времени с вычитанием 16
        } else if (methodChoice == 20 || methodChoice == 21 || methodChoice == 22) {
            gamma = generateTimeBasedGamma(methodChoice - 19); // Генерация гаммы на основе времени с вычитанием 19
        }

        resultText = xorBlock(text, gamma);

        if(methodChoice >=23)
        {
            wcout << L"Ошибка при выборе способа шифрования." << endl;
            return 3;
        }

        // Добавление байта открытой длины (Ш4.3.1-Ш4.3.2, Ш4.5.1-Ш4.5.6)
        if (methodChoice == 5 || methodChoice == 7 || methodChoice == 8 || methodChoice == 9) {
            unsigned char lengthByte = static_cast<unsigned char>(text.size());
            resultText += lengthByte; // Добавляем байт длины
        }

        // Добавление контрольной суммы, не зависящей от времени (Ш4.4.1-Ш4.4.2, Ш4.6.1.1-Ш4.6.1.6)
        if (methodChoice == 6 || methodChoice == 10 || methodChoice == 11 || methodChoice == 12) {
            unsigned char checksumByte = calculateChecksum(text);
            resultText += checksumByte; // Добавляем байт контрольной суммы
        }

        // Добавление контрольной суммы, зависящей от времени (Ш4.6.2.1-Ш4.6.2.6)
        if (methodChoice == 13 || methodChoice == 14 || methodChoice == 15) {
            unsigned char timeDependentChecksum = calculateTimeDependentChecksum(text);
            resultText += timeDependentChecksum; // Добавляем байт контрольной суммы
        }

        // Добаление байта длины и контрольной суммы, не зависящей от времени (Ш4.7.1-Ш4.7.2, Ш4.8.1.1-Ш4.8.1.6)
        if (methodChoice == 16 || methodChoice == 17 || methodChoice == 18 || methodChoice == 19)
        {
            unsigned char lengthByte = static_cast<unsigned char>(text.size());
            unsigned char checksumByte = calculateChecksum(text);
            resultText += lengthByte; // Добавляем байт длины
            resultText += checksumByte; // Добавляем байт контрольной суммы
        }

        // Добавление байта длины и контрольной суммы, зависящей от времени (Ш4.8.2.1-Ш4.8.2.6)
        if (methodChoice == 20 || methodChoice == 21 || methodChoice == 22)
        {
            unsigned char lengthByte = static_cast<unsigned char>(text.size());
            unsigned char timeDependentChecksum = calculateTimeDependentChecksum(text);
            resultText += lengthByte; // Добавляем байт длины
            resultText += timeDependentChecksum; // Добавляем байт контрольной суммы
        }

        wcout << L"Шифрование файла..." << endl;
        sleep_for(seconds(1));

        wcout << L"Зашифрованный текст: ";
        cout << resultText << endl;

        wcout << L"Гамма: ";
        cout << gamma << endl;

        // Сохраняем гамму в файл для последующего дешифрования
        ofstream gammaFile("gamma.txt", ios::binary);
        gammaFile << gamma;
        gammaFile.close();

        ofstream outputFile(inputFileName, ios::binary | ios::trunc);
        outputFile << resultText;
        outputFile.close();

    }   else if (command == "decrypt") {

        if(argc != 6) {
            wcout << L"Недопустимое количество аргументов." << endl;
            wcout << L"Проверьте правильность выполнения команды: <decrypt> <inputFile> <gammaFile> <lenghtByte> <checksumByte>" << endl;
            wcout << L"Используйте команду 'help' для просмотра подробной справочной информации." << endl;
            return 4;
        }

        inputFileName = argv[2];
        gammaFileName = argv[3];
        hasLengthByte = stoi(argv[4]) != 0;
        hasSumByte = stoi(argv[5]) != 0;

        // Открытие файла
        ifstream inputFile(inputFileName, ios::binary);
        if (!inputFile.is_open()) {
            wcout << L"Ошибка при открытии файла." << endl;
            return 5;
        }

        // Чтение содержимого
        string text((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
        inputFile.close();

        // Чтение файла с гаммой
        ifstream gammaFile(gammaFileName, ios::binary);
        if (!gammaFile.is_open()) {
            wcout << L"Не удалось открыть файл с гаммой: ";
            cout << gammaFileName << endl;
            return 6;
        }

        getline(gammaFile, gamma);
        gammaFile.close();

        if (gamma.size() != BLOCK_SIZE) {
            wcout << L"Длина гаммы должна быть равна: " << BLOCK_SIZE << L" байт." << endl;
            return 7;
        }

        wcout << L"Дешифрование файла..." << endl;
        sleep_for(seconds(1));
        string resultText = xorBlock(text, gamma);

        // Удаление байта длины и контрольной суммы при необходимости
        if (hasLengthByte = 1) {
            resultText = removeLastByte(resultText);
        }

        if (hasSumByte == 1) {
            resultText = removeLastByte(resultText);
        }

        wcout << L"Расшифрованный текст: ";
        cout << resultText << endl;

        // Сохранение результата в файл
        ofstream outputFile(inputFileName, ios::binary | ios::trunc);
        outputFile << resultText;
        outputFile.close();

    } else {
        wcout << L"Ошибка при выборе режима работы программы." << endl;
        wcout << L"Используйте команду 'help' для просмотра подробной справочной информации." << endl;
        return 8;
    }

    return 0;
}
