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
#include <osg/BindImageTexture>
#include <osg/BufferIndexBinding>
#include <osg/BufferObject>
#include <osg/DispatchCompute>
#include <EffectCompositor/DispatchComputeWithBarrier.h>
#include <random>
#include <osgViewer/Viewer>

class ReflectiveShadowMapStageCallback :public Effect::SceneStageCallback
{
public:
	osg::Matrixf m_LightVPMatrix;
	osg::Vec3 m_LightDir;
	int m_RSMResolution = 256;
	virtual bool update() override
	{
		Effect::SceneStageCallback::update();

		// update ShadingWithRSM parameter
		{
			if (auto viewer = _pStage->getGraphicsPipeline()->getViewer()) {
				osg::StateSet* ss = _pStage->getGraphicsPipeline()->getOrCreateStateSet();
				osg::Matrix ViewMatrix = viewer->getCamera()->getViewMatrix();
				osg::Matrixf LightVPMatrixMulInverseCameraViewMatrix = osg::Matrix::inverse(ViewMatrix) * m_LightVPMatrix;
				ss->getOrCreateUniform("u_LightVPMatrixMulInverseCameraViewMatrix", osg::Uniform::FLOAT_MAT4)->set(LightVPMatrixMulInverseCameraViewMatrix);
				osg::Vec3 LightDirInViewSpace = m_LightDir * ViewMatrix;
				LightDirInViewSpace.normalize();
				ss->getOrCreateUniform("u_LightDirInViewSpace", osg::Uniform::FLOAT_VEC3)->set(LightDirInViewSpace);
				osg::Matrixf MainViewMatrix = ViewMatrix;
				ss->getOrCreateUniform("u_MainViewMatrix", osg::Uniform::FLOAT_MAT4)->set(MainViewMatrix);
			}
		}
		return true;

	}
	virtual bool init() override
	{
		osg::StateSet* rootSateSet = _pStage->getGraphicsPipeline()->getOrCreateStateSet();
		{ // gbuffer
			if (auto gbufferPass = createNewPass(Effect::FORWARD_PASS, "GBuffer"))
			{
				osg::Camera* gbuffer = gbufferPass->getCamera();
				gbuffer->setComputeNearFarMode(osg::Camera::ComputeNearFarMode::DO_NOT_COMPUTE_NEAR_FAR);
			 
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
				setTextureAttachmentType(TextureConfig4Depth, Effect::ETextureAttachmentType::DepthTexture);
				registerSharedData("AlbedoTexture", TextureConfig4Albedo);
				registerSharedData("NormalTexture", TextureConfig4Normal);
				registerSharedData("PositionTexture", TextureConfig4Position);
				registerSharedData("DepthTexture", TextureConfig4Depth); 
				attachCamera(gbuffer, { TextureConfig4Albedo,TextureConfig4Normal,TextureConfig4Position,TextureConfig4Depth });
				_pStage->addPass(gbufferPass);
			}

			// create quad
			if (auto quadPass = createNewPass(Effect::DEFERRED_PASS, "Quad"))
			{
				osg::Camera* quad = quadPass->getCamera();
				osg::StateSet* ss = quad->getOrCreateStateSet();
				osg::Program* program = new osg::Program;
				std::string vs = shaderPath + std::string("/ScreenQuadPass.vert");
				std::string fs = shaderPath + std::string("/ScreenQuadPass.frag");
				program->addShader(osgDB::readRefShaderFile(vs));
				program->addShader(osgDB::readRefShaderFile(fs));
				ss->setAttribute(program); 
				ss->setTextureAttributeAndModes(0, getSharedData("AlbedoTexture"));
				attachCamera(quad, {});
				_pStage->addPass(quadPass);
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
		addModel(Frame::getDefaultModel());
	}

	virtual void initCamera()override
	{
		auto manipulator = new Frame::FirstPersonManipulator(
			//osgGA::FirstPersonManipulator::SET_CENTER_ON_WHEEL_FORWARD_MOVEMENT|
			//osgGA::FirstPersonManipulator::PROCESS_MOUSE_WHEEL
		);
		manipulator->setSpeed(5);
		
		// 这里进行旋转的原有是osg导入模型会按照osg坐标系修正，opengl坐标系下看到的效果，需要osg中绕x轴旋转90度
		osg::Matrix rotate = osg::Matrix::rotate(osg::DegreesToRadians(90.0), osg::Vec3(1, 0, 0));
		osg::Vec3 eye = osg::Vec3(0.0, 0.0, 3.0)* rotate;
		osg::Vec3  dir = osg::Vec3(0.0, 0.0, -1.0)*rotate;
		osg::Vec3 center = (eye + dir);
		osg::Vec3 up = osg::Vec3(0.0, 1.0, 0.0)* rotate;
		osg::Matrix viewMatrix;
		viewMatrix.makeLookAt(eye, center, up);
		manipulator->setHomePosition(eye, center, up);
		auto vp = m_viewer->getCamera()->getViewport();
		m_viewer->getCamera()->setProjectionMatrix(osg::Matrix::perspective(45.0, float(m_w) / float(m_h), 0.1, 100));
		m_viewer->setCameraManipulator(manipulator);
	}
};

int main(int argc, char** argv)
{
	runMain<AppImpl>(argc,argv);
}