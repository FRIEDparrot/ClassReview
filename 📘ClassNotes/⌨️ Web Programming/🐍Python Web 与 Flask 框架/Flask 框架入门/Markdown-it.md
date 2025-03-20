https://www.jsdelivr.com/package/npm/markdown-it

在 Vue 中实现从后端加载 Markdown 内容并提取标题生成链接列表的功能，可以通过以下步骤完成。

### 步骤概述

1. **加载 Markdown 内容**：使用 Axios 或 Fetch API 从后端接口获取 Markdown 数据。
2. **提取标题**：使用正则表达式或 Markdown-it 的功能提取标题信息。
3. **生成标题列表**：将提取出的标题渲染为可点击的链接。
4. **渲染 Markdown**：使用 Markdown-it 渲染 Markdown 内容，并在每个标题前加上 `#` 符号。
5. **实现跳转**：通过点击标题列表中的链接，能够跳转到对应的 Markdown 内容位置。

### 示例代码实现

下面是一个可能的实现方式：

```vue
<template>
  <div>
    <div class="title-list">
      <h3>目录</h3>
      <ul>
        <li v-for="(title, index) in titles" :key="index">
          <a :href="'#' + title.id" @click.prevent="scrollToTitle(title.id)">
            {{ title.text }}
          </a>
        </li>
      </ul>
    </div>
    <div v-html="renderedMarkdown"></div>
  </div>
</template>

<script>
import axios from 'axios';
import MarkdownIt from 'markdown-it';

export default {
  data() {
    return {
      markdownContent: '',
      renderedMarkdown: '',
      titles: [],
    };
  },
  mounted() {
    this.loadMarkdown();
  },
  methods: {
    async loadMarkdown() {
      const response = await axios.get('/api/markdown'); // 替换为实际接口
      this.markdownContent = response.data;
      this.extractTitles();
      this.renderMarkdown();
    },
    extractTitles() {
      const titleRegex = /^(#{1,6})\s+(.*)/gm; // 匹配 Markdown 标题
      let match;

      while ((match = titleRegex.exec(this.markdownContent)) !== null) {
        const level = match[1].length; // 标题等级
        const text = match[2];
        const id = text.replace(/\s+/g, '-').toLowerCase(); // 创建 id
        this.titles.push({ level, text, id });
      }
    },
    renderMarkdown() {
      const md = new MarkdownIt();
      this.renderedMarkdown = md.render(this.markdownContent);
      // 添加 id 到每个标题
      this.titles.forEach(title => {
        const regex = new RegExp(`(<h${title.level}.*?>)\\s*${title.text}\\s*(</h${title.level}>)`, 'g');
        this.renderedMarkdown = this.renderedMarkdown.replace(regex, `$1<a id="${title.id}"></a>${title.text}$2`);
      });
    },
    scrollToTitle(id) {
      const element = document.getElementById(id);
      if (element) {
        element.scrollIntoView({ behavior: 'smooth' });
      }
    },
  },
};
</script>

<style>
.title-list {
  margin-bottom: 20px;
}
.title-list ul {
  list-style-type: none;
  padding: 0;
}
.title-list li {
  margin: 5px 0;
}
</style>
```

### 代码解析

1. **加载 Markdown 内容**：
   - 使用 `axios.get` 方法请求后端接口。
  
2. **提取标题**：
   - 使用正则表达式 `/^(#{1,6})\s+(.*)/gm` 匹配 Markdown 中的标题。
   - 每个标题生成一个对应的 ID（将空格转换为 `-`，并转为小写）。

3. **渲染 Markdown**：
   - 使用 Markdown-it 来渲染 Markdown 内容。
   - 在渲染后，将每个标题添加一个 `<a>` 标签，以便可以通过 `href` 跳转。

4. **跳转至指定位置**：
   - `scrollToTitle` 方法使用 `scrollIntoView()` 方法平滑滚动到对应标题。

该实现提供了一个完整的解决方案，可以根据具体需求进行修改和扩展。若需进一步探讨或调整功能，请告诉我！



`html_content.replace(match[0], label_with_id)` 这行代码是否会替换 **所有** 匹配的标题，还是仅替换第一个匹配项。

### 回答：

在 JavaScript 中，`String.prototype.replace()` 默认情况下 **只会替换第一个匹配的部分**，即使是全局正则表达式。如果你希望替换所有匹配的内容，你需要确保正则表达式中使用了全局标志 `g`。

在你的代码中，正则表达式 `/(<h[1-6])>(.*?)<\/h[1-6]>/gm` 已经使用了 `g` 标志（全局搜索），这意味着它会查找文档中所有匹配的 `<h1> - <h6>` 标签，并且会通过 `regexp.exec(html_content)` 获取到每一个匹配项。

不过，你在替换的代码行中是通过 `html_content.replace(match[0], label_with_id)` 来替换每一个匹配项，而这个方法每次执行时会替换 **当前匹配项**。如果你用 `replace()` 方法替换一个匹配的部分，下一次调用 `replace()` 时会继续从上一次替换后的结果继续查找下一个匹配项。

### 关键点：

- **`replace()`** 会 **替换** 当前匹配到的 **第一个** 结果（如果不使用全局标志）。
- 如果你希望替换所有匹配项，正如你已经做的那样，你可以在 `replace()` 方法中使用正则表达式的全局标志 `g`，但是需要注意，`regexp.exec(html_content)` 的迭代方式，替换操作会在每次找到匹配项后更新 `html_content`，并继续查找下一个匹配项。

### 进一步解释：

假设你有以下 HTML 内容：

```html
<h1>Section 1</h1>
<h2>Subsection 1</h2>
<h1>Section 2</h1>
```

并且你想用 `replace()` 替换每个 `<h1>` 和 `<h2>` 标签：

```javascript
const html_content = `<h1>Section 1</h1><h2>Subsection 1</h2><h1>Section 2</h1>`;
const regexp = /(<h[1-6])>(.*?)<\/h[1-6]>/gm;

html_content.replace(regexp, (match, tag, contentWithoutTag) => {
    console.log("Matched:", match);  // 输出匹配的标签内容
    // 在这里可以返回替换后的标签
    return `<${tag} id="unique-id">${contentWithoutTag}</${tag}>`;
});
```

### 安全性：

- **只替换第一个匹配**：`replace()` 会按顺序替换每一个匹配项，直到全部匹配项被处理完。因此，在你的代码中，`regexp.exec(html_content)` 会一个一个地匹配标题，然后每个标题都替换一次。
    
- **避免遗漏**：你的代码中的 `regexp.exec(html_content)` 和 `replace()` 方法配合使用，确保了每次执行替换时，它都能处理一个新的匹配项。如果 `html_content.replace(match[0], label_with_id)` 只替换当前找到的匹配项，那么它不会漏掉其他匹配项，所有标题都会被逐一替换。
    

### 结论：

由于你已经使用了全局标志 `g`，并且在 `replace()` 中逐个处理匹配项， **它会逐一替换每个匹配项**，并不会只替换第一个。因此，**这种方式是安全的**，每个标题会被准确替换。