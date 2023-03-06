#include <EffectCompositor/DispatchComputeWithBarrier.h>

namespace Effect
{
	DispatchComputeWithBarrier::DispatchComputeWithBarrier(GLint numGroupsX, GLint numGroupsY, GLint numGroupsZ,GLbitfield barriers) :
		DispatchCompute(numGroupsX, numGroupsY, numGroupsZ), m_barriers(barriers)
	{
	}

 
	DispatchComputeWithBarrier::DispatchComputeWithBarrier(const DispatchComputeWithBarrier& o, const osg::CopyOp& copyop):
		DispatchCompute(o),m_barriers(o.m_barriers)
	{

	}
	void DispatchComputeWithBarrier::drawImplementation(osg::RenderInfo& renderInfo) const
	{
		osg::DispatchCompute::drawImplementation(renderInfo);
		if (!m_barriers)
		{
			renderInfo.getState()->get<osg::GLExtensions>()->glMemoryBarrier(m_barriers);
		}
	}
}