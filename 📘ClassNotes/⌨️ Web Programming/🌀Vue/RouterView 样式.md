注意: 
Extraneous non-props attributes (class) were passed to component but could not be automatically inherited because component renders fragment or text or teleport root nodes. 

即当某个组件渲染部分页面时, 例如 RouterView 不能自动传递外部传入的属性;
一般方法是 RouterView 上不能加 class 标签