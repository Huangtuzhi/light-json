## struct 前向声明（forward declare）

以前的 lept_value 是这样定义的 

```
typedef struct lept_value {
    union {
        struct { lept_value* e; size_t size; }a; /* array */
        struct { char* s; size_t len; }s;
        double n;
    }u;
    lept_type type;
}lept_value;

```

现在加入了数组的类型，数组的元素也是 lept_value 类型的，它使用了自身类型。所以需要在使用前进行前向声明，写为

```
typedef struct lept_value lept_value;

struct lept_value {
    union {
        struct { lept_value* e; size_t size; }a; /* array */
        struct { char* s; size_t len; }s;
        double n;
    }u;
    lept_type type;
};
```
