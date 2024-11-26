#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <locale>
#include <codecvt>
#include <regex>
#include <windows.h>

using namespace std;
vector<wstring> tokenize(const wstring& text) {
    wregex word_regex(L"\\w+(?:-\\w+)*");
    wsregex_iterator it(text.begin(), text.end(), word_regex);
    wsregex_iterator end;
    vector<wstring> words;

    while (it != end) {
        words.push_back(it->str());
        ++it;
    }

    return words;
}

bool isAllLetters(const wstring& str) {
    locale loc("ru_RU.UTF-8");
    for (wchar_t ch : str) {
        if (!iswalpha(ch)) {
            return false;
        }
    }
    return true;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    locale::global(locale(""));
    wcin.imbue(locale());
    wcout.imbue(locale());

    wstring input;
    getline(wcin, input);

    wstringstream wss(input);
    wstring filename, word1, word2;
    double distance;

    if (!(wss >> filename >> word1 >> word2 >> distance)) {
        wcout << "Ошибка во входных параметрах" << endl;
        return 0;
    }

    wstring extra;
    if (wss >> extra) {
        wcout << "Ошибка во входных параметрах" << endl;
        return 0;
    }

    if (distance < 0) {
        wcout << "Расстояние отрицательное" << endl;
        return 0;
    }

    if (distance != static_cast<int>(distance)) {
        wcout << "Расстояние нецелое" << endl;
        return 0;
    }

    if (!isAllLetters(word1) || !isAllLetters(word2)) {
        wcout << "В словах недопустимые символы" << endl;
        return 0;
    }

    wifstream file(filename);
    file.imbue(locale(locale(), new codecvt_utf8<wchar_t>));

    if (!file.is_open()) {
        wcout << "Не удалось открыть файл" << endl;
        return 0;
    }

    wstring line;
    vector<wstring> words;

    while (getline(file, line)) {
        vector<wstring> line_words = tokenize(line);
        words.insert(words.end(), line_words.begin(), line_words.end());
    }

    file.close();

    int count = 0;
    vector<bool> used(words.size(), false);

    for (size_t i = 0; i < words.size(); ++i) {
        if (used[i]) continue;
        if (words[i] == word1) {
            for (size_t j = i + 1; j < words.size() && j - i <= distance + 1; ++j) {
                if (used[j]) continue;
                if (words[j] == word2) {
                    count++;
                    used[i] = true;
                    used[j] = true;
                    break;
                }
            }
        }
        else if (words[i] == word2) {
            for (size_t j = i + 1; j < words.size() && j - i <= distance + 1; ++j) {
                if (used[j]) continue;
                if (words[j] == word1) {
                    count++;
                    used[i] = true;
                    used[j] = true;
                    break;
                }
            }
        }
    }

    wcout << count << endl;

    return 0;
}