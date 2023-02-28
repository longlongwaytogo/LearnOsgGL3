
#ifndef OSGEDITOR_INCLUDE_EFFECT_PASS_H
#define OSGEDITOR_INCLUDE_EFFECT_PASS_H
#include <string>
#include <map>
#include <vector>

#include <osg/camera>
namespace Effect
{
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
	typedef std::vector<PassData> PassList;
	typedef std::map<std::string, PassList> PassListMap;


  /*  class GraphicsPipelineTechnique
    {
    public:
       PassList m_passList;
    };*/

  /*  typedef std::vector<GraphicsPipelinePass> Techinque;
    typedef std::map<std::string, GraphicsPipelinePass> TechinqueMap;*/
}

#endif // 