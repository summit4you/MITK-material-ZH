MITK 入门指南 -- 创建三维视图
===========================

和上一节相似，在这一节我们完成多个数据集的加载（多种图像、曲面和其它格式）。不同的时，我们以一个3维视图显示。 通过向mapper槽设置为Standard3D，则窗口类[QmitkRenderWindow](http://docs.mitk.org/2012.06/classQmitkRenderWindow.html)可以用作3维视图。因为体绘制是一个相当慢的处理过程 ，因此默认图像不在3D视图显示。

> 译注：槽是QT信号机制的概念，是一个响应函数的代称。mapper是3D场景中的术语，它来源于舞台的背景，即展示的道具。在这里即是场景的一些设置。除此之外还会遇见术语Actor，它来源于舞台的演员。在3D场景就是指施动的类，用于根据用户交互来渲染场景。

为了说明打开体绘制的区别，这里分别列出。a是默认的显示结果，b是打开体绘制的结果。和上一节一样，要得到显示的图片效果，需要在运行程序时传入

- 图像文件`bin/CMakeExternals/Source/MITK-Data/Pic3D.nrrd`

- 曲面文件`src/MITK/Modules/MitkExt/Testing/Data/lungs.vtk`

若你的MITK源文件中没有包括此测试数据，你可以通过[在线下载](http://mitk.org/git/?p=MITK-Data.git;a=tree)。

* [step3.cpp](http://docs.mitk.org/2012.06/Step3_8cpp-example.html) 包含创建3维视图和打开体绘制的代码。

A - 关闭体绘制
-------------
<center>![关闭体绘制](http://docs.mitk.org/2012.06/step3a_result.png)</center>

告知渲染器创建3D视图

	// Use it as a 3D view!
  	renderWindow.GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);

其余代码不需要修改。

上述代码读入了图像数据和曲面数据，但是由于关闭体绘制，所以图片数据没有显示。

B - 打开体绘制
-------------

<center>![打开体绘制](http://docs.mitk.org/2012.06/step3b_result.png)</center>

如上图所示，现在体绘制已经被打开

包含图像数据的数据结点的属性"volumerendering"必须置**真**。


      // Check if the data is an image by dynamic_cast-ing the data
      // contained in the node. Warning: dynamic_cast's are rather slow,
      // do not use it too often!
      mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(node->GetData());
      if(image.IsNotNull())
      {
        // Set the property "volumerendering" to the Boolean value "true"
        node->SetProperty("volumerendering", mitk::BoolProperty::New(true));

[上一节](step2.md)[下一节](step4.md)[返回](../MITK-tutorial.md)