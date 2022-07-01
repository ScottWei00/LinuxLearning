## 第一章 理解网络编程和套接字

### 1.1 构建接电话套接字

#### 调用socket函数（安装电话机）时进行的对话：

有了电话机才能安装电话，于是就要准备一个电话机，下面函数相当于电话机的套接字：

```c++
#include<sys/socket.h>
int socket(int domain, int type, int protocol);
//成功时返回文件描述符，失败时返回-1
```

#### **调用bind函数（分配电话号码）时进行的对话：**

给套接字分配IP地址和端口号来确定地址信息

```c++
#include<sys/socket.h>
int bind(int sockfd, struct sockaddr *myaddr, socklen_t addrlen);
//成功时返回0，失败时返回-1
```

#### 调用listen函数（连接电话线）时进行的对话：

连接电话线，电话机就可以转换为可接听状态，其他人可以打电话到该电话机上。同样，listen函数就是把套接字转化成可接受连接状态。

```c++
#include<sys/socket.h>
int listen(int sockfd, int backlog);
//成功时返回0，失败时返回-1
```

#### 调用accept函数（拿起话筒）时进行的对话：

```c++
#include<sys/socket.h>
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
//成功时返回文件描述符，失败时返回-1
```

#### 网络编程中和接受连接请求的套接字创建过程可整理如下：

1. 调用socket函数创建套接字
2. 调用bind函数分配IP地址和端口号
3. 调用listen函数转换为可接受状态
4. 调用accept函数受理套接字请求

### 1.2 构建打电话套接字

#### 调用connect函数（拨打号码）时进行的对话：

```c++
#include<sys/socket.h>
int connect(int sockfd, struct sockaddr *serv_addr, socklen_t *addrlen);
//成功时返回0，失败时返回-1
```

客户端（打电话）只有调用socket函数创建套接字和connect函数向服务端发起连接请求步骤。

### 1.3 基于Linux的文件操作

#### 文件描述符

| 文件描述符 | 对象                      |
| ---------- | ------------------------- |
| 0          | 标准输入：standard input  |
| 1          | 标准输出：standard output |
| 2          | 标准错误：standard error  |

#### 文件打开

```c++
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int open(const char *path, int flag);
//成功时返回文件描述符，失败时返回-1
//path: 文件名的字符串地址
//flag: 文件打开模式信息
```

文件打开模式如下：

| 打开模式 | 含义                       |
| -------- | -------------------------- |
| O_CREAT  | 必要时创建文件             |
| O_TRUNC  | 删除全部现有数据           |
| O_APPEND | 维持原有数据，保存到其后面 |
| O_RDONLY | 只读打开                   |
| O_WRONLY | 只写打开                   |
| O_RDWR   | 读写打开                   |

#### 文件关闭

```c++
#include<unistd.h>
int close(int fd);
//成功时返回0，失败时返回-1
//fd:需要关闭的文件或者套接字的文件描述符
```

若调用此函数同时传递文件描述符参数，则关闭（终止）响应文件。另外需要注意的是，此函数不仅可以关闭文件，还可以关闭套接字。再次证明了「Linux 操作系统不区分文件与套接字」的特点。

#### 数据写入文件

```c++
#include<unsitd.h>
ssize_t write(int fd, const void *buf, int size_t nbytes);
//成功时返回写入的字节数，失败时返回-1
//fd: 要传输对象的文件描述符
//buf: 要传输数据的缓冲值地址
//nbytes: 要传输数据的字节数
```

在此函数的定义中，size_t 是通过 typedef 声明的 unsigned int 类型。对 ssize_t 来说，ssize_t 前面多加的 s 代表 signed ，即 ssize_t 是通过 typedef 声明的 signed int 类型。

#### 数据从文件读取

```c++
#include<unsitd.h>
ssize_t read(int fd, void *buf, size_t nbytes);
//成功时返回接收的字节数（遇到文件结尾返回0），失败时返回-1
//fd: 显示数据接收对象的文件描述符
//buf: 要保存接收的数据的缓冲地址值
//size_t:要接收数据的最大字节数
```



## 第二章 套接字类型与协议设置

### 2.1 创建套接字

```c++
#include<sys/socket.h>
int socket(int domain, int type, int protocol);
/*
成功时返回文件描述符，失败时返回-1
domain:套接字中使用的协议族
type: 套接字数据传输的类型信息
protocol:计算机通信中使用的协议信息
*/
```

### 2.2 协议族（Protocol　Family）

| PF_INET   | IPv4网络协议     |
| --------- | ---------------- |
| PF_INET6  | IPV6网络协议     |
| PF_LOCAL  | 本地通信Unix协议 |
| PF_PACKET | 底层套接字协议   |
| PF_IPX    | IPX Novel协议    |

