//--------------------------------------------------------------------------------------
// Copyright (c) longlongwaytogo. All rights reserved.
// longlongway2012@hotmail.com
//--------------------------------------------------------------------------------------

#include <Frame/Frame_export.h>
#include <osg/Node>
#include <osg/Texture2D>
#include <osg/Texture3D>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>
#include <osg/io_utils>
#include <common/common.h>
#include "utils.h"

namespace Frame
{
	osg::Node* getDefaultModel()
	{
        osg::Node* model = nullptr;
        std::string saveFilePath = Comm::GlobalVars::getInstance().getDataPath() + "/sponza/sponza.osgb";
        if (osgDB::fileExists(saveFilePath))
            model = osgDB::readNodeFile(saveFilePath);
        else {
            std::string filePath = Comm::GlobalVars::getInstance().getDataPath() + "/sponza/sponza.obj";
            model = osgDB::readNodeFile(filePath);
			osgDB::Options* opt = new osgDB::Options("WriteImageHint=UseExternal Compressor=zlib");
            osgDB::writeNodeFile(*model, saveFilePath, opt);
        }
        return model;
	}
    osg::Texture2D* createDefaultTex2d()
    {
		auto tex2d = new osg::Texture2D();
		tex2d->setInternalFormat(GL_RGBA);
		tex2d->setSourceFormat(GL_RGBA);
		tex2d->setSourceType(GL_UNSIGNED_BYTE);
		tex2d->setFilter(osg::Texture::MIN_FILTER, osg::Texture::FilterMode::NEAREST);
		tex2d->setFilter(osg::Texture::MAG_FILTER, osg::Texture::FilterMode::NEAREST);
		tex2d->setWrap(osg::Texture::WRAP_S, osg::Texture::WrapMode::CLAMP_TO_EDGE);
		tex2d->setWrap(osg::Texture::WRAP_T, osg::Texture::WrapMode::CLAMP_TO_EDGE);
		tex2d->setWrap(osg::Texture::WRAP_R, osg::Texture::WrapMode::CLAMP_TO_EDGE);
		return tex2d;
    }
}