RSM （Reflective ShadowMaps ） 

[论文地址](https://users.soe.ucsc.edu/~pang/160/s13/proposal/mijallen/proposal/media/p203-dachsbacher.pdf)

 # what - 什么是 RSM算法



**RSM全称：Reflective shadow Maps** 

翻译成中文就是：反射阴影贴图，但它并不是计算阴影的，而是用于计算全局光照的算法。

光照分为两种：

- 直接光照： 直接接受光源计算出来的结果。
- 间接光照：通往被照亮物体多次反射产生的次级光照效果。例如，被阳光照射的山洞入口，洞内部的墙壁也会看的到。

RSM的核心思想是把直接光源照亮的区域又作为发光物（虚拟点光源）来进行计算。

#  How - RSM是如何工作的

RSM算法实现的原理和shadowmap有类似之处，它是通过shadowmap获取的数据作为次级光源进行简介光照计算的算法，具体步骤如下：

- 第一步：使用直接光照对场景进行渲染，记录颜色，法线、位置信息

- 第二步：类似shadowmap，从光源出发，渲染场景，记录$p_x$点的反射通量（reflected radiant flux）、法线、位置

- 第三步：使用第一步，第二步的结果，以延迟光照方式计算直接光照和简介光照。直接光照可以使用phong模型，间接光照需要特殊处理，把发光区域的每个像素（shadowmap所得）看成一个虚拟的点光源（Virual point light:VPL)，然后用这些虚拟的点光源计算光照

  <img src="images/001-RSM.png" style="zoom:70%;" >

  着色点x收到的某一个虚拟点光源$x_p$的辐射强度如下所示：

  ​	$$E_p(x,n)=\Phi_p\frac{max(n.(x-x_p),0).max(n.(x_p-x))}{||x-x_p||^4}$$

  如果大家把 $\Phi_p$ 当做颜色的话，这就是一个点光源对着色点x的光照公式，这里最大的变数就在$\Phi_p$。

  

​	如果我们的直接光源是平行光（如太阳，不衰减）：

​        $\Phi_p = c_pI$

​	$c_pI$就是$x_p$点的颜色，$I$为平行光的光照强度。



​	如果直接光源是点光源$v$:

   $\Phi_p=I\frac{max(n_p.(v-x_p),0)}{||v-x_p||^4}$

其中$I$为点光源的光照强度。

- 第四步：将间接光照和直接光照融合起来（直接相加即可）

# Why - 为什么会产生这个算法

RSM算法，主要简化了全局光照的计算和开销。对于传统的光线追踪（Ray Tracing）和光能辐射（Radiosity）——这里只是提了全局光照算法中的2种主要类型——就需要几分钟或者几个小时来生成具有完整全局光照的单个图像。最近，人们在使光线追踪变得更具有交互性的方面做出了很多努力

RSM的主要流程便是使用shadowmap收集次级光源，通过延迟光照进行光照计算。

#  优化：



算法的实现非常简单，但是效率很低，如果对例如1024\*1024大小的图片进行采样，那么虚拟光源的数量也将变成1024\*1024个，因此原论文提到：**假设屏幕空间里面这个着色点越近，对光源贡献越大**。因此，多选择一个点离着色点近的VPL，并且为了弥补越往外采样点越少可能带来的问题，作者引入权重，越近权重越小，越远，权重越大，如下图：

<img src="images/001-RSM-2.png" style="zoom:50%;" />

可以看到随着采样距离变大，采样密度在逐渐减小。采样公式（选取虚拟点光源）如下：

​																	$s+rmaxξ1 sin(2πξ2),t +rmaxξ1 cos(2πξ2))$

 $(s,t)$是当前着色点的屏幕空间的坐标（UV坐标），我们可以通过生成多组（这个一组就代表多计算一次间接光照，有几个就相当于用多少个虚拟点光源），$ξ1$，$ξ2$为随机数（一组采样就需要一对随机数）代入上面的公式就可以得到一个采样坐标，然后再根据上面的光照计算公式来计算这个VPL对着色点的光照公式。注意：这种方法会有很大的误差，为了进行一些弥补，在每计算完一个虚拟点光源对着色点的光照之后应该再乘以权重$ ξ1^2$ 。

# RSM的缺点

​      其缺点**首先**在于其继承了shadow map对于直接光源数量增加而导致shadow map增加带来的开销问题；**其次，**由于忽略了可见性而导致不真实性；**再次，**假设了反射物的diffuse、假设了shadow map距离与世界空间距离近似相同导致的不真实性；**最后**，采样导致的不真实性。





# 参考

- [实时渲染｜Reflective Shadow Maps - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/369465477)

- [【论文复现】Reflective Shadow Maps - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/357259069)
- [Reflective Shadow Maps（RSM）| 翻译 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/567793415)
- [p203-dachsbacher.pdf (ucsc.edu)](https://users.soe.ucsc.edu/~pang/160/s13/proposal/mijallen/proposal/media/p203-dachsbacher.pdf)