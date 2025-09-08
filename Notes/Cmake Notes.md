# Cmake Notes

## 1.Cmake概述

​	Cmake就是一个跨平台构建工具项目工具，他可以通过cmake语言组织项目进行构建某平台项目文件，比如windows平台vstudio的sln解决项目文件，前提是得有各平台的编译工具比如vstudio的cl.exe。假如通过的是msys的MinGw Makefiles----指能生成makefile文件的构建形式，他会生成一个makefile文件，我们通过make命令就可以编译该项目。

​	大多数的IDE都集成了make，比如vs的nmake，linux下的GNU make、Qt的qmake等。

## 2.Cmake语言

​	Cmake支持大写、小写、混合大小写的命令。

### 2.1注释

#### 2.1.1注释行

​	Cmake通过行开头的#来注释该行：

```cmake
#这是一个行注释
```

#### 2.1.2注释块

​	使用的是'#[[注释的内容]]'来进行注释

```cmake
#[[
这是一块注释
]]
```

### 2.2构建一个基础简单项目

#### 2.2.1cmake_minumum_required

​	指定项目构建的最低版本，假如不指定可能会报警告

```cmake
cmake_minimum_required(VERSION 3.0)
```

#### 2.2.2project

​	用于指定项目名字，指定项目版本、工程描述、web主页地址、支持的语言、假如不需要这些可以忽略只需要制定项目名字即可。

```cmake
# PROJECT 指令的语法是：
project(<PROJECT-NAME> [<language-name>...])
project(<PROJECT-NAME>
       [VERSION <major>[.<minor>[.<patch>[.<tweak>]]]]
       [DESCRIPTION <project-description-string>]
       [HOMEPAGE_URL <url-string>]
       [LANGUAGES <language-name>...])
```

#### 2.2.3add_executable

​	假如不需要寻找头文件头文件路径，能直接找到，不需要额外指定头文件搜索路径，那么可以使用该命令添加生成exe需要的实现文件。

```cmake
add_executable(可执行程序名 源文件名称)
```

##### 为什么创建build文件夹

​	程序在编译过程中还会生成一些中间文件和一个可执行文件，这样会导致整个项目目录看起来很混乱，不太容易管理和维护，此时我们就可以把生成的这些与项目源码无关的文件统一放到一个对应的目录里边，比如将这个目录命名为build。

#### 2.2.4set命令

```
# SET 指令的语法是：
# [] 中的参数为可选项, 如不需要可以不写
SET(VAR [VALUE] [CACHE TYPE DOCSTRING [FORCE]])
```

**1.定义普通变量**

```cmake
set(MY_VAR "hello")
message(${MY_VAR})   # 输出 hello
```

作用域：默认在当前 **CMakeLists.txt 及其子目录** 中有效。

如果同名变量在子目录里被修改，不会影响父目录里的值。

2.**定义列表**

```
set(MY_LIST a b c)
message(${MY_LIST})      # 输出 a;b;c
list(LENGTH MY_LIST len)
message(${len})          # 输出 3
```

CMake 用 `;` 分隔列表项。

3.**作用域控制**

3.1 **PARENT_SCOPE**

把变量传递给父作用域：

```
set(VAR "from child" PARENT_SCOPE)
```

3.2 **CACHE**

定义 **缓存变量**，在 `cmake-gui` 或 `ccmake` 可见，也能在命令行通过 `-D` 覆盖：

```
set(MY_OPTION "default" CACHE STRING "My option description")
```

等价于：

```
cmake -DMY_OPTION=custom ..
```

4.**条件控制用法**

```
set(BUILD_TESTS ON)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
```

5. 修改 Cmake **内置变量**

比如修改编译选项、输出路径：

```
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
```

### 2.3构建需要搜索头文件的项目

#### 2.3.1file（个人觉得好用）

file可以做很多功能（官方文档里），现在只用它做文件搜索

