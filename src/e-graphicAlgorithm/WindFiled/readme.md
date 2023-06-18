# WindFiled风场模拟

# 流程
# 1. drawscreen： 绘制屏幕

## 1.1 drawTexture

- pass 1： drawTexture：绘制背景图
  - 禁用depth_test
  - 禁用Stencil_test
  - 使用program3
  - 使用纹理2016112000（风场数据图）
  - 使用纹理：前一帧结果纹理图texture15
  - 绑定framebuffer到背景图纹理
  - 绘制quad

## 1.2 drawParticles 绘制粒子

- pass 2： 绘制粒子
  - 使用program 0：
  - 绑定纹理 Texture11：颜色渐变图纹理
  - 绘制粒子点

## 1.3 绘制背景缓存 

- pass3 缓存背景
  - 启用blend
  - 使用program3
  - 使用texture17，pass1渲染结果
  - 绘制quad

# 2 updateParticles 更新风场数据缓存

- pass4：更新风场矢量数据缓存
  - 绑定framebuffer到texture14，本帧texture14，作为下一帧texture15使用
  - 禁用blend
  - 使用program6
  - 绘制quad

