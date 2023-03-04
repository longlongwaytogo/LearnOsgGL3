#include <Frame/app.h>
#include <Frame/utils.h>
#include <Frame/FirstPersonMainipulator.h>
#include <osg/Shader>
#include <osg/Program>
#include <osgDB/ReadFile>
#include <osg/texture2D>
#include <osg/vec3>
#include <osg/matrix>
#include <osgGA/TrackballManipulator>
#include <osgGA/FirstPersonManipulator>
#include <osg/Math>

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

				osg::ref_ptr<osg::Texture> TextureConfig4Position = Frame::createDefaultTex2d();
				osg::ref_ptr<osg::Texture> TextureConfig4Normal = Frame::createDefaultTex2d();
				osg::ref_ptr<osg::Texture> TextureConfig4Flux = Frame::createDefaultTex2d();
				TextureConfig4Position->setInternalFormat(GL_RGBA32F);
				TextureConfig4Normal->setInternalFormat(GL_RGBA32F);
				TextureConfig4Flux->setInternalFormat(GL_RGBA32F);
				TextureConfig4Position->setSourceType(GL_FLOAT);
				TextureConfig4Normal->setSourceType(GL_FLOAT);
				TextureConfig4Flux->setSourceType(GL_FLOAT);
				TextureConfig4Position->setWrap(osg::Texture::WRAP_S,osg::Texture::WrapMode::CLAMP_TO_BORDER);
				TextureConfig4Normal->setWrap(osg::Texture::WRAP_S, osg::Texture::WrapMode::CLAMP_TO_BORDER);
				TextureConfig4Flux->setWrap(osg::Texture::WRAP_S, osg::Texture::WrapMode::CLAMP_TO_BORDER);
				TextureConfig4Position->setWrap(osg::Texture::WRAP_T, osg::Texture::WrapMode::CLAMP_TO_BORDER);
				TextureConfig4Normal->setWrap(osg::Texture::WRAP_T, osg::Texture::WrapMode::CLAMP_TO_BORDER);
				TextureConfig4Flux->setWrap(osg::Texture::WRAP_T, osg::Texture::WrapMode::CLAMP_TO_BORDER);
				TextureConfig4Position->setBorderColor(osg::Vec4(0,0,0,0));
				TextureConfig4Normal->setBorderColor(osg::Vec4(0, 0, 0, 0));
				TextureConfig4Flux->setBorderColor(osg::Vec4(0, 0, 0, 0));

				registerSharedData("RSMPositionTexture", TextureConfig4Position);
				registerSharedData("RSMNormalTexture", TextureConfig4Normal);
				registerSharedData("RSMFluxTexture", TextureConfig4Flux);

				attachCamera(rsm, { TextureConfig4Flux,TextureConfig4Normal,TextureConfig4Position });

				/*osg::Vec3 LightPos(-0.15, -1.13, -0.58);
				osg::Vec3 LightDir(-1.0, -0.7071, 0);*/
				osg::Vec3 LightPos(0,0, 100);
				osg::Vec3 LightDir(0,0,-100);
				LightDir.normalize();
				osg::Matrix LightViewMatrix = osg::Matrix::rotate(osg::DegreesToRadians(90.0),osg::Vec3(1,0,0))
					*osg::Matrix::lookAt(LightPos, LightPos + LightDir, osg::Vec3(0, 0, 1));
				osg::Matrix LightProjectionMatrix = osg::Matrix::ortho2D(-200.0f, 200.0f, -200.0f, 200.0f);
				osg::Matrixf LightVPMatrix = LightViewMatrix * LightProjectionMatrix;

				//ss->addUniform(new osg::Uniform("LigtPos", LightPos));
				//ss->addUniform(new osg::Uniform("u_LightVPMatrix", LightVPMatrix));
				//rsm->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
				//rsm->setProjectionMatrix(LightProjectionMatrix);
				//rsm->setViewMatrix(LightViewMatrix);
				//rsm->setViewMatrixAsLookAt(osg::Vec3(0,0,100), osg::Vec3(0, 0, 0), osg::Vec3(0, 1, 0));
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
					ss->setTextureAttributeAndModes(0, getShaderedData("RSMFluxTexture"));
					//ss->setTextureAttributeAndModes(1, getShaderedData("NormalTexture"));
					attachCamera(quad, {});
				}

				{
					// light line 
					
				}
			}
		}
		return true;
	}

};
class AppImpl : public Frame::App
{
public:
	virtual void initScene() override
	{
		float w = 1920;
		float h = 1152;
		setWindowSize(w, h);
		// create gbuffer
		m_pipeline->registerStage<ReflectiveShadowMapStageCallback>(0,"ReflectiveShadowMap");
	}

	virtual void initCamera()override
	{
		auto manipulator = new Frame::FirstPersonManipulator(
			//osgGA::FirstPersonManipulator::SET_CENTER_ON_WHEEL_FORWARD_MOVEMENT|
			//osgGA::FirstPersonManipulator::PROCESS_MOUSE_WHEEL
		);
		manipulator->setSpeed(5);
		//osg::Vec3 eye(0,0,100), center(0,0,0), up(0, 0, 1);
		//manipulator->setHomePosition(eye, center, up);
		osg::Vec3 LightPos(0, 0, 100);
		osg::Vec3 LightDir(0, 0, -100);
		//LightDir.normalize();
		// 这里进行旋转的原有是osg导入模型会按照osg坐标系修正，opengl坐标系下看到的效果，需要osg中绕x轴旋转90度
		osg::Matrix rotate = osg::Matrix::rotate(osg::DegreesToRadians(90.0), osg::Vec3(1, 0, 0));
		osg::Vec3 eye = osg::Vec3(0.0, 0.0, 3.0)* rotate;
		osg::Vec3  dir = osg::Vec3(0.0, 0.0, -1.0)*rotate;
		osg::Vec3 center = (eye + dir);
		osg::Vec3 up = osg::Vec3(0.0, 1.0, 0.0)* rotate;
		
		
		//m_viewer->setUpViewInWindow(0, 0, m_w, m_h);
		 manipulator->setHomePosition(eye, center, up);
		 auto vp = m_viewer->getCamera()->getViewport();
		 

		// m_viewer->getCamera()->setProjectionMatrix(osg::Matrix::perspective(45.0, m_w / m_h, 0.1, 100));
		 m_viewer->setCameraManipulator(manipulator);

		//osg::Matrix LightViewMatrix = osg::Matrix::lookAt(LightPos, LightPos + LightDir, osg::Vec3(0, 0, 1));
		//osg::Matrix LightProjectionMatrix = osg::Matrix::ortho2D(-200.0f, 200.0f, -200.0f, 200.0f);
		////osg::Matrixf LightVPMatrix = LightViewMatrix * LightProjectionMatrix;
		//manipulator->setHomePosition(LightPos, center, up);
		//m_viewer->getCamera()->setProjectionMatrix(LightProjectionMatrix);
		//m_viewer->setCameraManipulator(manipulator);

	
	}
};

int main(int argc, char** argv)
{
	runMain<AppImpl>(argc,argv);
}