```
file(GLOB/GLOB_RECURSE 变量名 要搜索的文件路径和文件类型)
```

- GLOB: 将指定目录下搜索到的满足条件的所有文件名生成一个列表，并将其存储到变量中。
- GLOB_RECURSE：递归搜索指定目录，将搜索到的满足条件的文件名生成一个列表，并将其存储到变量中。

搜索当前目录的src目录下所有的源文件，并存储到变量中

```
file(GLOB MAIN_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB MAIN_HEAD ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
```

**CMAKE_CURRENT_SOURCE_DIR 宏表示当前访问的 CMakeLists.txt 文件所在的路径。**

关于要搜索的文件路径和类型可加双引号，也可不加:

```
file(GLOB MAIN_HEAD "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
```

#### 2.3.2include_directories

​	包用于指定头文件搜索路径

```
include_directories(headpath)
```

### 2.4构建一个包含静态库的项目

#### 2.4.1add_library

制作静态库的命令

```
add_library(库名称 STATIC 源文件1 [源文件2] ...) 
```

在Linux中，静态库名字分为三部分：lib+库名字+.a，此处只需要指定出库的名字就可以了，另外两部分在生成该文件的时候会自动填充。

在Windows中虽然库名和Linux格式不同，但也只需指定出名字即可。

#### 2.4.2set_target_properties（通用静动态库，而且最推荐）

用于设置输出路径的（动态库和静态库通用），也有其他用法，如名字一样设置属性的都可以，比如指定编译语言的版本c++17等。

```
set_target_properties(<target1> <target2> ...
    PROPERTIES <prop1> <value1>
               <prop2> <value2> ...)
```

- `<target>`：用 `add_executable()` 或 `add_library()` 定义过的目标。
- `PROPERTIES` 后面是 **键值对**，可以一次性设置多个属性。

```
set_target_properties(myLib PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"   # 静态库
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"   # 动态库
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"   # 可执行文件 / DLL
)
```

**注意**：必须在target定义之后才可以用。不用这个命令就用set命令去改变宏的值。

**Windows和Linux下的区别:**

- Windows:
  - .exe和.dll属于RUNTIME_OUTPUT_DIRECTORY
  - .lib用ARCHIVE_OUTPUT_DIRECTORY
- Linux：
  - .so是LIBRARY_OUTPUT_DIRECTORY，.a是用ARCHIVE_OUTPUT_DIRECTORY，.exe使用RUNTIME_OUTPUT_DIRECTORY

### 2.5链接静态库

​	首先要认识的就是我们链接库首先就是要知道：

1. 库的头文件在哪（也是使用include_directories这个命令）
2. 库的路径
3. 库的名字

#### 2.5.1link_directories

指定库的路径

```
link_directories(<lib path>)
```

#### 2.5.2link_libraries（静态动态都可）

想要链接库的名字

```
link_libraries(<static lib> [<static lib>...])
```

### 2.6构建动态库

​	和构建动态库不同的地方只有需要把add_library中的字段从**STATIC**改为**SHARED**

### 2.7链接动态库

​	和链接静态库的做法不同是因为原理不同，一个是整合到代码里去一个是链接。所以target_link_libararies命令需要写到add_executable后面

#### 2.6.1target_link_libraries

```
target_link_libraries(
    <target> 
    <PRIVATE|PUBLIC|INTERFACE> <item>... 
    [<PRIVATE|PUBLIC|INTERFACE> <item>...]...)
```

用于指定一个目标（如可执行文件或库）在编译时需要链接哪些库。它支持指定库的名称、路径以及链接库的顺序。

- target：指定要加载的库的文件的名字

该文件可能是一个源文件
该文件可能是一个动态库/静态库文件
该文件可能是一个可执行文件
PRIVATE|PUBLIC|INTERFACE：动态库的访问权限，默认为PUBLIC

如果各个动态库之间没有依赖关系，无需做任何设置，三者没有没有区别，一般无需指定，使用默认的 PUBLIC 即可。

