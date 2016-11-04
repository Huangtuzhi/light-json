## 结构体相互引用

light_value 和 light_number 相互引用，应该怎么定义呢？直接编译会报错

> error: storage size of 'm' isn't known  light_member m;

```
struct light_value{
	union {
		struct { char* s; size_t len; }s; // customed string
		struct { light_value* e; size_t size; }array; // array
		struct { light_member* m; size_t size; }object; // object
		double n;
	}u;
	light_type type;
};

struct light_number {
	char* key; 
	size_t klen;
	light_value value;
};

```

参考 [ C语言中结构体自引用和相互引用](http://blog.csdn.net/daheiantian/article/details/6233058)可以找到答案，只需要在上面加入

```
struct light_value;
struct light_number;
```

使用不完全声明的方式。
