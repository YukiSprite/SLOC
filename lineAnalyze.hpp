#ifndef MYSLOC_LINEANALYZE_HPP
#define MYSLOC_LINEANALYZE_HPP

#include <iostream>
#include <filesystem>
#include <fstream>
#include <windows.h>
#include <iomanip>
#include <locale>
#include <OpenXLSX.hpp>
#include <codecvt>
using namespace std;
namespace fs = std::filesystem;

class fileNode {
public:
    fileNode(wstring _fileName, int _comment, int _blank, int _statement, int _fileNum)
            : fileName(_fileName), commentNum(_comment), blankNum(_blank),
              statementNum(_statement), fileNum(_fileNum), next(nullptr) {}

    wstring fileName;
    int commentNum;
    int blankNum;
    int statementNum;
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
    void excelPrint(wstring directoryOutputPath);
    void add(wstring _fileName, int _commentNum, int _blankNum, int _statementNum, int _fileNum);
    void lineAnalyze(fs::path directoryPath);
    void print();
    ~fileProcess();
};

#endif // MYSLOC_LINEANALYZE_HPP
