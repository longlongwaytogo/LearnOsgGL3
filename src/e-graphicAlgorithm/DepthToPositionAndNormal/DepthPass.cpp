#include "DepthPass.h"
#include <EffectCompositor/GraphicsPipeline/RenderStage.h>
#include <EffectCompositor/GraphicsPipeline/GraphicsPipeline.h>
#include <EffectCompositor/GraphicsPipeline/RenderStageCallback.h>
#include <osg/Texture2D>
#include <osg/Camera>

bool DepthPass::init()
{
	 Effect::RenderPass::init();

	 if (!m_pRenderStage)
		 return false;
	// create gbuffer for depthPass

	if (osg::Camera* gbuffer = m_pRenderStage->getGraphicsPipeline()->createNewPass(Effect::FORWARD_PASS, "GBuffer"))
	{
		setPassData(Effect::FORWARD_PASS, "GBuffer", gbuffer);
		gbuffer->setComputeNearFarMode(osg::Camera::ComputeNearFarMode::DO_NOT_COMPUTE_NEAR_FAR);

		osg::StateSet* ss = gbuffer->getOrCreateStateSet();
		osg::ref_ptr<osg::Program> pProgram = new osg::Program;
		std::string vs = shaderPath + std::string("/GBufferPass.vert");
		std::string fs = shaderPath + std::string("/GBufferPass.frag");
		pProgram->addShader(osgDB::readRefShaderFile(vs));
		pProgram->addShader(osgDB::readRefShaderFile(fs));
		ss->setAttribute(pProgram);

		osg::ref_ptr<osg::Texture> depth = new osg::Texture2D;

		osg::ref_ptr<osg::Texture> TextureConfig4Position = new osg::Texture2D;
		osg::ref_ptr<osg::Texture> TextureConfig4Normal = new osg::Texture2D;
		osg::ref_ptr<osg::Texture> TextureConfig4Albedo = new osg::Texture2D;
		osg::ref_ptr<osg::Texture> TextureConfig4Depth = new osg::Texture2D;

		TextureConfig4Normal->setInternalFormat(GL_RGBA32F);
		TextureConfig4Normal->setSourceFormat(GL_RGBA);
		TextureConfig4Normal->setSourceType(GL_FLOAT);

		TextureConfig4Position->setInternalFormat(GL_RGBA32F);
		TextureConfig4Position->setSourceFormat(GL_RGBA);
		TextureConfig4Position->setSourceType(GL_FLOAT);

		TextureConfig4Albedo->setInternalFormat(GL_RGBA32F);
		TextureConfig4Albedo->setSourceFormat(GL_RGBA);
		TextureConfig4Albedo->setSourceType(GL_FLOAT);

		TextureConfig4Depth->setInternalFormat(GL_DEPTH_COMPONENT32F);
		TextureConfig4Depth->setSourceFormat(GL_DEPTH_COMPONENT);
		TextureConfig4Depth->setSourceType(GL_FLOAT);
		TextureConfig4Depth->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture::FilterMode::NEAREST);
		TextureConfig4Depth->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture::FilterMode::NEAREST);
		m_pRenderStage->getRenderStageCallback()->setTextureAttachmentType(TextureConfig4Depth, Effect::ETextureAttachmentType::DepthTexture);
		registerSharedData("AlbedoTexture", TextureConfig4Albedo);
		registerSharedData("NormalTexture", TextureConfig4Normal);
		registerSharedData("PositionTexture", TextureConfig4Position);
		registerSharedData("DepthTexture", TextureConfig4Depth);
		m_pRenderStage->getRenderStageCallback()->attachCamera(gbuffer, { TextureConfig4Albedo,TextureConfig4Normal,TextureConfig4Position,TextureConfig4Depth });
	}

	return true;
}

bool DepthPass::update()
{
	 Effect::RenderPass::update();



	return true;
}
