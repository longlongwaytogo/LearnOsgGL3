#include "ReConstructPass.h"
#include <EffectCompositor/GraphicsPipeline/RenderStage.h>
#include <EffectCompositor/GraphicsPipeline/GraphicsPipeline.h>

bool ReConstructPass::init()
{
	__super::init();
	if (auto quad = m_pRenderStage->getGraphicsPipeline()->createNewPass(Effect::DEFERRED_PASS, "Quad"))
	{
		setPassData(Effect::DEFERRED_PASS, "Quad", quad);
		quad->setComputeNearFarMode(osg::Camera::ComputeNearFarMode::DO_NOT_COMPUTE_NEAR_FAR);

		osg::StateSet* ss = quad->getOrCreateStateSet();
		osg::Program* program = new osg::Program;
		std::string vs = shaderPath + std::string("/ReConstructPass.vert");
		std::string fs = shaderPath + std::string("/ReConstructPass.frag");
		program->addShader(osgDB::readRefShaderFile(vs));
		program->addShader(osgDB::readRefShaderFile(fs));
		ss->setAttribute(program);
		auto callback = m_pRenderStage->getRenderStageCallback();
		osg::Texture* tex0 = callback->getSharedData("AlbedoTexture");
		osg::Texture* tex1 = callback->getSharedData("NormalTexture");
		osg::Texture* tex2 = callback->getSharedData("PositionTexture");
		osg::Texture* tex3 = callback->getSharedData("DepthTexture");

		ss->setTextureAttributeAndModes(0, tex0);
		ss->setTextureAttributeAndModes(1, tex1);
		ss->setTextureAttributeAndModes(2, tex2);
		ss->setTextureAttributeAndModes(3, tex3);
		m_pRenderStage->getRenderStageCallback()->attachCamera(quad, {});
	}
	return true;
}

bool ReConstructPass::update()
{
	return	__super::update();
}