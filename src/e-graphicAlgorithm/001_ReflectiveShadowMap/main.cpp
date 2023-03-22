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
			}
			
			if(auto rsmPass = createNewPass(Effect::FORWARD_PASS, "RSMBuffer"))
			{
				osg::Camera* rsm = rsmPass->getCamera();
				rsm->setClearColor(osg::Vec4(0, 0, 0, 1));
				
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

				dynamic_cast<osg::Texture2D*>(TextureConfig4Position.get())->setTextureSize(m_RSMResolution, m_RSMResolution);
				dynamic_cast<osg::Texture2D*>(TextureConfig4Normal.get())->setTextureSize(m_RSMResolution, m_RSMResolution);
				dynamic_cast<osg::Texture2D*>(TextureConfig4Flux.get())->setTextureSize(m_RSMResolution, m_RSMResolution);

				registerSharedData("RSMPositionTexture", TextureConfig4Position);
				registerSharedData("RSMNormalTexture", TextureConfig4Normal);
				registerSharedData("RSMFluxTexture", TextureConfig4Flux);

				attachCamera(rsm, { TextureConfig4Flux,TextureConfig4Normal,TextureConfig4Position });
				rsm->setComputeNearFarMode(osg::Camera::ComputeNearFarMode::DO_NOT_COMPUTE_NEAR_FAR);

				osg::Matrix rotate = osg::Matrix::rotate(osg::DegreesToRadians(90.0), osg::Vec3(1, 0, 0));
				osg::Vec3 LightPos(-0.15, -1.13, -0.58);
				osg::Vec3 LightDir(-1.0, -0.7071, 0);
				osg::Vec3 up(0, 1, 0);
				LightPos = LightPos * rotate;
				LightDir = LightDir * rotate;
				up = up * rotate;
				rsm->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
				osg::Matrix LightProjectionMatrix = osg::Matrix::ortho(-2.0f, 2.0f, -2.0f, 2.0f,0.1f, 10.0f);
				rsm->setProjectionMatrix(LightProjectionMatrix);
				rsm->setViewMatrixAsLookAt(LightPos, LightPos + LightDir, up);
				rsm->setViewport(0,0,m_RSMResolution, m_RSMResolution);
				m_LightVPMatrix = rsm->getViewMatrix() * LightProjectionMatrix;
				m_LightDir = LightDir;
				osg::Matrixf LightViewMatrixInverse = osg::Matrix::inverse(rsm->getViewMatrix());
				rootSateSet->addUniform(new osg::Uniform("LightVPMatrix", m_LightVPMatrix));
				rootSateSet->addUniform(new osg::Uniform("u_LightViewMatrixInverse", LightViewMatrixInverse));
				rootSateSet->addUniform(new osg::Uniform("u_RSMSize", m_RSMResolution));
				rootSateSet->addUniform(new osg::Uniform("LightDir", LightDir));
			}

			// ShadingWithRSMPass
			if (auto ShadingWithRSMPass = createNewPass(Effect::COMPUTE_PASS, "ShadingWithRSMBuffer"))
			{
				osg::Camera* ShadingWithRSM = ShadingWithRSMPass->getCamera();
				float w = 1920;
				float h = 1152;
				osg::ref_ptr<osg::Node> sourceNode = new Effect::DispatchComputeWithBarrier((w + 16 - 1) / 16, (h + 16 - 1) / 16, 1, GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
				sourceNode->setDataVariance(osg::Object::DYNAMIC);
				ShadingWithRSM->addChild(sourceNode);
			
				osg::StateSet* ss = ShadingWithRSM->getOrCreateStateSet();
				auto TextureConfig = Frame::createDefaultTex2d();
				TextureConfig->setInternalFormat(GL_RGBA32F);
				TextureConfig->setSourceFormat(GL_RGBA);
				TextureConfig->setSourceType(GL_FLOAT);
				TextureConfig->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
				TextureConfig->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
				TextureConfig->setTextureSize(w, h);
				osg::Texture* tex = TextureConfig;
				osg::ref_ptr<osg::BindImageTexture> imagbinding = new osg::BindImageTexture(0, tex, osg::BindImageTexture::WRITE_ONLY, GL_RGBA32F);

				//ss->setTextureAttributeAndModes(0, TextureConfig);
				ss->setAttribute(imagbinding);

				registerSharedData("ShadingTexture", TextureConfig);

				osg::Program* program = new osg::Program();
				std::string path = shaderPath + std::string("/ShadingWithRSMPass.glsl");
				auto shader = osgDB::readRefShaderFile(path);
				program->addShader(new osg::Shader(osg::Shader::COMPUTE, shader->getShaderSource()));
				ss->setAttribute(program);

				int m_VPLNum = 32;
				float m_MaxSampleRadius = 25;
				//__initVPLsSampleCoordsAndWeights();
				{
					osg::UniformBufferObject* ssbo = new osg::UniformBufferObject();
					osg::Vec4Array* handles = new osg::Vec4Array();
					handles->setBufferObject(ssbo);
					handles->reserve(m_VPLNum);
					std::default_random_engine e;
					std::uniform_real_distribution<float> u(0, 1);
					for (int i = 0; i < m_VPLNum; ++i)
					{
						float xi1 = u(e);
						float xi2 = u(e);
						handles->push_back({ xi1 * sin(2 * osg::PIf * xi2), xi1 * cos(2 * osg::PIf * xi2), xi1 * xi1, 0 });
					}
					osg::UniformBufferBinding* ssbb = new osg::UniformBufferBinding(0, handles, 0, sizeof(osg::Vec4) * m_VPLNum);
					ss->setAttributeAndModes(ssbb, osg::StateAttribute::ON);
					program->addBindUniformBlock("VPLsSampleCoordsAndWeights", 0);
				}

				ss->setTextureAttributeAndModes(0,getSharedData("AlbedoTexture"));
				ss->setTextureAttributeAndModes(1,getSharedData("NormalTexture"));
				ss->setTextureAttributeAndModes(2, getSharedData("PositionTexture"));
				ss->setTextureAttributeAndModes(3, getSharedData("RSMFluxTexture"));
				ss->setTextureAttributeAndModes(4, getSharedData("RSMNormalTexture"));
				ss->setTextureAttributeAndModes(5,getSharedData("RSMPositionTexture"));

				ss->addUniform(new osg::Uniform("u_AlbedoTexture",0));
				ss->addUniform(new osg::Uniform("u_NormalTexture", 1));
				ss->addUniform(new osg::Uniform("u_PositionTexture", 2));
				ss->addUniform(new osg::Uniform("u_RSMFluxTexture", 3));
				ss->addUniform(new osg::Uniform("u_RSMNormalTexture", 4));
				ss->addUniform(new osg::Uniform("u_RSMPositionTexture", 5));

				ss->addUniform(new osg::Uniform("u_MaxSampleRadius", m_MaxSampleRadius));
				ss->addUniform(new osg::Uniform("u_VPLNum", m_VPLNum));
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
				ss->setTextureAttributeAndModes(0, getSharedData("ShadingTexture"));
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