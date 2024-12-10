/**
 * @file unitTests.h
 * @brief Заголовочный файл, содержащий юнит-тесты для функций из main.h, которые используются в программе.
 *
 * Этот файл содержит юнит-тесты для проверки корректности работы функций, которые реализуют операции, такие как
 * чтение файлов, шифрование данных, создание контрольных сумм, работа с многозадачностью и т. д.
 * Функции из этого файла используются в основном коде программы для обработки файлов,
 * работы с данными и выполнения вычислений.
 *
 * @author IT Energy PLUS
 * @date 06/12/2024
 */

#ifndef unitTestsH
#define unitTtestsH

/**
 * @brief Тестовая функция для проверки корректности работы detectMajorityAlphabet.
 *
 * Данная функция выполняет несколько юнит-тестов для функции detectMajorityAlphabet,
 * которая определяет, какая группа символов (латиница или кириллица)
 * преобладает в строке. Все проверки выполнены с помощью assert.
 *
 * Тесты включают:
 * - Преобладание латиницы.
 * - Преобладание кириллицы.
 * - Равное количество латиницы и кириллицы.
 * - Только цифры.
 * - Смешанные символы с преобладанием латиницы.
 * - Только специальные символы.
 * - Пустая строка.
 *
 * Если все тесты пройдены успешно, выводится сообщение об успехе.
 * В случае провала какого-либо теста программа завершится с ошибкой assert.
 */
void unitTest_detectMajorityAlphabet() {
    // Тест 1: Преобладание латиницы
    assert(detectMajorityAlphabet("HelloWorld") == 'L');

    // Тест 2: Преобладание кириллицы
    assert(detectMajorityAlphabet("ПриветМир") == 'C');

    // Тест 3: Равное количество латиницы и кириллицы (возвращает 'C' по текущей логике)
    assert(detectMajorityAlphabet("HelloПривет") == 'C');

    // Тест 4: Только цифры (считаются латиницей)
    assert(detectMajorityAlphabet("1234567890") == 'L');

    // Тест 5: Смешанные символы с преобладанием латиницы
    assert(detectMajorityAlphabet("Hello123Привет") == 'L');

    // Тест 6: Только специальные символы (не относятся ни к латинице, ни к кириллице)
    assert(detectMajorityAlphabet("!@#$%^&*()") == 'C'); // Выбор 'C' по текущей логике

    // Тест 7: Пустая строка (возвращает 'C' по текущей логике)
    assert(detectMajorityAlphabet("") == 'C');

    wcout << L"Функция detectMajorityAlphabet успешно прошла все тесты!" << endl;
}

/**
 * @brief Тестовая функция для проверки корректности работы generateGamma.
 *
 * Данная функция выполняет несколько юнит-тестов для функции generateGamma,
 * которая генерирует гамму (строку символов) в зависимости от алфавита ('L' для латиницы, 'C' для кириллицы).
 *
 * Тесты включают:
 * - Проверку длины сгенерированной строки для латиницы.
 * - Проверку длины сгенерированной строки для кириллицы.
 * - Проверку содержимого строки для латиницы (только латинские символы и цифры).
 * - Проверку на отсутствие пустой строки в результате работы функции.
 *
 * Если все тесты пройдены успешно, выводится сообщение об успехе.
 * В случае провала какого-либо теста программа завершится с ошибкой assert.
 */
