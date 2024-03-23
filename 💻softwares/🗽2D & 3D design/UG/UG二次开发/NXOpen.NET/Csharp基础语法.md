demical  精确数值类型
int.Parse 强制类型转换为int 
.toString 转换为 string 类型 
Array.IndexOf 判断数组中是否有某种元素

随机数产生方法 static Random random = new Random();

另外，在C# 中， 可以直接初始化变长度的数组
```c#
int[] arr = new int[length];
// 带参数初始化
Feature[] curvefeatures = new Feature[] { sketch1.Feature };

holebuilder1.StartHoleData.BooleanOperation.SetTargetBodies(new Body[]{ null });
```

```c# fold title:csharp数据类型
using System;

namespace Data_Types
{   //数据类型
    class Program
    {
        //public struct Boollean: IComparable,ICloneable,IConvertible
        //{
        //}
        static void Main(string[] args)
        {
            /*
             * 数据类型分为值类型和引用类型
             * 值类型存储数据，但是引用类型存储数据的地址
             * 值类型
             * 声明和存储都在栈中
             * 其中，值类型分为结构和枚举，结构的数据类型包括数值类型，bool和char   (其实是结构体)
             * 引用类型分为接口和类，类比如string,Array,委托等
             * 引用类型声明在栈中，数据存储在堆中
             
             * 内存 
             * 内存的分配:CLR将申请的内存空间从逻辑上进行划分
             * 分为 栈区 
             *  读取速度快，空间小
             * 和 堆区 （速度相对慢）
             * 
             * 调用方法时生成栈帧存储方法的数据，方法执行完成后，栈帧被清除
             * 
             * 堆区用于存储引用类型的数据
             * 
             * 局部变量（略）: 方法被调用时，存储在栈中，结束后清除
             * int[]是引用类型
             * 
             * 
             * 对于string类型，修改的是栈中存储的引用类型，
             * 修改就是转换在栈中的引用地址（在栈中，为两个数据类型分别开辟一块地址）
             * s1指向堆里面开辟的数据"sec"，
             * s2存储的是指向这个数据的地址,这个地址指向"sec"
             * s1 = cos会新开辟一个
             * 字符串是不可变的，修改字符串
             * 
             * object也是引用类型
             */

            //举例如下
            int a = 1;  //数值类型，调用a时调用的是a的值
            int b = a;   //由于是数值类型，所以给a赋值不会改变b
            a = 2;
            Console.WriteLine(b);

            int[] arr = new int[] { 1 };
            int[] arr2 = arr;  //由于arr是地址名，所以修改arr中的内容修改的是堆里的
            arr[0] = 2;  // 这个是修改堆里面的数据
            Console.WriteLine(arr2[0]); //引用后，也会成为2

            string s1 = "sec";
            string s2 = s1;
            s1 = "cos";  //修改栈中存储的应用
            //   s1[0] = 'd';不能运行，由于是只读的（堆中的文字不能修改，这是由于字符串的不可变性）
            Console.WriteLine(s2);

            //比如在函数中开辟栈
            int b2 = 1;
            Func(b2);   //不会修改main中栈的数值
            Console.WriteLine(b2);  
        }
        static void Func(int b2)
        {

            // 因为方法都在栈中，所以方法中声明变量都在栈中
            // 值类型存储数据，所以数据在栈中

            // 注意:可以修改数组，由于是存储相应的地址，所以通过索引可以取得地址的内容,
            // 所以可以得到堆中的数据
            b2 = 3;    // 是栈的空间
        }
    }
}

```

```c# fold title:数组与Array类型
// 常见的Array操作包括 
indexof,LastIndexOf,Sort,Reverse

// 循环的终止方法:定义一个cycle,当需要重复时，令cycle =1即可;
// Array.sort叫全排，

// 在system 中提供了Array对象, 
/Array.Sort(array,startIndex,length)  // 对数组某些部分进行排序;
Array.Sort(array,2,6)    // 从第二个下标（第三个元素）来排序，排序长度为6的部分排序
//可以使用三元运算符    .....?:来进行相应的简化

// IndexOf 还有（int startinde, int count）的方法，来指定搜索的开始下标和搜索的长度


//格式控制方法
Console.WriteLine("您中了{0}等奖，花了{1:c}圆",0,0.6);
//其中{1:c}是使用货币形式进行显示

//二维数组的声明语法
int[,] array = new int[5, 3];   //创建一个5行3列的数组

Console.WriteLine(array.Length);  //15
//注意赋值
array[0,2] = 6;  //设置第1行第3列（注意索引的调用）
//遍历方法略去（包括赋值和输出）
```

