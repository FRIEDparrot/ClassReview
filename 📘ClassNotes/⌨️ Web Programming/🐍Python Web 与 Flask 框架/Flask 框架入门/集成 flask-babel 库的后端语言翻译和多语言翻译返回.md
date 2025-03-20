为了前端更好的用户体验, 我们后端在返回信息时, 需要根据后端支持语言返回不同信息: 
```sh
pip install flask-babel
```

首先假设 `supported_lang= ['en', 'zh']`, 则有:
```python 
app.config["BABEL_DEFAULT_LOCALE"] = 'en'  
app.config["BABEL_SUPPORTED_LOCALES"] = supported_lang  
app.config['BABEL_TRANSLATION_DIRECTORIES'] = 'translations'  
app.config['BABEL_DEFAULT_TIMEZONE'] = 'UTC'

# Initialize babel for translation  
babel = Babel(app)  

def get_locale():  
    """  
    we use from flask_babel import gettext as _t for translation    :return:  
    """    user = getattr(g, 'user', None)  # if user logged in, then get its prefer settings  
    if user is not None:  
        return user.lang if user.lang in supported_lang else 'en'  
    return request.args.get("lang") or request.accept_languages.best_match(supported_lang)  
  
  
def get_timezone():  
    user = getattr(g, 'user', None)  
    if user is not None:  
        return user.timezone  

# Initialize babel for translation  
babel = Babel(app, locale_selector=get_locale, timezone_selector=get_timezone)
```

此时在任何蓝图中, 采用 
```python 
from flask_babel import gettext as _t 
```

翻译文件存储路径如下: 
![[attachments/Pasted image 20250121191538.png]]

由于.po 文件不会被识别, 需要通过如下进行编译:
```sh
pybabel compile -d translations
```

即可得到如下的语言响应: 
![[attachments/Pasted image 20250121170151.png]]

另外语言 .po 文件可以通过 [poedit](https://poedit.net/download) 方便编辑;
