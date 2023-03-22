#pragma once
#include <EffectCompositor/GraphicsPipeline/RenderPass.h>
class DepthPass :public Effect::RenderPass
{
public:

	virtual bool init() final;
	virtual bool update() final;

};