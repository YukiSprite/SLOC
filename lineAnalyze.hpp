#ifndef MYSLOC_LINEANALYZE_HPP
#define MYSLOC_LINEANALYZE_HPP

#include <iostream>
#include <filesystem>
#include <fstream>
#include <windows.h>
#include <iomanip>
#include <locale>
using namespace std;
namespace fs = std::filesystem;

class fileNode {
public:
    fileNode(wstring _fileName, int _comment, int _blank, int _statement, int _fileNum)
            : fileName(_fileName), comment(_comment), blank(_blank),
              statement(_statement), fileNum(_fileNum), next(nullptr) {}

    wstring fileName;
    int comment;
    int blank;
    int statement;
    int fileNum;
    fileNode* next;
};

class fileProcess {
private:
    fs::path directoryPath;
    fileNode* p;
    int len;

public:
    fileProcess(fs::path add) : directoryPath(add), p(nullptr), len(0) {}

    void add(wstring _fileName, int _comment, int _blank, int _statement, int _fileNum);
    void lineAnalyze(fs::path directoryPath);
    void print();
    ~fileProcess();
};

#endif // MYSLOC_LINEANALYZE_HPP
