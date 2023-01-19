### Use the html method to create the table:

<table>
    <tr>
        <td>Foo</td> <td>Fo</td>
    </tr>
</table>

there are two characters that demand special treatment: `<` and `&`. 
Left angle brackets are used to start tags; ampersands are used to denote HTML entities.

If you want to use them as literal characters, you must escape them as entities, e.g. `&lt;`, and `&amp;`

Ampersands in particular are bedeviling for web writers. If you want to write about ‘AT&T’, you need to write ‘`AT&amp;T`’. You even need to escape ampersands within URLs. Thus, if you want to link to:

```
http://images.google.com/images?num=30&q=larry+bird
```

you need to encode the URL as:

```
http://images.google.com/images?num=30&amp;q=larry+bird
```

The only restrictions are that block-level HTML elements — e.g. `<div>`, `<table>`, `<pre>`, `<p>`

So, if you want to include a copyright symbol in your article, you can write:

```
&copy;
```

and Markdown will leave it alone. But if you write:

```
AT&T
```

Markdown will translate it to:

```
AT&amp;T
```

* * *


 
 