## 1. JWT 基本概念 
JWT（JSON Web Token）**的一个重要特点**就是它通常 **不需要在后端服务器存储**。JWT 本质上是一个 **自包含的** 令牌，其中包含了足够的用户信息（例如用户 ID、权限、过期时间等），以及签名信息来验证其合法性。因此，只要你在生成和验证时使用了相同的密钥，后端无需保存任何 token 状态，即**无状态认证**。

在后端生成 token 的过程中，通常会使用 JWT（JSON Web Token）来进行身份验证。JWT 是一种轻量级的认证机制，它由三部分组成：头部（Header）、有效载荷（Payload）、签名（Signature）。整个过程的主要步骤如下：

### 1. 用户登录

用户在前端输入用户名和密码后，后端会验证这些信息是否正确。如果验证通过，后端就会生成一个 token 并返回给前端。这个 token 用于后续请求的身份验证。

### 2. 生成 Token

JWT token 的生成过程如下：
- **头部（Header）**: 通常包含签名的算法类型（例如 HMAC SHA256 或 RSA）。
- **有效载荷（Payload）**: 包含声明（claims），比如用户的 ID、角色、过期时间等。这些信息可以是公开的（非敏感数据），但是需要保证不被篡改。
- **签名（Signature）**: 用来验证 token 是否被篡改，并确保它是由服务器签发的。签名是通过使用头部中指定的算法（例如 HMAC SHA256）与密钥（通常是服务器的私密密钥）一起对头部和有效载荷进行签名。

JWT token 生成的过程大致是：

1. 将头部和有效载荷编码成 base64Url 格式。
2. 将编码后的头部和有效载荷使用签名算法（例如 HMAC SHA256）进行签名。
3. 将编码后的头部、有效载荷和签名拼接成一个完整的 JWT。

### 3. 用户请求带上 Token

前端在后续的请求中，将 token 添加到 HTTP 请求的头部（通常是 `Authorization` 头）。后端可以从请求中提取 token，然后进行验证。

### 4. 验证 Token

后端接收到请求后，会从请求头部提取 token，验证它的有效性。验证步骤包括：

- 检查 token 是否存在。
- 使用相同的签名算法和密钥来验证签名是否有效。
- 验证 token 是否过期（根据有效载荷中的过期时间 `exp` 字段）。

### Flask 中生成 Token
在 Flask 中，通常使用 **PyJWT** 库来生成和验证 JWT token。PyJWT 是一个 Python 库，它提供了简单的接口来生成和解码 JWT。

你可以通过以下命令来安装 PyJWT：
```bash
pip install pyjwt
```

#### 生成 Token 的示例

```python
import jwt
import datetime

# 密钥，建议使用一个复杂的私密字符串
SECRET_KEY = 'your-secret-key'

def generate_token(user_id):
    payload = {
        'user_id': user_id,
        'exp': datetime.datetime.utcnow() + datetime.timedelta(hours=1)  # 设置过期时间
    }
    token = jwt.encode(payload, SECRET_KEY, algorithm='HS256')
    return token
```

#### 验证 Token 的示例

```python
def verify_token(token):
    try:
        # 解码 token，如果验证成功，返回有效载荷
        payload = jwt.decode(token, SECRET_KEY, algorithms=['HS256'])
        return payload
    except jwt.ExpiredSignatureError:
        return "Token has expired"
    except jwt.InvalidTokenError:
        return "Invalid token"
```

通常情况下，**每次用户登录时都会生成新的 token**，而不是每次请求都生成新的 token。因为 token 中包含了过期时间，生成一个新的 token 也就意味着刷新了过期时间。前端在之后的请求中只需要携带这个 token，直到 token 过期。过期后，用户需要重新登录或刷新 token。

## 二、基于 RS256 的密钥加密

如果你需要更高的安全性，通常会考虑使用非对称加密算法，例如：

- **RS256 (RSA SHA-256)**：这是一种基于 RSA 非对称加密的算法，使用一对公钥和私钥来生成和验证签名。RSA 的优势是<b><mark style="background: transparent; color: orange">即使攻击者得到了公钥，也无法伪造签名，因此它提供了更高的安全性，但也相对较慢</mark></b>，特别是在处理大规模数据时。

- **ES256 (ECDSA with SHA-256)**：基于椭圆曲线的算法（Elliptic Curve Digital Signature Algorithm），这也是一种非对称加密算法，相比于 RSA，ECDSA 在相同安全强度下使用更短的密钥，效率较高，因此适用于性能要求更高的场景。

一般可以选用 2048 位的私钥进行加密 


