/**
 * @file main.h
 * @brief Заголовочный файл, содержащий объявления функций, используемых в программе.
 *
 * Этот файл содержит прототипы функций, которые реализуют операции, такие как
 * чтение файлов, шифрование данных, создание контрольных сумм, работа с многозадачностью и т. д.
 * Функции из этого файла используются в основном коде программы для обработки файлов,
 * работы с данными и выполнения вычислений.
 *
 * @author IT Energy PLUS
 * @date 06/12/2024
 */

#ifndef mainH
#define mainH
/**
 * @brief Определяет преобладающий алфавит в тексте.
 *
 * Данная функция анализирует входной текст и определяет, какой алфавит преобладает в нем:
 * латинский или кириллический.
 *
 * @param text Входной текст, который нужно проанализировать.
 * @return Возвращает 'L', если в тексте больше латинских символов, и 'C', если больше кириллических.
 */
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

/**
 * @brief Генерирует случайную гамму на основе указанного алфавита.
 *
 * Эта функция генерирует случайную гамму для шифрования в зависимости от выбранного алфавита.
 *
 * @param alphabetType Тип алфавита, который используется для генерации гаммы ('L' для латинского, 'C' для кириллического).
 * @return Возвращает строку, представляющую сгенерированную гамму.
 */
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

/**
 * @brief Генерирует гамму на основе текущего времени.
 *
 * Эта функция генерирует гамму, основанную на текущем времени, с различными методами в зависимости от выбора.
 *
 * @param methodChoice Выбор метода гаммирования.
 * @return Возвращает строку, представляющую сгенерированную гамму.
 */
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

/**
 * @brief Выполняет побитовую операцию XOR для текста и гаммы.
 *
 * Эта функция применяет операцию XOR между текстом и гаммой для шифрования или дешифрования.
 *
 * @param text Текст для шифрования/дешифрования.
 * @param gamma Гамма для операции XOR.
 * @return Возвращает результат выполнения операции XOR.
 */
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

/**
 * @brief Вычисляет контрольную сумму для текста.
 *
 * Функция вычисляет контрольную сумму текста с использованием операции XOR.
 *
 * @param text Входной текст для вычисления контрольной суммы.
 * @return Возвращает контрольную сумму в виде одного байта.
 */
unsigned char calculateChecksum(const string& text) {
    unsigned char checksum = 0;
    for (char c : text) {
        checksum ^= c; // XOR для вычисления контрольной суммы
    }
    return checksum;
}

/**
 * @brief Вычисляет контрольную сумму, зависимую от времени.
 *
 * Эта функция вычисляет контрольную сумму для переданного текста, применяя операцию XOR ко всем символам строки.
 * После этого к результату добавляется значение текущей секунды, чтобы контрольная сумма зависела от времени.
 * Можно использовать другие временные параметры, такие как минуты или часы, для создания разных вариантов зависимости от времени.
 *
 * @param text Входной текст, для которого необходимо вычислить контрольную сумму.
 * @return Возвращает контрольную сумму, представляющую собой байт, зависящий от времени.
 */
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

/**
 * @brief Удаляет последний байт из текста (например, байт длины или контрольной суммы).
 *
 * @param text Входной текст.
 * @return Текст без последнего байта.
 */
string removeLastByte(const string& text) {
    return (text.size() > 1) ? text.substr(0, text.size() - 1) : text;
}

/**
 * @brief Читает содержимое файла параллельно с использованием нескольких потоков.
 *
 * Эта функция читает файл в несколько потоков для улучшения производительности при чтении больших файлов.
 * Размер файла делится на несколько частей, каждая из которых обрабатывается отдельным потоком.
 * После чтения всех частей результаты объединяются в одну строку.
 *
 * @param fileName Имя файла, который нужно прочитать.
 * @return Возвращает строку, содержащую все данные файла.
 *
 * @note Используется фиксированное количество потоков (4) для чтения файла.
 * Размер каждой части для каждого потока рассчитывается автоматически.
 */
