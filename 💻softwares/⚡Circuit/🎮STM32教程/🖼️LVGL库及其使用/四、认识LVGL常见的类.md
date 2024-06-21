## lv_obj_t 类型
lvgl 中的对象均使用 lv_obj_t 为其类型名, 并且lv_obj_t 至少都有以下的五个属性:
- **Position**   
- **Size**
- **Parent** 
- **Styles**
- **Event handlers**
主要的函数都可以从 API 库中找到: https://docs.lvgl.io/master/API/ 

最基本的尺寸和位置设置方法如下:
```cpp 
/*Set basic object attributes*/
lv_obj_set_size(btn1, 100, 50);   /*Set a button's size*/
lv_obj_set_pos(btn1, 20,30);      /*Set a button's position*/
```

对于基本的创建对象和删除对象函数,包括: 

```cpp
lv_obj_t * lv_<widget>_create(lv_obj_t * parent, <other parameters if any>);
void lv_obj_delete(lv_obj_t * obj);
```

一般而言, screen 对象可以通过函数创建并且通过lv_screen_active();获取当前的 screnn 对象;如果重新载入, 则使用lv_screen_load函数。
```
lv_obj_t * scr1 = lv_obj_create(NULL);
```

## 动画类型: 
lv_screen_load_anim();