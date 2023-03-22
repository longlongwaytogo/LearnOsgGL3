
#include "GraphicsPipeline/RenderStageCallback.h"
#include "GraphicsPipeline/RenderStage.h"
#include "GraphicsPipeline/GraphicsPipeline.h"
#include "GraphicsPipeline/RenderPass.h"
#include "GLMarker.h"
#include "osg/Texture2D"
#include <osgdb/ReadFile>
#include <assert.h>
  
#if OSG_VERSION_GREATER_THAN(3,4,0)
#include <osg/BindImageTexture>
#include <osg/DispatchCompute>
#endif 
namespace Effect
{
	void RenderStageCallback::setGraphicPipelineStage(RenderStage* stage)
	{
		_pStage = stage;
		_pStage->m_bLoadXml = m_bLoadXml;
	}
	bool RenderStageCallback::init()
	{
		for (auto itr = _pStage->getPassList().begin(); itr != _pStage->getPassList().end(); itr++)
		{
			(*itr)->init();
		}
		return true;
	}
	bool RenderStageCallback::update()
	{
		for (auto itr = _pStage->getPassList().begin(); itr != _pStage->getPassList().end(); itr++)
		{
			(*itr)->update();
		}
		return true;
	}
	void RenderStageCallback::traverse(RenderStage* node,osg::NodeVisitor& nv)
    {
        if(!node) return ;

		if (osg::NodeVisitor::CULL_VISITOR == nv.getVisitorType())
		{
			update();
		}
        for(auto itr =  node->getPassList().begin(); itr != node->getPassList().end(); itr++)
        {
			PassData& data = (*itr)->getPassData();
			if (data.pass && data.activated)
				data.pass->accept(nv);
        }
    }

    void DisplayStageCallback::traverse(RenderStage* node,osg::NodeVisitor& nv)
    {
        __super::traverse(node,nv);
    }


	static const char* computeSrc = {
		"#version 430\n"
		"layout (rgba8, binding =0) uniform image2D inTex;\n"
		// "uniform sampler2D inTex;\n"
		 "layout (rgba16f, binding =7) uniform image2D  outTex;\n"
		 "layout (local_size_x = 8, local_size_y = 8,local_size_z = 1) in;\n"
		 "void main() {\n"
		 "ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);\n"
		 "ivec2 size =imageSize(outTex);\n"
		 "ivec2 p = ivec2(int(gl_GlobalInvocationID.x),int(gl_GlobalInvocationID.y));\n"
		  "vec4 clr =imageLoad(inTex,storePos);\n"
		//  "vec4 clr = texture2DLod(inTex,vec2(float(gl_GlobalInvocationID.x)/float(size.x),float(gl_GlobalInvocationID.y)/float(size.y)),0.0);\n"
		//  "clr.x = 1.0;\n"
		"imageStore(outTex, p,clr);\n"
   //"clr.x += 0.5;\n"
  // "imageStore(outTex, storePos,clr);\n"
   // "imageStore(outTex, ivec2(gl_GlobalInvocationID.xy), vec4(vec2(gl_LocalInvocationID.xy) / vec2(gl_WorkGroupSize.xy),.0, .0));\n"
    "}\n"
};

