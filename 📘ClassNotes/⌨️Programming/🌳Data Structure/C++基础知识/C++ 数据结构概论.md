# (一)数据结构基本知识
## 一、定义
数据结构由某一数据元素的集合和该集合中的数据元素之间的关系构成
$$Data Structure = \{ D,R\}$$
数据结构的分类： 
1. 线性结构
有两种传统的实现方式：
基于数组的顺序存储表示和基于链表的链接存储表示

1. 非线性结构
分为层次结构和非层次结构
 层次结构是按层次划分的数据元素集合 
 层次结构中的元素称为节点
 树可以为空，也可以不为空
 若树不为空，则有一个根节点

基本的4种存储方法
 （1）顺序存储方法
 （2）链接存储方法
 （3）索引存储方法
 （4）散列存储方法

在C++中，使用struct或者class来定义抽象数据类型

## 二、数据类型

类型可以分为原子类型和结构类型两种
原子数据是不可再分的整体，如整数，浮点数，字符串，指针，枚举量等等
结构类型是存储原子类型和结构类型的数据类型

## 三、抽象数据类型

在c++中，使用关键字struct或者class定义抽象数据类型

为了保证抽象数据类型中每个操作的正确性， 操作中需要给出前置条件IF以及...then.....else
c++ 中使用断言(assert.h)来实现。

c++ 的抛出异常机制是 try-throw-catch

抽象数据类型的特征是使用与实现分离，实行封装性与屏蔽性

## 四、面向对象的概念，C++类

#### 1. 面向对象的概念

Coad和Yourdon给出定义： 
面向对象  = 对象 + 类 + 继承 + 消息通信

定义中的对象是指在应用问题中出现的各种实体，事件，规格说明等，是由==一组属性值和在这组值上的一组服务（或操作）构成== ，==其中，属性值确定了对象的状态== , 而服务可以改变对象的状态

面向对象的方法：将具有相同属性和服务的对象归结到同一个类(class) , 而把一个类中的每个对象称为该类的一个实例，它们具有相同的服务

通过引入继承机制，可以使类型各自拥有不同的属性和各异的服务，同时共享部分属性和服务
其中，类有基类(base class)和派生类(derived classes)
基类又称父类 ，超类或者泛化类， 派生类又称子类，特化类

通过在体系上实现类的继承， 可以调用相应的方法

消息实际上是一个类的对象要求另一个类的对象执行某个服务的命令， 指明要求哪一个对象执行这个服务，必要时传递调用参数

#### 2. C++中的类
##### (1) struct 和class
C++在类的成员上有三级存取:  public,  private , protected
对于在public域声明的数据成员(成员函数), 程序中的其他类均可进行调用和访问
在private域和protected域内声明的数据成员和数据为类所私有，只能由该类的对象函数和成员函数，以及被声明为友元(friend)的函数中才能使用

在protected中声明的函数，还允许该类的派生类访问， 而在private中声明的数据成员和派生函数则不允许派生类访问

对于一个Point类的声明头文件(point.h)的定义如下: 

```c++
#ifndef POINT_H
#define POINT_H      // in the header file point.h
#include <iostream>

class Point{

public:    //  the public part
	Point(int, int);   // contruct a function 
	Point(Point&);     // copy the Constructor
	~Point();          // destructor
	int get_x();
	int get_y();
	void put_x();
	void put_y();
	Point operator+(point p); //reload the function
	Point operator*(int i);   
	int operator>(Point p);
	int operator==(Point p);
private:	// the private part
	int x;
	int y;
	friend istram & operator >> (istream & in, Point& p); // announce the friend function(input)
	friend ostream& operator << (ostream& out, Point& p); // announce the friend function(output)
};
#endif
```

switch ... case ....default.....

C++ 中，struct默认的类访问类型是public, 而在class类型中,默认的访问级别是private
除此之外，struct和 class是等价的

##### (2) C++中的union类
在C++中，union也可以定义类，C++中的union可以包含函数，变量，也可以构造函数，析构函数

与struct和class相比，union可以节省存储
为了让数据成员共享存储空间，union默认存取级别是public

##### (3) C++中的对象
建立类的对象也称为实例化，可以自动或者静态地或者通过动态内存分配来建立

```C++
Point p(6,3);
Point q;        // Static
Point*t = new Point(1,1);  // allocate dynamically
```

构造函数(constructor)
构造函数即为调用函数
如Point p(a,b)  ->调用Point(int,int)

析构函数(destructor)
`~Point()`
退出作用域或者使用delete命令释放动态内存时，调用析构函数
用于在删除类的对象时做清除工作


### 3. C++的输入与输出
基本的输入与输出方式有两种,键盘屏幕输出和文件输入输出
键盘的输入和输出是cin,cout,cerr 


文件输入和输出的方法是
```C++
#include <iostream.h>
#include <fstream.h>  // 这个必须加
#include <stdlib.h>

void main(){
ifstream inFile; // 定义输入流对象
ofstream outFile; // 输出流
outFile.open("my.dat", ios::out);   // 建立输出文件my.dat(作为输出或写入对象)
char univ[] = "Tsinghua", name[10];   // 分别定义两个类
int course = 2401, number;
outFile << univ << endl;  //   << 用于写出对象，而>>用于读入类istream的一个对象 -> 本句用于将univ写入outFile
inFile.open("my.dat", ios::in|ios::nocreate);  // 打开输入文件
if (!inFile){
cerr << "打开失败" << endl;
exit(1);
}
char c;
inFile >> name >> number;  // 从inFile读入char[10]类型的name,int型 number
outFile << name << endl; // 写入outFile
outFile << number << endl;
}
```

