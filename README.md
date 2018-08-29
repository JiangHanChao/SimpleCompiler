# SimpleCompiler
that is a very simple compiler for top-down iterator analysis from Arithmetic with Custom operation to simple 32-bit virtual machine only-stack register.

## 目录
-------
- src/main.cpp 程序入口
  - analysis 语法及语义分析，主要是对语法分析树类型检查和逻辑检查
  - basic    基类，相当于global文件，所有分析相关类直接继承
  - codemk   目标代码生成，根据目标代码规则和注释语法树生成执行代码
  - compiler 编译器类，不直接继承basic，但集成了相关工具分析类
  - lexay    词法分析，从源代码识别读出词素token
  - syntax   语法分析，读取token生成语法分析树
- spm.cpp    自定义目标虚拟机，运行目标代码
### src
------ 
- 存放上述所有组件cpp文件
-------
### include
-------
- 存放所有组件的头文件
-------
### build
-------
- 存放生成的执行文件和测试文件以及目标代码生成的文件
- win和Linux两种测试文件，原因是回车的不同编码问题
  - test.sp    测试源代码文件，无语法错误
  - etest.sp   测试源代码文件，存在语法错误
  - test.sm    目标代码文件，可以被自定义虚拟机运行
------
## 规则文法
------
加减乘除四则运算+自定义特殊规则
ps: 无函数、无循环、无判断的三无产品

### 特殊文法规则
exp：
* a +\*= b   =>  a = a+a\*b
* a -/= b   =>  a = a-a/b
* a /+= b   =>  a= a/(a+b)
* 类似的同理可得
------

## 语法分析
采用自顶向下的递归迭代语法分析，并进行注释标记生成语义（注释）分析树，更多原理请参照《编译原理》（龙书）:)

## 虚拟机
采取简单的 栈+常量区+寄存器 结构，简单的运行上述规则成功即可

## 运行
-------
- Windows
  - 建立工程（VS、DEVC、VC等）后导入src/ include/文件夹，设置输出路径build/,然后编译运行即可
- Linux
  - 执行cmake CMakeLists.txt生成makefile文件，然后make运行即可
-------

## 按注
主要是理解编译器构造原理，没有往复杂语法方面思考，所以不可避免会有点过于简单，献丑了
