# pict-js
微软PICT的node-js绑定，并扩展了约束条件的使用功能

## 复制安装
- 从lib下复制 pict_js-native(Linux64).node 或 pict_js-name(Win64).node
- 重命名为 pict_js-native.node

## 源码安装
```
git clone https://github.com/solidest/pict-js.git
cd pict-js
git clone https://github.com/microsoft/pict.git ms_pict
npm install
```

## 重新编译
```
node-gyp rebuild
```

## 执行测试
```
node test.js
```