如果你的 RSA 私钥已保存到文件中，你可以通过加载这个私钥并从中生成相应的公钥。以下是如何实现这一点的示例代码。

假设你的私钥存储在名为 `private_key.pem` 的文件中，下面的代码将读取私钥并生成公钥：

```python
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.backends import default_backend

# 从PEM文件加载私钥
def load_private_key(file_path):
    with open(file_path, "rb") as key_file:
        private_key = serialization.load_pem_private_key(
            key_file.read(),
            password=None,  # 如果私钥文件有密码，需提供密码
            backend=default_backend()
        )
    return private_key

# 从私钥生成公钥
def get_public_key(private_key):
    return private_key.public_key()

# 导出公钥为PEM格式
def export_public_key_to_pem(public_key):
    return public_key.public_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PublicFormat.SubjectPublicKeyInfo
    )

# 示例使用
private_key_path = 'private_key.pem'  # 指定私钥文件路径
private_key = load_private_key(private_key_path)

# 生成公钥
public_key = get_public_key(private_key)

# 导出公钥为PEM格式并打印
pem_public_key = export_public_key_to_pem(public_key)
print("生成的公钥:\n", pem_public_key.decode('utf-8'))
```

### 代码解析

1. **加载私钥**：
   - `load_private_key()` 函数从指定的 PEM 文件中读取私钥，使用 `serialization.load_pem_private_key()` 方法。

2. **获取公钥**：
   - `get_public_key()` 函数接受一个私钥对象并返回对应的公钥。

3. **导出公钥为 PEM 格式**：
   - `export_public_key_to_pem()` 函数将公钥转换为 PEM 格式字符串，以便于存储和查看。

4. **示例使用**：
   - 在示例代码中，私钥从 `private_key.pem` 文件中加载，接着生成公钥并将其导出为 PEM 格式，最后打印公钥。


- 确保你的私钥文件是安全和可读的。如果你的私钥文件有密码保护，需要在 `load_pem_private_key()` 中提供合适的密码。
- 你可以将生成的公钥保存到文件中或打印出来，以供后续使用。


### JWT 验证过程
使用 RSA 进行 JWT（JSON Web Token）验证时，需要同时使用 JWT 令牌和相应的公钥。以下是详细的解释:  

1. **JWT 令牌**：JWT 是一种用于安全传输信息的令牌，它通常由三部分组成——头部（Header）、有效载荷（Payload）和签名（Signature）。在使用 RSA 算法生成的 JWT 中，签名是使用私钥生成的。
2. **公钥的作用**：在验证 JWT 时，你需要用相应的公钥来验证签名。公钥是通过 RSA 私钥生成的，确保只有持有相应私钥的主体能够生成有效的 JWT，而任何人都可以使用公钥来验证 JWT 的有效性。

### 验证步骤
- 在收到一个 JWT 时，你会通过以下步骤来验证它：
  1. **解析 JWT**：将 JWT 分成三部分：头部、有效载荷和签名。
  2. **生成签名**：使用同样的算法和公钥对头部和有效载荷进行签名。
  3. **比较签名**：将生成的签名与原始 JWT 中的签名进行比较。如果它们相同，则 JWT 是有效的，且未被篡改。

### 示例伪代码

以下是一个简单的验证示例：

```python
import jwt

# 假设我们有生成的token和public_key
token = "eyJhbGciOiJSUzI1NiIsInR..."
public_key = <从私钥生成的公钥>

# 验证JWT
try:
    # 使用公钥解码JWT令牌
    decoded_payload = jwt.decode(token, public_key, algorithms=["RS256"])
    print("JWT有效，解码的有效载荷:", decoded_payload)
except jwt.ExpiredSignatureError:
    print("该JWT已过期")
except jwt.InvalidTokenError:
    print("无效的JWT")
```

### 总结

- **需要**：使用 JWT 进行验证时，需要有 **令牌** 和 **公钥**。
- **安全性**：此方法确保只有持有私钥的实体能够生成有效的 JWT，而所有可以访问公钥的人都可以验证其有效性。

### 1. **密钥轮换的常见做法** 
存储方法:  **可以将公钥存储在数据库中**，并定期更换它。实际上，这种方法是非常常见的，特别是在具有动态密钥管理和轮换需求的分布式系统中。 

在密钥轮换过程中，**不一定必须更新私钥**，但通常会更新私钥以确保安全性。然而，如果 **仅更新公钥**，而私钥保持不变，用户 **仍然可以继续登录**，只要公钥和私钥是配对的，签名验证就能正确进行。

