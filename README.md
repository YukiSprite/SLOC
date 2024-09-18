### Sloc（count the Source Line Of Code)
#### 项目介绍
本项目是一个代码统计项目，可以分析你的代码的风格。我把代码分为三种：comment line, blank line and statement line.
在这个版本中，我只处理"//"类型的注释，不处理"*/"类型的注释
- 如果一行代码以//开头，则为comment line
- 若整一行都是空的，则为blank line
- 否则就是statement line
目前默认处理c语言和c++的文件，可以在lineAnalyze.cpp中增加你们需要的文件类型
#### 指令表如下：
- \H: list out all command
- \E: regular start
- \Q: quit


#### 期望输出：
输出以表格形式呈现（考虑后续改进为excel表格），第一行是标题。从左到右为totally x items, 这一列从上往下为tatal，average和扫描到的各个文件的文件名。之后是statement, comment和blank。内容是不同文件中这些行的行数。然后是perBlank，perComment，这两列按照百分比的形式表达了blank与comment在不同文件中的占比。sources列的内容是包含的文件数量，avgStmt是平均statement数量。avgLine是平均每个文件的代码行数


