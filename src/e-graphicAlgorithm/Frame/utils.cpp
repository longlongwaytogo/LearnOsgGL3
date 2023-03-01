#pragma once
#include <Frame/Frame_export.h>
#include <osg/Node>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>
#include <osg/io_utils>
#include <common/common.h>
namespace Frame
{
	osg::Node* getDefaultModel()
	{
        osg::Node* model = nullptr;
        std::string saveFilePath = Comm::GlobalVars::getInstance().getDataPath() + "/sponza/Scale300Sponza.osgb";
        if (osgDB::fileExists(saveFilePath))
            model = osgDB::readNodeFile(saveFilePath);
        else {
            std::string filePath = Comm::GlobalVars::getInstance().getDataPath() + "/sponza/Scale300Sponza.obj";
            model = osgDB::readNodeFile(filePath);
            osgDB::writeNodeFile(*model, saveFilePath);
        }
        return model;
	}
}