本书重点是PF_INET对应的IPV４网络协议。其他协议不常用。

注意：套接字中采用的最终的协议信息是通过socket函数的第三个参数传递的。在指定的协议族范围内通过第一个参数决定第三个参数。

### 2.3 套接字类型（Type）

**套接字类型指的是套接字的数据传输方式**。

一个协议族中有多种数据传输方式。

#### **面向连接的套接字（SOCK_STREAM）**

- 传输过程中数据不会消失
- 按序传输数据
- 传输的数据不存在数据边界

收发数据的套接字内部有缓冲（buffer），简言之就是字节数组。只要不超过数组容量，那么数据填满缓冲后过 1 次 read 函数的调用就可以读取全部，也有可能调用多次来完成读取。

**套接字缓冲已满是否意味着数据丢失？**

​	缓冲并不总是满的。如果读取速度比数据传入速度慢，则缓冲可能会被填满，但这是数据也不会丢失，因为传输套接字此时停止数据传输，所以不会丢失数据。

**可靠的、有序的、基于字节的面向连接的数据传输方式的套接字**

#### 面向消息的套接字（SOCK_DGRAM）

- 强调快速传输而非有序传输
- 传输的数据有可能丢失和损毁
- 传输的数据有边界
- 限制每次传输数据的大小

**不可靠的、无序的、以数据的高速传输为目的的套接字**

## 第三章　地址族与数据序列

### 3.1 IP地址和端口号

**网络地址**

- IPV４　四字节地址
- IPV６　六字节地址

注意：**以下讨论的都是IPV4的地址**

**IP地址的边界区分**

- A类地址：0~127
- B类地址：128~191
- C类地址：192~223

**端口号**

- 端口号由16位组成，可表示范围0~65525。其中0~1023是已分配给特定应用程序的端口。
- 端口号不能重复使用，但TCP套接字和UDP套接字不会共同端口号

注意：**IP地址用于区分计算机主机，端口号用于区分具体的应用程序**

### 3.2 地址信息的表示

表示Ipv4地址的结构体

```c++
struct sockaddr_in
{
	sa_family_t sin_family; //地址族
	uint16_t sin_port;//16位端口号
	struct in_addr sin_addr;//32位IP地址
	char sin_zero[8];//不使用
}
```

```c++
struct in_addr
{
	in_addr_t s_addr;//32位ipv4地址
}
```

```c++
struct sockaddr
{
	sa_family_t sin_family;//地址族
	char sa_data[14];//地址信息，包含ip地址和port端口号，其余位补0
    //通过sockaddr_in强制转换为socketaddr，可以生产符号的bind参数
}
```



### 3.3 字符串转换为网络字节序

CPU保存数据的方式有两种

- 大端序：高位字节存放在低地址上

- 小端序：高位字节存放在高地址上

  <img src="C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20220630183627995.png" alt="image-20220630183627995" style="zoom:33%;" />

  但是防止在网络传输中出现问题，必须约定为统一的格式，这种约定为网络字节序。**统一为大端序**。即先把数据数组转化成大端序在进行网络传输

  ```c++
  unsigned short htons(unsigned short);
  unsigned short ntohs(unsigned short);
  unsigned long htonl(unsigned long);
  unsigned long tonhl(unsigned long);
  ```

  

  ### 3.4 网络地址的初始化与分配

```c++
#include<arpa/inet.h>
in_addr_t inet_addr(const char* string);//成功时返回32位大端整数型值，失败时返回INADDR_NONE

例如：
    char *addr1="1.2.3.4";
	unsigned long conv = inet_addr(addr1);//结果为0x4030201
	char *addr2="1.2.3.256";
	conv = inet_addr(addr2);//结果为INADDR_NONE
	
```

inet_aton与inet_addr函数功能相同，也是将字符串转换为整形地址。但该函数使用了in_addr结构体，使用更高频

```c++
#include<arpa/inet.h>
int inet_aton(const char* string, struct in_addr *addr);
//成功时返回1，失败时返回0
```

与inet_aton相反的函数是inet_ntoa，可以把网络字节序整数型IP地址转换为熟悉的字符串形式

```c++
#include<arpa/inet.h>
char* inet_ntoa(struct in_addr adr);//成功返回字符串数组，失败返回-1
/*但要小心，返回值为 char 指针，返回字符串地址意味着字符串已经保存在内存空间，但是该函数未向程序员要求分配内存，而是在内部申请了内存保存了字符串。也就是说调用了该函数候要立即把信息复制到其他内存空间。因此，若再次调用 inet_ntoa 函数，则有可能覆盖之前保存的字符串信息。总之，再次调用 inet_ntoa 函数前返回的字符串地址是有效的。若需要长期保存，则应该将字符串复制到其他内存空间。*/
```



