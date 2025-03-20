在 SQLAlchemy 中，`session.get()` 和 `session.query()` 都可以用于通过主键查询数据库记录，但它们在性能和使用场景上有一些区别。

### 1. **`session.get()`**
   - **用途**: `session.get()` 是专门用于通过主键查询单个记录的快捷方法。
   - **性能**: 由于 `session.get()` 是专门为通过主键查询设计的，它通常会比 `session.query()` 更高效。它会直接利用 SQLAlchemy 的缓存机制（即 Identity Map），如果对象已经在会话缓存中，它会直接返回缓存的对象，而不需要再次查询数据库。
   - **适用场景**: 当你明确知道要通过主键查询单个记录时，`session.get()` 是首选方法。

   **示例**:
   ```python
   user = session.get(User, user_id)
   ```

### 2. **`session.query()`**
   - **用途**: `session.query()` 是一个更通用的查询方法，可以用于复杂的查询，包括通过主键查询。
   - **性能**: 当通过主键查询时，`session.query()` 的性能通常与 `session.get()` 相当，但它可能会稍微慢一些，因为它需要构建一个查询对象并执行查询。此外，`session.query()` 不会像 `session.get()` 那样直接利用缓存机制。
   - **适用场景**: 当你需要执行复杂的查询（如过滤、排序、连接等）时，`session.query()` 是更好的选择。

   **示例**:
   ```python
   user = session.query(User).get(user_id)
   ```

### 3. **性能对比**
   - **缓存机制**: `session.get()` 会优先检查会话缓存（Identity Map），如果对象已经在缓存中，它会直接返回缓存的对象，而不需要查询数据库。而 `session.query()` 不会优先检查缓存，除非你显式地使用 `query.options(contains_eager(...))` 或其他缓存相关的选项。
   - **查询生成**: `session.get()` 生成的 SQL 查询通常更简单，因为它只针对主键查询。而 `session.query()` 生成的查询可能会稍微复杂一些，尤其是在你添加了额外的过滤条件或连接时。
   - **数据库交互**: 如果对象已经在会话缓存中，`session.get()` 不会与数据库交互，而 `session.query()` 仍然会生成并执行 SQL 查询。

### 4. **总结**
   - **性能**: 在通过主键查询时，`session.get()` 通常比 `session.query()` 更高效，尤其是在对象已经在会话缓存中的情况下。
   - **使用场景**:
     - 如果你只需要通过主键查询单个记录，使用 `session.get()`。
     - 如果你需要执行复杂的查询（如过滤、排序、连接等），使用 `session.query()`。

### 5. **示例对比**
   ```python
   # 使用 session.get()
   user = session.get(User, user_id)

   # 使用 session.query()
   user = session.query(User).get(user_id)
   ```

   在这个简单的例子中，`session.get()` 是更简洁和高效的选择。而如果你需要更复杂的查询，`session.query()` 是更好的工具。

### 6. **建议**
   - **优先使用 `session.get()`**: 当你只需要通过主键查询单个记录时，优先使用 `session.get()`，因为它更简洁且性能更好。
   - **使用 `session.query()` 处理复杂查询**: 当你需要执行复杂的查询时，使用 `session.query()`，因为它提供了更多的灵活性和功能。




