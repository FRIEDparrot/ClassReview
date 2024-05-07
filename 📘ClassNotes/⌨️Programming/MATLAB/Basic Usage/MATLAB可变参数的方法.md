除了使用 nargin, nargout, 可以使用可变参数来给部分的参数赋默认值
方法是创建 name-value pairs ,  首先， 这个==需要使用 varargin 函数来接收之后的参数==

首先， 可以<b><mark style="background: transparent; color: blue">使用 celldisp 来显示varargin 接收的内容</mark></b>; 

然后使用 `inputParser`  函数来创建解析器， 解析每一个字符串参数对应的参数传入值。


```matlab title:根据文档找到如下的输入验证函数
function a = findArea(width,varargin)
   defaultHeight = 1;
   defaultUnits = 'inches';
   defaultShape = 'rectangle';
   expectedShapes = {'square','rectangle','parallelogram'};

   p = inputParser;
   validScalarPosNum = @(x) isnumeric(x) && isscalar(x) && (x > 0);
   addRequired(p,'width',validScalarPosNum);
   addOptional(p,'height',defaultHeight,validScalarPosNum);
   addParameter(p,'units',defaultUnits,@isstring);
   addParameter(p,'shape',defaultShape,...
                 @(x) any(validatestring(x,expectedShapes)));
   parse(p,width,varargin{:});
   
   a = p.Results.width*p.Results.height; 
end
```

其中主要有如下的几个方法调用:

| 方法           | 解释                     |
| ------------ | ---------------------- |
| addOptional  | 将可选的位置参数添加到输入解析器模式中    |
| addParameter | 在输入解析器模式中添加可选的名称-值对组参数 |
| addRequired  | 将必需的位置参数添加到输入解析器模式中    |
| parse        | 解析函数输入                 |
我们只需调用addOptional函数即可添加可选参数。

注意: 调用使用<mark style="background: transparent; color: red">输入解析器的函数(parse)</mark>时，需要按以下顺序传递参数:
1. 必需参数    
2. 任何可选的位置参数
3. 任何名称-值对组
```matlab title:示例代码
 parse(p,width,varargin{:});   % 其中width是原先传入参数, varargin是可选参数
```

解析函数 (parse) 用于生成一个inputParse的同时验证某个输入的参数是否符合输入的要求;
检查某个参数输入是否符合要求时， 必须调用parse函数(详见parse文档)
```matlab title:正整数检查示例代码
validScalarPosNum = @(x) isnumeric(x) && isscalar(x) && (x > 0);  % 检查正整数
```
注意: 调用Parse之后, 就会生成对应参数的输入对， 然后就可以在


我们以编写下面一个简单的函数为例子:
```matlab
function xp = buncher_sample(x_i, sigma_d, varargin)
%buncher_sample buncher 抽样, 以点x_i为中心, 获取 2n+1 个buncher设计点
%f 插值系数, 默认为1
    p = inputParser;
    validScalarPosNum = @(x) isnumeric(x) && isscalar(x) && (x > 0);   % 检查输入的部分是不是正整数
    addRequired (p,'x_i');      % 占据第一个位置
    addRequired (p,'sigma_d');  % 占据第二个位置 
    addParameter(p, 'f', 1, validScalarPosNum);  % 添加可选参数(插值系数f)
    parse(p, x_i, sigma_d ,varargin{:});          % 匹配输入参数, 同时进行检查

    f = p.Results.f;  % 获取对应的参数

    n = size(x_i,2);
    xp = zeros(2 * n + 1, n);  % 设计点组
    xp(1,:) = x_i;
    for i = 1:length(x_i)
        f_delta = zeros(1,n); f_delta(i) = sigma_d(i) * f;
        xp(2 * i,:) = x_i + f_delta;
        xp(2*i+1,:) = x_i - f_delta;
    end
end
```

需要说明的是， 这个虽然能够传入参数， 但是仍然不能在传参时进行代码建议提示。


如果需要进行提示， 则可以在(unctionSignatures.json文件中添加对应代码:
```json
{
    "_schemaVersion": "1.0.0",
    "buncher_sample":
    {
        "inputs":
        [
            {"name":"mu", "kind":"required", "type":["vector"], "purpose":"均值"},
            {"name":"sigma", "kind":"required", "type":["vector"], "purpose":"协方差矩阵"},
            {"name":"f", "kind":"namevalue", "type":["numeric"],"purpose":"插值系数f"}
        ]
    }
}
```
重启即可获得对应提示:
![[attachments/Pasted image 20240328131333.png]]
