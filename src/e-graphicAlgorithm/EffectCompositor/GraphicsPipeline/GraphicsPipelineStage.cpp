
#include "GraphicsPipeline/GraphicsPipelineStage.h"
#include "GraphicsPipeline/GraphicsPipeline.h"

namespace Effect
{
    bool GraphicsPipelineStage::init()
    {
        bool bRet = false;

        // load from 
		if (m_bLoadXml)
		{
			if (_xmlRoot)
			{
				bRet = m_pipeline->loadFromXML(_xmlRoot, NULL);
			}
			else
			{
				// 使用stage名称作为xml配置进行加载
				osg::ref_ptr<osgDB::Options> option = new osgDB::Options;
				option->setUserData(this);
				_xmlRoot = m_pipeline->loadXML(m_name + ".xml", option);
				bRet = _xmlRoot.valid();
				if (!bRet)
					m_bLoadXml = false;
			}
		}
       

        if(m_callback)
            m_callback->init();
        else
        {
            // default init
        }
        return bRet;
    }

    void GraphicsPipelineStage::traverse(osg::NodeVisitor& nv)
    {
        if(m_callback)
            m_callback->traverse(this,nv);
        else
        {
            for(auto itr =  m_passList.begin(); itr != m_passList.end(); itr++)
            {
				PassData& data = (*itr)->getPassData();
				if (data.pass && data.activated)
					data.pass->accept(nv);
            }
        }
    }

       bool  GraphicsPipelineStage::addPass(GraphicsPipelinePass* pass)
       {
           for(auto itr = m_passList.begin(); itr != m_passList.end(); itr++)
           {
               if((*itr)->getPassData().name  == pass->getPassData().name)
                   return false;
           }
           m_passList.push_back(pass);
           return true;
       }
        bool GraphicsPipelineStage::getPassData( const std::string& name, PassData& data ) const
        {
            
         for ( unsigned int i=0; i<m_passList.size(); ++i )
         {
            const PassData& pd = m_passList[i]->getPassData();
            if ( pd.name==name )
            {
                data = pd;
                return true;
            }
         }
         return false;
        }

        Effect::PassList& GraphicsPipelineStage::getPassList()
        {
            return m_passList;
        }

}

