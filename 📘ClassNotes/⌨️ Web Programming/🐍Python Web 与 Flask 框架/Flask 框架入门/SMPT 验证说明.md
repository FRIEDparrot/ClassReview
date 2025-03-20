在实际业务中，通常**不会使用 SMTP 验证**，而是**仅使用格式检查 + MX 记录检查**。这是因为 SMTP 验证的**弊端较多**，而 MX 记录检查已经能满足大部分需求。以下是具体分析：

---

## **SMTP 验证的问题**

1. **速度慢（影响用户体验）**
    
    - 你当前代码 `timeout=300`（即 5 分钟超时），但即使是 10-30 秒的 SMTP 连接也会明显拖慢业务流程，尤其是前端注册、登录等场景。
    - 现代的邮件服务器通常有**反垃圾邮件机制**，可能会延迟响应，导致验证时间更长。
2. **可能导致被邮件服务器封禁**
    
    - 许多邮件服务器（如 Gmail、Outlook、Yahoo）会限制未认证的 SMTP 连接，甚至**主动封禁**来自高频验证的 IP，认为它是**垃圾邮件行为**。
    - 如果你的服务器**短时间内发送大量 SMTP 请求**，很可能被标记为**恶意爬虫**，进而被加入黑名单。
3. **部分服务器不支持 VRFY 命令**
    
    - 很多 SMTP 服务器**不允许直接验证邮箱地址**（即 `RCPT TO` 命令），因此 SMTP 验证可能会出现大量**误判**。
    - 例如，Gmail **总是返回 250 OK**，即使邮箱不存在，所以 SMTP 检测 Gmail 地址通常是**无效的**。
4. **多种错误导致误判**
    
    - 某些邮件服务器配置了**临时错误（4xx）**，可能会导致 SMTP 连接失败，而实际邮箱仍然可用。
    - 例如，某些公司内网邮件服务器**限制外部 SMTP 连接**，但邮箱本身是有效的。

---

## **推荐方案**

### ✅ **方式 1：仅使用格式检查 + MX 记录检查（推荐）**

```python
def check_valid_email_addr(email) -> int:
    """
    Use format checking and MX checking to check if the email is valid

    :param email:
    :return:
            - ``0`` : succeed
            - ``-1`` : "email format is invalid"
            - ``-2`` : "no MX record in the address, the domain is invalid"
    """
    if not check_format_email(email):
        return -1
    try:
        domain = email.split('@')[1]
        resolver = dns.resolver.Resolver()
        mx_records = resolver.resolve(domain, 'MX')
        if len(mx_records) == 0:
            return -2
    except (dns.resolver.NoAnswer, dns.resolver.NXDOMAIN, dns.exception.DNSException):
        return -2
    return 0
```

**优点**

- **速度快**（几毫秒级别）
- **不会触发邮件服务器封禁**
- **足够准确**（99% 以上误输入的邮箱会被过滤）

**适用场景**

- **用户注册、登录、找回密码**等涉及用户体验的业务。

---

### ✅ **方式 2：补充 SMTP 验证（可选，仅在后台异步使用）**

如果你仍然需要 SMTP 验证（如批量清理无效邮箱），建议：

- **在后台**（如 Celery 任务队列）**异步执行 SMTP 验证**，避免影响用户体验。
- **使用邮件 API**（如 SendGrid、Mailgun）来检测，而不是直接发 SMTP 连接。

---

## **结论**

**不推荐 `use_smtp=True` 在用户交互流程中使用**。  
**推荐** 仅使用 **格式检查 + MX 记录** 来判断邮箱是否存在（方式 1）。  
如果确实需要 SMTP 方式，应**在后台异步运行**（方式 2）。