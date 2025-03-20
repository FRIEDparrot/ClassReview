在 CSS 中，**垂直对齐**主要是用来控制一个元素在其父容器（通常是块级元素或行内块元素）中的垂直位置。如果你想要垂直对齐元素，并且 `justify-content` 不起作用（因为它是用于 **Flexbox** 或 **Grid** 容器的），可以考虑以下几种方法：

### 1. **使用 `vertical-align`**（针对行内或行内块元素）

`vertical-align` 可以用于控制行内元素或行内块元素的垂直对齐。这对于单个行内元素（如 `span`, `img`, `a` 等）非常有用。

例如，你的 `.user-profile-column-avatar-border` 是一个块级元素，而内部的 `.user-avatar` 是行内块元素（或图片）。你可以用 `vertical-align` 来调整对齐方式：

```css
.user-profile-column-avatar-border {
    width: 85px;
    height: 85px;
    text-align: center;
    background-color: red;
    border-radius: 1000px;
    border-style: solid;
    border-width: 1px;
    border-color: transparent;
    margin: 5px 10px;
    display: inline-block; /* 或者 inline-flex 等 */
    vertical-align: middle; /* 设置垂直对齐方式 */
}

.user-profile-column-avatar-border .user-avatar {
    width: 75px;
    height: 75px;
    border-radius: 1000px;
    vertical-align: middle; /* 垂直对齐到父元素的中间 */
}
```

在这里，`vertical-align: middle` 会使得 `.user-avatar` 垂直居中于 `.user-profile-column-avatar-border` 的容器。

`vertical-align` 还有几个常用的值，具体根据你的需求来选择：

- `top`：将元素对齐到父容器的顶部。
- `middle`：将元素对齐到父容器的中间。
- `bottom`：将元素对齐到父容器的底部。

### 2. **使用 Flexbox**（推荐的方法，适用于大多数情况）

如果你希望更灵活地处理垂直对齐，使用 **Flexbox** 是一种很好的方法。Flexbox 是专门用来解决对齐问题的，它的 `align-items` 属性非常适合垂直对齐。

例如，使用 `display: flex` 来包裹 `.user-profile-column-avatar-border` 并设置 `align-items: center` 来实现垂直居中：

```css
.user-profile-column-avatar-border {
    width: 85px;
    height: 85px;
    text-align: center;
    background-color: red;
    border-radius: 1000px;
    border-style: solid;
    border-width: 1px;
    border-color: transparent;
    margin: 5px 10px;
    display: flex; /* 设置为flex容器 */
    align-items: center; /* 垂直居中 */
    justify-content: center; /* 水平居中 */
}

.user-profile-column-avatar-border .user-avatar {
    width: 75px;
    height: 75px;
    border-radius: 1000px;
}
```

### 3. **使用 `line-height`**（适用于单行文本的垂直居中）

如果你的容器内部只有文本或者是单行元素，`line-height` 是一种简便的方式来实现垂直对齐。通过设置 `line-height` 为和容器高度一样的值，可以使文本垂直居中。

```css
.user-profile-column-avatar-border {
    width: 85px;
    height: 85px;
    text-align: center;
    background-color: red;
    border-radius: 1000px;
    border-style: solid;
    border-width: 1px;
    border-color: transparent;
    margin: 5px 10px;
    line-height: 85px; /* 设置 line-height 为高度 */
}

.user-profile-column-avatar-border .user-avatar {
    width: 75px;
    height: 75px;
    border-radius: 1000px;
}
```

但需要注意的是，`line-height` 主要用于单行文本或行内元素，不适合复杂布局。

### 总结

- **`vertical-align`** 适用于行内元素或行内块元素的垂直对齐。
- **`display: flex`** 和 `align-items: center` 是实现垂直对齐的现代、灵活方式，适用于大多数布局。
- **`line-height`** 适用于文本或简单的单行元素的垂直居中。

如果你的目标是使 `.user-avatar` 垂直居中于 `.user-profile-column-avatar-border`，推荐使用 Flexbox（方法 2），它是最常见的方式并且适应性强。