
实际存储是在 /var/hellofriedparrot.club_nginx 文件夹下, 只需上传到 ubuntu 下, 然后替换证书文件
```sh
sudo cp -r /home/ubuntu/hellofriedparrot.club_nginx /var/
rm -r /home/ubuntu/hellofriedparrot.club_nginx 
```

```sh
sudo nginx -t
sudo nginx -s reload
```
