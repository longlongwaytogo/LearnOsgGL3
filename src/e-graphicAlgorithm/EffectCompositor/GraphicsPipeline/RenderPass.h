#pragma once

#include <string>
#include <map>
#include <vector>

#include <osg/camera>
#include <osg/Shader>
#include <osg/Program>
#include <osgDB/ReadFile>
namespace Effect
{
	class RenderStage;

	enum PassType { 
	FORWARD_PASS,
	DEFERRED_PASS,
	COMPUTE_PASS,
	};
	struct PassData
	{
		bool activated;
		PassType type;
		std::string name;
		osg::ref_ptr<osg::Camera> pass;

		PassData() : activated(true), type(FORWARD_PASS) {}

		PassData& operator=(const PassData& pd)
		{
			activated = pd.activated; type = pd.type;
			name = pd.name; pass = pd.pass;
			return *this;
		}

		/** Check if the pass works like an HUD camera to display results */
		bool isDisplayPass() const
		{
			return pass.valid() && pass->getRenderOrder() != osg::Camera::PRE_RENDER;
		}
	};

	class RenderPass : public osg::Referenced
	{
	public:
		RenderPass():m_pRenderStage(nullptr)
		{
			m_passData.activated = false;
		}

		RenderPass(PassType type, const std::string& name, osg::Camera* camera) :m_pRenderStage(nullptr)
		{
			setPassData(type, name, camera);
		}
		inline osg::Camera* getCamera()
		{
			return m_passData.pass.get();
		}

		inline void setPassData(PassType type, const std::string& name, osg::Camera* camera)
		{
			m_passData.activated = true;
			m_passData.type = type;
			m_passData.name = name;
			m_passData.pass = camera;
		}

		void setRenderStage(RenderStage* rs);
		RenderStage* getRenderStage();

		void registerSharedData(const std::string& name, osg::Texture* tex);

		RenderPass(const PassData& passData):m_passData(passData)
		{
		}
		

		virtual bool init();
		virtual bool update();
		PassData& getPassData() { return m_passData; }
		/*bool operator<(const RenderPass& rhs)
		{
			return m_passData < rhs.m_passData;
		}*/
	protected:
		PassData m_passData;
		std::vector<osg::ref_ptr<osg::Shader>> m_shaders;
		RenderStage* m_pRenderStage;
	};

	typedef std::vector<osg::ref_ptr<RenderPass>> PassList;
	typedef std::map<std::string, PassList> PassListMap;


}
