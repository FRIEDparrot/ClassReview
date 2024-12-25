触发器可以在一个操作之前或者之后检查插入的部分是否符合逻辑或者检查范围要求  

需要说明:  在 MySQL 中，触发器的作用域是 **每行**（`FOR EACH ROW`），它并不会遍历整个表的所有行，而是仅针对当前正在插入或更新的行（即 "新数据" 和 "旧数据"）。这意味着触发器天然就是基于 "新的插入或更新数据" 的。并使用  NEW 进行检查新的数据是否符合要求。

触发器**可以通过以下特殊关键字访问插入或更新时的数据**：
- **`NEW`**：表示**当前触发操作中的新数据（即插入的新值或更新后的新值）**。
- **`OLD`**：表示**更新或删除操作中，行原始的数据。**

```sql 
DELIMITER $$

CREATE TRIGGER check_valid_cloud_space_insert
BEFORE INSERT ON `user-info`
FOR EACH ROW
BEGIN
  -- cloud space should less than 8192 MB and greater than zero 
  IF NOT (NEW.cloud_space BETWEEN 0 AND 8192) THEN
    SIGNAL SQLSTATE '45000' -- create the failed message 
    SET MESSAGE_TEXT = "the cloud space should between 0  and 8192";
  END IF;
END;

$$
```
