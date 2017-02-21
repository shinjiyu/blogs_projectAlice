#龙骨的flash插件分析
##背景介绍
我们项目是使用 spine来制做骨骼动画，然后在flash中补画动作特效（光影，爆炸这之类的），然后导出dragonbone的动画格式使用。
因为龙骨插件要考虑通用性，对flash元件的组织有一定要求 ，这对于特效动画来说可能是额外负担
另外特效有比较高的复用要求，目前插件无法导出多个动画组成的timeline.
所以考虑研究一下龙骨插件的原理，看能不能进行一些定制修改
##龙骨插件源码部署
###IDE
FLASH BUILDER 4.7，虽然README里面说只要是4.0以上版本即可，但我试过4.0版本会因为打包的flash player库版本过低而出很多问题.

###工程
下载相关代码后 把所有库的lib scr local都考贝到DesignPanel 项目的路径下，在此路径创建工程。注意其中 dragonboneas库要使用3.0版本

###调试
打开FLASH RPO,打开插件面板 (这很重要).  然后在flash builder中启动调试就可以了

##插件原理
 * as代码通过调用jsfl代码访问flash中的对象.
 * armature ,bone ,slot这些都与图元对
 * atlastexture的生成原理，用jsfl取所有使用到的图元组成一个swf文件保存,在as中读取swf进行布局后生成atlas.png
 
##TODO

* 更优的合图算法,考虑增加切图的顶点数，不用正方形
* 增加对flash中 element的识别，而不再基于item进行转换
