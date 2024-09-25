
```cpp
typedef struct ItemTodo {
    int start;
    int end;
    char id;
    ItemTodo(int start, int end, int id) :start(start), end(end), id(id) {};
    friend bool operator< (const ItemTodo A,const ItemTodo B) {
        return A.end < B.end;
    };
}Item;
```