# C# 中的类的成员定义
对于cs 中的每个类对应的对象， 都可以设置get 和set，来设置可访问性;
一般对象访问的方法一是后面加上{get; set;} 
还有一种方法是直接写成静态变量(static)
```cs
public string identityID { get; set; }
public static Length = 5;
```

如果希望创建一个对外只读的对象， 一般新创建一个对象, 并重载其get, set函数如下:
```cs
public struct coor_directions
        {
            public Direction x_Direction { get; set; }
            public Direction y_Direction { get; set; }
            public Direction z_Direction { get; set; }
        };
        private static coor_directions directions;
        
        public static coor_directions direction_ref
        {
            get{ return directions;}  // 对外只读
            private set { directions = value; }   // 对内读写(也可以直接访问directions)
        }
```

另外, static对象不需要初始化， 在类初始化时已经初始化好了。 


# C# 方法重载

C# 支持在一个类下定义多个相同的方法;
```cs
    class GetSeconds   //实现了方法的重载
    {
        public static int getSeconds(int mins,int seconds)
        {  // 通过使用相同的方法名来进行方法的重载
           // 在方法名字相同时，通过参数来进行区分
           // python 中的方法重载是通过默认参数来进行的
            int sec;
            sec = mins * 60 + seconds;
            return sec;
        }
        public static int getSeconds(int Hour,int mins,int seconds)
        {
            return getSeconds(Hour * 60 + mins, seconds);
        }
        public static int getSeconds(int Day,int Hour,int mins, int seconds) 
        {
            return getSeconds(24 * Day + Hour, mins, seconds);
        }
    }
```

# C# 中的类与继承声明方法
C#  中类允许自带一个带参的初始化构造函数， 具体**需要使用public + 类名， 同时要求不带返回值;** 

```cs fold
    /*
     * static 的使用方法
     * static可以实现方法之间的数据共享
     * static 定义在方法内部
     * static使用单独的空间存储，存储在内存空间的静态区，实例成员每个对象存储一份
     *
     * static的使用包括静态成员变量，静态构造函数和静态方法
     * 1. 静态成员变量:
     * 静态成员变量属于类，实例成员属于对象,静态成员变量使用对象名来调用，而实例成员通过类名来调用
     * 静态成员在类加载时就存在了,出生时机不同
     * 静态成员常驻内存，存在优先于对象，被所有对象所共享
     *
     * 2. 静态构造函数: static 方法的使用
     * (1)静态构造函数不能使用访问级别
     * 初始化类的静态数据成员 ->提供创建对象的方式,初始化类的实例数据成员
     * 只在类加载时, 调用一次, 但是其他的在new调用时才调用
     * 2. 静态方法，只能访问静态成员(this不能使用),不可访问实例成员
	 * 注意:在静态方法中，所有的变量都应当是静态的
	 * p1++;   //报错
	 * 3. 静态方法执行时，**可能没有创建对象**
	 * 通过类名调用静态方法时，因为没有具体对象，所以在static方法中不能访问实例成员
	 * 
	 * 如果希望方法可以被继承但是不可以被调用，可以使用访问级别protected
	 * 变量使用 protected 时， 可以**被子类的函数和友元等函数访问, 但是不能被子类的对象访问**; 
     */
//静态方法的存储: 调用方法时
// 1 . 非静态字段要求“对象引用”
// 每次通过引用调用实例方法时，都会隐式地传递引用
// 每一次进行引用的时候，调用的是同一个方法
// 调用时会开辟一个空间（栈帧）,存储对象的引用
// 每个对象调用该方法的时候，会传递它的引用，方法内部才知道访问哪个对象的数据

// 首先, 我们初始化一个类Person, 并给出Stident的继承方法: 
    class Person
    {
        public static int Count; 
        public Person() // 方法
        {
            Count++;  //统计程序运行中Person被调用的次数
        }
        public string Name { get; set; }
        public string identityID { get; set; }
        protected int Password = 0; // 可以被继承, 但是不能从对象访问
    }
    /// <summary>
    /// 学生类   -> 继承了人类中的Name和ID数字
    /// </summary>
    class Student : Person  //继承Person类
    {
        public int p1 = 0;
        static int c = 1;
        private Random random;
        public Student(int a)
        {
            random = new Random();  // 注意在这里进行初始化
            /* 规范的写法是构造函数中写相应的代码，
             * 声明放在上面，初始化在实例构造函数里面做
             * 
             */
        }
        static Student()
        {
        }
	}
	class Teacher : Person  //继承Person类
	    {
	        public string Salary { get; set; }
	    }

    class Program
    {
        static void Main(string[] args)
        {
            Teacher t1 = new Teacher();
            t1.identityID = "1234";

            Person p02 = new Student();
            // 父类型的引用指向子类型的对象-->只能使用父类成员
            Student st01 = (Student)p02;
            // 如果需要使用子类型的成员，必须进行强制类型转换

            // Teacher t02 = (Teacher)p02;//(代码会发生异常)
            // t02.Salary = "1000";
            // 由于p02是student类，所以没有相应的Salary项，会产生报错
            // 所以我们使用as来进行类型转换
            Teacher t03 = p02 as Teacher;  // 这个如果转换失败,不会产生异常
                                           // 但是，如果给相应的部分进行赋值，得到的结果将成为null

            Console.WriteLine(Person.Count);  // 统计类创建了多少个对象
            //(两个new是两个对象)
        }
    }
```


