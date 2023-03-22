#include <GraphicsPipeline/RenderPass.h>
#include <GraphicsPipeline/RenderStage.h>
#include <GraphicsPipeline/GraphicsPipeline.h>
namespace Effect
{
	bool RenderPass::init()
	{
		return true;
	}
	bool RenderPass::update()
	{
		return true;
	}

	void RenderPass::setRenderStage(RenderStage* rs)
	{
		m_pRenderStage = rs;
	}
	RenderStage* RenderPass::getRenderStage()
	{
		return m_pRenderStage;
	}

	void RenderPass::registerSharedData(const std::string& name, osg::Texture* tex)
	{
		m_pRenderStage->getGraphicsPipeline()->setTexture(name, tex);
	}

}