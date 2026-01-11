
# 提示 {#title}

- `#info` 作为错误或警告的信息提示
- `#todo` 标记待办事项

---

- `#+error`
  - `#+error E2045` 开启编号为 E2045 的错误
  - `#+error all` 开启所有错误
- `#-error`
  - `#-error E2045` 关闭编号为 E2045 的错误
  - `#-error all` 关闭所有错误
- `#error`
  - `#error "错误信息"`
  - `#error E2045 "错误信息"` 定义错误编号，自定义的用 `EX` 开头

---

- `#+warning` 开启某个警告
  - `#+warning W1145` 开启编号为 W1145 的警告
  - `#+warning all` 开启所有警告
- `#-warning` 关闭某个警告
  - `#-warning W1145` 关闭编号为 W1145 的警告
  - `#-warning all` 关闭所有警告
- `#warning` 发出某个警告
  - `#warning "提示信息"`
  - `#warning W1145 "提示信息"` 定义警告编号，自定义的用 `WX` 开头

---

相关内容：权限与副作用见 [副作用权限系统](../grammar/permission.md#title)。
