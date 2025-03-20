## 一、RESTful Api 基本概念
### (1) 概念简介
**RESTful API** 是基于 **REST**（Representational State Transfer）架构风格设计的 Web 服务接口。它通过 HTTP 协议，使用标准的 HTTP 方法（如 GET、POST、PUT、DELETE）来提供数据服务和操作资源。

![[attachments/Pasted image 20250112163929.png|600]]
Restful Api 中所有的 Http Response 会被转换为 Json 之后进行返回。


RESTful Api 中, <b><mark style="background: transparent; color: orange">主要的核心概念如下</mark></b>:  
1. **资源（Resource）**  
    REST 中的每个 URL 都代表一种资源。资源可以是用户、订单、商品等实体。例如:  
    - `/users` 表示所有用户资源。
    - `/users/123` 表示 ID 为 123 的单个用户。
	- 表现形式 (Representational)   将资源具体呈现出的 json, xml, image, txt 等称为<b><mark style="background: transparent; color: orange">表现层或者表现形式</mark></b>, 客户端和服务器之间,  传递资源的表现形式
2. **HTTP 方法**(Verb) 
Http 方法也称为动作，不同的 HTTP 方法对资源执行不同的操作: 
    - `GET`：从服务器获取资源（只读操作）。
    - `POST`：在服务器上创建资源。
    - `PUT`：更新服务器上的资源(客户端提供更新后的整个资源) 。
    - `DELETE`：删除服务器上的资源。
    - `PATCH` : 更新服务器上的资源 (客户端提供更新属性), 但是使用较少  
3. 路径 : 表示 API 的具体网址。实际开发中常见的规范如下：
- 网址中不能有动词，只能有名词，API 中的名词也应该使用复数。 因为 REST 中的资源往往和数据库中的表对应，而数据库中的表都是同种记录的"集合"（collection）。如果 API 调用并不涉及资源（如计算，翻译等操作）的话，可以用动词。比如：GET /calculate?param1=11&param2=33 。
- 不用大写字母，建议用中杠 - 不用下杠 _ 。 比如邀请码写成 invitation-code而不是 invitation_code。
善用版本化 API。 当我们的 API 发生了重大改变而不兼容前期版本的时候，我们可以通过 URL 来实现版本化，比如 http://api.example.com/v1、http://apiv1.example.com 。版本不必非要是数字，只是数字用的最多，日期、季节都可以作为版本标识符，项目团队达成共识就可。
接口尽量使用名词，避免使用动词。 RESTful API 操作（HTTP Method）的是资源（名词）而不是动作 (动词)

4. **统一接口** 
通过标准化 URL 和方法来访问资源。返回数据通常是 JSON 或 XML 格式。

例如,  提供班级信息的 API 设计，可以采用如下设计方法 : 
```python
GET    /classes：列出所有班级
POST   /classes：新建一个班级
GET    /classes/{classId}：获取某个指定班级的信息
PUT    /classes/{classId}：更新某个指定班级的信息（一般倾向整体更新）
PATCH  /classes/{classId}：更新某个指定班级的信息（一般倾向部分更新）
DELETE /classes/{classId}：删除某个班级
GET    /classes/{classId}/teachers：列出某个指定班级的所有老师的信息
GET    /classes/{classId}/students：列出某个指定班级的所有学生的信息
DELETE /classes/{classId}/teachers/{ID}：删除某个指定班级下的指定的老师的信息
```

**无状态（Stateless）**:每次请求都是独立的，服务器不会保存客户端的上下文状态。需要的所有信息（如身份认证信息）必须包含在每次请求中。



### (2) **RESTful API 的返回格式**

一个标准的 RESTful API 响应通常包含以下内容：

1. **状态码**  
使用 HTTP 状态码描述请求结果：
- `200 OK`：请求成功。
- `201 Created`：资源创建成功。
- `400 Bad Request`：请求有误。
- `403 Forbidden` : 禁止访问 
- `404 Not Found`：资源不存在。
- `500 Internal Server Error`：服务器错误。

具体如下 : 
![[attachments/Pasted image 20250112164623.png|700]]

2. **数据主体（Body）**  
返回资源的详细信息，通常是 JSON 格式。例如: 
```json
{
	"succeed": true,
	"data": {
		"id": 123,
		"name": "John Doe"
	},
	"message": "User retrieved successfully"
}
```
3. **错误信息** 
如果发生错误，返回明确的错误说明，例如：
```json
{
	"succeed": false,
	"error": {
		"code": 4001,
		"message": "Invalid username or password"
	}
}
```

