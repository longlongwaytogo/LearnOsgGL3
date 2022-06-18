# opengl 3.3 以后版本的osg例子

	模式可以使用高级shader进行编程，更好玩，更有意思
	
- 
- 基础篇
	- 环境搭建
		- osg gl3 模式编译
		- 基于osg gl3的环境搭建
		- 基于osg gl3的基本设置
		- 兼容osg早期格式的布局模式
		- shader调试功能
	    - renderdoc使用
		- renderdoc集成
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