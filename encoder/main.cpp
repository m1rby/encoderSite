/**
 * @file main.cpp
 * @brief Главный файл программы, содержащий реализацию логики работы с файлами и многозадачности.
 *
 * Этот файл включает в себя основную логику программы. Он использует функции, объявленные в
 * заголовочном файле `main.h`, для чтения и записи файлов, шифрования данных, вычисления
 * контрольных сумм и многозадачности. Все операции выполняются с использованием стандартных
 * библиотек C++ для обработки файлов и потоков.
 *
 * Основные этапы работы программы:
 * 1. Чтение данных из файла с использованием многозадачности.
 * 2. Шифрование данных с использованием гамма-шифрования.
 * 3. Сохранение зашифрованных данных в новый файл с применением параллельной записи.
 * 4. Обработка ошибок и синхронизация потоков.
 *
 * @author IT Energy PLUS
 * @date 06/12/2024
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <mutex>
#include <ctime>
#include <cassert>
#include <thread>
#include <chrono>
#include <stdexcept>

using namespace std;
using namespace chrono;
using namespace this_thread;

/// Размер блока для шифрования/дешифрования.
const int BLOCK_SIZE = 16;

/// Модуль гаммирования, определяемый пользователем
int MODULUS;

#include "main.h"
#include "unitTests.h"

/**
 * @brief Главная функция программы.
 *
 * Обрабатывает аргументы командной строки, выбирает режим работы программы (шифрование или дешифрование),
 * генерирует гамму и выполняет соответствующие операции шифрования или дешифрования.
 *
 * @param argc Количество аргументов командной строки.
 * @param argv Аргументы командной строки.
 * @return Код завершения программы:
 * - 0 в случае успешного завершения
 * - 1, 2, 3, 4, 5, 6, 7, 8 при ошибках
 */
int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    srand(time(0));

    wcout << L"Программа encoder.exe запущена" << endl;

    int methodChoice, hasLengthByte, hasSumByte;;
    string inputFileName, gamma, gammaFileName, outputFileName;
    vector<string> encryptedParts;

    if (argc < 2) {
        wcout << L"Используйте команду 'help' для просмотра подробной справочной информации" << endl;
        return 8;
    }

    string command = argv[1];

    if (command == "help") {
        showHelp();
    } else if (command == "list") {
        showListMethods();
    } else if (command == "encrypt") {

        if(argc != 5) {
            wcout << L"Недопустимое количество аргументов" << endl;
            wcout << L"Проверьте правильность выполнения команды: <encrypt> <inputFile> <MODULUS> <methodChoice>" << endl;
            wcout << L"Используйте команду 'help' для просмотра подробной справочной информации" << endl;
            return 1;
        }

        inputFileName = argv[2];
        MODULUS = stoi(argv[3]);
        methodChoice = stoi(argv[4]);

        auto start = chrono::high_resolution_clock::now();

        wcout << L"Открытие и считывание файла: ";
        cout << inputFileName << endl;
        string text = readFileInParallel(inputFileName);
        wcout << L"Файл успешно открыт и считан" << endl;
        wcout << L"Размер файла: ";
        cout << text.size();
        wcout << L" байт" << endl;

        wcout << L"Генерируется гамма на основе выбранного метода..." << endl;

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
        } else {
            wcout << L"Ошибка при выборе способа шифрования" << endl;
            return 3;
        }

        wcout << L"Сгенерированная гамма: ";
        cout << gamma << endl;

        wcout << L"Шифрование содержимого файла..." << endl;
        string resultText = encryptFile(text, gamma);

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

        wcout << L"Содержимое файла успешно зашифровано" << endl;
        wcout << L"Размер содержимого файла после шифрования: ";
        cout << resultText.size();
        wcout << L" байт" << endl;

        string outputFileName = createNewFileName(inputFileName, "encrypted_");

        wcout << L"Сохранение гаммы и зашифрованного содержимого в новый файл..." << endl;
        saveFileParallel(outputFileName, resultText);

        wcout << L"Файл с зашифрованным содержимым успешно сохранен: ";
        cout << outputFileName << endl;

        // Сохраняем гамму в файл для последующего дешифрования
        string gammaFileName = createGammaFileName(inputFileName, "gamma.txt");
        ofstream gammaFile(gammaFileName, ios::binary);
        gammaFile << gamma;
        gammaFile.close();

        wcout << L"Гамма успешно сохранена в файл: ";
        cout << gammaFileName << endl;

        auto end = chrono::high_resolution_clock::now();

        double elapsedTime = chrono::duration<double>(end - start).count();
        wcout << L"Время работы программы: " << elapsedTime << L" секунд.\n";

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

        auto start = chrono::high_resolution_clock::now();

        wcout << L"Открытие и считывание файла: ";
        cout << inputFileName << endl;
        string text = readFileInParallel(inputFileName);
        wcout << L"Файл успешно открыт и считан" << endl;
        wcout << L"Размер файла: ";
        cout << text.size();
        wcout << L" байт" << endl;

        // Чтение файла с гаммой
        ifstream gammaFile(gammaFileName, ios::binary);
        if (!gammaFile.is_open()) {
            wcout << L"Не удалось открыть файл с гаммой: ";
            cout << gammaFileName << endl;
            return 6;
        }

        getline(gammaFile, gamma);
        gammaFile.close();

        wcout << L"Полученная гамма: ";
        cout << gamma << endl;

        if (gamma.size() != BLOCK_SIZE) {
            wcout << L"Длина гаммы должна быть равна: " << BLOCK_SIZE << L" байт." << endl;
            return 7;
        }

        wcout << L"Дешифрование содержимого файла..." << endl;
        string resultText = encryptFile(text, gamma);

        // Удаление байта длины и контрольной суммы при необходимости
        if (hasLengthByte == 1) {
            resultText = removeLastByte(resultText);
        }

        if (hasSumByte == 1) {
            resultText = removeLastByte(resultText);
        }

        wcout << L"Содержимое файла успешно расшифровано" << endl;
        wcout << L"Размер содержимого файла после дешифрования: ";
        cout << resultText.size();
        wcout << L" байт" << endl;

        string outputFileName = createNewFileName(inputFileName, "decrypted_");

        wcout << L"Сохранение расшифрованного содержимого в новый файл..." << endl;
        saveFileParallel(outputFileName, resultText);

        wcout << L"Файл с расшифрованным содержимым успешно сохранен: ";
        cout << outputFileName << endl;

        auto end = chrono::high_resolution_clock::now();

        double elapsedTime = chrono::duration<double>(end - start).count();
        wcout << L"Время работы программы: " << elapsedTime << L" секунд\n";

    } else if(command == "unitTests") {
        MODULUS = 69;
        unitTest_detectMajorityAlphabet();
        unitTest_generateGamma();
        unitTest_generateTimeBasedGamma();
        unitTest_xorBlock();
        unitTest_calculateChecksum();
        unitTest_calculateTimeDependentChecksum();
        unitTest_removeLastByte();
        unitTest_readFileInParallel();
        unitTest_encryptFile();
        unitTest_saveFileParallel();
        unitTest_getDirectoryFromPath();
        unitTest_createNewFileName();

        return 5;
    } else {
        wcout << L"Ошибка при выборе режима работы программы." << endl;
        wcout << L"Используйте команду 'help' для просмотра подробной справочной информации." << endl;
        return 8;
    }

    return 0;
}
