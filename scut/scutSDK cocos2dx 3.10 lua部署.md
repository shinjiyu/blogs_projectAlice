#scutSDK cocos2dx 3.10 lua部署
##下载SDK
github地址：[https://github.com/ScutGame/Scut](https://github.com/ScutGame/Scut)
##代码物理部署
* SDK/ScutSDK  目录考贝到项目工程路径 frameworks\runtime-src\ 下
	*SDK自带文档建议考贝到 frameworks\ 下，并不建议这样做。
* res 下新建ScutRes 目录，用于存放协议相关lua ,新建两个文件[NetHelper.lua]() [ScutManager.lua]()

##win32编译（IDE VS2015)
* 导入ScutSDK工程 frameworks\runtime-src\ScutSDK\proj.win32\ScutSDK.vcxproj
* 由于文件的include路径是基于旧版本 cocos2dx,所以这里有大量的includepath配置需要修改，建议直接把libcocos2d工程的配置全部抄过来。
* 全局查找curl.h  easy.h的include预编译宏，替换成<curl/curl.h>和<curl/easy.h>
* 太具体的记不清了，总之就是再根据一些编译器错误提示修改就能完成ScutSDK工程的编译
* 游戏工程 include路径中增加 $(ProjectDir)..\ScutSDK 
* 游戏工程lib库增加  ScutSDK.lib
* appdelegate.cpp 增加头文件 #include "ScutExt.h"
* appdelegate.cpp 增加函数 
	```c++
static void initLuaGlobalVariables(const std::string& entry)
{
	auto pEngine = LuaEngine::getInstance();
	LuaStack* pStack = pEngine->getLuaStack();
	FileUtils* pFileUtils = FileUtils::getInstance();
	using namespace std; 
	string path = pFileUtils->fullPathForFilename(entry.c_str());
	int pos = string::npos;
	while ((pos = path.find_first_of("\\")) != string::npos)
	{
		path.replace(pos, 1, "/");
	}
	string script_dir = path.substr(0, path.find_last_of("/"));
	string root_dir = script_dir.substr(0, script_dir.find_last_of("/"));
	string env = "GLOBAL_ROOT_DIR=\""; env.append(root_dir); env.append("\"");
	pEngine->executeString(env.c_str());
	env = "__LUA_STARTUP_FILE__=\""; env.append(path); env.append("\"");
	pEngine->executeString(env.c_str());

	pStack->addSearchPath(script_dir.c_str());
	pFileUtils->addSearchPath(root_dir.c_str());
	pFileUtils->addSearchPath(script_dir.c_str());

	ScutExt::Init(root_dir + "/");
}
```
* 增加调用
	
	```c++
initLuaGlobalVariables("src/main.lua");
if (engine->executeScriptFile("src/main.lua"))
{
	return;
}
```

#android编译
* 修改frameworks\runtime-src\ScutSDK\proj.android\Android.mk 把LOCAL_C_INCLUDES 中 cocos 相关路径全部增加一次..

* 修改frameworks\runtime-src\proj.android\jni\Android.mk 增加

	```LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../ScutSDK```

	```LOCAL_STATIC_LIBRARIES += scutsdk_static```

	```$(call import-module,../runtime-src/ScutSDK\proj.android)```到相应位置

* 修改frameworks\cocos2d-x\cocos\Android.mk 打开curl的开关
	```$(call import-module,curl/prebuilt/android)```