其中，fstream.h定义了类ifstream,ofstream, fstram
定义一个输入流，需要声明为ifstream的实例
定义一个输出流，需要声明为ofstream的实例
执行输入和输出操作的流，需要声明为fstream的实例

ifstream, ofstream, fstream是由istream,ostream派生而来,而上述两类由ios类派生出来的
```C++
ios::app   将对文件的输出加在文件尾
ios::binary 以二进制打开文件
ios::nocreate 文件不存在则打开失败
ios::out 表明文件用于输出,可省略
ios::in            输入
```

### 4. C++中的函数
参数传递省略
1. 函数传递返回时可以通过引用方式，此时使用函数类型后加上&
```C++
using namespace std;
#include  <iostream>

char& replace(int m);   // 声明函数
// 函数返回时通过引用方式

char s[80] = "Hello There";

void Prac_func(){
replace(5) = 'x';
cout << s;
}

int main(){
    Prac_func();

    return 0;
}

char& replace(int m){   // 返回一个char的引用(字符引用类型)
// (返回的是对应返回的字符串本身)
return s[m];   //
}
```

### 5. C++中的参数传递方法

#### (1) 引用类型的传递方法

对于体积较大的参数传递，使用引用形式进行传递将大大节省参数传递的时间

```cpp
#include  <iostream>

void squareByRef(int&); 

int main(){
int z= 4;
squareByRef(z);
cout << z;  // 2
}

// 在参数名前面加&
int squareByValue{return a*=a;}
void squareByRef(int& a){a* =a;}  // 使用引用时，可以直接传入

```

对于递归函数，传送的值是一个对象或者实例时，则函数中就创建了一个副本

需要注意：一个类在构造函数中采用了new为指针成员分配了内存空间，并在析构函数中使用delete进行释放，则必须手动定义复制构造函数(自动创建的复制构造函数只能==进行指针的简单复制==，不会分配新的内存空间,副本空间释放后母本空间释放而造成错误)

采用引用传递时，不创建副本，也不存在撤销副本问题


### 6. 成员函数的返回值

```C++
class Temperature{
public:Temperature(){}

void UpdateTemp(float)const;   // transmit the value
float GetHighTemp()const;      // return the const
}

// define the function of the  class outside the class
float Temperature::UpdateTemp(float temp){  // inherite the function
// the return value is the temperature 
}

float Temperature::GetLowTemp()const{  // add const after the definition of the funciton
return lowTemp;
}

```

### 7.友元函数

注意：在private 函数中声明的成员仅允许其友元函数存取

使用friend关键字定义友元函数

友元函数可以是另一个类的成员函数，如果希望通过这种类型的函数==存取类的私有成员和保护成员==，则需要在==类的声明==中给出函数的原型并加上friend 

```cpp
class Point{
friend istream& operator >> (istream&, Point&);  // input friend funciton
friend ostream& operator << (ostream&, Point&);  // output friend function
// overload the operator >>  and  << 
public:
...
private:
....
}

istream &operator>> (istream  &strm, Point &p) {
.......
}

ostream &operator<< (ostream &strm, Point &p) {
	return strm << "(" << p.get_x() << "," << p.get_y() << ")";
}
```
两个重载操作符 <<  和 >> 都被声明为Point类的友元函数

### 8 . 动态存储分配
C语言中，使用malloc动态为程序变量分配存储空间, 执行时需要使用函数sizeof来提供所需的空间量， 分配完后需要对返回指针类型做强制转换

<mark style="background: transparent; color: yellow">c++带有了new和delete</mark>，增强了动态分配的功能
它们操纵可利用的空间存储，同时取代了C中的库函数malloc和free
```cpp 
int* ip = new int;
Point* p = new Point[10];
delete []p;      // release the storge
```

### 9. 继承和多态

```cpp 
class Quadrilateral: public Plygon{   // inherite

}
```
多态性是
1) 编译的多态性，表现为函数名或操作符重载
2) 运行时的多态性，通过虚函数和派生类来实现

<< 插入操作符     >> 提取操作符

虚函数与动态绑定
```cpp 
int operator+ (const Point&p)  // note that the parameters should all be the reference type

// virtual function --> the virtual function should be asserted as "virtual" and redefined in the child classes 

virtual 

```

如果不提供函数的实现。则声明的函数称为纯虚函数，有纯虚函数的类叫抽象类
virtual 指明实际调用的成员将在运行时进行自动确定，这种做法就是动态绑定
在父类中声明的virtual函数，没有提供实现,
其具体实现是通过子类Quadrilateral进行实现的

### 10. C++ 的模板(template)

C++中支抽象的方式主要有两种：使用抽象类， 使用模板

```cpp
template <class T>  // 类属类  generic class
template <itemtype Y>
```

# (二)算法基本概念讲解
## 一、算法基本内容
### 1. 定义
(1) 有输入
(2) 有输出
(3) 确定性
(4) 有穷性 (程序可能不满足)
(5) 可行性
将具体的问题转变为算法，往往使用<mark style="background: transparent; color: yellow">自顶向下的方法</mark>

### 2. 算法的性能标准
(1) 正确性
(2) 可使用性
(3) 可读性
(4) 效率
(5) 健壮性
(6) 简单性   --> 出错率
