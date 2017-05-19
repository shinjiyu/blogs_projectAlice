 # cocosstudio生成的lua布局文件异步加载

 ## 问题背景
 * 同事编辑了一个含有8000个node左右的页面。
 * 暂时没有精力去做优化，只是先保证加载时候不卡死。


 ## 思路
 *  将布局包装成一个协程，然后动态地在里面插入yield实现异步加载。
 *  初步设计是在每一个的addChild完成的时候插入yield。


 ## 操作
* ### 不修改原来的代码，实现对cc.Node.addChild的装饰
``` lua
  --HOOK ADDCHILD来实现自动对生成大布局的代码插入yield
  --aimFunc想要hook的函数
  --hookFunc 插入的函数
  --step 是个频率控制，有时候不是每一个都想回调，而是每隔几个回调一下
  cc.exports.Hook = function(obj,aimFuncName,hookFunc,step)
  	local temp_old_func  = obj[aimFuncName]
  	local index = 0
  	obj[aimFuncName] = function(...)
  		local res ={temp_old_func(...)}
  		index = index+1
  		if index == step then
  			hookFunc(...)
  			index = 0
  		end
  		return unpack(res)
    end

    return temp_old_func

  end
```
* ### 封装cocoutine 在每次resume之前对addChild进行装饰，resume返回后恢复，已免其它代码进入装饰代码。
  ```
      local old_setName = Hook(cc.Node,"addChild",function(...)

					coroutine.yield()

			end,process_node_count_per_frame)
      --do resume
      Unhook(cc.Node,"addChild",old_setName)

      ```


 ## 问题

  * ### cocos的垃圾回收
  一个node在创建的那个消息循环里面如果没有显示retain 或者 被addTo 父节点，就会被回收。
  所以异步执行创建过程会导致大量节点被回收。

 ## 解决

  修改思路是对所有cocoutine中运行的创建多执行一次retain,并记录涉及的节点为，在 cocoutine完成以后统一release
  cocos不同ui对象使用的create()方法各不相同，而根据cocosstudio生成布局文件的规则，会发现所有node创建后都会调用setName，
  所以对setName进行装鉓即可。

  ```
  ocal old_setName = Hook(cc.Node,"setName",function(a,...)
		a:retain()
		table.insert(node_table,#node_table+1,a)
		--print("set Name count:"..tostring(#node_table))
		acts =  acts+1
		if self.AnsyFlag and acts > yield_perActs then
			print("yield")
			coroutine.yield()
			acts = 0
		end
	end,process_node_count_per_frame)
  ```
