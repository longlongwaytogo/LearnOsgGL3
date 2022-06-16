# opengl 3.3 以后版本的osg例子

	模式可以使用高级shader进行编程，更好玩，更有意思
- 基础篇，环境搭建	
	- 基于osg gl3的环境搭建
	- 基于osg gl3的基本设置
- gl3 实现被废弃的opengl特性
	- 点精灵实现点大小设置
	- line stipple ： 线的点化效果
	- polygon stipple ： 多边形点化效果
	- line width：有宽度的线
	- xor 按位异或操作（fbo不支持xor，需要单独实现）
	- clip效果实现
	
- 性能优化
 - 无顶点输入绘制三角形
 - fbo使用统一缓存作为输入和输出
	- image
	- texture