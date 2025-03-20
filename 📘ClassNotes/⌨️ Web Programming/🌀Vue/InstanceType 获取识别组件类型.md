对于从其他部分 import 得到的 components 类型, 我们使用如下方法声明其显式类型 : 
```ts
import newBlogPage from '@/pages/blogs/common/newBlogPage.vue'; 

mounted() {
	if (this.draftContent) {
		const blogPage = this.$refs.newBlogPage as InstanceType<
			typeof newBlogPage
>;
		blogPage.title = this.draftContent.title;
	}
},
```

需要说明， 如果有可能 `display = false` , 一定要加上 id 判断 
