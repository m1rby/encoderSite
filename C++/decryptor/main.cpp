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

// ������� ��� �������� ����� �� ������ ���������� ��������
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
            gamma = (ltm->tm_sec / 10 % 2 == 0) ? "ABCDEFGHIJKLMNOP" : "GHEFABCDOPMNIJKL";
            break;

        case 2: // �4.2.5 (������� �� ��� ������)
            gamma = (ltm->tm_mday % 2 == 0) ? "ABCDEFGHIJKLMNOP" : "GHEFABCDOPMNIJKL";
            break;

        case 3: // �4.2.6 (������� �� ��� ������)
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

// ������� ����������/������������ ������� XOR
string xorBlock(const string& text, const string& gamma) {
    string result;
    for (size_t i = 0; i < text.size(); i++) {
        result += text[i] ^ gamma[i % BLOCK_SIZE];  // XOR � ������
    }
    return result;
}

// ������� ��� ���������� ����������� �����
unsigned char calculateChecksum(const string& text) {
    unsigned char checksum = 0;
    for (char c : text) {
        checksum ^= c;
    }
    return checksum;
}

// ������� ��� �������� ����� ����� ��� ����������� �����
string removeLastByte(const string& text) {
    return (text.size() > 1) ? text.substr(0, text.size() - 1) : text;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian"); // ������������� ������ ��� Windows-1251

    if (argc < 5) {
        cerr << "�������� ���������� ����������." << endl;
        return 1;
    }

    string inputFileName = argv[1];
    string gammaFileName = argv[2];
    char hasLengthByte = (argv[3][0] == 'y');
    char hasSummByte = (argv[4][0] == 'y');

    // ������ ����� � �������� �������
    ifstream inputFile(inputFileName, ios::binary);
    if (!inputFile.is_open()) {
        cerr << "�� ������� ������� ����: " << inputFileName << endl;
        return 1;
    }
    string text((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    inputFile.close();

    // ������ ����� � ������
    ifstream gammaFile(gammaFileName, ios::binary);
    if (!gammaFile.is_open()) {
        cerr << "�� ������� ������� ���� � ������: " << gammaFileName << endl;
        return 1;
    }
    string gamma;
    getline(gammaFile, gamma);
    gammaFile.close();

    if (gamma.size() != BLOCK_SIZE) {
        cerr << "����� ����� ������ ���� ����� " << BLOCK_SIZE << endl;
        return 1;
    }

    // ������������ ������
    string resultText = xorBlock(text, gamma);

    // �������� ����� ����� � ����������� ����� ��� �������������
    if (hasLengthByte) {
        resultText = removeLastByte(resultText);
    }
    if (hasSummByte) {
        resultText = removeLastByte(resultText);
    }

    // ���������� ���������� � ����
    ofstream outputFile(inputFileName, ios::binary | ios::trunc);
    outputFile << resultText;
    outputFile.close();

    cout << "������������ ���������." << endl;
    return 0;
}
