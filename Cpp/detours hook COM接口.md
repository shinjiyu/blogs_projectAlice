#detours hook COM接口
##detours使用教程
  [API Hooking with MS Detours](http://www.baidu.com/link?url=wsUTeMQMAyhXf_8iM8WXMtbboYXomjBhjPOI3OGozz0bandnj8A-iFunKMwP0VQFfly3EdgZUP9JkQZlg1HkX_9ClB_BgSonrkJQ5L9XrdO&wd=&eqid=c208ddde000ef9970000000658b9a430)
##COM接口简介
* Com对象拥有很单纯内存结构，就一个虚表指针。 在这个意义看，比C++那些多继承、虚拟继承的妖艳贱货高不知道哪里去了。
* Com接口的函数地址可以通过Com对象实例取到。假如```pIComPtr ```为对象指针,```(LPVOID)*((DWORD*)pIComPtr+n)```则是第```n```个函数地址,```n```从```0```起。
* 前三个函数属于```IUnkown```接口。

##函数签名
* 所以COM API的调用方式都是 ```__stdcall```(然而我不确定)
* 参数增加一个this指针
* ```IComPtr```上一个名为```Test(DWORD,DWORD)```的接口其签名为
```typedef HRESULT (__stdcall *pTestType)(IComPtr* This,DWORD,DWORD)```

##细节问题1

```DetourAttach``` 第一个参数是in out型的，接收一个存有函数地址的内存的地址，这里需要复制虚表中的值到栈中，再调用```DetourAttach```，否则虚表会被修改。

##细节问题2 怎么取得接口在虚表中的位置

  * 去头文件里数
  
  * 去汇编中看跳转前add了多少偏移
  
  * 用奇技淫巧通过函数名获取，具体实现
  
  [Detect the the vtable index/ordinal of a specific virtual function (using Visual C++)](http://stackoverflow.com/questions/5635212/detect-the-the-vtable-index-ordinal-of-a-specific-virtual-function-using-visual)