### (3) Flask 的 RESTful Api **响应返回方法**

```sh
pip install flask-restful
```


对于成功的请求，可以返回如下: 
```json
{
    "succeed": true,
    "data": {
        "user_id": 123,
        "name": "John Doe",
        "email": "john.doe@example.com"
    },
    "message": "User data retrieved successfully"
}
```

示例代码：

```python
from flask import Flask, jsonify

app = Flask(__name__)

@app.route('/users/<int:user_id>', methods=['GET'])
def get_user(user_id):
    user = {
        "user_id": user_id,
        "name": "John Doe",
        "email": "john.doe@example.com"
    }
    return jsonify({
        "succeed": True,
        "data": user,
        "message": "User data retrieved successfully"
    }), 200
```

当<b><mark style="background: transparent; color: orange">请求失败时，可以返回错误信息</mark></b>:  

```json
{
    "succeed": false,
    "error": {
        "code": 404,
        "message": "User not found"
    }
}
```

示例代码：

```python
@app.route('/users/<int:user_id>', methods=['GET'])
def get_user(user_id):
    # 假设用户数据不存在
    if user_id != 123:
        return jsonify({
            "succeed": False,
            "error": {
                "code": 404,
                "message": "User not found"
            }
        }), 404
```

### **完整的 RESTful API 示例**

以下是一个完整的用户管理 API 的简单示例，支持用户的增删改查。

#### **代码实现**

```python
from flask import Flask, request, jsonify

app = Flask(__name__)

# 模拟数据库
users = {}

# 获取用户
@app.route('/users/<int:user_id>', methods=['GET'])
def get_user(user_id):
    if user_id in users:
        return jsonify({
            "succeed": True,
            "data": users[user_id],
            "message": "User retrieved successfully"
        }), 200
    else:
        return jsonify({
            "succeed": False,
            "error": {
                "code": 404,
                "message": "User not found"
            }
        }), 404

# 创建用户
@app.route('/users', methods=['POST'])
def create_user():
    data = request.json
    user_id = data.get("id")
    if not user_id or user_id in users:
        return jsonify({
            "succeed": False,
            "error": {
                "code": 400,
                "message": "Invalid or duplicate user ID"
            }
        }), 400
    users[user_id] = {
        "name": data.get("name"),
        "email": data.get("email")
    }
    return jsonify({
        "succeed": True,
        "data": users[user_id],
        "message": "User created successfully"
    }), 201

# 更新用户
@app.route('/users/<int:user_id>', methods=['PUT'])
def update_user(user_id):
    if user_id not in users:
        return jsonify({
            "succeed": False,
            "error": {
                "code": 404,
                "message": "User not found"
            }
        }), 404
    data = request.json
    users[user_id].update({
        "name": data.get("name"),
        "email": data.get("email")
    })
    return jsonify({
        "succeed": True,
        "data": users[user_id],
        "message": "User updated successfully"
    }), 200

# 删除用户
@app.route('/users/<int:user_id>', methods=['DELETE'])
def delete_user(user_id):
    if user_id in users:
        del users[user_id]
        return jsonify({
            "succeed": True,
            "message": "User deleted successfully"
        }), 200
    else:
        return jsonify({
            "succeed": False,
            "error": {
                "code": 404,
                "message": "User not found"
            }
        }), 404

if __name__ == '__main__':
    app.run(debug=True)
```



在使用 **Flask-RESTful** 的情况下，确实可以利用其内置功能简化响应的返回，避免直接使用 `jsonify` 每次手动构建返回格式。Flask-RESTful 提供了更简洁的工具来处理 JSON 响应，例如 `marshal_with` 和 `abort`，以及对返回格式的自动处理。

### **简化返回响应的方法**

#### **1. 使用 `Resource` 类和直接返回**

在 Flask-RESTful 中，`Resource` 类中的方法可以直接返回 Python 的字典和 HTTP 状态码，框架会自动将其转换为 JSON 响应。

**示例代码：**

