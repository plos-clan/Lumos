# 文件操作 `std.file`

## 文件打开和关闭

### 打开文件

```lumos
// 打开文件模式
\type> FileMode {
    read = 1,       // 只读
    write = 2,      // 只写（覆盖）
    append = 4,     // 追加写入
    read_write = 3, // 读写（覆盖）
}

// 打开文件
act[fs{read,write}] open(str path, FileMode mode) -> Result<File, Error>;

// 以读模式打开
act[fs.read] open_read(str path) -> Result<File, Error>;

// 以写模式打开
act[fs.write] open_write(str path) -> Result<File, Error>;

// 以追加模式打开
act[fs.write] open_append(str path) -> Result<File, Error>;
```

### 关闭文件

```lumos
\type> File {
    // 关闭文件
    act[fs.write] close() -> unit;

    // 检查文件是否打开
    fun is_open() -> bool;
}
```

## 读文件操作

### 读取内容

```lumos
\type> File {
    // 读取指定字节数
    act[fs.read] read_bytes(usize n) -> Result<[u8], Error>;

    // 读取一行（包含换行符）
    act[fs.read] read_line() -> Result<str, Error>;

    // 读取整个文件
    act[fs.read] read_all() -> Result<str, Error>;

    // 读取直到指定字符
    act[fs.read] read_until(u8 delimiter) -> Result<str, Error>;

    // 按行迭代读取
    act[fs.read] lines() -> Iterator<Result<str, Error>>;
}
```

### 文件指针操作

```lumos
\type> File {
    // 获取当前文件指针位置（字节）
    act[fs.read] position() -> u64;

    // 设置文件指针位置
    // whence: 0=开始, 1=当前, 2=末尾
    act[fs{read,write}] seek(i64 offset, i32 whence) -> Result<u64, Error>;

    // 跳到文件开始
    act[fs{read,write}] rewind() -> unit;

    // 跳到文件末尾
    act[fs{read,write}] seek_end() -> Result<u64, Error>;

    // 检查是否到达文件末尾
    act[fs.read] is_eof() -> bool;
}
```

## 写文件操作

### 写入内容

```lumos
\type> File {
    // 写入字节
    act[fs.write] write_bytes([u8] data) -> Result<usize, Error>;

    // 写入字符串
    act[fs.write] write_str(str s) -> Result<usize, Error>;

    // 写入单个字符
    act[fs.write] write_char(u8 ch) -> Result<unit, Error>;

    // 写入格式化字符串
    act[fs.write] write_fmt(str fmt, ...) -> Result<usize, Error>;

    // 刷新缓冲
    act[fs.write] flush() -> Result<unit, Error>;
}
```

## 文件信息

### 文件元数据

```lumos
\type> Metadata {
    // 文件大小（字节）
    fun size() -> u64;

    // 最后修改时间
    fun modified_time() -> i64;  // Unix timestamp

    // 最后访问时间
    fun accessed_time() -> i64;

    // 创建时间
    fun created_time() -> i64;

    // 检查是否是文件
    fun is_file() -> bool;

    // 检查是否是目录
    fun is_dir() -> bool;

    // 检查是否是符号链接
    fun is_symlink() -> bool;

    // 获取权限位
    fun permissions() -> u32;  // Unix permissions
}

// 获取文件元数据
act[fs.read] metadata(str path) -> Result<Metadata, Error>;
```

### 文件属性检查

```lumos
// 检查文件是否存在
act[fs.read] exists(str path) -> bool;

// 获取文件大小
act[fs.read] file_size(str path) -> Result<u64, Error>;

// 检查文件可读性
act[fs.read] is_readable(str path) -> bool;

// 检查文件可写性
act[fs.read] is_writable(str path) -> bool;

// 检查是否为目录
act[fs.read] is_directory(str path) -> bool;
```

## 文件系统操作

### 复制和移动

```lumos
// 复制文件
act[fs{read,write}] copy(str src, str dst) -> Result<unit, Error>;

// 移动或重命名文件
act[fs.write] rename(str old_path, str new_path) -> Result<unit, Error>;

// 删除文件
act[fs.write] remove(str path) -> Result<unit, Error>;
```

### 目录操作

```lumos
// 创建目录
act[fs.write] mkdir(str path) -> Result<unit, Error>;

// 递归创建目录
act[fs.write] mkdir_all(str path) -> Result<unit, Error>;

// 删除空目录
act[fs.write] rmdir(str path) -> Result<unit, Error>;

// 递归删除目录及其内容
act[fs.write] remove_dir_all(str path) -> Result<unit, Error>;

// 列出目录内容
act[fs.read] read_dir(str path) -> Result<Iterator<DirEntry>, Error>;

// 获取当前工作目录
act[fs.read] current_dir() -> Result<str, Error>;

// 改变当前工作目录
act[fs.write] set_current_dir(str path) -> Result<unit, Error>;
```

### 目录项

```lumos
\type> DirEntry {
    // 获取文件名
    fun file_name() -> str;

    // 获取完整路径
    fun path() -> str;

    // 获取元数据
    act[fs.read] metadata() -> Result<Metadata, Error>;

    // 检查是否是文件
    act[fs.read] is_file() -> bool;

    // 检查是否是目录
    act[fs.read] is_dir() -> bool;

    // 检查是否是符号链接
    act[fs.read] is_symlink() -> bool;
}
```