string readFileInParallel(const string& fileName) {
    ifstream file(fileName, ios::in | ios::binary);
    if (!file.is_open()) {
        wcout << L"Произошла ошибка при открытии файла: ";
        cout << fileName << endl;
    }

    // Определяем размер файла
    file.seekg(0, ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, ios::beg);

    // Разделяем чтение на потоки
    const size_t numThreads = 4; // Количество потоков
    size_t chunkSize = fileSize / numThreads;

    vector<string> parts(numThreads);
    vector<thread> threads;
    mutex mtx;

    for (size_t i = 0; i < numThreads; ++i) {
        threads.emplace_back([&, i]() {
            ifstream localFile(fileName, ios::in | ios::binary);
            localFile.seekg(i * chunkSize, ios::beg);

            // Вычисляем размер части для потока
            size_t readSize = (i == numThreads - 1) ? (fileSize - i * chunkSize) : chunkSize;

            // Читаем часть файла
            string localBuffer(readSize, '\0');
            localFile.read(&localBuffer[0], readSize);

            // Добавляем результат
            lock_guard<mutex> lock(mtx);
            parts[i] = move(localBuffer);

            localFile.close();
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // Объединяем результаты
    string content;
    for (auto& part : parts) {
        content += move(part);
    }

    file.close();

    return content;
}

/**
 * @brief Шифрует текст с использованием гаммы с параллельной обработкой данных.
 *
 * Эта функция шифрует переданный текст с использованием гаммы через операцию XOR.
 * Для повышения производительности текст разбивается на несколько частей,
 * каждая из которых шифруется в отдельном потоке. После выполнения параллельного шифрования
 * все части объединяются в одну строку.
 *
 * @param text Текст, который нужно зашифровать.
 * @param gamma Гамма, которая используется для шифрования.
 * @return Возвращает зашифрованный текст, собрав все части из потоков.
 *
 * @note Используется фиксированное количество потоков (4) для шифрования текста.
 * Размер каждого блока текста рассчитывается автоматически.
 */
string encryptFile(const string& text, const string& gamma) {
    string resultText;
    const size_t numThreads = 4;
    size_t chunkSize = text.size() / numThreads;

    vector<string> encryptedParts(numThreads);
    vector<thread> threads;

    // Параллельное шифрование
    for (size_t i = 0; i < numThreads; ++i) {
        threads.emplace_back([&, i]() {
            size_t start = i * chunkSize;
            size_t end = (i == numThreads - 1) ? text.size() : start + chunkSize;

            string block = text.substr(start, end - start);
            encryptedParts[i] = xorBlock(block, gamma); // Используем xorBlock
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // Объединяем части из вектора
    for (const auto& part : encryptedParts) {
        resultText += part;
    }

    return resultText;
}

/**
 * @brief Сохраняет текст в файл с использованием параллельной записи.
 *
 * Эта функция сохраняет переданный текст в файл, разделяя его на блоки размером 1 MB и записывая каждый блок в файл в отдельном потоке.
 * Для синхронизации потоков используется мьютекс, что позволяет безопасно записывать данные в файл параллельно.
 * После записи всех блоков функция ожидает завершения всех потоков.
 *
 * @param inputFileName Имя файла, в который будет записан результат.
 * @param resultText Текст, который нужно записать в файл.
 *
 * @throws runtime_error Если возникает ошибка при создании или открытии файла, либо при записи данных в файл.
 *
 * @note Размер блока записи составляет 1 MB.
 * Используются потоки для параллельной записи данных в файл.
 */
void saveFileParallel(const string& inputFileName, const string& resultText) {
    // Разделяем результат на блоки для записи
    size_t blockSize = 1024 * 1024; // Размер блока: 1 MB
    vector<string> dataParts;
    for (size_t i = 0; i < resultText.size(); i += blockSize) {
        dataParts.emplace_back(resultText.substr(i, blockSize));
    }

    // Создаем пустой файл нужного размера
    ofstream outputFile(inputFileName, ios::binary | ios::trunc);
    if (!outputFile.is_open()) {
        throw runtime_error("Failed to create output file: " + inputFileName);
    }
    outputFile.seekp(resultText.size() - 1); // Устанавливаем указатель в конец файла
    outputFile.put('\0'); // Заполняем файл символом
    outputFile.close();

    // Мьютекс для синхронизации потоков
    mutex writeMutex;

    // Параллельно записываем данные
    vector<thread> threads;
    streampos offset = 0;
    for (const auto& part : dataParts) {
        threads.emplace_back([&, offset](string data, streampos currentOffset) {
            lock_guard<mutex> lock(writeMutex); // Защищаем запись в файл
            fstream outputFile(inputFileName, ios::binary | ios::in | ios::out);
            if (!outputFile.is_open()) {
                throw runtime_error("Failed to open output file: " + inputFileName);
            }
            outputFile.seekp(currentOffset); // Переходим к нужному смещению
            outputFile.write(data.data(), data.size());
            if (!outputFile) {
                throw runtime_error("Failed to write to file: " + inputFileName);
            }
            outputFile.close();
        }, part, offset);
        offset += part.size();
    }

    // Ожидаем завершения всех потоков
    for (auto& t : threads) {
        t.join();
    }

    outputFile.close();
}

/**
 * @brief Извлекает имя директории из указанного пути.
 *
 * Эта функция принимает строку, представляющую путь к файлу, и извлекает часть пути до последнего символа разделителя ("/" или "\").
 * Если путь не содержит разделителя, функция возвращает пустую строку, что означает, что путь представляет собой только имя файла.
 *
 * @param path Строка, представляющая путь к файлу (включая имя файла).
 * @return Строка, представляющая путь к директории, или пустая строка, если путь не содержит разделителя.
 */
string getDirectoryFromPath(const string& path) {
    size_t pos = path.find_last_of("\\/");
    if (pos != string::npos) {
        return path.substr(0, pos);
    }
    return ""; // Если нет пути (только имя файла)
}

/**
 * @brief Создает новое имя файла с добавлением префикса к имени исходного файла.
 *
 * Эта функция принимает имя исходного файла и префикс, добавляемый к имени файла.
 * Она извлекает директорию из исходного пути (если она существует) и создает новое имя файла,
 * в котором к оригинальному имени файла добавляется указанный префикс.
 *
 * @param inputFileName Полный путь к исходному файлу (включая имя файла).
 * @param prefix Префикс, который будет добавлен к имени файла.
 * @return Строка, представляющая новое имя файла, с добавленным префиксом и соответствующим путем.
 */
string createNewFileName(const string& inputFileName, const string& prefix) {
    // Извлекаем директорию исходного файла
    string directory = getDirectoryFromPath(inputFileName);

    // Получаем имя файла без пути
    string originalFileName = inputFileName.substr(inputFileName.find_last_of("\\/") + 1);

    // Формируем новое имя файла с префиксом
    string newFileName = prefix + originalFileName;

    // Если директория не пуста, возвращаем полный путь, иначе только имя файла
    if (!directory.empty()) {
        return directory + "\\" + newFileName;
    }
    return newFileName;  // Если директория не указана, возвращаем только имя файла
}

string createGammaFileName(const string& inputFileName, const string& prefix) {
    // Извлекаем директорию исходного файла
    string directory = getDirectoryFromPath(inputFileName);

    // Получаем имя файла без пути
    string originalFileName = inputFileName.substr(inputFileName.find_last_of("\\/") + 1);

    // Формируем новое имя файла с префиксом
    string newFileName = prefix;

    // Если директория не пуста, возвращаем полный путь, иначе только имя файла
    if (!directory.empty()) {
        return directory + "\\" + newFileName;
    }
    return newFileName;  // Если директория не указана, возвращаем только имя файла
}

/**
 * @brief Отображает справочную информацию по использованию программы.
 */
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

/**
 * @brief Отображает список доступных методов шифрования.
 */
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

#endif // mainH
