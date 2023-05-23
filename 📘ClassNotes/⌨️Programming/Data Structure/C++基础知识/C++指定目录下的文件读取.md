
```cpp 
#include <io.h>
// 读取path目录下的所有文件名,并存储在vector向量中
vector<string> listdir(string path) {
    intptr_t hFile = 0; 
    struct _finddata_t fileinfo;
    string p;
    vector<string> files;
    const char* name = p.assign(path).append("/*").c_str();
    if ((hFile = _findfirst64i32(name, &fileinfo)) != -1)
    {
        do {
            string filename = path + "/" + string(fileinfo.name);
            if (strcmp( fileinfo.name, ".") && *fileinfo.name!= '.' && *fileinfo.name!= '..') {
                // 比较保证fileinfo中不含.
                files.push_back(filename);
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    };
    return files;
}
```

文件写入操作
```cpp 
#include <fstream>
std::ofstream File("data.txt");
    if (!File) {
        cout << "error when opening the file" << endl;
        exit(0);
    }
    else {
        File << "ZONE I=" << nodeNum << ",F=POINT" << endl;
        for (int i = 0; i < U.size(); i++) File << dt * i << "," << U[i] << endl;
        File.close();
    }
```

文件读取操作
```cpp 
// 读取文件至矩阵中
void Sparse_Matrix::read_file(string filename) {
    ifstream ifs;
    ifs.open(filename, ios::in);
    if (!ifs.is_open()) {
        cout << "failed when opening the file" << endl;
        return;
    }
    string word;
    while (ifs >> word) {
        cout << word << " ";
    }
    ifs.close();
}
```