 /*   static const char* computeSrc = {
        "#version 430\n"
        "layout (r32f, binding =0) uniform image2D outTex;\n"
        "layout (local_size_x = 16, local_size_y = 16) in;\n"
        "void main() {\n"
        "   ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);\n"
        "   imageStore(outTex, storePos, vec4(vec2(glLocalinvocationID.xy)/vec2(glWorkGroupSize.xy),0,1)); \n"
        "}\n"
    };
*/

bool TiledShadingCallback::init()
{
#if OSG_VERSION_LESS_OR_EQUAL(3,4,0)

	/* {
		osg::ref_ptr<osg::Texture2D> tex2D = new osg::Texture2D;
		tex2D->setTextureSize(512, 512);
		tex2D->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
		tex2D->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
		tex2D->setInternalFormat(GL_R32F);
		tex2D->setSourceFormat(GL_RED);
		tex2D->setSourceType(GL_FLOAT);
		tex2D->bindToImageUnit(0, osg::Texture::WRITE_ONLY);
	}
	*/
	if (_pStage)
	{
		osg::ref_ptr<osg::Texture> pHDRTarget = nullptr;
		osg::ref_ptr<osg::Texture> pColorBuffer = nullptr;
		GraphicsPipeline* pPipeline = _pStage->getGraphicsPipeline();
		if (pPipeline)
		{
			pColorBuffer = pPipeline->getTexture("ColorBuffer");
			pHDRTarget = pPipeline->getTexture("HDRTarget");
			pColorBuffer->bindToImageUnit(0, osg::Texture::READ_WRITE, GL_RGBA8);
			pHDRTarget->bindToImageUnit(7, osg::Texture::READ_WRITE, GL_RGBA16F_ARB);

		}

		Effect::PassData passData;
		_pStage->getPassData("computePass", passData);
		osg::ref_ptr<osg::Camera> pass = passData.pass;
		if (pass)
		{
#if 0
			if (pass->getNumChildren() > 0)
			{
				osg::Node* quad = pass->getChild(0);
				if (quad)
				{
					osg::StateSet* ss = quad->getOrCreateStateSet();
					osg::ref_ptr<osg::Program> computeProg = new osg::Program;
					computeProg->setComputeGroups(512 / 16, 512 / 16, 1);
					computeProg->addShader(new osg::Shader(osg::Shader::COMPUTE, computeSrc));
					ss->setAttribute(computeProg);
				}
			}

#else
			osg::ref_ptr<osg::Node> screenQuad = pass->getChild(0);
			// pass->removeChild(0,pass->getNumChildren());

			// for test
			//// Create the texture as both the output of compute shader and the input of a normal quad
			//osg::ref_ptr<osg::Texture2D> tex2D = new osg::Texture2D;
			//tex2D->setTextureSize( 512, 512 );
			//tex2D->setFilter( osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR );
			//tex2D->setFilter( osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR );
			//tex2D->setInternalFormat( GL_RGBA16F_ARB );
			//tex2D->setSourceFormat( GL_RGBA );
			//tex2D->setSourceType( GL_FLOAT );
			//tex2D->bindToImageUnit( 0, osg::Texture::WRITE_ONLY );  // So we can use 'image2D' in the compute shader
			//  osg::ref_ptr<osg::Texture2D> outputTex = tex2D;


			osg::ref_ptr<osg::Texture> outputTex = pHDRTarget;
			// The compute shader can't work with other kinds of shaders
			// It also requires the work group numbers. Setting them to 0 will disable the compute shader
			osg::ref_ptr<osg::Program> computeProg = new osg::Program;
			computeProg->setComputeGroups(outputTex->getTextureWidth() / 16, outputTex->getTextureHeight() / 16, 1);
			computeProg->addShader(new osg::Shader(osg::Shader::COMPUTE, computeSrc));


			// Create a node for outputting to the texture.
			// It is OK to have just an empty node here, but seems inbuilt uniforms like osg_FrameTime won't work then.
			// TODO: maybe we can have a custom drawable which also will implement glMemoryBarrier?
			osg::ref_ptr<osg::Node> sourceNode = new osg::Geometry;
			sourceNode->setDataVariance(osg::Object::DYNAMIC);
			sourceNode->getOrCreateStateSet()->setAttributeAndModes(computeProg.get());
			sourceNode->getOrCreateStateSet()->addUniform(new osg::Uniform("inTex", (int)0));
			sourceNode->getOrCreateStateSet()->addUniform(new osg::Uniform("outTex", (int)7));

			sourceNode->getOrCreateStateSet()->setTextureAttributeAndModes(0, pColorBuffer.get());
			sourceNode->getOrCreateStateSet()->setTextureAttributeAndModes(7, outputTex.get());

			screenQuad->getOrCreateStateSet()->setTextureAttributeAndModes(0, outputTex.get());

			// Create the scene graph and start the viewer
			pass->insertChild(0, sourceNode);
			pass->setViewport(0, 0, 0, 0); // 不绘制screen，只计算compute着色器结果
										   /* pass->addChild(screenQuad);*/
#endif 
		}
	}
#else
	if (_pStage)
	{
		osg::ref_ptr<osg::Texture> pHDRTarget = nullptr;
		osg::ref_ptr<osg::Texture> pColorBuffer = nullptr;
		GraphicsPipeline* pPipeline = _pStage->getGraphicsPipeline();
		if (pPipeline)
		{
			pColorBuffer = pPipeline->getTexture("ColorBuffer");
			pHDRTarget = pPipeline->getTexture("HDRTarget");
		}

		Effect::PassData passData;
		_pStage->getPassData("computePass", passData);
		osg::ref_ptr<osg::Camera> pass = passData.pass;
		if (pass)
		{
			// So we can use 'image2D' in the compute shader
			osg::ref_ptr<osg::BindImageTexture> colorBufferbinding = new osg::BindImageTexture(0, pColorBuffer.get(), osg::BindImageTexture::READ_ONLY, GL_RGBA8);
			osg::ref_ptr<osg::BindImageTexture> HDRBufferbinding = new osg::BindImageTexture(7, pHDRTarget.get(), osg::BindImageTexture::WRITE_ONLY, GL_RGBA16F_ARB);

			osg::ref_ptr<osg::Node> screenQuad = pass->getChild(0);
			osg::ref_ptr<osg::Texture> outputTex = pHDRTarget;
			// The compute shader can't work with other kinds of shaders
			// It also requires the work group numbers. Setting them to 0 will disable the compute shader
			osg::ref_ptr<osg::Program> computeProg = new osg::Program;

			computeProg->addShader(new osg::Shader(osg::Shader::COMPUTE, computeSrc));

			// Create a node for outputting to the texture.
			// It is OK to have just an empty node here, but seems inbuilt uniforms like osg_FrameTime won't work then.
			// TODO: maybe we can have a custom drawable which also will implement glMemoryBarrier?

			osg::Vec3 renderSize = pPipeline->getRenderTargetResolution();
			//osg::ref_ptr<osg::Node> sourceNode = new osg::DispatchCompute(renderSize.x() / 8, renderSize.y() / 8, 1);
			int x = (1 + (renderSize.x() - 1) / 8);
			int y = (1 + (renderSize.y() - 1) / 8);
			osg::ref_ptr<osg::Node> sourceNode = new osg::DispatchCompute(x, y, 1);
			sourceNode->setDataVariance(osg::Object::DYNAMIC);
			sourceNode->getOrCreateStateSet()->setAttributeAndModes(computeProg.get());
			sourceNode->getOrCreateStateSet()->addUniform(new osg::Uniform("inTex", (int)0));
			sourceNode->getOrCreateStateSet()->addUniform(new osg::Uniform("outTex", (int)7));

			sourceNode->getOrCreateStateSet()->setTextureAttributeAndModes(0, pColorBuffer.get());
			sourceNode->getOrCreateStateSet()->setTextureAttributeAndModes(7, outputTex.get());

			screenQuad->getOrCreateStateSet()->setTextureAttributeAndModes(0, outputTex.get());
			screenQuad->getOrCreateStateSet()->setAttributeAndModes(colorBufferbinding);
			screenQuad->getOrCreateStateSet()->setAttributeAndModes(HDRBufferbinding);
			// Create the scene graph and start the viewer
#define SHOW_COMPUTE_OUTPUT_TO_SCREEN 0
#if SHOW_COMPUTE_OUTPUT_TO_SCREEN
			pass->insertChild(0, sourceNode);

#else
#if 1
			pass->insertChild(0, sourceNode);
			//pass->replaceChild(0, sourceNode);
			pass->setViewport(0, 0, 0, 0); // 不绘制screen，只计算compute着色器结果
										   /* pass->addChild(screenQuad);*/
#else 
			pass->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER);
			pass->replaceChild(0, sourceNode);
#endif 
#endif 
		}
	}
#endif 
	return true;
}
RenderPass* RenderStageCallback::createNewPass(PassType type, const std::string& name)
{
	if (!_pStage)
		return nullptr;
	auto renderPass = _pStage->getGraphicsPipeline()->createRenderPass(type, name, _pStage);
	assert(renderPass && "create camera failed!");
	return renderPass;
	
}

