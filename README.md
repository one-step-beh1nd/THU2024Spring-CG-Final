# THU 2024 春 计算机图形学基础大作业（更新中，未完结）
## 说在前面

本仓库的价值**不在代码，而在该README文件**

该仓库是贵系的一个菜鸡在被图形学折磨了一个星期后有感而发。

如果您是贵系大佬，请您移步其他仓库，因为本仓库可能并不能满足您的胃口。你并不能从该仓库中获得许多功能点的实现，甚至其中有的功能点至今仍有问题。

那我为什么要建立这个仓库呢？

图形学大作业之难度，讲解之匮乏，ppt之混乱，是我前所未见的。

本学期我有两个大作业：软工和图形学。这两个大作业都有着`讲解缺乏、上手困难`的问题。

软工给人的感觉是虽上手难，但熟练之后工作效率提高很多，“条条大路通罗马”，而且对查重要求很低；图形学给人的感觉是很难，而且路径十分有限，自己硬想一天不如别人点醒一句。

再加上课程ppt中对于参数定义十分繁杂且混乱，有时候看公式根本不知道对应什么东西，甚至能出现 `变量 + 常量`的公式（说的就是cook torrance）。

所以被折磨后，我希望能借此文件帮助同学`快速上手图形学作业`。我将在该文件中，`讲解我实现的功能点的实现路径`，防止出现看着学长的代码都不知道怎么抄的情况。

## 致谢与参考
在这里远程感谢以下代码对我写大作业的帮助：