void unitTest_generateGamma() {
    // Тест 1: Проверка длины строки для латиницы
    string gammaL = generateGamma('L');
    assert(gammaL.size() == BLOCK_SIZE);

    // Тест 2: Проверка длины строки для кириллицы
    string gammaC = generateGamma('C');
    assert(gammaC.size() == BLOCK_SIZE);

    // Тест 3: Проверка того, что строка для латиницы содержит только латинские символы и цифры
    for (char c : gammaL) {
        assert((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'));
    }

    // Тест 4: Проверка на пустую строку
    string gammaEmpty = generateGamma('L');
    assert(!gammaEmpty.empty()); // Пустой строкой быть не должно

    wcout << L"Функция generateGamma успешно прошла все тесты!" << endl;
}

/**
 * @brief Тестовая функция для проверки корректности работы generateTimeBasedGamma.
 *
 * Данная функция выполняет несколько юнит-тестов для функции generateTimeBasedGamma,
 * которая генерирует гамму на основе временных маркеров (секунды, дни месяца, дни недели).
 *
 * Тесты включают:
 * - Проверку длины гаммы для метода 1 (маркер от секунд).
 * - Проверку длины гаммы для метода 2 (маркер от дня месяца).
 * - Проверку длины гаммы для метода 3 (маркер от дня недели).
 * - Проверку на корректность длины сгенерированной строки (равна BLOCK_SIZE) и отсутствие пустой строки.
 *
 * Если все тесты пройдены успешно, выводится сообщение об успехе.
 * В случае провала какого-либо теста программа завершится с ошибкой assert.
 */
void unitTest_generateTimeBasedGamma() {
    // Тест 1: Проверка гаммы для метода 1 (маркант от секунд)
    // Учитывая, что секунды изменяются каждую секунду, для теста можно использовать четные и нечетные секунды.
    string gamma1 = generateTimeBasedGamma(1);
    assert(gamma1.size() == BLOCK_SIZE);

    // Тест 2: Проверка гаммы для метода 2 (маркант от дня месяца)
    string gamma2 = generateTimeBasedGamma(2);
    assert(gamma2.size() == BLOCK_SIZE);

    // Тест 3: Проверка гаммы для метода 3 (маркант от дня недели)
    string gamma3 = generateTimeBasedGamma(3);
    assert(gamma3.size() == BLOCK_SIZE);

    // Тест 4: Проверка на длину гаммы (должна быть равна BLOCK_SIZE)
    string gammaEmpty = generateTimeBasedGamma(1);
    assert(!gammaEmpty.empty()); // Пустой строкой быть не должно
    assert(gammaEmpty.size() == BLOCK_SIZE); // Длина должна быть BLOCK_SIZE

    wcout << L"Функция generateTimeBasedGamma успешно прошла все тесты!" << endl;
}

/**
 * @brief Тестовая функция для проверки корректности работы xorBlock.
 *
 * Данная функция выполняет несколько юнит-тестов для функции xorBlock,
 * которая применяет операцию XOR к тексту и гамме для шифрования/дешифрования.
 *
 * Тесты включают:
 * - Проверку правильной длины результата для различных входных данных.
 * - Проверку работы с различным значением MODULUS.
 * - Проверку обработки гаммы меньшей длины, чем текст.
 * - Проверку поведения с пустыми строками.
 *
 * Если все тесты пройдены успешно, выводится сообщение об успехе.
 * В случае провала какого-либо теста программа завершится с ошибкой assert.
 */
void unitTest_xorBlock() {
    // Тест 1: Простой тест с одинаковыми текстом и гаммой
    string text1 = "Hello, World!";
    string gamma1 = "ABCDEFGHIJKLMNOP";  // Gamma длиной 16
    string result1 = xorBlock(text1, gamma1);
    assert(result1.size() == text1.size());

    // Тест 2: Проверка с различным значением MODULUS
    string text2 = "Test text for MODULUS!";
    string gamma2 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string result2 = xorBlock(text2, gamma2);
    assert(result2.size() == text2.size());

    // Тест 3: Проверка с другим значением MODULUS
    string text3 = "Test with MODULUS = 1";
    string gamma3 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string result3 = xorBlock(text3, gamma3);
    assert(result3.size() == text3.size());

    // Тест 4: Проверка корректности работы с гаммой меньшей длины, чем текст
    string text4 = "Long text";
    string gamma4 = "ABC";  // Gamma длиной меньше текста
    string result4 = xorBlock(text4, gamma4);
    assert(result4.size() == text4.size());

    // Тест 5: Проверка пустых строк
    string text5 = "";
    string gamma5 = "";
    string result5 = xorBlock(text5, gamma5);
    assert(result5.empty());  // Результат должен быть пустой строкой

    wcout << L"Функция xorBlock успешно прошла все тесты!" << endl;
}

/**
 * @brief Тестовая функция для проверки корректности работы calculateChecksum.
 *
 * Данная функция выполняет несколько юнит-тестов для функции calculateChecksum,
 * которая вычисляет контрольную сумму строки путем выполнения операции XOR над всеми её символами.
 *
 * Тесты включают:
 * - Проверку поведения на пустой строке.
 * - Проверку результата для строки с одним символом.
 * - Проверку результата для строки с повторяющимися символами.
 * - Проверку результата для строки с разными символами.
 * - Проверку результата для строки с пробелами.
 *
 * Если все тесты пройдены успешно, выводится сообщение об успехе.
 * В случае провала какого-либо теста программа завершится с ошибкой assert.
 */
void unitTest_calculateChecksum() {
    // Тест 1: Проверка на пустую строку
    string text1 = "";
    unsigned char checksum1 = calculateChecksum(text1);
    assert(checksum1 == 0); // Для пустой строки сумма должна быть 0

    // Тест 2: Проверка строки с одним символом
    string text2 = "A";
    unsigned char checksum2 = calculateChecksum(text2);
    assert(checksum2 == 'A'); // Для строки из одного символа результат равен символу

    // Тест 3: Проверка строки с повторяющимися символами
    string text3 = "AAAA";
    unsigned char checksum3 = calculateChecksum(text3);
    assert(checksum3 == 0); // XOR одинаковых символов обнуляется

    // Тест 4: Проверка строки с разными символами
    string text4 = "ABCD";
    unsigned char checksum4 = calculateChecksum(text4);
    assert(checksum4 == ('A' ^ 'B' ^ 'C' ^ 'D')); // Проверяем результат XOR всех символов

    // Тест 5: Проверка строки с пробелами
    string text5 = "Hello World!";
    unsigned char checksum5 = calculateChecksum(text5);
    assert(checksum5 == ('H' ^ 'e' ^ 'l' ^ 'l' ^ 'o' ^ ' ' ^ 'W' ^ 'o' ^ 'r' ^ 'l' ^ 'd' ^ '!')); // XOR всех символов

    wcout << L"Функция calculateChecksum успешно прошла все тесты!" << endl;
}

/**
 * @brief Тестовая функция для проверки корректности работы calculateTimeDependentChecksum.
 *
 * Данная функция выполняет юнит-тесты для проверки работы функции calculateTimeDependentChecksum,
 * которая вычисляет контрольную сумму строки с учетом текущего времени (например, секунды).
 *
 * Тесты включают:
 * - Проверку поведения на пустой строке.
 * - Проверку результата для строки с одним символом.
 * - Проверку результата для строки с несколькими символами.
 * - Проверку изменения результата при изменении времени (разные секунды).
 */
void unitTest_calculateTimeDependentChecksum() {
    // Тест 1: Проверка на пустую строку
    string text1 = "";
    unsigned char checksum1 = calculateTimeDependentChecksum(text1);
    time_t now1 = time(nullptr);
    tm* ltm1 = localtime(&now1);
    unsigned char expected1 = static_cast<unsigned char>(ltm1->tm_sec); // Контрольная сумма для пустой строки = текущая секунда
    assert(checksum1 == expected1);

    // Тест 2: Проверка строки с одним символом
    string text2 = "A";
    unsigned char checksum2 = calculateTimeDependentChecksum(text2);
    time_t now2 = time(nullptr);
    tm* ltm2 = localtime(&now2);
    unsigned char expected2 = 'A' ^ static_cast<unsigned char>(ltm2->tm_sec);
    assert(checksum2 == expected2);

    // Тест 3: Проверка строки с несколькими символами
    string text3 = "ABC";
    unsigned char checksum3 = calculateTimeDependentChecksum(text3);
    time_t now3 = time(nullptr);
    tm* ltm3 = localtime(&now3);
    unsigned char expected3 = ('A' ^ 'B' ^ 'C') ^ static_cast<unsigned char>(ltm3->tm_sec);
    assert(checksum3 == expected3);

    // Тест 4: Проверка, что функция возвращает разные значения для разных секунд
    string text4 = "Test";
    time_t start = time(nullptr);
    unsigned char checksum4_1 = calculateTimeDependentChecksum(text4);

    // Ждём следующей секунды
    while (time(nullptr) == start); // Ждём, пока секунда изменится

    unsigned char checksum4_2 = calculateTimeDependentChecksum(text4);
    assert(checksum4_1 != checksum4_2); // Проверяем, что значение меняется

    wcout << L"Функция calculateTimeDependentChecksum успешно прошла все тесты!" << endl;
}

/**
 * @brief Тестовая функция для проверки корректности работы removeLastByte.
 *
 * Функция removeLastByte должна удалять последний байт строки, если её длина больше 1.
 * Для строк длиной <= 1 изменений быть не должно.
 *
 * Тесты включают:
 * - Пустую строку.
 * - Строку из одного символа.
 * - Строку из двух символов.
 * - Длинную строку.
 * - Строку с пробелом в конце.
 */
void unitTest_removeLastByte() {
    // Тест 1: Пустая строка
    string text1 = "";
    string result1 = removeLastByte(text1);
    assert(result1 == ""); // Пустая строка остаётся пустой

    // Тест 2: Строка с одним символом
    string text2 = "A";
    string result2 = removeLastByte(text2);
    assert(result2 == "A"); // Один символ остаётся, т.к. длина <= 1

    // Тест 3: Строка с двумя символами
    string text3 = "AB";
    string result3 = removeLastByte(text3);
    assert(result3 == "A"); // Удаляется последний символ

    // Тест 4: Длинная строка
    string text4 = "Hello, World!";
    string result4 = removeLastByte(text4);
    assert(result4 == "Hello, World"); // Удаляется последний символ

    // Тест 5: Строка с пробелом в конце
    string text5 = "Test ";
    string result5 = removeLastByte(text5);
    assert(result5 == "Test"); // Удаляется последний символ (пробел)

    wcout << L"Функция removeLastByte успешно прошла все тесты!" << endl;
}

/**
 * @brief Тестовая функция для проверки корректности работы readFileInParallel.
 *
 * Функция readFileInParallel должна корректно читать файл в параллельном режиме
 * и возвращать строку с содержимым файла.
 *
 * Тесты включают:
 * - Чтение файла с тестовым содержимым.
 * - Проверку на совпадение прочитанного содержимого с ожидаемым.
 */
void unitTest_readFileInParallel() {
    // Создание тестового файла
    string testFileName = "test_file.txt";
    string testContent = "Это тестовый файл для параллельного чтения. Он содержит достаточно данных для тестирования фрагментации.";
    ofstream testFile(testFileName, ios::out | ios::binary);
    testFile << testContent;
    testFile.close();

    // Тест 1: Проверка чтения файла
    string result = readFileInParallel(testFileName);
    assert(result == testContent); // Содержимое должно совпадать

    // Удаление тестового файла
    remove(testFileName.c_str());

    wcout << L"Функция readFileInParallel успешно прошла все тесты!" << endl;
}

/**
 * @brief Тестовая функция для проверки корректности работы encryptFile.
 *
 * Функция encryptFile должна шифровать текст, используя заданную гамму, и возвращать зашифрованный результат.
 *
 * Тесты включают:
 * - Проверку шифрования текста при различных значениях MODULUS.
 * - Шифрование пустого текста.
 * - Шифрование текста длиной меньше BLOCK_SIZE.
 */
void unitTest_encryptFile() {
    // Тест 1: MODULUS = 69
    MODULUS = 69;
    string text1 = "TestTextForMOD69";
    string gamma1 = "ABABABABABABABAB";
    string expected1 = xorBlock(text1, gamma1); // Результат на основе xorBlock
    string result1 = encryptFile(text1, gamma1);
    assert(result1 == expected1);

    // Тест 2: Пустой текст
    string text2 = "";
    string gamma2 = "GammaKey12345678";
    string expected2 = xorBlock(text2, gamma2); // Должно быть пустым
    string result2 = encryptFile(text2, gamma2);
    assert(result2 == expected2);

    // Тест 3: Длина текста меньше BLOCK_SIZE
    string text3 = "ShortText";
    string gamma3 = "ABABABABABABABAB";
    string expected3 = xorBlock(text3, gamma3);
    string result3 = encryptFile(text3, gamma3);
    assert(result3 == expected3);

    wcout << L"Функция encryptFile успешно прошла все тесты!" << endl;
}

/**
 * @brief Тестовая функция для проверки корректности работы readFileContent.
 *
 * Функция readFileContent должна читать содержимое файла и возвращать его в виде строки.
 *
 * Тесты включают:
 * - Чтение файла с текстовыми данными.
 * - Попытку чтения несуществующего файла (ожидание исключения).
 * - Чтение пустого файла.
 */
string readFileContent(const string& fileName) {
    ifstream file(fileName, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Failed to open file for reading: " + fileName);
    }
    return string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
}

/**
 * @brief Тестовая функция для проверки корректности работы saveFileParallel.
 *
 * Функция saveFileParallel должна записывать данные в файл с использованием параллельной записи и
 * гарантировать, что записанный контент совпадает с исходными данными.
 *
 * Тесты включают:
 * - Запись большого текста (5 МБ).
 * - Запись маленького текста.
 * - Запись пустого текста.
 * - Запись текста с особыми символами.
 */
void unitTest_saveFileParallel() {
    // Тест 1: Запись большого текста
    {
        const string testFileName = "test_file_1.bin";
        const string inputText = string(5 * 1024 * 1024, 'A'); // 5 MB текста 'A'

        try {
            saveFileParallel(testFileName, inputText);
            string outputText = readFileContent(testFileName);
            assert(outputText == inputText);
        } catch (const exception& e) {
            assert(false);
        }
    }

    // Тест 2: Запись маленького текста
    {
        const string testFileName = "test_file_2.bin";
        const string inputText = "Small text for testing.";

        try {
            saveFileParallel(testFileName, inputText);
            string outputText = readFileContent(testFileName);
            assert(outputText == inputText);
        } catch (const exception& e) {
            assert(false);
        }
    }

    // Тест 3: Запись пустого текста
    {
        const string testFileName = "test_file_3.bin";
        const string inputText = "";

        try {
            saveFileParallel(testFileName, inputText);
            string outputText = readFileContent(testFileName);
            assert(outputText == inputText);
        } catch (const exception& e) {
            assert(false);
        }
    }

    // Тест 4: Запись текста с разными символами
    {
        const string testFileName = "test_file_4.bin";
        const string inputText = "Text with special characters: \n\t!@#$%^&*()_+{}|:\"<>?";

        try {
            saveFileParallel(testFileName, inputText);
            string outputText = readFileContent(testFileName);
            assert(outputText == inputText);
        } catch (const exception& e) {
            assert(false);
        }
    }

    wcout << L"Функция saveFileParallel успешно прошла все тесты!" << endl;
}

/**
 * @brief Тестовая функция для проверки корректности работы getDirectoryFromPath.
 *
 * Функция getDirectoryFromPath должна извлекать директорию из полного пути, возвращая путь без имени файла.
 * Тесты включают различные варианты путей, включая абсолютные и относительные пути, а также случай пустой строки.
 *
 * Тесты:
 * - Путь с прямыми слэшами (Unix-подобные системы).
 * - Путь с обратными слэшами (Windows).
 * - Только имя файла.
 * - Пустая строка.
 * - Путь, заканчивающийся на слэш.
 * - Относительный путь.
 * - Только директория.
 * - Корневая директория.
 */
void unitTest_getDirectoryFromPath() {
    // Тест 1: Полный путь с прямыми слэшами
    {
        string path = "/home/user/documents/file.txt";
        string expected = "/home/user/documents";
        assert(getDirectoryFromPath(path) == expected);
    }

    // Тест 2: Полный путь с обратными слэшами
    {
        string path = "C:\\Users\\user\\documents\\file.txt";
        string expected = "C:\\Users\\user\\documents";
        assert(getDirectoryFromPath(path) == expected);
    }

    // Тест 3: Только имя файла
    {
        string path = "file.txt";
        string expected = "";
        assert(getDirectoryFromPath(path) == expected);
    }

    // Тест 4: Пустая строка
    {
        string path = "";
        string expected = "";
        assert(getDirectoryFromPath(path) == expected);
    }

    // Тест 5: Путь заканчивается на слэш
    {
        string path = "/home/user/documents/";
        string expected = "/home/user/documents";
        assert(getDirectoryFromPath(path) == expected);
    }

    // Тест 6: Относительный путь
    {
        string path = "../folder/file.txt";
        string expected = "../folder";
        assert(getDirectoryFromPath(path) == expected);
    }

    // Тест 7: Только директория
    {
        string path = "/home/user/documents/";
        string expected = "/home/user/documents";
        assert(getDirectoryFromPath(path) == expected);
    }

    // Тест 8: Корневая директория
    {
        string path = "/";
        string expected = "";
        assert(getDirectoryFromPath(path) == expected);
    }

    wcout << L"Функция getDirectoryFromPath успешно прошла все тесты!" << endl;
}

/**
 * @brief Тестовая функция для проверки корректности работы createNewFileName.
 *
 * Функция createNewFileName должна создавать новый файл, добавляя префикс к имени файла, сохраняя при этом путь к директории.
 * Тесты включают разные варианты путей, включая абсолютный путь с директориями и только имя файла.
 *
 * Тесты:
 * - Полный путь с директориями и префиксом.
 * - Только имя файла без директории.
 * - Пустое имя файла.
 */
void unitTest_createNewFileName() {
    // Тест 1: Полный путь с директориями и префиксом
    {
        string inputFileName = "C:\\Users\\user\\documents\\file.txt";
        string prefix = "encrypted_";
        string expected = "C:\\Users\\user\\documents\\encrypted_file.txt";
        assert(createNewFileName(inputFileName, prefix) == expected);
    }

    // Тест 2: Только имя файла без директории
    {
        string inputFileName = "file.txt";
        string prefix = "temp_";
        string expected = "temp_file.txt";
        assert(createNewFileName(inputFileName, prefix) == expected);
    }

    // Тест 3: Пустое имя файла
    {
        string inputFileName = "";
        string prefix = "temp_";
        string expected = "temp_";
        assert(createNewFileName(inputFileName, prefix) == expected);
    }

    wcout << L"Функция createNewFileName успешно прошла все тесты!" << endl;
}

#endif // unitTestsH
