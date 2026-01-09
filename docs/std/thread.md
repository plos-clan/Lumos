# 线程库 `std.thread`

## 基础概念

线程是操作系统能够进行运算调度的最小单位。Lumos 提供了安全的并发编程接口，支持创建、同步和管理线程。

## 线程创建与管理

### 创建线程

```lumos
// 创建新线程
act[sys.thread] spawn(act[sys.thread] f) -> \type> Thread;

// 创建带参数的线程
act[sys.thread] spawn(act[sys.thread] f, ...) -> \type> Thread;

// 创建命名线程
act[sys.thread] spawn_with_name(str name, act[sys.thread] f) -> \type> Thread;
```

### 线程操作

```lumos
\type> Thread {
    // 等待线程完成
    fun join() -> unit;

    // 获取线程 ID
    fun \type> id() -> ThreadID;

    // 获取线程名称
    fun name() -> str;

    // 检查线程是否活跃
    fun is_alive() -> bool;

    // 分离线程（后台运行）
    act[sys.thread] detach() -> unit;
}
```

### 当前线程操作

```lumos
// 获取当前线程
fun \type> current() -> \type> Thread;

// 线程睡眠（毫秒）
act[sys.time] sleep(u64 ms) -> unit;

// 让出 CPU 时间片
act[sys.thread] yield() -> unit;

// 获取逻辑处理器数量
fun num_cpus() -> usize;
```

## 互斥锁（Mutex）

互斥锁用于保护共享资源，确保同一时间只有一个线程访问。

### 互斥锁基础

```lumos
\type> Mutex<typename T> {
    // 创建新互斥锁
    fun new(T value) -> \type> Mutex<T>;

    // 获取锁并访问数据
    fun lock() -> \type> Guard<T>;

    // 尝试获取锁（非阻塞）
    fun try_lock() -> Option<\type> Guard<T>>;

    // 判断锁是否被持有
    fun is_locked() -> bool;
}
```

### 使用互斥锁

```lumos
use std.thread.*;

act main() {
    val counter = Mutex::new(0);

    // 获取锁
    var guard = counter.lock();
    guard[] = guard[] + 1;
    // 离开作用域时自动释放锁
}
```

## 读写锁（RWLock）

读写锁允许多个读者或单个写者访问资源。

```lumos
\type> RWLock<typename T> {
    // 创建读写锁
    fun new(T value) -> \type> RWLock<T>;

    // 读锁
    fun read() -> \type> ReadGuard<T>;

    // 写锁
    fun write() -> \type> WriteGuard<T>;

    // 尝试读锁
    fun try_read() -> Option<\type> ReadGuard<T>>;

    // 尝试写锁
    fun try_write() -> Option<\type> WriteGuard<T>>;
}
```

## 条件变量（Condvar）

条件变量用于线程间的通信和同步。

```lumos
\type> Condvar {
    // 创建条件变量
    fun new() -> \type> Condvar;

    // 等待条件
    act[sys.thread] wait(\type> Guard<unit>) -> unit;

    // 带超时的等待
    act[sys.thread] wait_timeout(u64 ms) -> bool; // true if notified

    // 唤醒一个等待线程
    act[sys.thread] notify_one() -> unit;

    // 唤醒所有等待线程
    act[sys.thread] notify_all() -> unit;
}
```

## 屏障（Barrier）

屏障用于同步一组线程。

```lumos
\type> Barrier {
    // 创建屏障
    fun new(usize count) -> \type> Barrier;

    // 等待所有线程到达屏障
    act[sys.thread] wait() -> unit;
}
```

## 自旋锁（Spin）

轻量级锁，线程在获取不到锁时持续自旋而不是阻塞。

```lumos
\type> Spin<typename T> {
    // 创建自旋锁
    fun new(T value) -> \type> Spin<T>;

    // 获取锁
    fun lock() -> \type> Guard<T>;

    // 尝试获取锁
    fun try_lock() -> Option<\type> Guard<T>>;
}
```

## 原子类型（Atomic）

提供原子操作，用于无锁同步。

```lumos
\type> Atomic<typename T> {
    // 创建原子类型
    fun new(T value) -> \type> Atomic<T>;

    // 读取值
    fun load() -> T;

    // 存储值
    act[sys.thread] store(T value) -> unit;

    // 交换值
    act[sys.thread] swap(T value) -> T;

    // 比较并交换
    fun compare_exchange(T expected, T new) -> bool;

    // 原子加法
    act[sys.thread] add(T delta) -> unit;

    // 原子减法
    act[sys.thread] sub(T delta) -> unit;
}
```

## 通道（Channel）

用于线程间发送消息。

```lumos
\type> Sender<typename T> {
    // 发送消息（阻塞）
    act[sys.thread] send(T value) -> unit;

    // 尝试发送（非阻塞）
    fun try_send(T value) -> bool;

    // 克隆发送端
    fun clone() -> \type> Sender<T>;
}

\type> Receiver<typename T> {
    // 接收消息（阻塞）
    act[sys.thread] recv() -> T;

    // 尝试接收（非阻塞）
    fun try_recv() -> Option<T>;

    // 迭代接收
    fun iter() -> Iterator<T>;
}

// 创建通道
fun channel<typename T>() -> (Sender<T>, Receiver<T>);
```

### 通道使用示例

```lumos
use std.thread.*;

act main() {
    val (tx, rx) = channel<i32>();

    spawn {
        tx.send(42);
    };

    i32 value = rx.recv();
    println(value);
}
```

## 线程池

提供线程池用于高效处理异步任务。

```lumos
\type> ThreadPool {
    // 创建线程池
    fun new(usize num_threads) -> \type> ThreadPool;

    // 提交任务
    act[sys.thread] execute(act[sys.thread] f) -> unit;

    // 关闭线程池
    act[sys.thread] shutdown() -> unit;

    // 等待所有任务完成
    act[sys.thread] wait() -> unit;
}
```

## 同步原语速查表

| 类型 | 用途 | 读多写少 | 高竞争 | 无阻塞 |
|------|------|---------|--------|--------|
| Mutex | 通用保护 | ✗ | ✓ | ✗ |
| RWLock | 读多写少 | ✓ | ✗ | ✗ |
| Spin | 轻量同步 | - | - | ✗ |
| Atomic | 无锁同步 | ✓ | ✓ | ✓ |
| Channel | 消息传递 | - | - | ✓ |

## 最佳实践

1. **优先使用消息传递**而非共享内存
2. **避免嵌套锁获取**以防死锁
3. **在持有锁时避免耗时操作**
4. **使用 RAII 模式**（Guard 自动释放）
5. **对高竞争场景使用原子类型**

---

相关内容：权限系统见 [副作用权限系统](../grammar/permission.md#permission-system-side-effect-permission-system)。
