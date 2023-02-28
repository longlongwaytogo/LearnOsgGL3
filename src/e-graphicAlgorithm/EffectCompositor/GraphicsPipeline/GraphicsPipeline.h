//////////////////////////////////////////////////////////////////////////
//@file: GraphicsPipeline.h
//@brief: ��Ⱦ���߻���
//@autor: longlongwaytogo
//@date: 2021/01/18


#ifndef __EFFECT_GRAPICSPIPLINE_H__
#define __EFFECT_GRAPICSPIPLINE_H__

#include <memory>
#include <map>
#include <array>

#include <osg/Group>
#include <osg/Geode>
#include <osg/Texture>
#include <osg/Program>
#include <osg/Camera>
#include <osgDB/Options>
#include <osgDB/XmlParser>


#include "EffectCompositor"
#include "GraphicsPipeline/GraphicsPipelineStage.h"


namespace Effect
{

enum GraphicPipelineStateType
{

    STAGE_SHADOW_MAP,
    STAGE_SCENE_BUFFER,
    STAGE_SCENE_FORWARD,
    STATE_SCENE_CUSTOM,
    STATE_SCENE_NUM,

    STATE_FIRST_ASYNC_COMPUTE = STATE_SCENE_NUM,
     STAGE_TILEDSHADING,// add 
    //regular stages
    STAGE_SCENE_DEPTH,
    STAGE_HEIGHTMAP_AO,
    STAGE_SCREENSPACE_OBSCURANCE,
    STAGE_SCREENSPACE_REFLECTIONS,
    STAGE_SCRRENSPACE_SSS,
    STAGE_VOLUMETRIC_FOG,
    STAGE_FOG,
    STAGE_SKY,
    STAGE_WATER_RIPPLE,
    STAGE_WATER,
    STAGE_MOTION_BLUR,
    STAGE_DEPTH_OF_FIELD,
    STAGE_AUTO_EXPOSURE,
    STAGE_BLOOM,
    STAGE_COLOR_GRADING,
    STAGE_TONE_MAPPING,
    STAGE_SUN_SHAFTS,
    STAGE_POSTAA,
    STAGE_CLIP_VOLUMES,
    STAGE_DEFERRED_DECALS,
    STAGE_SHADOW_MASK,
    STAGE_LENS_OPTICS,
    STAGE_POST_EFFECT,
    STAGE_RAIN,
    STAGE_SNOW,
  
    STAGE_DISPLAY,
    STAGE_DEBUG,

    STAGE_COUNT,

};

// ��������Ⱦ�ܾQ
class EFFECTCOMPOSITOR_API GraphicsPipeline : public  EffectCompositor
{
public:
    friend   GraphicsPipelineStage;

    virtual void init();
	virtual void resize(int renderWidth, int renderHeight);
	// Prepare all stages before actual drawing starts
	//virtual void Update(EShaderRenderingFlags renderingFlags);

    virtual void traverse(osg::NodeVisitor& nv);

     template<typename StageCallback>
     void registerStage(int id,const std::string& name)
     {
         m_stages[id] = new GraphicsPipelineStage(this,id,name,new StageCallback);
     }
	
    GraphicsPipeline();
    GraphicsPipeline( const GraphicsPipeline& copy, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY );
    META_Node( Effect, GraphicsPipeline );

     /** Create new pass and add it to the end of the pass list */
   /* virtual osg::Camera* createNewPass( PassType type, const std::string& name,GraphicsPipelineStage* stage );*/

       // ���´�xmlnode����
   bool reLoadFromXmlNode();
protected:
    // �����ļ�������xmlnode
     osg::ref_ptr<osgDB::XmlNode> loadXML( const std::string& filename, const osgDB::Options* options = NULL);

  
protected:

    std::map<int,osg::ref_ptr<GraphicsPipelineStage>> m_stages;
    /*osg::ref_ptr<osgDB::XmlNode> m_preDefXmlRoot*/;
    //std::array<std::ref_ptr<GraphicsPipelineStage>,STAGE_COUNT> m_stages;

};  

}

#endif // __EFFECT_GRAPICSPIPLINE_H__