动态库的链接具有传递性，如果动态库 A 链接了动态库B、C，动态库D链接了动态库A，此时动态库D相当于也链接了动态库B、C，并可以使用动态库B、C中定义的方法。

```
target_link_libraries(A B C)
target_link_libraries(D A)
```

- PUBLIC：在public后面的库会被Link到前面的target中，并且里面的符号也会被导出，提供给第三方使用。
  PRIVATE：在private后面的库仅被link到前面的target中，并且终结掉，第三方不能感知你调了啥库
  INTERFACE：在interface后面引入的库不会被链接到前面的target中，只会导出符号。
  链接系统动态库
  动态库的链接和静态库是完全不同的：

静态库会在生成可执行程序的链接阶段被打包到可执行程序中，所以可执行程序启动，静态库就被加载到内存中了。
动态库在生成可执行程序的链接阶段不会被打包到可执行程序中，当可执行程序被启动并且调用了动态库中的函数的时候，动态库才会被加载到内存

##### link_libraries和target_link_libraries区别

🔹 `link_libraries`

- **作用范围：全局** 或 **目录作用域**。（全局的意思是子目录构建的exe也会自动链接该库）
- 它会影响在它之后定义的所有 target。
- 等价于给每个 target 都加上这些库。

```
link_libraries(mylib anotherlib)

add_executable(app1 main1.cpp)  # 自动链接 mylib 和 anotherlib
add_executable(app2 main2.cpp)  # 也会自动链接
```

⚠️ 缺点：

- 不够精确，容易误把库链接到不需要的 target 上。
- 在大项目里可能导致链接顺序和依赖混乱。

------

🔹 `target_link_libraries`

- **作用范围：单个 target**。
- 推荐用法，精确控制每个目标需要链接的库。

```
add_executable(app1 main1.cpp)
target_link_libraries(app1 PRIVATE mylib)

add_executable(app2 main2.cpp)
target_link_libraries(app2 PRIVATE anotherlib)
```

这样 `app1` 只链接 `mylib`，`app2` 只链接 `anotherlib`。

------

🔹 `target_link_libraries` 的可见性关键字

- **PRIVATE**：只在自己编译时用到（不会传递给依赖者）。
- **PUBLIC**：自己需要，依赖者也会自动继承。
- **INTERFACE**：自己不用，依赖者需要（典型用在头文件库）。

例子：

```
add_library(mathLib src/math.cpp)
target_link_libraries(mathLib PUBLIC utilsLib)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE mathLib)
```

这里 `app` 会自动继承 `utilsLib`，因为 `mathLib` 里是 `PUBLIC`。

### 2.8构建一个能够打印消息的库

#### 2.8.1message

```
message([STATUS|WARNING|AUTHOR_WARNING|FATAL_ERROR|SEND_ERROR] "message to display" ...)
```

- (无) ：重要消息
- STATUS ：非重要消息
- WARNING：CMake 警告, 会继续执行
- AUTHOR_WARNING：CMake 警告 (dev), 会继续执行
- SEND_ERROR：CMake 错误, 继续执行，但是会跳过生成的步骤
- FATAL_ERROR：CMake 错误, 终止所有处理过程

​	CMake的命令行工具会在stdout上显示STATUS消息，在stderr上显示其他所有消息。CMake的GUI会在它的log区域显示所有消息。

​	CMake警告和错误消息的文本显示使用的是一种简单的标记语言。文本没有缩进，超过长度的行会回卷，段落之间以新行做为分隔符。

### 2.9对变量操作

#### 2.9.1 set追加

​	有时候项目中的源文件并不一定都在同一个目录中，但是这些源文件最终却需要一起进行编译来生成最终的可执行文件或者库文件。如果我们通过file命令对各个目录下的源文件进行搜索，最后还需要做一个字符串拼接的操作，关于字符串拼接可以使用set命令也可以使用list命令。