# C#数组拼接方法

在C#中，你可以使用`List<T>`来动态地添加元素，然后再将其转换为数组。这是因为数组的长度在创建时就已经确定，不能更改。但是`List<T>`的大小可以在运行时更改。以下是一个示例，它将两个数组连接到一起：

```csharp
using System;
using System.Collections.Generic;

class Program
{
    static void Main()
    {
        // 创建两个数组
        int[] array1 = new int[] { 1, 2, 3 };
        int[] array2 = new int[] { 4, 5, 6 };

        // 创建一个列表并添加数组
        List<int> list = new List<int>();
        list.AddRange(array1);
        list.AddRange(array2);

        // 将列表转换为数组
        int[] result = list.ToArray();

        // 打印结果
        foreach (int i in result)
        {
            Console.Write(i + " ");
        }
    }
}
```

这个程序首先创建了两个数组`array1`和`array2`，然后创建了一个`List<int>`对象。使用`AddRange`方法将两个数组的元素添加到列表中，最后使用`ToArray`方法将列表转换为数组。这样，`result`数组就是`array1`和`array2`的连接。

# 参数引用传递

在C#中，可以通过使用`ref`或者`out`关键字来传递参数的引用，从而在函数内部修改参数的值。这种方式不需要使用指针或者数组。以下是一个示例：

```csharp
using System;

class Program
{
    static void Main()
    {
        int number = 10;
        Console.WriteLine("Before function call, number = " + number);

        ChangeNumber(ref number);
        Console.WriteLine("After function call, number = " + number);
    }

    static void ChangeNumber(ref int num)
    {
        num = 20;
    }
}
```

在这个示例中，`ChangeNumber`函数接受一个`ref int`类型的参数，这意味着它接受的是参数的引用，而不是参数的值。因此，当在`ChangeNumber`函数内部修改`num`的值时，实际上修改的是`number`的值。

注意，当你使用`ref`或者`out`关键字时，你需要在函数调用和函数定义中都使用这个关键字。

在C#中，`ref`和`out`关键字都用于按引用传递参数，这意味着在函数内部对参数的任何修改都会影响到调用函数中的原始变量。然而，它们在使用上有一些区别：
1. **初始化**：使用`ref`关键字传递参数时，参数必须在传递之前进行初始化。而使用`out`关键字传递参数时，参数在传递之前不需要进行初始化。
2. **赋值**：`out`关键字要求在方法内部对参数进行赋值，即使参数在传递之前已经赋值，`out`也会清空变量，所以在方法内部必须对其重新赋值。
3. **语义**：`ref`和`out`的语义不同。`ref`通常用于函数需要修改调用者的值，而`out`通常用于函数需要返回额外的信息。
