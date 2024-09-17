#include "lineAnalyze.hpp"

void fileProcess::lineAnalyze(fs::path directoryPath) {
    if (!fs::exists(directoryPath)) {
        wcerr << L"path doesn't exist: " << directoryPath << endl;
        return;
    }
    int fileNumOfThis = 0;

    wstring fileName = directoryPath.filename().wstring();
    int commentTotal = 0;
    int blankTotal = 0;
    int statementTotal = 0;

    for (const fs::path& entry : fs::directory_iterator(directoryPath)) {
        if (fs::is_directory(entry)) {
            lineAnalyze(entry);
        } else {
            wstring extension = entry.extension().wstring();
            if (extension.empty()) {
                continue;
            }
//            the line under can be freely changed, to analyze the code type
            if (extension == L".cpp" || extension == L".hpp" || extension == L".c" || extension == L".h") {
                fileNumOfThis++;
                wstring currName = entry.filename().wstring();
                if (currName.empty()) {
                    continue;
                }
                // 使用 wifstream 打开文件
                std::wifstream file(entry);
                // 设置本地化环境
                file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));

                if (!file.is_open()) {
                    wcerr << L"cannot open file: " << entry.wstring() << endl;
                    continue;
                }

                std::wstring line;
                int currBlank = 0;
                int currComment = 0;
                int currStatement = 0;

                while (std::getline(file, line)) {
                    if (line.empty()) {
                        currBlank++;
                        continue;
                    } else if (line.length() >= 2 && line[0] == L'/' && line[1] == L'/') {
                        currComment++;
                        continue;
                    } else {
                        currStatement++;
                    }
                }
                commentTotal += currComment;
                blankTotal += currBlank;
                statementTotal += currStatement;

                file.close();
                this->add(currName, currComment, currBlank, currStatement, 1);
            } else {
                continue;
            }
        }
    }
    if (fileNumOfThis > 0) {
        this->add(fileName, commentTotal, blankTotal, statementTotal, fileNumOfThis);
    }
}

void fileProcess::add(wstring _fileName, int _comment, int _blank, int _statement, int _fileNum) {
    if (this->p == nullptr) {
        this->p = new fileNode(_fileName, _comment, _blank, _statement, _fileNum);
        this->p->next = nullptr;
        len++;
        return;
    }
    fileNode* current = this->p;
    while (current->next != nullptr) {
        current = current->next;
    }
    current->next = new fileNode(_fileName, _comment, _blank, _statement, _fileNum);
    current->next->next = nullptr;
    len++;
}

bool is_fullwidth(wchar_t c) {
    // 根据 Unicode 码点范围判断
    // 简单地说，CJK（中文、日文、韩文）字符通常位于以下范围
    return (c >= 0x1100 && c <= 0x115F) || // Hangul Jamo
           (c >= 0x2E80 && c <= 0xA4CF) || // CJK Radicals Supplement .. Yi Radicals
           (c >= 0xAC00 && c <= 0xD7A3) || // Hangul Syllables
           (c >= 0xF900 && c <= 0xFAFF) || // CJK Compatibility Ideographs
           (c >= 0xFE10 && c <= 0xFE19) || // Vertical Forms
           (c >= 0xFE30 && c <= 0xFE6F) || // CJK Compatibility Forms
           (c >= 0xFF00 && c <= 0xFF60) || // Halfwidth and Fullwidth Forms
           (c >= 0xFFE0 && c <= 0xFFE6);
}

// 计算字符串的显示宽度
int get_display_width(const std::wstring& str) {
    int width = 0;
    for (wchar_t c : str) {
        if (is_fullwidth(c)) {
            width += 2;
        } else {
            width += 1;
        }
    }
    return width;
}
//上面这两个函数是从网上找的妙妙工具，专门用来处理非英文的字符串，不要修改，改了的话我也不会改回来
void fileProcess::print() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int width;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }

    const int nameWidth = 22;
    const int extraWidth = 2;  // 中文文件名额外的宽度补偿

    wcout << L"totally " << len << L" items           ";
    wcout << L"statement" << L" " << L"comment" << L"  " << L"blank  ";
    wcout << L"perBlank  " << L"perComment  " << L"avgStmt  " << L"avgLine  " << endl;
    for (int i = 0; i < width; i++) {
        wcout << L"=";
    }
    wcout << endl;

    fileNode* count = p;
    while (count != nullptr) {
        int totalLine = count->statement + count->blank + count->comment;
        if (totalLine == 0) {
            totalLine = 1;
        }

        std::wstring fileName = count->fileName;
        int nameDisplayWidth = get_display_width(fileName);

        // 对含有中文字符的文件名行进行额外宽度补偿
        if (nameDisplayWidth > nameWidth) {
            nameDisplayWidth += extraWidth;  // 增加额外的宽度
        }

        int namePadding = nameWidth - nameDisplayWidth;
        if (namePadding < 0) namePadding = 0;  // 防止负数空格补偿

        wcout << left << fileName;
        if (namePadding > 0) {
            wcout << std::wstring(namePadding, L' ');
        }
//        这里是为了显示对齐而找的代码，不要动，我不会修

        wcout << right << setw(3) << count->fileNum;
        wcout << setw(10) << count->statement;
        wcout << setw(8) << count->comment;
        wcout << setw(7) << count->blank;
        wcout << setw(9) << fixed << setprecision(2)
              << (static_cast<double>(count->blank) / totalLine) * 100 << L'%';
        wcout << setw(11) << fixed << setprecision(2)
              << (static_cast<double>(count->comment) / totalLine) * 100 << L'%';
        wcout << setw(9) << fixed << setprecision(2)
              << static_cast<double>(count->statement) / count->fileNum;
        wcout << setw(11) << fixed << setprecision(2)
              << static_cast<double>(totalLine) / count->fileNum << endl;

        count = count->next;
    }
}



fileProcess::~fileProcess() {
    fileNode* current = p;
    while (current != nullptr) {
        fileNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    p = nullptr;
}