密钥轮换一般是指 **公钥和私钥** 的更新过程，但有两种不同的处理方式：

- **同时更新公钥和私钥**：这种做法在密钥的有效期过长或担心私钥泄露的情况下进行。在这种情况下，**私钥** 和 **公钥** 都会更新。
- **只更新公钥，私钥保持不变**：这种方式较为常见，尤其是为了避免对现有用户的影响。在公钥更新时，私钥保持不变，这样就不会影响现有的签名验证过程。

使用公钥和 `kid`（Key ID）来管理公钥的轮换是一个有效且简洁的策略。以下是一些关键点和实现建议，帮助你更好地理解和实施这种方法：
### 1. 使用 `kid` 管理公钥

- **`kid` 的作用**：`kid` 是 JWT 头部中的一个字段，用于标识使用的公钥。每次生成新的公钥时，可以简单地递增 `kid` 的值。这使得验证方可以轻松识别和使用正确的公钥来验证 JWT。
  
- **文件存储**：将公钥存储在服务器端的文件中，文件可以是 JSON 格式，包含所有有效的公钥及其相关信息（如 `kid`、创建时间、过期时间等）。这样，你可以轻松读取和更新公钥。

### 2. 轮换和验证流程
#### a. 生成和存储公钥
- 每次生成新的公钥时，递增 `kid` 并将新公钥存储到文件中。可以使用简单的文件读写操作来管理公钥。

#### b. 验证 JWT

- 在验证 JWT 时，首先从 JWT 的头部获取 `kid`，然后根据 `kid` 从文件中加载相应的公钥进行验证。这样可以确保即使在公钥轮换期间，仍然可以使用有效的公钥进行验证。

### 3. 示例代码

以下是一个简单的示例，演示如何使用公钥和 `kid` 来管理公钥轮换：

```python
import json
from datetime import datetime, timedelta
from cryptography.hazmat.primitives.asymmetric import rsa
import jwt

# 公钥存储文件路径
KEY_STORAGE_FILE = 'keys.json'

# 生成新的 RSA 密钥对
def generate_key_pair():
    private_key = rsa.generate_private_key(public_exponent=65537, key_size=2048)
    public_key = private_key.public_key()
    return private_key, public_key

# 存储公钥
def store_public_key(public_key, kid):
    key_data = {
        "kid": kid,
        "public_key": public_key.public_bytes(
            encoding=serialization.Encoding.PEM,
            format=serialization.PublicFormat.SubjectPublicKeyInfo
        ).decode('utf-8'),
        "created_at": datetime.utcnow().isoformat(),
        "expires_at": (datetime.utcnow() + timedelta(days=30)).isoformat()  # 有效期30天
    }
    
    # 读取现有公钥
    try:
        with open(KEY_STORAGE_FILE, 'r') as f:
            keys = json.load(f)
    except FileNotFoundError:
        keys = []

    # 添加新公钥
    keys.append(key_data)

    # 写入文件
    with open(KEY_STORAGE_FILE, 'w') as f:
        json.dump(keys, f)

# 验证JWT
def verify_token(token):
    # 解析JWT头部以获取kid
    headers = jwt.get_unverified_header(token)
    kid = headers['kid']
    
    # 读取公钥
    with open(KEY_STORAGE_FILE, 'r') as f:
        keys = json.load(f)
    
    # 查找对应的公钥
    for key in keys:
        if key['kid'] == kid:
            public_key = serialization.load_pem_public_key(key['public_key'].encode('utf-8'))
            try:
                decoded = jwt.decode(token, public_key, algorithms=["RS256"])
                return decoded
            except jwt.InvalidTokenError:
                continue
    
    raise Exception("无效的JWT，未找到有效公钥。")

# 示例使用
private_key, public_key = generate_key_pair()
kid = 1  # 初始 kid
store_public_key(public_key, kid)

# 生成JWT
token = jwt.encode({"some": "data"}, private_key, algorithm="RS256", headers={"kid": kid})

# 验证JWT
decoded = verify_token(token)
print("解码结果:", decoded)
```

### 4. 优势

- **简化管理**：通过将所有公钥存储在一个文件中，且使用 `kid` 进行标识，可以简化公钥的管理和轮换。
- **有效性**：在 JWT 的有效期内，系统可以使用相应的公钥进行验证，确保安全性和灵活性。

### 5. 注意事项

- **文件安全**：确保存储公钥的文件具有适当的访问权限，以防止未授权访问。
- **过期处理**：在验证时，可以检查公钥的 `expires_at` 字段，以确保公钥仍然有效。
- **备份和恢复**：定期备份公钥文件，以防数据丢失。


