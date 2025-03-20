需求: 在 v-list-item , 使用 to 且其中有按钮需要被点击， 同时阻止默认的 to 行 : 

方法是采用如下 `click.naive.stop` 办法 : 
```html
<v-menu
	v-if="menus && menus.items.length > 0"
	location="start"
>
	<template v-slot:activator="{ props }">
		<v-btn
			@click.native.stop="
				handleMenuClick($event)
			"
			v-bind="props"
			variant="text"
			size="tiny"
			icon="mdi-dots-horizontal"
			class="pa-1"
		>
		</v-btn>
	</template>
```

```ts 
handleMenuClick(event: Event) {
	event.stopPropagation();
	event.preventDefault();
},
```