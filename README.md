
# Lumos

# 反正就是瞎写，我也不知道最后能不能写完

Lumos 是一个正在开发中的编程语言，这个储存库中的文档描述了已实现和计划实现的语言功能。

但因为这个项目仍处于早期开发阶段，所以文档和代码可能随时发生变化。

中文名为 光耀

---

**Lumos 编程语言**

Lumos 是一个简单而灵活的编程语言，旨在提供现代化的编程体验和强大的功能。它结合了 C/C++ 的灵活性和现代编程语言的简洁性，同时具有自动引用计数的内存管理和其他高级特性。

## 特性

我希望制作一个这样的语言：

- **简洁易学**：语法简洁清晰，易于学习和使用。
- **自动内存管理**：支持自动引用计数与标记清除的内存管理，减少内存泄漏风险。
- **支持引用和指针**：提供类似于 C++ 中的引用和指针的语法和语义，灵活性更高。
- **支持栈上和堆上对象**：支持栈上对象和堆上对象，方便灵活地管理对象的生命周期。
- **现代特性**：支持诸如类型推断、Lambda 表达式等现代特性，提高开发效率。

## 示例

待补充

## 安装与使用

暂时无法使用

## 贡献

抱歉，在早期开发阶段您无法贡献代码，但您仍然可以通过提交 issue、提供建议或参与讨论等方式为 Lumos 的发展做出贡献。

## 许可证

该项目开发完成后计划基于 MIT 许可证进行许可。

## 结构

- **ast** 语法树结构
- **bytecode**
- **codegen**
- **lexer** 词法分析器 将输入转换为token
- **parser** 语法分析器 将token转换为语法树
