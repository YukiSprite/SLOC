#include <iostream>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include "lineAnalyze.hpp"

using namespace std;

void printCommand();

int main() {
    // 设置控制台输入和输出编码为 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // 设置标准输出和输入流为 Unicode 模式
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

    // 设置全局的 locale，确保可以正确处理中文输入输出
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale());
    std::wcin.imbue(std::locale());
//    不要管上面这几行，网上抄的输出中文的代码，别动

    printCommand();
    while (true) {
        wstring command;
        wcin >> command;
        if (command == L"-H") {
            printCommand();
        } else if (command == L"-E") {
            try {
                wstring isExcelChar;
                bool isExcel;
                wcout<<L"do you want to ouput as an excel document? if yes enter yes, or no"<<endl;
                wcin>>isExcelChar;
                wcout << L"please enter path: "<<endl;
                wstring tmp;
                wcin >> tmp;
                if (isExcelChar == L"yes") {
                    isExcel = true;
                }else if (isExcelChar == L"no") {
                    isExcel = false;
                }
                else {
                    wcerr<<"please enter yes or no"<<endl;
                    return 1;
                }
                fs::path directoryPath = tmp;
                fileProcess fp(directoryPath);
                fp.lineAnalyze(directoryPath);
                if (isExcel){
                    wcout<<L"please enter ouput path and file name(with'.xlsx')"<<endl;
                    wstring outputPath;
                    wcin>>outputPath;
                    wcout << L"Debug: Output path is " << outputPath << endl;
                    fp.excelPrint(outputPath);
                } else {
                    fp.print();
                }
                wcout<<L"finish processing"<<endl;
            } catch (const fs::filesystem_error& e) {
                wcerr << L"file system error: " << e.what() << endl;
            } catch (const std::exception& e) {
                wcerr << L"std error: " << e.what() << endl;
            } catch (const char* msg) {
                wcerr << L"error: " << msg << endl;
            }
        } else if (command == L"-Q") {
            break;
        }
    }

    return 0;
}

void printCommand() {
    wcout << L"-H: list all command" << endl;
    wcout << L"-E: start program" << endl;
    wcout << L"-Q: quit" << endl;
}
