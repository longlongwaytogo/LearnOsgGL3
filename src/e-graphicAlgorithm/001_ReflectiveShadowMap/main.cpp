#include <Frame/app.h>
#include <osg/Shader>
#include <osg/Program>
#include <osgDB/ReadFile>
#include <osg/texture2D>

class ReflectiveShadowMapStageCallback :public Effect::SceneStageCallback
{
public:
	virtual bool init() override
	{
		{ // gbuffer
			if (osg::Camera* gbuffer = createNewPass(Effect::FORWARD_PASS, "GBuffer"))
			{
				osg::StateSet* ss = gbuffer->getOrCreateStateSet();
				osg::ref_ptr<osg::Program> pProgram = new osg::Program;
				std::string vs = shaderPath + std::string("/GBufferPass.vert");
				std::string fs = shaderPath + std::string("/GBufferPass.frag");
				pProgram->addShader(osgDB::readRefShaderFile(vs));
				pProgram->addShader(osgDB::readRefShaderFile(fs));
				ss->setAttribute(pProgram);

				osg::ref_ptr<osg::Texture> depth = new osg::Texture2D;

				osg::ref_ptr<osg::Texture> TextureConfig4Position	= new osg::Texture2D;
				osg::ref_ptr<osg::Texture> TextureConfig4Normal		= new osg::Texture2D;
				osg::ref_ptr<osg::Texture> TextureConfig4Albedo		= new osg::Texture2D;
				osg::ref_ptr<osg::Texture> TextureConfig4Depth		= new osg::Texture2D;

				TextureConfig4Normal->setInternalFormat(GL_RGBA);
				TextureConfig4Normal->setSourceFormat(GL_RGBA);
				TextureConfig4Normal->setSourceType(GL_UNSIGNED_BYTE);

				TextureConfig4Position->setInternalFormat(GL_RGBA);
				TextureConfig4Position->setSourceFormat(GL_RGBA);
				TextureConfig4Position->setSourceType(GL_UNSIGNED_BYTE);

				TextureConfig4Albedo->setInternalFormat(GL_RGBA32F);
				TextureConfig4Albedo->setSourceFormat(GL_RGBA);
				TextureConfig4Albedo->setSourceType(GL_FLOAT);

				TextureConfig4Depth->setInternalFormat(GL_DEPTH_COMPONENT32F);
				TextureConfig4Depth->setSourceFormat(GL_DEPTH_COMPONENT);
				TextureConfig4Depth->setSourceType(GL_FLOAT);
				TextureConfig4Depth->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture::FilterMode::NEAREST);
				TextureConfig4Depth->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture::FilterMode::NEAREST);
				setTextureAttachmentType(TextureConfig4Depth, Effect::ETextureAttachmentType::DepthTexture);
				registerSharedData("AlbedoTexture", TextureConfig4Albedo);
				registerSharedData("NormalTexture", TextureConfig4Normal);
				registerSharedData("PositionTexture", TextureConfig4Position);
				registerSharedData("DepthTexture", TextureConfig4Depth); 
				attachCamera(gbuffer, { TextureConfig4Albedo,TextureConfig4Normal,TextureConfig4Position,TextureConfig4Depth });
				
				
			

			}
			
			if(osg::Camera* rsm = createNewPass(Effect::FORWARD_PASS, "RSMBuffer"))
			{
				// render at light pos
				osg::StateSet* ss = rsm->getOrCreateStateSet();
				osg::Program* program = new osg::Program;
				std::string vs = shaderPath + std::string("/RSMBufferPass.vert");
				std::string fs = shaderPath + std::string("/RSMBufferPass.frag");
				program->addShader(osgDB::readRefShaderFile(vs));
				program->addShader(osgDB::readRefShaderFile(fs));
				ss->setAttribute(program);

				
			}
			{
				// create quad
				if (osg::Camera* quad = createNewPass(Effect::DEFERRED_PASS, "Quad"))
				{
					osg::StateSet* ss = quad->getOrCreateStateSet();
					osg::Program* program = new osg::Program;
					std::string vs = shaderPath + std::string("/ScreenQuadPass.vert");
					std::string fs = shaderPath + std::string("/ScreenQuadPass.frag");
					program->addShader(osgDB::readRefShaderFile(vs));
					program->addShader(osgDB::readRefShaderFile(fs));
					ss->setAttribute(program);
					ss->setTextureAttributeAndModes(0, getShaderedData("AlbedoTexture"));
					attachCamera(quad, {});
				}
			}
			
		}
		return true;
	}

};
class AppImpl : public Frame::App
{
public:
	virtual void initScene()
	{
		// create gbuffer
		m_pipeline->registerStage<ReflectiveShadowMapStageCallback>(0,"ReflectiveShadowMap");
	}
};

int main(int argc, char** argv)
{
	runMain<AppImpl>(argc,argv);
}