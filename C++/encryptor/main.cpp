#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cassert>

#include <clocale>
using namespace std;

// ������ ����� � ������ ������������
const int BLOCK_SIZE = 16;
int MODULUS;


// ������� ��� �����������, ����� �������� � ������ ������ � ��������� ��� �������������
char detectMajorityAlphabet(const string& text) {
    int latinCount = 0;
    int cyrillicCount = 0;

    for (unsigned char c : text) {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) {
            latinCount++;
        } else if (c >= 0xC0 && c <= 0xFF) { // ������������� ������� � Windows-1251
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
        gamma = "������������������������������";
    }
    // ���������� ����� ������ BLOCK_SIZE � �������������� ������
    string result;
    srand(static_cast<unsigned int>(time(nullptr))); // �������������� ��������� ��������� �����
    for (int i = 0; i < BLOCK_SIZE; ++i) {
        result += gamma[(rand() % gamma.size()) % MODULUS]; // ���������� ������ ������������
    }
    return result;
}

// ������� ��� �������� ����� �� ������ ������� (�4.2.4, �4.2.5, �4.2.6)
string generateTimeBasedGamma(int methodChoice) {
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    string gamma;

    switch (methodChoice) {
        case 1: // �4.2.4 (������� �� ������)
            if (ltm->tm_sec / 10 % 2 == 0) {
                gamma = "ABCDEFGHIJKLMNOP"; // ׸���� 10 ������
            } else {
                gamma = "GHEFABCDOPMNIJKL"; // �������� 10 ������
            }
            break;

        case 2: // �4.2.5 (������� �� ��� ������)
            if (ltm->tm_mday % 2 == 0) {
                gamma = "ABCDEFGHIJKLMNOP"; // ׸���� ����� ������
            } else {
                gamma = "GHEFABCDOPMNIJKL"; // �������� ����� ������
            }
            break;

        case 3: // �4.2.6 (������� �� ��� ������)
            switch (ltm->tm_wday) {
                case 0: // �����������
                case 6: // �������
                    gamma = "GHEFCDABOPMNIJKL"; // ��, ��
                    break;
                case 1: // �����������
                case 3: // �����
                case 5: // �������
                    gamma = "ABCDEFGHIJKLMNOP"; // ��, ��, ��
                    break;
                case 2: // �������
                case 4: // �������
                    gamma = "GHEFOPMNABCDIJKL"; // ��, ��
                    break;
            }
            break;
    }

    // ������������ ����� ����� ������ BLOCK_SIZE
    while (gamma.size() < BLOCK_SIZE) {
        gamma += gamma; // ��������� �����, ����� ������� ������ �����
    }
    gamma.resize(BLOCK_SIZE); // �������� �� ������ �����

    return gamma;
}


// ������� ����������/������������ ������� XOR
string xorBlock(const string& text, const string& gamma, int MODULUS) {
    string result;
    for (size_t i = 0; i < text.size(); i++) {
            if (MODULUS == 69){
                result += text[i] ^ gamma[i % BLOCK_SIZE % MODULUS];  // XOR � ������, ������ ������������ ��� ������������ ������� � �����
            }
            else {
                 result += text[i] ^ gamma[i % BLOCK_SIZE];  // `BLOCK_SIZE` � ��� ����� �����
            }
    }
    return result;
}

// ������� ��� ���������� ����������� �����, �� ��������� �� �������
unsigned char calculateChecksum(const string& text) {
    unsigned char checksum = 0;
    for (char c : text) {
        checksum ^= c; // XOR ��� ���������� ����������� �����
    }
    return checksum;
}

