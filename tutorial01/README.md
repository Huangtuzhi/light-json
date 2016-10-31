## gcc 编译动态库

miloyip 使用 CMake 来编译链接程序，这里使用原始的 gcc。

```
gcc lightjson.c -fPIC -shared -o liblightjson.so
```

* -fPIC 选项表示编译为位置独立的代码，不用此选项的话编译后的代码是位置相关的，所以动态载入时是通过代码拷贝的方式来满足不同进程的需要，而不能达到真正代码段共享的目的

* -shared 选项表示生成 .so 形式的动态链接库

```
gcc TDDtest.c -L. -llightjson -o TDDtest
./TDDtest
6/6 (100.00%) passed
```

* -L. 为执行链接指定一个库的路径（当前路径）
* -llightjson 为执行链接指定具体的库  liblightjon.so，只需要写为 lightjon
