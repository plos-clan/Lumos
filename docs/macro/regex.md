
# 正则表达式 {#title}

## 匹配并替换 {#match-replace}

```lumos
#replace rule0 "xxx" -> "yyy"  // 将 xxx 替换为 yyy
#remove rule0                  // 删除 rule0
#replace rule1 /xxx$/ -> /yyy/ // 将行结尾的 xxx 替换为 yyy
#remove rule1                  // 删除 rule1
```

## 匹配并执行 {#match-exec}

```lumos
#match /[A-Z]+/ -> #system echo $0
```

---

相关内容：字符串字面量见 [词法/字符串](../lexical/string.md#title)。