[学长的代码](https://github.com/zhanglbthu/THU2023Spring-CG-FinalProject) 实现了

- whitted style光线追踪（疑似有bug）
- path tracing，不包括NEE、glossy材质
- 参数曲面求交（有明显的bug，可能他报告里的图不是他生成的）
- **OPENMP加速（不作为得分点，但可以大大减少渲染时间，从而更快出结果）**
- 光子映射（不再得分，因为太多人抄往年的这个算法）
- 纹理贴图
- 景深相机
- cos-weighted采样
- 抗锯齿（然而抗的效果并不好）
- 复杂三角网格（我没写，不敢保证他的一定对）
- 包围盒（同上）
- BVH树（同上）
- 法向插值（同上）


[smallpt](https://blog.csdn.net/g11d111/article/details/108189198) 请不要看99行的，去看二百多行的。其中包含

- path tracing，包括NEE，不包括glossy材质
- 抗锯齿（效果很好，但是时间耗费更多，约是上面学长的四倍）
- OPENMP加速
- cos-weighted采样

[基于cook-torrance模型实现glossy材质](https://zhuanlan.zhihu.com/p/160804623)
- 如题，其效果确实好，但有需要注意的地方，见后 [glossy材质](#42-glossy材质)

## 一、 效果图
### 路径追踪，包括漫反射（左右墙壁）、理想反射（左边球）、理想折射（右边球）、抗锯齿

![路径追踪](./results/cornell_anti.bmp)
-
### glossy材质：一种哑光粗糙面

![glossy](./results/glossy.bmp)
-
### NEE:以更少的samplesPerPixel（spp，这个概念在后面讲到）获得更好的效果。第一张图为 NEE在20spp的结果，第二张图为 无NEE在20spp的结果

![Nee](./results/NEE_20.bmp)
![noNee](./results/noNEE_20.bmp)
-
### 景深：焦距处的物体清晰，其他地方的物体模糊
![Depth](./results/depth.bmp)
-
### 纹理贴图：把二维图片贴到三维物体上
![texture](./results/texture.bmp)
-
### 参数曲面求交：PA2中我们是用OPENGL自动算求交的（当然大作业肯定是不能用OPENGL），所以如果你发现自己的ray tracing或者path tracing跑PA2的测例是空的，就是因为你没有自己写求交。下面的图在与视线平齐的地方有瑕疵，但比学长的好。学长的半个花瓶都变成背面了
![curve](./results/china.bmp)
-
### 运动模糊
![move](./results/move.bmp)

## 二、 大作业要求
详见本仓库 “大作业要求.txt”
## 三. 对大作业的感受
### 3.1 对我实现的功能点的感受

由于本菜鸡并没有实现其他功能，所以对其他的难度并不知晓。

如果你照着smallpt（99行）写的基础的path tracing，那么已经有了cos-weighted采样、抗锯齿这两个加分项。如果你照着smallpt（二百多行）写的，那么应该有NEE了。

cos-weighted采样，smallpt本身就用的这个方法，所以照着smallpt写自动获得这个加分项。**但是助教说：因为smallpt有，所以这个采样相较于其他的采样，分值会更低**

抗锯齿，smallpt本身就有抗锯齿，效果很好但是**需要更多渲染时间**

glossy材质**不要看ppt！！！不要看ppt！！！不要看ppt！！！**看ppt不如看我给你讲。[glossy](#42-glossy材质)

参数曲面求交，作为高加分项，难度确实大，而且直到ddl我都没有解决在与视线平齐的求交问题。不知道能拿几分。

景深较为简单，只需要改一个文件，写一个类即可。

运动模糊也较为简单。如果你只写球的运动模糊，只需要在大作业前的代码基础上加一行代码即可（前提是你已经有了path tracing）

纹理贴图的代码量相比较于分值来说较大。如果只实现球的贴图、平面贴图的话难度不大。**注意：实现曲面贴图的前置条件是实现曲面求交**



### 3.2 对实现顺序的感受
虽然要求中建议你先实现whitted-style ray tracing，但是我建议直接写path tracing。

一方面，path tracing有现成的smallpt的代码，照抄再改改一定能出来，而whitted-style的代码少的可怜，至少我没找到很好的，不管是学长的还是其他的

另一方面，最后验收分数，如果你有path tracing的话，直接给到path tracing的分（包括了whitted-style），而whitted-style的给分只是在你没写path tracing的时候才细看的。

所以我也不讲whitted-style了，仓库里也没有。因为我的whitted-style可能遗留了未解决的问题。

## 四、具体功能实现讲解
在这一节中，我不会将具体的公式原理，因为我也看不懂。我会从基于PA2的代码开始，告诉你需要修改哪些文件、需要添加哪些参数。具体的代码还得自己写。

**查重越来越严格了！！！**

### 4.1 参考smallpt的path tracing：漫反射、理想反射、理想折射、cos-weighted采样、抗锯齿
path tracing的原理，不讲，网上一堆。

#### 4.1.1 path tracing的过程：

1. 和光线投射一样，从相机到像素点产生光线，然后光线与场景中的物体求交，这个光线称为**入射光线**

2. 求交之后，我们根据不同的材质，计算出不同的出射方向，以交点为新的起点，出射方向为光线方向，这个光线成为**出射光线**，然后对出射光线递归，递归的结果乘交点处的`diffuseColor`，再加上交点处的自发光的颜色。这就是计算得到的颜色

#### 4.1.2 实现path tracing的前置条件
上面的`diffuseColor`，和光线投射中的`diffuseColor`是同一个。为什么是同一个呢？你可以把`diffuseColor`理解为，光线打到这个点后，颜色（三维向量）的**RGB三个分量的衰减率**。

而我们所谓的“入射光线”，其实是按照光路的可逆原理的，从交点到相机的光线的反向。所谓的“出射光线”，才是真实的光线。所以我们要把递归得到的结果乘以这个点的衰减率。

既然是衰减率，那么一定是在[0,1]之间。这从直观上也很好理解：这个点最多就把递归的结果（即递归得到的颜色）原样返回，而不会增强返回。如果增强的话，颜色值凭空变大，意味着能量凭空变多。**这个概念有助于我们理解glossy材质的实现。**

由于不同的材质，出射方向也不同，所以我们需要对不同的材质做区分。这就需要对`Matetial`类进行修改。

目前我们需要区分三种材质：漫反射、理想反射、理想折射。通俗地可以把他们理解成：水泥墙、镜子、玻璃。所以可以用一个`Vector3f`来区分，三个分量分别对应三种材料。每种材料对应的分量为 1，其他分量为 0。
```
Vector3f type;  // (漫反射，理想反射，理想折射)
```

注意：我们无需实现混合效果，所以互斥的分量是完全可以的。

注意：所谓“理想折射材质”，也就是“玻璃”材质，在效果上既有折射现象又有反射现象。

既然有折射，那就需要有折射率，所以又需要加变量：
```
float refractiveIndex;    // 折射率
```

然后，上面提到 “要加上交点处的自发光的颜色”。这是因为path tracing我们是对光线递归，如果递归的光线没有打到光源，则就是一片漆黑。

而**点光源很小，光线很难打得到**。所以我们需要面光源！

如何实现面光源呢？我们可以让物体发光！

物体如何发光？给`Material`类加变量，这就是自发光项。一个三维变量，代表这个材质发光的颜色。
```
Vector3f emission;    // 自发光项
```

总结一下，到此你的`Material`类，加上之前PA的框架，应该有这些变量：

```
Vector3f diffuseColor;   // 漫反射颜色
Vector3f specularColor;  // 反射颜色
float shininess;         // 锐利度

float refractiveIndex;   // 折射率
Vector3f type;           // （漫反射，镜面反射, 折射）
Vector3f emission;       // 自发光项
```

这些参数都是从测例中读取的，而读取的函数是`SceneParser`类，所以我们还需要修改`SceneParser`类中对`Material`类的读取。

后面的讲解中，如果修改了某些类，一般都需要修改对应的`SceneParser`类的读取函数。如果我忘了，读者别忘。

#### 4.1.3 path tracing 出射方向的计算
目前，我们能够区分三种材质，那么接下来我们就需要计算不同材质的出射光线的方向。

首先，最简单的是理想反射，直接套公式即可，不讲。

然后是漫反射。什么是漫反射？光线打到面上后向半球面的每个方向都会有光线出去。这就有两个问题：

1. 不同方向的出射光线的占比是多少？（我对概念的理解非常混乱，懂我意思就行）

这个问题对应到要求中的“采样方法”，采样方法就是，不同方向的出射光线占多少比例。

我们当然可以采取均匀取样，即不同方向的出射光线的比例都相同。这里我们讲解smallpt实现的cos-weighted采样。

cos-weighted采样，即出射光线的立体角与交点的法向的夹角的余弦值越大，占比越高。具体的推导请上网查。

2. 半球面理论上有无穷多个方向，而不同的方向可能占比都不一样，我们该怎么计算如此多的方向？

如果每次仅递归有限个方向，少了：计算结果并不准确；多了：递归爆炸。

这就是基础要求中的**蒙特卡洛积分**。如果你这学期上了《人工智能导论》，那么此时你应该对四子棋大作业中的蒙特卡洛模拟有了感受。我对蒙特卡洛方法浅显的理解是，**占比即概率，用概率 + 大量模拟**的方法，来近似计算我们需要的结果。

所以我们的处理方法是：**按照概率来每次随机选取一个出射的方向，然后每层递归只递归这一个方向，通过对每个像素点发出许多条光线，来近似模拟得到不同方向的占比**。

最后是折射。首先根据公式计算折射后的方向。

注意需要判断折射是从介质（折射率大于1）到空气（折射率等于1），还是从空气到介质。如果是前者，需要判断是否发生全反射现象。

然而，光线打到折射的物体表面，并不会完全进入物体，还是有部分光线被反射了的。也就是说，**我们定义的“理想折射”材质，最终实现效果既有折射现象，又有反射现象**

反射的光线占多少呢？这就需要菲涅尔方程来计算。然而这个方程计算十分复杂，所以我们用 Schlick近似来近似计算反射光线的占比。具体的计算请自行查阅。

然后“占比”即概率，按照概率随机选择我们要取反射方向还是折射方向，通过大量模拟来近似占比。

那折射的方向是固定的，怎么办呢？同样进行蒙特卡洛模拟，既然是固定的，采样很多次也不会影响结果，取平均就好。

#### 4.1.4 OPENMP加速
在`path_tracing.hpp`中你能看到这个
```
#pragma omp parallel for schedule(dynamic, 1)
```
这是使用了OPENMP进行多线程（并行）的计算后面的循环，能够明显减少渲染的时间。

**重要：如果你在代码中使用了打印内容来debug，请关闭OPENMP加速，因为并行的运行循环会让你的输出混乱**

如果想要使用OPENMP（我也建议使用），请在`CMakeLists.txt`中加入以下内容，加入的位置参考我的文件：

```
SET(CMAKE_BUILD_TYPE "Release")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb -fopenmp")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall -fopenmp")
```

#### 4.1.5 代码讲解：path_tracing.hpp
在path_tracing.hpp代码中，有两个函数：`render`, `pathTracer`。

`render`负责文件的读取、相机到像素的光线的产生。

`pathTracer`负责对于一条光线，计算返回的颜色。

在`render`中，你能看到对每个像素点，调用了`samplesPerPixel`次`pathTracer`，然后取平均。这里就是在利用蒙特卡洛方法，大量模拟次数。`samplesPerPixel`，即前面提到的`spp`，就是我们对每个像素采样多少次。

理论上采样次数多一些效果更好。如果你在debug期间，建议采样几百到1k次，并将图片的像素调低，以减少渲染时间（在testcases的相机中修改width和height，例如改为200）。如果你已经实现好，要得到报告用图片，建议渲染1k次以上，像素在640*480以上（作业要求）。

这个spp是不使用NEE的情况下，能有比较好的效果。如果使用NEE，那么用更少的spp即可获得差不多的效果。


在`render`中产生像素(i, j)对应光线的时候，加入了一个随机数的扰动。这就是比较弱的抗锯齿。比较强的抗锯齿请参考smallpt。其方法是在计算(i, j)处像素颜色时，取其附近四个像素点，分别采样`spp`次取平均，再对四个像素取平均。效果更好，速度更慢，建议不要将这个在debug期间使用。

在`pathTracer`中，我们是按照`Material->type`的三个分量，作为该材质在三种材质上的占比。这种实现实际上包含了三种材质的混合，然而我们无需实现混合。

所以如果你保证在输入文件中，`type`的三个分量互斥（只有一个为1），那么也可以通过判断三个分量是否大于零来判断是哪种材质。

`pathTracer`中，对于漫反射、理想反射、理想折射的方向的计算，调用了对应的函数，请自己阅读源代码。

俄罗斯轮盘赌：递归达到一定深度后，按照某个概率来决定该次递归是否要停止。这个概率自己定，可以是常量（如我的代码），也可以是动态计算的量（如smallpt的代码）。
```
hit_color = hit_color * (1.0 / P_RR);
```
这里为什么要乘后一项，我觉得是为了使得蒙特卡洛模拟计算出的为无偏期望。具体概念请参考《概率论与数理统计》课程。

### 4.2 glossy材质

### 4.3 Next Event Estimation

### 4.4 运动模糊：以球为例

### 4.5 景深相机   

### 4.6 纹理贴图

### 4.7 参数曲面求交