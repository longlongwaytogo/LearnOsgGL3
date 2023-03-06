#pragma once
/*
自定义渲染管线，使用代码生成stage和pass
*/
#include <GraphicsPipeline/GraphicsPipeline.h>
namespace Effect
{
	class CustomPipeline :public GraphicsPipeline
	{
	public:
		using GraphicsPipeline::GraphicsPipeline;
		virtual void init() override;

	};
}