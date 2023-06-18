#include <Frame/app.h>
#include <Frame/utils.h>
#include <Frame/FirstPersonMainipulator.h>
#include <osg/Shader>
#include <osg/Program>
#include <osgDB/ReadFile>
#include <osg/texture2D>
#include <osg/vec3>
#include <osg/matrix>
#include <osg/Math>
#include <osg/BindImageTexture>
#include <osg/BufferIndexBinding>
#include <osg/BufferObject>
#include <osg/DispatchCompute>
#include <EffectCompositor/DispatchComputeWithBarrier.h>
#include <random>
#include <osgViewer/Viewer>

#include "DepthPass.h"
#include "ReConstructPass.h"

//https://zhuanlan.zhihu.com/p/367257314?utm_id=0
http://www.klayge.org/2014/06/19/klayge-4-5%e4%b8%ad%e6%b8%b2%e6%9f%93%e7%9a%84%e6%94%b9%e8%bf%9b%ef%bc%88%e4%ba%8c%ef%bc%89%ef%bc%9a%e5%86%8d%e6%8e%a2normal%e7%9a%84%e5%ad%98%e5%82%a8/
class DepthToPositionAndNormalStageCallback :public Effect::RenderStageCallback
{
public:
	osg::Matrixf m_LightVPMatrix;
	osg::Vec3 m_LightDir;
	int m_RSMResolution = 256;
	virtual bool update() override
	{
		Effect::RenderStageCallback::update();
		
		return true;
	}
	virtual bool init() override
	{
		osg::StateSet* rootSateSet = _pStage->getGraphicsPipeline()->getOrCreateStateSet();
		{ // gbuffer
			_pStage->addPass(new DepthPass);
		}
		// create quad
		{
			_pStage->addPass(new ReConstructPass);
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
		m_pipeline->registerStage<DepthToPositionAndNormalStageCallback>(0,"ReflectiveShadowMap");

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
		osg::StateSet* ss = m_viewer->getCamera()->getOrCreateStateSet();
		ss->addUniform(new osg::Uniform("u_Fov",45.0f));
		ss->addUniform(new osg::Uniform("u_WindowWidth",float(m_w)));
		ss->addUniform(new osg::Uniform("u_WindowHeight",float(m_h)));
		ss->addUniform(new osg::Uniform("u_Near",0.1f ));
		ss->addUniform(new osg::Uniform("u_Far",100.0f ));
	}
};

int main(int argc, char** argv)
{
	runMain<AppImpl>(argc,argv);
}