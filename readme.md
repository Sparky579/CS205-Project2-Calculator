## CS205 C/C++ Programming
### Project 2 A Better Calculator
#### Name: 程司哲
#### Sid: 12112104
___

### 项目简介：

本项目实现了一个可以自动识别表达式并完成数学运算的计算器。该计算器支持复杂的带括号的加减乘除表达式运算。除此之外，还支持给变量赋值。程序能自动区分赋值语句和运算语句，并且在输出中体现出来。分别为："Assigned Successfully!" 和 "Solution:..." 。在运算语句中，输入的信息会被程序自动标准格式化。 最后，程序支持两个数学函数操作，分别为abs取绝对值，sqrt开根号。开根号使用二分法配合自己实现的乘法重载实现。

### Part 1: Analysis
需求：实现一个可以完成更多功能的计算器。

1. 输入表达式时可以得出结果，如 $2+3$， $5+2*3$等
2. 支持括号描述优先级，如 $(5+2)*3$
3. 支持定义变量，如 $x=3$
4. 支持数学函数，如 $sqrt(3)$
5. 支持高精度，如 $9999999999999999999.222222222+1.11$

### Part 2: Code
关键部分代码（利用结构体存储Number类）：

```
    int errorCode = 0;//0: no error 1: length error 2: type error
    // intPart: higher digit has higher index
    // fracPart: higher digit has lower index
    // this is in order to process digital carrying easier
    // 12.45: intPart:{2,1} fracPart:{4,5} 
    int intPart[maxLen + 10], fracPart[maxLen + 10], numSign = 1;
    int intLen = 0, fracLen = 0;
}
```

### Part 3: Result& Verification

**基本功能1之加减：**
```
Case 1:
Input: 2+3
Output: 2 + 3 = 5
Input2: 2-3
Output: 2 - 3 = -1

```
**基本功能2之复合运算：**
```
Case 2:
Input: 5+2*3
Output: 5 + 2 * 3 = 11

```
**基本功能3之括号优先级**
```
Case 3:
Input: (5+2)*3
Output: (5 + 2) * 3 = 21
Input2: 5+(2*3)
Output2: 5 + (2 * 3) = 11
Input3: (5+2)*3+5+2*3
Output3: (5 + 2) * 3 + 5 + 2 * 3 = 32
```
**拓展功能1之变量赋值与运算**
```
Case 4:
Input1: x=3
Output1: Assigned Successfully!
Input2: x*5+3
Output2: x * 5 + 3 = 18
Input3: x=7
Output3: Assigned Successfully!
Input4: (x+x)*x
Output4: (x + x) * x = 98

```
**拓展功能2之数学函数**

```
Input1: sqrt(5)
Output1: sqrt(5) = 2.23
Input2: sqrt(4)
Output2: sqrt(4) = 2
Input3: sqrt((2+3)*2)
Output3: sqrt((2 + 3) * 2) = 3.16
Input4: abs(-23.232323)
Output4: abs( - 23.232323) = 23.232323
Input5: sqrt(1e6)
Output5: sqrt(1e6) = 1000

```
**拓展功能3之高精度**

```
Input1: 100000000000000000000000000000.22222222222222222222+200000000000000000000000000000.111111111
Output1: 100000000000000000000000000000.22222222222222222222 + 200000000000000000000000000000.111111111 = 300000000000000000000000000000.33333333322222222222
Input2: sqrt(1e100)
Output2: sqrt(1e100) = 100000000000000000000000000000000000000000000000000

```
**异常处理**

```
Input1: 1e888*1e888
Output1: Error occurred! Please examine your typing
//最多支持整数，小数各1000位运算
Input2: 3+3=6
Output2: This var name is illegel!
Input3: x=aaa
Output3: Can not assign this value!
Input4: sqrt(-1)
Output4: Error occurred! Please examine your typing
//暂时不支持复数运算，故报出异常
Input5: a=1=1
Output5: You are typing a wrong equation!
```

**标准格式化**

```
Input1: 1     +      1
Output1: 1 + 1 = 2
Input2: sqrt  (  1  +   1  )
Output2: sqrt(1 + 1) = 1.41
```



