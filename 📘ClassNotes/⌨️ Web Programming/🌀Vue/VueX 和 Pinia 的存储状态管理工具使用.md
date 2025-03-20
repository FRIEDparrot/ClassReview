### (1) Vuex 和 Pinia 简介
Vuex 是适用于较大型项目的**状态管理工具**, 参考 [state-management 部分](https://vuejs.org/guide/scaling-up/state-management.html#simple-state-management-from-scratch) , 类似的有 [Pinia](https://pinia.vuejs.org/) (实际上, Vuex 目前已经停止维护 , Pinia 为更加接近Vuex5的工具), 具体教程参考[vuex guide](https://vuex.vuejs.org/guide)部分。

在 JavaScript 和 TypeScript 中，你可以有多个 `export`，但只能有一个 `export default`。这是模块系统的一个规范，用于明确区分默认导出和命名导出。 

Vuex 是类似 [Flux](https://facebook.github.io/flux/docs/overview), [Redux](http://redux.js.org/) 状态管理器的设计思路, 
最基本架构如下: 
![[attachments/Pasted image 20250104234014.png|300]]
由于状态变量 `state` 部分往往是多个组件共享内存, 并共同改变的,  

实际上 Vuex 是在组件之外进行共享状态修改该状态内容，并支持多个组件对该状态的修改。
在 Vuex 中, 唯一修改状态变量的办法是显式地提交 `mutation` , 具体给出入下的示例代码: 
```ts
import { createStore } from 'Vue' 

const store = createStore({
	state(){
		return {
			count: 0
		}
	},
	mutations: {
		add(state) : {
			state.count ++;
		}
	}
})

store.commit('add')
console.log(store.state.count) // -> 1
```
此外可以采用 `this.$store` 访问存储元素,  如下: 
```ts
this.$store.commit('increment')
console.log(this.$store.state.count)
```
一般地, 可以在 method 部分, 加入 store.commit, 用于修改数据等等和提交结果。

## (2) 基于 Vuex 的缓存机制 
在 Vuex 中，虽然 `state` 是存储应用状态的主要地方，但你并不需要将所有状态变量都放在 `state` 中。Vuex 的核心概念包括 `state`、`getters`、`mutations`、`actions` 和 `modules`，你可以根据需求灵活使用这些概念来管理状态。

### 1. **State**
`state` 是存储应用状态的地方，通常用于存储需要在多个组件中共享的数据。如果你有几个缓存（cache）需要在多个地方复用，`state` 是一个合适的地方来存储它们。

```javascript
const store = new Vuex.Store({
  state: {
    cache: {}
  },
  mutations: {
    setCache(state, { key, value }) {
      state.cache[key] = value;
    }
  },
  actions: {
    updateCache({ commit }, { key, value }) {
      commit('setCache', { key, value });
    }
  },
  getters: {
    getCache: (state) => (key) => state.cache[key]
  }
});
```

### 2. **Getters**
`getters` 用于从 `state` 中派生出一些状态。你可以使用 `getters` 来获取缓存中的数据。

```javascript
getters: {
  getCache: (state) => (key) => state.cache[key]
}
```

在组件中使用：

```javascript
this.$store.getters.getCache('someKey');
```

### 3. **Mutations**
`mutations` 是唯一可以修改 `state` 的地方。你可以通过 `mutations` 来更新缓存。

```javascript
mutations: {
  setCache(state, { key, value }) {
    state.cache[key] = value;
  }
}
```

### 4. **Actions**
`actions` 用于处理异步操作或批量的同步操作。你可以通过 `actions` 来触发 `mutations` 更新缓存。

```javascript
actions: {
  updateCache({ commit }, { key, value }) {
    commit('setCache', { key, value });
  }
}
```

在组件中使用：

```javascript
this.$store.dispatch('updateCache', { key: 'someKey', value: 'someValue' });
```

### 5. **Modules**
如果你的应用比较复杂，可以将状态分割到不同的 `modules` 中。每个模块可以有自己的 `state`、`getters`、`mutations` 和 `actions`。

```javascript
const cacheModule = {
  state: {
    cache: {}
  },
  mutations: {
    setCache(state, { key, value }) {
      state.cache[key] = value;
    }
  },
  actions: {
    updateCache({ commit }, { key, value }) {
      commit('setCache', { key, value });
    }
  },
  getters: {
    getCache: (state) => (key) => state.cache[key]
  }
};

const store = new Vuex.Store({
  modules: {
    cache: cacheModule
  }
});
```

### 总结
- **State**: 用于存储需要在多个组件中共享的状态。
- **Getters**: 用于从 `state` 中派生出一些状态。
- **Mutations**: 用于同步修改 `state`。
- **Actions**: 用于处理异步操作或批量的同步操作。
- **Modules**: 用于将状态分割到不同的模块中。

你可以根据需求将缓存（cache）存储在 `state` 中，并通过 `getters`、`mutations` 和 `actions` 来管理和使用这些缓存。



##   独立状态存储模块设计
是的，你可以将 `cache` 模块作为一个独立的 Vuex store 来使用，并调用其中的 `getInterestByKeys` 函数来获取返回值。以下是具体的使用方法：

---

### 1. 创建独立的 Vuex Store

你已经正确地创建了一个独立的 Vuex store，代码如下：

```typescript
import { createStore } from "vuex";
import InterestTag from "@/interface/interestTags";
import { getTagByKeys } from "@/api/interest_tags/interest_tags_api";

const cache = createStore({
    state: {
        interest_tag_cache: [] as InterestTag[],
    },
    mutations: {
        setInterestTagCache(state, tags: InterestTag[]) {
            // filter the existing tags to avoid duplication
            const newTags = tags.filter(
                (tag) =>
                    !state.interest_tag_cache.some(
                        (cachedTag) => cachedTag.key === tag.key,
                    ),
            );
            state.interest_tag_cache.push(...newTags); // push new tags to the end of the cache
        },
    },
    getters: {
        getInterestTag(state) {
            return state.interest_tag_cache;
        },
        getInterestTagByKey: (state) => (key: string) => {
            return state.interest_tag_cache.find((tag) => tag.key === key);
        },
    },
    actions: {
        async getInterestByKeys(
            { commit, state },
            key_list: string[],
        ): Promise<InterestTag[]> {
            const cached_keys = state.interest_tag_cache.map((tag) => tag.key);
            const new_keys = key_list.filter(
                (key) => !cached_keys.includes(key),
            );
            if (new_keys.length === 0) {
                return Promise.resolve(state.interest_tag_cache);
            }
            try {
                // for new tags, fetch from backend and cache them
                const response: any = await getTagByKeys(new_keys);
                const newTags: InterestTag[] = response;
                commit("setInterestTagCache", newTags);
                return Promise.resolve(state.interest_tag_cache);
            } catch (error) {
                return Promise.reject(error);
            }
        },
    },
});

export default cache;
```

---

### 2. 在组件中调用 `getInterestByKeys` 并获取返回值

在你的 Vue 组件中，你可以通过以下步骤调用 `getInterestByKeys` 并获取返回值：

#### 2.1 引入独立的 Vuex Store

首先，在你的组件中引入这个独立的 Vuex store：

```typescript
import cacheStore from '@/store/cache'; // 根据你的实际路径引入
```

#### 2.2 调用 `getInterestByKeys` 函数

在组件的方法中，你可以通过 `cacheStore.dispatch` 调用 `getInterestByKeys` 并获取返回值：

```typescript
<template>
  <div>
    <!-- 你的组件模板 -->
  </div>
</template>

<script lang="ts">
import { defineComponent } from 'vue';
import cacheStore from '@/store/cache'; // 引入独立的 Vuex store

export default defineComponent({
  name: 'YourComponent',
  methods: {
    async fetchInterestTags() {
      const keys = ['key1', 'key2', 'key3']; // 替换为你需要的 keys
      try {
        const tags = await cacheStore.dispatch('getInterestByKeys', keys);
        console.log('Fetched tags:', tags);
      } catch (error) {
        console.error('Error fetching tags:', error);
      }
    },
  },
  mounted() {
    this.fetchInterestTags();
  },
});
</script>
```

---

### 3. 访问 Getters

如果你需要访问缓存中的数据，可以通过 `cacheStore.getters` 来访问：

```typescript
const cachedTags = cacheStore.getters.getInterestTag;
const specificTag = cacheStore.getters.getInterestTagByKey('someKey');
```

---

### 4. 总结

- 你可以将 `cache` 模块作为一个独立的 Vuex store 使用。
- 通过 `cacheStore.dispatch('getInterestByKeys', keys)` 调用 `getInterestByKeys` 函数并获取返回值。
- 通过 `cacheStore.getters` 访问缓存中的数据。

这种方式适合将某个功能模块独立出来，避免与主 Vuex store 耦合。