**使用set拼接**
如果使用set进行字符串拼接，对应的命令格式如下：

```
set(变量名1 ${变量名1} ${变量名2} ...)
```

​	关于上面的命令其实就是将从第二个参数开始往后所有的字符串进行拼接，最后将结果存储到第一个参数中，如果第一个参数中原来有数据会对原数据就行覆盖。

#### 2.9.2list追加

使用list拼接
如果使用list进行字符串拼接，对应的命令格式如下：

```
list(APPEND <list> [<element> ...])
```

​	list命令的功能比set要强大，字符串拼接只是它的其中一个功能，所以需要在它第一个参数的位置指定出我们要做的操作，APPEND表示进行数据追加，后边的参数和set就一样了。
​	在CMake中，使用set命令可以创建一个list。一个在list内部是一个由分号;分割的一组字符串。例如，set(var a b c d e)命令将会创建一个list:a;b;c;d;e，但是最终打印变量值的时候得到的是abcde。

#### 2.9.3 list操作字符串移除

我们在通过file搜索某个目录就得到了该目录下所有的源文件，但是其中有些源文件并不是我们所需要的，比如：

0 directories, 5 files
在当前这么目录有五个源文件，其中main.cpp是一个测试文件。如果我们想要把计算器相关的源文件生成一个动态库给别人使用，那么只需要add.cpp、div.cp、mult.cpp、sub.cpp这四个源文件就可以了。此时，就需要将main.cpp从搜索到的数据中剔除出去，想要实现这个功能，也可以使用list。

```
list(REMOVE_ITEM <list> <value> [<value> ...])
```

通过上面的命令原型可以看到删除和追加数据类似，只不过是第一个参数变成了REMOVE_ITEM。

移除 main.cpp

```
list(REMOVE_ITEM SRC_1 ${PROJECT_SOURCE_DIR}/main.cpp)
```

关于list命令还有其它功能，但是并不常用，在此就不一一进行举例介绍了。

**获取 list 的长度**

```
list(LENGTH <list> <output variable>)
```

LENGTH：子命令LENGTH用于读取列表长度

- \<list>：当前操作的列表

- \<output variable>：新创建的变量，用于存储列表的长度。

**读取列表中指定索引的的元素，可以指定多个索引**

```
list(GET <list> <element index> [<element index> ...] <output variable>)
```

- \<list>：当前操作的列表
- \<element index>：列表元素的索引
  从0开始编号，索引0的元素为列表中的第一个元素；
  索引也可以是负数，-1表示列表的最后一个元素，-2表示列表倒数第二个元素，以此类推
  当索引（不管是正还是负）超过列表的长度，运行会报错
- \<output variable>：新创建的变量，存储指定索引元素的返回结果，也是一个列表。

**list还有不少功能，可以查官方文档**

### 2.10如何构建嵌套的复杂项目

​	如果项目很大，或者项目中有很多的源码目录，在通过CMake管理项目的时候如果只使用一个CMakeLists.txt，那么这个文件相对会比较复杂，有一种化繁为简的方式就是给每个源码目录都添加一个CMakeLists.txt文件（头文件目录不需要），这样每个文件都不会太复杂，而且更灵活，更容易维护。

#### 2.10.1add_subdirectory

​	添加子目录，在此CmakeLists.txt文件执行到这个命令的时候会进入到该目录下的CmakeLists.txt文件中进行执行。

```Cmake
add_subdirectory(source_dir [binary_dir] [EXCLUDE_FROM_ALL])
```

- source_dir : 指定了CmakeLists.txt源文件和代码文件的位置，其实就是指定子目录
- binary_dir:指定输出文件的路径，一般不需要指定，可以忽略。
- EXCLUDE_FROM_ALL:在子路径下的目标默认不会被包含到父路径的ALL目标里，并且也会被排除在IDE工程文件之外。用户必须显示的构建在子路径下的目标。也可以忽略一般。