void RenderStageCallback::registerSharedData(const std::string& name, osg::Texture* tex)
{
	_pStage->getGraphicsPipeline()->setTexture(name, tex);
}
osg::Texture* RenderStageCallback::getSharedData(const std::string& name)
{
	return _pStage->getGraphicsPipeline()->getTexture(name);
}
void RenderStageCallback::attachCamera(osg::Camera* camera,const std::initializer_list<osg::ref_ptr<osg::Texture>>& vioTextureAttachments)
{
	int numAttached = 0;
	for (auto& e : vioTextureAttachments)
	{
		ETextureAttachmentType  type = ETextureAttachmentType::ColorTexture;
		type = getTextureAttatchmentType(e);
		osg::Camera::BufferComponent bc = osg::Camera::COLOR_BUFFER;
		switch (type)
		{
			case ETextureAttachmentType::ColorTexture:
				bc = (osg::Camera::BufferComponent)(osg::Camera::COLOR_BUFFER0 + numAttached++);
				break;
			case ETextureAttachmentType::DepthTexture:
				bc = osg::Camera::DEPTH_BUFFER;
				break;
			case ETextureAttachmentType::DepthAndStencilTexture:
				bc = osg::Camera::PACKED_DEPTH_STENCIL_BUFFER;
				break;
			case ETextureAttachmentType::StencilTexture:
				bc = osg::Camera::STENCIL_BUFFER;
				break;

			case ETextureAttachmentType::CubeDepthTexture: // cubemapBuffer
			case ETextureAttachmentType::DepthArrayTexture:
			{
				_ASSERT(0);// todo
			}
					break;
		default:
			bc = osg::Camera::COLOR_BUFFER;
		}
	
		camera->attach(bc, e.get());
	}
	if (!numAttached)
	{
		camera->setRenderOrder(osg::Camera::NESTED_RENDER);
		camera->setClearMask(0);
		
		if (camera->getNumChildren() > 0)
		{
			if (auto geode = camera->getChild(0)->asGeode())
			{
				if (geode->getNumChildren() > 0)
				{
					if (auto drawable = geode->getChild(0)->asDrawable())
					{
						drawable->setDrawCallback(new DrawableCallbackMarker());
						drawable->setName(camera->getName());
					}
				}
			}
		}
		// Note that FBO should be changed back to FRAME_BUFFER for handling camera resizing
		camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER);
	}
}
}

