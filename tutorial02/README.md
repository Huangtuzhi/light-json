## strtod 用法

string to double，将字符串转为 double。原型如下。

```
double strtod (const char* str, char** endptr);
```

str 为要转换的字符串，endstr 为第一个不能转换的字符的指针。

ANSI C 规范定义了 `stof()`、`atoi()`、`atol()`、`strtod()`、`strtol()`、`strtoul()` 共 6 个可以将字符串转换为数字的函数，使用 `strtod(str, NULL)` 与使用 `atof()` 结果相同。另外在 C99 / C++11 规范中又新增了5个函数，分别是 `atoll()`、`strtof()`、`strtold()`、`strtoll()`、`strtoull()`。

示例 

```
#include <stdio.h>
#include <stdlib.h>
int main()
{
    char szOrbits[] = "365.24 29.53";
    char* pEnd;
    double d1, d2;
    d1 = strtod (szOrbits, &pEnd);
    d2 = strtod (pEnd, NULL);
    printf ("月球每年绕地球运行 %.2f 周。\n", d1/d2);
    system("pause");
    return 0;
}
```
