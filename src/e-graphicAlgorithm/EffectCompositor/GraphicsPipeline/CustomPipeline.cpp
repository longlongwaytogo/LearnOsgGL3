#include "CustomPipeline.h"
#include <common/common.h>
#include <osgDB/FileUtils>
#include <GraphicsPipeline/GraphicsPiplelineEventCallback.h>
namespace Effect
{
	void Effect::CustomPipeline::init()
	{
	/*	std::string path = Comm::GlobalVars::getInstance().getDataPath();
		osgDB::FilePathList& filePaths = osgDB::getDataFilePathList();
		filePaths.push_back(osgDB::getFilePath(path));
		filePaths.push_back(osgDB::getFilePath(path + "/GFX/PostEffectProcessors/"));
	
	 */
		// for each stage
		for (auto itr = m_stages.begin(); itr != m_stages.end(); itr++)
		{
			/* if(*itr)
				 (*itr)->init();*/
			if (itr->second)
				itr->second->init();
		}

		addEventCallback(new GraphicPipelineEventCallback());
		
	}
}