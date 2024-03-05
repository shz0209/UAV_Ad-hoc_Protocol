## How to contribute
## Branchs
|Name| Description|
|---|---|
|main| Main branch|
|master| Merge your branch to master and test|

- You should merge your branch to master.
- There is a code test when you create a pull request to `master` or `main`
- Please merge after the test passed.

## Reference
### Netlink
了解更多关于`netlink`套接字和路由表的详细信息，你可以查阅以下资源：
1. **Linux man 手册：**
   - `man 7 netlink`: 这个手册提供了`netlink`套接字的一般介绍，包括套接字的工作原理和使用方法。
   - `man 7 rtnetlink`: 提供了关于路由信息的`netlink`通信协议的详细说明。
2. **Linux源代码：**
   - `include/uapi/linux/netlink.h`: 包含了`netlink`协议的头文件，你可以在Linux源代码中的这个文件中找到详细的定义和说明。
   - `include/uapi/linux/rtnetlink.h`: 包含了路由表的`netlink`消息的头文件，提供了路由表相关的定义和结构。
3. **Linux文档：**
   - [Linux Documentation](https://www.kernel.org/doc/html/latest/index.html): Linux官方文档包含了广泛的主题，你可以在这里找到关于网络子系统、套接字编程以及路由表的详细信息。

## Notebook
### Route
- Add: `sudo ip route add 192.168.1.0/24 via 0.0.0.0 dev eth0`
- Delete: `sudo ip route del 192.168.1.0/24 via 0.0.0.0 dev eth0`
`