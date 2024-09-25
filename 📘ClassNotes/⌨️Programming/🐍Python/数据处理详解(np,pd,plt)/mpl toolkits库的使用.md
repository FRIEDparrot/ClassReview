mpl tookits 是 matplotlib 自带的绘制图形库, 基本包括如下三个 模块 
```python
import mpl_toolkits.mplot3d
import mpl_toolkits.axes_grid1 
import mpl_toolkits.axisartist 
```

其中 mplot3d 包含了简单的三维曲线,曲面图绘制, 具体参考 [mplot3d tutorial](https://matplotlib.org/stable/users/explain/toolkits/mplot3d.html#mplot3d), 一般也可以采用:

```python
import matplotlib.pyplot as plt
fig = plt.figure()
ax = fig.add_subplot(projection='3d')
```

matplotlib.animation.Funcanimation 部分: 对于 Funcanimation 会传入一个带有 num 和 fargs 参数的函数, 而且每个 interval 调用一次 update_lines 函数, 总共 num_steps 步。
```python
import matplotlib.animation as animation

def update_lines(num, walks, lines):
    for line, walk in zip(lines, walks):
        line.set_data_3d(walk[:num, :].T)
    return lines
# Data: 40 random walk points as (num_steps, 3) arrays
num_steps = 30
walks = [random_walk(num_steps) for index in range(40)]    
# Attaching 3D axis to the figure
fig = plt.figure()
ax = fig.add_subplot(projection="3d")
# Create lines initially without data
lines = [ax.plot([], [], [])[0] for _ in walks]
# Creating the Animation object
ani = animation.FuncAnimation(fig, update_lines, num_steps, fargs=(walks, lines), interval=100)
plt.show()
```