// ������� ��� ���������� ����������� �����, ��������� �� �������
unsigned char calculateTimeDependentChecksum(const string& text) {
    unsigned char checksum = 0;
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);

    for (char c : text) {
        checksum ^= c; // XOR ��� ������� ����������� �����
    }

    // ��������� �������� ������� ������� ��� ����������� �� �������
    checksum ^= static_cast<unsigned char>(ltm->tm_sec); // ��� tm_min, tm_hour ��� ������ ��������� ����������

    return checksum;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian"); // ������������� ������ ��� Windows-1251
    // ��������� ���������� ����������
        string inputFileName = argv[1]; // ��� �������� �����
        MODULUS = stoi(argv[2]); // ������
        int methodChoice = stoi(argv[3]); // ����� ������
        string gamma;


        ifstream inputFile(inputFileName, ios::binary);
        if (!inputFile.is_open()) {
            return 1;
        }

        // ������ ���������� �����
        string text((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
        inputFile.close();

        string resultText;

        // ��������� ����� �� ������ ������
        if (methodChoice == 1 || methodChoice == 5 || methodChoice == 6 || methodChoice == 16) {
            char majorityAlphabet = detectMajorityAlphabet(text);
            gamma = generateGamma(majorityAlphabet); // ��������� �����
        } else if (methodChoice == 2 || methodChoice == 3 || methodChoice == 4) {
            gamma = generateTimeBasedGamma(methodChoice - 1); // ��������� ����� �� ������ ������� � ���������� 1
        } else if (methodChoice == 7 || methodChoice == 8 || methodChoice == 9) {
            gamma = generateTimeBasedGamma(methodChoice - 6); // ��������� ����� �� ������ ������� � ���������� 6
        } else if (methodChoice == 10 || methodChoice == 11 || methodChoice == 12) {
            gamma = generateTimeBasedGamma(methodChoice - 9); // ��������� ����� �� ������ ������� � ���������� 9
        } else if (methodChoice == 13 || methodChoice == 14 || methodChoice == 15) {
            gamma = generateTimeBasedGamma(methodChoice - 12); // ��������� ����� �� ������ ������� � ���������� 12
        } else if (methodChoice == 17 || methodChoice == 18 || methodChoice == 19) {
            gamma = generateTimeBasedGamma(methodChoice - 16); // ��������� ����� �� ������ ������� � ���������� 16
        } else if (methodChoice == 20 || methodChoice == 21 || methodChoice == 22) {
            gamma = generateTimeBasedGamma(methodChoice - 19); // ��������� ����� �� ������ ������� � ���������� 19
        }

        resultText = xorBlock(text, gamma, MODULUS);

        if(methodChoice >=23)
        {
            wcout << L"������ ��� ������ ������� ����������." << endl;
            return 3;
        }

        // ���������� ����� �������� ����� (�4.3.1-�4.3.2, �4.5.1-�4.5.6)
        if (methodChoice == 5 || methodChoice == 7 || methodChoice == 8 || methodChoice == 9) {
            unsigned char lengthByte = static_cast<unsigned char>(text.size());
            resultText += lengthByte; // ��������� ���� �����
        }

        // ���������� ����������� �����, �� ��������� �� ������� (�4.4.1-�4.4.2, �4.6.1.1-�4.6.1.6)
        if (methodChoice == 6 || methodChoice == 10 || methodChoice == 11 || methodChoice == 12) {
            unsigned char checksumByte = calculateChecksum(text);
            resultText += checksumByte; // ��������� ���� ����������� �����
        }

        // ���������� ����������� �����, ��������� �� ������� (�4.6.2.1-�4.6.2.6)
        if (methodChoice == 13 || methodChoice == 14 || methodChoice == 15) {
            unsigned char timeDependentChecksum = calculateTimeDependentChecksum(text);
            resultText += timeDependentChecksum; // ��������� ���� ����������� �����
        }

        // ��������� ����� ����� � ����������� �����, �� ��������� �� ������� (�4.7.1-�4.7.2, �4.8.1.1-�4.8.1.6)
        if (methodChoice == 16 || methodChoice == 17 || methodChoice == 18 || methodChoice == 19)
        {
            unsigned char lengthByte = static_cast<unsigned char>(text.size());
            unsigned char checksumByte = calculateChecksum(text);
            resultText += lengthByte; // ��������� ���� �����
            resultText += checksumByte; // ��������� ���� ����������� �����
        }

        // ���������� ����� ����� � ����������� �����, ��������� �� ������� (�4.8.2.1-�4.8.2.6)
        if (methodChoice == 20 || methodChoice == 21 || methodChoice == 22)
        {
            unsigned char lengthByte = static_cast<unsigned char>(text.size());
            unsigned char timeDependentChecksum = calculateTimeDependentChecksum(text);
            resultText += lengthByte; // ��������� ���� �����
            resultText += timeDependentChecksum; // ��������� ���� ����������� �����
        }


        // ��������� ����� � ���� ��� ������������ ������������
        ofstream gammaFile("gamma.txt", ios::binary);
        gammaFile << gamma;
        gammaFile.close();

         ofstream outputFile(inputFileName, ios::binary | ios::trunc);
        outputFile << resultText;
        outputFile.close();
        return 0;
}
