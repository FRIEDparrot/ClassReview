## 一、Markdown 渲染库
由于整体规则 rules 下的路由较多, 且整体路由很麻烦，因此我们可以考虑直接采用 markdown 渲染在 HTML 页面上, 从而减轻之后的 rules 维护负担

一般 vue 提供了如下的 markdown 解析库: 
- [`marked`](https://github.com/markedjs/marked)（轻量级）
- [`markdown-it`](https://github.com/markdown-it/markdown-it)（更可扩展）

为了有更高的可扩展性，采用 markdown-it 进行构建 : 相关文档参考 : https://markdown-it.github.io/markdown-it/ 

```sh
sudo npm install markdown-it 
```

此时, 可以采用 [highlight.js](https://highlightjs.org/) 进行高亮代码块, 经过较多测试, 仅能用 cdn 引入同时 npm install 包实现协同 markdown 代码块部分的语法高亮: 
```html title:index.html 
<!-- import highlight.js globally -->
<link
	rel="stylesheet"
	href="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/styles/github-dark.min.css"
/>
<script src="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/highlight.min.js"></script>
```

然后需要的界面最后还要加上渲染:
```ts
<template lang="html">
    <div class="rules-collection">
        <div v-html="renderHTML"></div>
    </div>
</template>

<script lang="ts">
import {
    defineComponent,
    defineAsyncComponent,
    markRaw,
    ref,
    nextTick,
} from "vue";

import markdownIt from "markdown-it";
import hljs from "highlight.js";

const md = markdownIt({
    highlight: function (str, lang): string {
        if (lang && hljs.getLanguage(lang)) {
            try {
                return hljs.highlight(str, { language: lang }).value;
            } catch (__) {}
        }
        return ""; // use external default escaping
    },
});

export default defineComponent({
    name: "RulesIndex",
    data() {
        return {
            renderHTML: "",
        };
    },
    mounted() {
        const markdownContent = ref(`
# This is a title
see [highlight.js](https://highlightjs.org/)

\<script\>
alert("This script is executed!");
\</script\>

\`\`\`cpp
#include <stdio.h>
using namespace std;
int main() {
    printf("Hello, world!");
    return 0;
}
\`\`\`
`);
        this.renderHTML = md.render(markdownContent.value);
        // wait for DOM Loaded
        nextTick(() => {
            document.querySelectorAll("pre code").forEach((block) => {
                hljs.highlightElement(block as HTMLElement); // 高亮
            });
        });
    },
});
</script>
```

效果如下所示:
![[attachments/Pasted image 20250119010304.png|350]]

如果需要定制, 则采用: 
```ts
import cpp from "highlight.js/lib/languages/cpp";

hljs.registerLanguage("cpp", cpp);import cpp from "highlight.js/lib/languages/cpp";
hljs.registerLanguage("cpp", cpp);
```

## 二、后端部署和前端渲染
### (1) 实际需求
对于网站规则等界面, 为了有更好的可维护性, 我们采用 markdown 编译规则并将其存储在后端, 而前端可以根据实际浏览器语言向后端发起请求, 从而获取 Markdown 并在界面渲染不同的规则;

为了可用性更高, 我们希望前端能够检测后端的 rules 文件夹并且渲染出对应的界面， 为了支持文件夹和文件名称翻译，可以考虑加后端每个语言加一个 json 文件支持翻译)

同时, 为了防范路径遍历攻击, 首先我们先使用 rules 作为安全文件夹 (其下所有开放访问), 而由于langcode 是可以伪造的, 因此需要首先检查 langcode 格式(用正则匹配严格检查)

```python 
filepath_tot = os.path.join(RULES_DIR, lang_code, path)  
file_path = os.path.abspath(os.path.normpath(filepath_tot))
```

显然此时 file_path 中不含 `..` 项, 此时只需检查 file_path 是否以安全文件夹路径开头即可。


### (2) 多级路径匹配 

Flask 端参考 [[📘ClassNotes/⌨️ Web Programming/🐍Python Web 与 Flask 框架/Flask 框架入门/多级文件夹路径匹配|多级文件夹路径匹配]] 
对于 Vue 端参考 