### 2.11流程控制

#### 2.11.1条件判断if命令

```cmake
if(<condition>)
	<commands>
elseif(<condition>) #可选块可以重复
	<commands>
else()		#可选块
	<commands>
endif()
```

##### condition为表达式的情况

- 如果是1，ON，YES，TRUE，Y，非零值，非空字符串，条件返回真
- 如果是0，OFF，NO，FALSE，N，IGNORE，NOTFOUND，空字符串，条件返回否

#### 2.11.2逻辑运算符

- NOT(就是取反)
- ADN
- OR(或)

#### 2.11.3比较运算符·

- LESS
- GREATER
- EQUAL
- LESS_EQUAL
- GREATER_EQUAL

**基于字符串的比较**

- STRLESS
- STRGREATER
- STREQUAL
- STRLESS_EQUAL
- STRGREATER_EQUAL

#### 2.11.4文件相关的逻辑操作

- 判断文件是否存在或者目录是否存在

  - ```
    if(EXISTS path-to-file-or-directory)
    ```

- 判断是否是目录

  - ```
    if(IS_DIRECTORY path)
    #path必须是绝对路径
    ```

- 判断是否是软连接

  - ```
    if(IS_SYMLINK file-name)
    #file-name也必须是绝对路径
    ```

- 判断是不是绝对路径

  - ```
    if(IS_ABSOLUTE path)
    ```

#### 2.11.5其他

- 判断某个元素是否在列表中

  - ```
    if(<variable|string> IN_LIST <variable>)
    ```

- 比较两个路径是否相同

  - ```
    if(<variable|string> PATH_EQUAL <variable|string>)
    ```

  - cmake版本大于3.24

### 2.12循环

#### 2.12.1foreach

```
foreach(<loop_var> <items>)
	<commands>
endforeach()
```

​	通过foreach我们可以对items中的数据进行遍历，然后通过loop_var将遍历到的当前值取出，在取值的时候有以下几种用法：

方法1：

```
foreach(<loop_var> RANGE <step>)
```

- RANGE:关键词，表示要遍历的范围
- stop:这是一个正整数，表示的范围的结束值，在遍历的时候从0开始，最大值为stop。
- loop_var：存储每次循环取出来的值

还有其他几种方法，要用到的时候在去官方文档进行查阅吧。

3.Cmake的宏

### 3.1宏定义

​	在代码中有debug版本和release版本，我们想要灵活的进行版本调用，在gcc和g++中编译的时候是加上-D<宏的名字>来做操作的如：

```
#ifdef DBUG
```

那么我们想打开这个宏我们就需要在编译的时候加上-DDBUG这个选项

​	那我们想要在cmake中打开这个操作就需要用到这个命令

#### 3.1.1add_definitions(-D<宏名称>)

### 3.2预定义宏

|                 宏名称                  |                             功能                             |
| :-------------------------------------: | :----------------------------------------------------------: |
|           PROJECT_SOURCE_DIR            |                  当前project()的源码根目录                   |
|           PROJECT_BINARY_DIR            |         当前project()的构建目录，通常是-B指定的路径          |
|        CMAKE_CURRENT_SOURCE_DIR         |              当前处理的CmakeLists.txt所在的目录              |
|        CMAKE_CURRENT_BINARY_DIR         |                      当前指定的构建目录                      |
|         EXECUTABLE_OUTPUT_PATH          |              重新定义二进制可执行程序的存放位置              |
| LIBRARY_OUTPUT_PATH（太老了不推荐使用） |                 重新定义链接库文件的存放位置                 |
|            CMAKE_BINARY_DIR             | 项目实际构建路径，假设在build目录进行的构建，那么得到的也是这个目录的路径（是最顶层） |
|            CMAKE_SOURCE_DIR             |     最顶层Cmake项目的源码目录（整个构建树的顶层源目录）      |