```python
from flask import Flask
from flask_restful import Resource, Api

app = Flask(__name__)
api = Api(app)

# 模拟数据库
users = {
    1: {"name": "John Doe", "email": "john.doe@example.com"}
}

class User(Resource):
    def get(self, user_id):
        if user_id in users:
            return {"succeed": True, "data": users[user_id], "message": "User retrieved successfully"}, 200
        else:
            return {"succeed": False, "error": {"code": 404, "message": "User not found"}}, 404

    def delete(self, user_id):
        if user_id in users:
            del users[user_id]
            return {"succeed": True, "message": "User deleted successfully"}, 200
        else:
            return {"succeed": False, "error": {"code": 404, "message": "User not found"}}, 404

# 注册路由
api.add_resource(User, '/users/<int:user_id>')

if __name__ == '__main__':
    app.run(debug=True)
```

---

#### **2. 使用 `marshal_with` 简化返回格式**

Flask-RESTful 提供了 `marshal_with` 装饰器来定义响应的格式，避免每次手动构建 JSON。

**示例代码：**

```python
from flask import Flask
from flask_restful import Resource, Api, fields, marshal_with

app = Flask(__name__)
api = Api(app)

# 模拟数据库
users = {
    1: {"name": "John Doe", "email": "john.doe@example.com"}
}

# 定义返回格式
user_fields = {
    "name": fields.String,
    "email": fields.String
}

response_fields = {
    "succeed": fields.Boolean,
    "data": fields.Nested(user_fields),
    "message": fields.String
}

class User(Resource):
    @marshal_with(response_fields)
    def get(self, user_id):
        if user_id in users:
            return {"succeed": True, "data": users[user_id], "message": "User retrieved successfully"}, 200
        else:
            return {"succeed": False, "data": None, "message": "User not found"}, 404

# 注册路由
api.add_resource(User, '/users/<int:user_id>')

if __name__ == '__main__':
    app.run(debug=True)
```

---

#### **3. 使用 `abort` 简化错误响应**

`abort` 是 Flask-RESTful 提供的一个简单方法，用来快速返回错误响应，省去手动构建错误信息的步骤。

**示例代码：**

```python
from flask import Flask
from flask_restful import Resource, Api, abort

app = Flask(__name__)
api = Api(app)

# 模拟数据库
users = {
    1: {"name": "John Doe", "email": "john.doe@example.com"}
}

class User(Resource):
    def get(self, user_id):
        if user_id not in users:
            abort(404, message="User not found")
        return {"succeed": True, "data": users[user_id], "message": "User retrieved successfully"}, 200

# 注册路由
api.add_resource(User, '/users/<int:user_id>')

if __name__ == '__main__':
    app.run(debug=True)
```

- `abort(404, message="User not found")` 会自动返回：
    
    ```json
    {
        "message": "User not found"
    }
    ```
    

---

#### **4. 自定义响应格式的辅助函数**

如果需要统一的返回格式，可以定义一个辅助函数来简化返回逻辑。

**示例代码：**

```python
from flask import Flask
from flask_restful import Resource, Api

app = Flask(__name__)
api = Api(app)

users = {
    1: {"name": "John Doe", "email": "john.doe@example.com"}
}

def make_response(success, data=None, message=None, status=200):
    response = {"succeed": success}
    if data is not None:
        response["data"] = data
    if message:
        response["message"] = message
    return response, status

class User(Resource):
    def get(self, user_id):
        if user_id in users:
            return make_response(True, users[user_id], "User retrieved successfully", 200)
        return make_response(False, message="User not found", status=404)

    def delete(self, user_id):
        if user_id in users:
            del users[user_id]
            return make_response(True, message="User deleted successfully", status=200)
        return make_response(False, message="User not found", status=404)

api.add_resource(User, '/users/<int:user_id>')

if __name__ == '__main__':
    app.run(debug=True)
```

---

### **返回的示例**

1. **成功响应**:
    
    ```json
    {
        "succeed": true,
        "data": {
            "name": "John Doe",
            "email": "john.doe@example.com"
        },
        "message": "User retrieved successfully"
    }
    ```
    
2. **错误响应**:
    
    ```json
    {
        "succeed": false,
        "message": "User not found"
    }
    ```
    

---

### **总结**

- 使用 Flask-RESTful 的 `marshal_with` 或 `abort`，可以减少手动构建 JSON 响应的工作。
- 如果需要统一的返回格式，推荐定义一个辅助函数或封装到类中，像 `make_response`。
- 如果项目较复杂，可以使用专用库，如 `flask-marshmallow`，进一步增强返回格式化功能。