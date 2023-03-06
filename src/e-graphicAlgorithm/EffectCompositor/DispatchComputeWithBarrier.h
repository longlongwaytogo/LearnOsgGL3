#pragma once 
#include <osg/DispatchCompute>
namespace Effect
{
	//不设置Barriers，等同与DispatchCompute

	/*
	barriers
指定要插入的障碍。
必须为 
GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT,
GL_ELEMENT_ARRAY_BARRIER_BIT,
GL_UNIFORM_BARRIER_BIT,
GL_TEXTURE_FETCH_BARRIER_BIT,
GL_SHADER_IMAGE_ACCESS_BARRIER_BIT, 
GL_COMMAND_BARRIER_BIT, 
GL_PIXEL_BUFFER_BARRIER_BIT, 
GL_TEXTURE_UPDATE_BARRIER_BIT, 
GL_BUFFER_UPDATE_BARRIER_BIT, 
GL_FRAMEBUFFER_BARRIER_BIT,
GL_TRANSFORM_FEEDBACK_BARRIER_BIT,
GL_ATOMIC_COUNTER_BARRIER_BIT,
GL_SHADER_STORAGE_BARRIER_BIT的按位组合. 如果指定了特殊值GL_ALL_BARRIER_BITS，将插入所有受支持的障碍。

glMemoryBarrier定义一个屏障，相对于在屏障之后发出的事务，该屏障对在命令之前发出的内存事务进行排序。
出于此排序的目的，由着色器执行的内存事务被视为由触发着色器执行的渲染命令发出。
barriers是一个位域，指示与着色器存储同步的一组操作；
原文链接：https://blog.csdn.net/MSK1111/article/details/103839242
*/
	class DispatchComputeWithBarrier :public osg::DispatchCompute
	{
	public:
		DispatchComputeWithBarrier(GLint numGroupsX = 0, GLint numGroupsY = 0, GLint numGroupsZ = 0, GLbitfield barriers = 0);

		DispatchComputeWithBarrier(const DispatchComputeWithBarrier&, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY);

		META_Node(osg, DispatchComputeWithBarrier);
 
		inline void setBarrier(GLbitfield barriers) { m_barriers = barriers; }
		inline GLbitfield getBarriers() { return m_barriers; }

		virtual void drawImplementation(osg::RenderInfo& renderInfo) const;

	protected:
		GLbitfield m_barriers;
	};
}