## 路径操作

### Path 类型

```lumos
\type> Path {
    // 创建路径
    fun new(str s) -> \type> Path;

    // 获取路径字符串
    fun as_str() -> str;

    // 获取文件名
    fun file_name() -> str;

    // 获取扩展名
    fun extension() -> str;

    // 获取文件名（不含扩展名）
    fun stem() -> str;

    // 获取父目录
    fun parent() -> Option<\type> Path>;

    // 检查是否是绝对路径
    fun is_absolute() -> bool;

    // 拼接路径
    fun join(str segment) -> \type> Path;

    // 规范化路径（移除 .. 和 .）
    act[fs.read] canonicalize() -> Result<\type> Path, Error>;

    // 获取相对路径
    fun relative_to(str base) -> Result<str, Error>;
}
```

## 临时文件

### 临时文件和目录

```lumos
// 创建临时文件
act[fs.write] temp_file() -> Result<File, Error>;

// 创建指定前缀的临时文件
act[fs.write] temp_file_with_prefix(str prefix) -> Result<File, Error>;

// 创建临时目录
act[fs.write] temp_dir() -> Result<str, Error>;

// 获取系统临时目录
act[fs.read] temp_dir_path() -> str;
```

## 使用示例

### 读文件

```lumos
use std.file.*;
use std.io.*;

act main() {
    // 打开并读取整个文件
    match open_read("data.txt") {
        Ok(file) => {
            str content = file.read_all().unwrap();
            println("File contents:\n{}", content);
            file.close();
        }
        Err(err) => {
            println("Error: {}", err);
        }
    }
}
```

### 逐行读取

```lumos
use std.file.*;
use std.io.*;

act main() {
    match open_read("lines.txt") {
        Ok(file) => {
            for line_result in file.lines() {
                match line_result {
                    Ok(line) => println("{}", line),
                    Err(err) => println("Read error: {}", err),
                }
            }
            file.close();
        }
        Err(err) => println("Open error: {}", err),
    }
}
```

### 写文件

```lumos
use std.file.*;
use std.io.*;

act main() {
    match open_write("output.txt") {
        Ok(file) => {
            file.write_str("Hello, World!\n").unwrap();
            file.write_fmt("Number: {}\n", 42).unwrap();
            file.flush().unwrap();
            file.close();
            println("File written successfully");
        }
        Err(err) => {
            println("Error: {}", err);
        }
    }
}
```

### 追加写入

```lumos
use std.file.*;
use std.io.*;

act main() {
    match open_append("log.txt") {
        Ok(file) => {
            file.write_str("New log entry\n").unwrap();
            file.flush().unwrap();
            file.close();
        }
        Err(err) => {
            println("Error: {}", err);
        }
    }
}
```

### 列出目录

```lumos
use std.file.*;
use std.io.*;

act main() {
    match read_dir(".") {
        Ok(entries) => {
            for entry in entries {
                str name = entry.file_name();
                if entry.is_dir() {
                    println("[DIR] {}", name);
                } else {
                    println("[FILE] {}", name);
                }
            }
        }
        Err(err) => {
            println("Error: {}", err);
        }
    }
}
```

### 文件复制

```lumos
use std.file.*;
use std.io.*;

act main() {
    str src = "source.txt";
    str dst = "backup.txt";

    match copy(src, dst) {
        Ok(_) => {
            println("File copied: {} -> {}", src, dst);
        }
        Err(err) => {
            println("Copy failed: {}", err);
        }
    }
}
```

### 路径操作

```lumos
use std.file.*;
use std.io.*;

act main() {
    var path = Path::new("./data/file.txt");

    println("Path: {}", path.as_str());
    println("File name: {}", path.file_name());
    println("Extension: {}", path.extension());
    println("Parent: {}", path.parent().unwrap().as_str());
    println("Is absolute: {}", path.is_absolute());

    // 拼接路径
    var new_path = path.parent().unwrap().join("backup").join("file.txt");
    println("New path: {}", new_path.as_str());
}
```

### 文件监视

```lumos
use std.file.*;

act main() {
    // 监视文件修改
    if let Ok(meta) = metadata("config.txt") {
        println("Last modified: {}", meta.modified_time());
    }

    if is_readable("data.txt") && is_writable("data.txt") {
        println("File is readable and writable");
    }
}
```

## 错误处理

### Error 类型

```lumos
\type> Error {
    // 错误代码
    fun code() -> i32;

    // 错误消息
    fun message() -> str;

    // 转换为字符串
    fun to_string() -> str;
}

// 常见错误代码
lit i32 ERR_NOT_FOUND = 1;
lit i32 ERR_PERMISSION = 2;
lit i32 ERR_IO = 3;
lit i32 ERR_ALREADY_EXISTS = 4;
```

## 性能提示

1. **批量操作**：使用迭代器而非多次单独读取
2. **缓冲大小**：大文件建议使用更大的缓冲区
3. **内存映射**：超大文件考虑使用内存映射而非完整读取
4. **异步 I/O**：频繁文件操作使用异步接口避免阻塞

---

相关内容：权限系统见 [副作用权限系统](../grammar/permission.md)。
