# ARGUMENT_PARSER

## 简述

参数解析器（argument parser）用于解析程序 / main 函数的参数信息。

## 概述

### 使用

在项目的具体使用方法，详见 [1nchy/project_template](https://github.com/1nchy/project_template)。

### 示例

我们模拟 gcc 对指定源文件进行编译的过程，详情见 [gcc compiler](./example/compiler/)

### 设计

详情见 [设计文档](./doc/design.md)。

## 项目结构

~~~txt
finite_state_machine
├── .gitignore
├── CMakeLists.txt
├── LICENSE.md
├── README.md
├── doc                         # 项目文档
│   ├── design.md               # 设计文档
│   └── compiler
│       └── usage.md            # 使用说明
├── include                     # 头文件（对外使用）
│   └── ...
├── src                         # 源文件
│   └── ...
└── example                     # 示例
    └── compiler                # 编译示例
        └── ...
~~~