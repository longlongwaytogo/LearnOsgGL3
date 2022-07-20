- 简介：
[LearnOsgGL3](https://github.com/longlongwaytogo/LearnOsgGL3)项目主要包含了基于OpengGL 3.3以后版本的osg相关例子研究。
示例多设计osg高级特性(opengl高级特性) shader编程等主题，亦可以成为OSG进阶(Advanced OSG)。

		auhor: longlongwaytogo
		email: longlongway2012@hotmail.com
		repo: https://github.com/longlongwaytogo/LearnOsgGL3

- 序
个人从事osg开发已经近12年，一直都是输入，没有太多输出，想把接触到osg3.3相关的例子整理分享出来。
GL3模式可以使用高级shader进行编程，更好玩，更有意思，以下将从以下几个方面进行示例整理：
	- 基础篇：主要讲解一些osg gl3 环境的搭建和调试工具的使用。
	- 中级篇: 基于shader的opengl废弃功能的一些shader模拟和opengl3.3以后的的新特性示例。
	- 高级篇：一些更高级、复杂主题的研究，包括bpr材质、动画、粒子、水、阴影等。
	- 性能篇：针对渲染效率优化，单独作为一个主题进行说明。
	
- 基础篇
	- 环境搭建
		- [osg gl3 模式编译](docs/a-base/01-01-环境搭建.md)
		- [基于osg gl3的环境搭建](docs/a-base/01-02-osgSimplegl3.md)
		- [基于osg gl3的基本设置](docs/a-base/01-03-osgSimplegl3-2.md)
		- 兼容osg早期顶点属性格式的输入布局
		- shader调试功能
	    - renderdoc使用
		- renderdoc集成
- 中级篇：		
	- gl3 实现被废弃的opengl特性
		- 点精灵实现点大小设置
		- line stipple ： 线的点化效果
		- polygon stipple ： 多边形点化效果
		- line width：有宽度的线
		- xor 按位异或操作（fbo不支持xor，需要单独实现）
		- clip效果实现
	- gl3 特性演示
		- 基于点精灵的粒子效果
		- ssbo
		- compute shader
		- geometry shader
	
- 高级功能：
	- [法线可视化](docs/d-advanced/01-normalVisualizer.md)
	- bpr效果
	- 环境纹理
	- 动画
	- 阴影
	- 粒子
	- 水效
	
- 性能优化
 - 无顶点输入绘制三角形
 - fbo使用统一缓存作为输入和输出
	- image
	- texture
	- no window rtt



个人公众号
![img](https://github.com/longlongwaytogo/LearnOsgGL3/blob/main/docs/a-base/images/qrcode.bmp)
![img](https://github.com/longlongwaytogo/LearnOsgGL3/blob/main/docs/a-base/images/qrcode.png)
![img](docs/a-base/images/qrcode.bmp)
![](https://raw.githubusercontent.com/longlongwaytogo/LearnOsgGL3/main/docs/a-base/images/qrcode.bmp)
![](https://raw.githubusercontent.com/longlongwaytogo/LearnOsgGL3/main/docs/a-base/images/qrcode.png)