## 第四章 基于TCP的服务器/客户端

### 4.1 理解TCP和UDP

### 4.2 实现基于TCP的服务器/客户端

#### TCP 服务端的默认函数的调用程序

<img src="C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20220629192103120.png" alt="image-20220629192103120" style="zoom:50%;" />

#### 等待连接请求状态

已经调用了bind函数给套接字分配地址，接下来就是要通过调用listen函数进入等待连接请求状态。只有调用了listen函数，客户端才能进入可发出请求状态。也就是说客户端可以调用connect函数

```c++
#include<sys/socket.h>
int listen(int sockfd, int backlog);//成功时返回0，失败时返回1
//sockfd: 进入等待连接请求状态的套接字文件描述符
//backlog:连接请求等待队列的长度
```

#### 受理客户端连接请求

调用listen函数后，若有新的连接请求，按序处理。受理请求意味着可接收数据的状态。进入这种状态使用的是套接字，但并不是服务器端的套接字，而是另一个新的套接字，由函数自动创建。

```c++
#include<sys/socket.h>

int accept(int sock, struct sockaddr* addr, socket_t* addrlen);//成功时返回套接字描述符，失败时返回-1
//sock服务器套接字描述符
//addr 保存发起连接的客户端地址信息的变量地址值
//addrlen addr的长度
```

accept函数受理连接请求队列中待处理的客户端连接请求。函数调用成功时，函数内部将产生用于数据IO的套接字，并返回字描述符。**注意：套接字时自动创建的，并自动发起与连接请求的客户端建立连接。**

#### 回顾Hello World服务端

代码思路：

1. 服务端实现过程中首先要创建套接字，但该套接字还不是真正的套接字
2. 为了完成套接字地址的分配，需要初始化结构体sockaddr_in并调用bind分配IP地址和端口号
3. 调用listen进入等待连接请求状态。设置队列的长度，此时的套接字才是服务端套接字。
4. 调用accept函数从队头去一个连接请求与客户端建立连接，并返回套接字描述符。若accept的队列为空，则不会返回，直到有新的客户端请求。
5. 调用write函数向客户端传送数据，调用close关闭连接

#### UDP 服务端的默认函数的调用程序

<img src="C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20220629201553436.png" alt="image-20220629201553436" style="zoom:50%;" />

与服务端相比，唯一不同的地方在于connect连接请求，它是创建客户端套接字后向服务端发起的连接请求。服务端调用listen函数后创建连接请求等待队列，之后客户端即可请求连接。

```c++
#include<sys/socket.h>
int connect(int sock, struct sockaddr *servaddr, socklen_t addrlen);
/*
	成功时返回0，失败时返回-1
	sock：客户端套接字文件描述符
	servaddr：保存目标服务器地址信息的变量地址值
	addrlen：第二个参数servaddr的变量地址长度
*/
```

客户端调用connect函数时，发生以下函数之一才会返回：

- 服务端接受连接请求
- 发生断网等状况而中断连接请求
- 注：接受连接请求不代表服务端调用accept函数，只是服务端把链接请求信息记录在等待队列。因此connect函数返回后并不应该立即进行数据交换。

#### 回顾Hello World客户端代码

重新理解这个程序：

1. 创建连接服务器的套接字，此时创建的是tcp的套接字
2. 结构体变量serv_addr中初始化Ip和端口号。初始化值为目标服务器端套接字的IP和端口信息
3. 调用connect函数向服务端发起连接请求
4. 完成连接后，接受服务端传输的数据
5. 接受数据后调用close函数，关闭套接字，结束与服务端的连接

#### 基于TCP的服务端/客户端函数调用关系

<img src="C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20220630173632779.png" alt="image-20220630173632779" style="zoom:67%;" />

### 4.3 实现迭代服务端/客户端

<img src="C:\Users\Admin\AppData\Roaming\Typora\typora-user-images\image-20220701142351352.png" alt="image-20220701142351352" style="zoom:33%;" />

程序运行的基本方式：

- 服务器端在同一时刻只与一个客户端相连，并提供回声服务
- 服务器端依次向5个客户端提供服务并退出
- 客户端接收用户输入的字符串并发送到服务端
- 服务端将接收的字符串数据传回客户端，即回声
- 服务器端与客户端之间的字符串回声一直执行到客户端输入Q为止

#### 问题：

​	