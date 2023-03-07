#pragma once
#include <GraphicsPipeline/RenderPass.h>
class GBufferPass :public Effect::RenderPass
{
public:

	bool init() override;
	bool update() override;

};