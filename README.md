## 生成外部排序的elf文件
```
rm -rf build
mkdir build
cd $_
cmake ..
make
```

此时bin目录下的external_sort就是可执行文件（需要自己生成一个input.txt）

## 生成测试用的input.txt
```
cd test
rm -rf build
mkdir build
cd $_
cmake ..
make
```
此时当前目录下就有两个elf文件（generate负责生成测试的input.txt；io_test负责验证最终的结果是否正确）