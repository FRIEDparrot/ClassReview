四节点单元刚度矩阵的计算
```erlang 
% 四节点单元刚度矩阵函数 E杨氏模量, nu 泊松比, h板厚度
function Ke = Elem401(E,nu,h)
syms x 
syms y 

M = [
    [1, x, y, x*y,0,0,0,0]
    [0,0,0,0, 1, x, y, x*y]
    ];

loc_e = [-1, -1, 1, -1, 1,1,-1, 1];

A = [
    [1 -1 -1 1 0 0 0 0]
    [0 0 0 0 1 -1 -1 1]
    [1 1 -1 -1 0 0 0 0]
    [0 0 0 0 1 1 -1 -1]
    [1 1 1 1 0 0 0 0]
    [0 0 0 0 1 1 1 1]
    [1 -1 1 -1 0 0 0 0]
    [0 0 0 0 1 -1 1 -1]
    ];

N = M/A;  % M * inv(A) , 为形状函数矩阵

% 对x, y等参变量求导并得到对应的应变矩阵

N = M/A;  % M * inv(A) , 为形状函数矩阵
% 对x, y等参变量求导并得到对应的应变矩阵
N_x = diff(N,x);
N_y = diff(N,y);

B = [N_x(1,:)
     N_y(2,:)
     N_y(1,:) + N_x(2,:)
     ];
clear N_x N_y

D = E * h/(1-nu^2) * [
[1 nu 0]
[nu 1 0]
[0 0 (1- nu)/2]
];

k_e = transpose(B) * D * B;
Ke = int(int(k_e,x,[-1, 1]), y, [-1,1]);  % 积分求解对应的四节点刚度矩阵
% 注意不能使用vpa函数, vpa函数会产生截断误差
end
% Ke = vpa(subs(K_e,[E,h,nu],[2e11,10,0.3]),4);
% Test Code :  Elem401(2e11, 10, 0.3)

```

八节点单元的刚度矩阵计算

```erlang 
% Test Code: Elem801(2e11,0.3, 10)
function Ke = Elem801(E, nu,h)
    syms x y
    % 对于平面八节点单元，共有
    M = [
        [1 x y x^2 x*y y^2 x^2*y x*y^2 0 0 0 0 0 0 0 0]
        [0 0 0 0 0 0 0 0 1 x y x^2 x*y y^2 x^2*y x*y^2]
        ];
    
    
    % 定义八节点单元的x_e和y_e
    x_e = [-1 0 1 1 1 0 -1 -1];
    y_e = [-1 -1 -1 0 1 1 1 0];
    
    % 创建初始矩阵A
    A = zeros(16,16);
    for i = 1:8
        A(2*i-1:1:2 *i,:) = subs(M,[x,y],[x_e(i),y_e(i)]);
    end
    
    % 计算形状函数矩阵 
    N = M/A;
    
    % 使用求导方法求解对应的位移函数
    N_x = diff(N,x);
    N_y = diff(N,y);
    B = [N_x(1,:)
         N_y(2,:)
         N_y(1,:) + N_x(2,:)
         ];
    clear N_x N_y
    % 刚度矩阵D
    D = E/(1-nu^2) * [
        [1 nu 0]
        [nu 1 0]
        [0 0 (1- nu)/2]
        ];
    
    ke_pre = B'*D*B;
    Ke = h * int(int(ke_pre,x,[-1, 1]), y, [-1,1]); % 积分得到刚度矩阵
    clear ke_pre
end
```