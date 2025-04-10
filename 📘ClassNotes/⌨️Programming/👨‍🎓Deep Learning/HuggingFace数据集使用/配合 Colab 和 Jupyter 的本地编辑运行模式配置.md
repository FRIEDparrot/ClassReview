
采用连接到本地运行时的方案， 让 Colab 前端连接本地 Jupyter 内核，完全使用本地环境  ：
```sh
pip install jupyter_http_over_ws
jupyter server extension enable --py jupyter_http_over_ws
```

启动本地 jupyter， 同时允许 colab 连接  
```sh 
jupyter notebook --NotebookApp.allow_origin='https://colab.research.google.com' --port=8888 
```

或者 :  
```sh
jupyter notebook  --NotebookApp.allow_origin='https://colab.research.google.com'  --port=8888 --NotebookApp.port_retries=0
```

选择连接到本地运行时 : 
![[attachments/Pasted image 20250408204759.png|350]]

需要说明，  jupyter note book 需要 token 作为连接， 因此连接时的 url 需要携带对应 token 如下 :  
![[attachments/Pasted image 20250408205418.png]] 

稍加刷新即可得到对应的文件 : 
![[attachments/Pasted image 20250408205723.png|550]]

Colab 的运行时环境是一个独立的云端虚拟机，默认无法直接读取本地文件。因此必须先上传本地文件运行。  
