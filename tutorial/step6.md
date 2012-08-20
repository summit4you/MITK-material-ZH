MITK 入门指南 -- 交互式区域增长
============================

本节继续前面的程序增加一个交互式的区域增长。在本节，我们将看到MITK图像类是如何访问ITK图像类的。在这个例子中，我们仅需要加载图像文件，因为曲面文件是区域增长的结果。它位于`bin/CMakeExternals/Source/MITK-Data/Pic3D.nrrd`。若你的MITK源文件中没有包括此测试数据，你可以通过[在线下载](http://mitk.org/git/?p=MITK-Data.git;a=tree)。

> [区域增长](http://en.wikipedia.org/wiki/Region_growing)是一个经典的基于灰度空间的分割算法，交互式区域增长是指可以由用户以鼠标交互方式选择空间点作为种子点进行图像分割。

![运行结果](http://docs.mitk.org/2012.06/step6_result.png)

本节源代码分开成几个文件：


- [Step6.cpp](http://docs.mitk.org/2012.06/Step6_8cpp-example.html)

- [Step6.h](http://docs.mitk.org/2012.06/Step6_8h-example.html)

- [Step6RegionGrowing.txx](http://docs.mitk.org/2012.06/Step6RegionGrowing_8txx-example.html)

- [Step6RegionGrowing1.cpp](http://docs.mitk.org/2012.06/Step6RegionGrowing1_8cpp-example.html)

- [Step6RegionGrowing2.cpp](http://docs.mitk.org/2012.06/Step6RegionGrowing2_8cpp-example.html)

- [Step6main.cpp](http://docs.mitk.org/2012.06/Step6main_8cpp-example.html)

在[Step6.cpp](http://docs.mitk.org/2012.06/Step6_8cpp-example.html)定义了一个继承于QMainWindow的类，它重载方法用于初始化程序中的widgets。Step6RegionGrowing.cpp包含用于执行区域增长的方法。[Step6main.cpp](http://docs.mitk.org/2012.06/Step6main_8cpp-example.html)包含main方法。与ITK和VTK类型，类成员以m_打头后接成员名，成员名的第一个字母大写(例如,m_Tree)。函数名以大写开头。想了解更新MITK代码风格，请阅读[The MITK Style Guide](http://docs.mitk.org/2012.06/StyleGuideAndNotesPage.html "The MITK Style Guide")。

widgets和之前的章节一样初始化，除了增加一个QVBox用于放置分割按钮。

	// Create controlsParent widget with horizontal layout
	QWidget *controlsParent = new QWidget(this);
	this->layout()->addWidget(controlsParent);
	QHBoxLayout* hlayout = new QHBoxLayout(controlsParent);
	hlayout->setSpacing(2);
	QLabel *labelThresholdMin = new QLabel("Lower Threshold:", controlsParent);
	hlayout->addWidget(labelThresholdMin);
	m_LineEditThresholdMin = new QLineEdit("-1000", controlsParent);
	hlayout->addWidget(m_LineEditThresholdMin);
	QLabel *labelThresholdMax = new QLabel("Upper Threshold:", controlsParent);
	hlayout->addWidget(labelThresholdMax);
	m_LineEditThresholdMax = new QLineEdit("-400", controlsParent);
	hlayout->addWidget(m_LineEditThresholdMax);

这个按扭的clicked()信号与方法StartRegionGrowing()关联：
	QPushButton* startButton = new QPushButton("start region growing",
	connect(startButton, SIGNAL(clicked()), this, SLOT(StartRegionGrowing()));

以ITK图像类型方式访问MITK图像类型
------------------------------

ITK图像类型是模板化的，而`mitk::Images`不是。为了使用ITK广泛的图像处理算法。为了完成这一任务，首先定义一个访问方法，它是以itkImage为参数的模板函数：

	template<TPixel, VImageDimension>
	MyAccessMethod(itk::Image<TPixel, VImageDimension>* itkImage)
	{
	...
	}

> 在ITK中，图像处理算法均称为filter。这一术语来源于数字信号处理。[ITK](http://www.itk.org/)是一个有名的医学图像处理开源包。

如果你不了解模板函数语法，你应该随便找一本C++教材读读。理解模板的概念对于正确使用ITK十分重要。

为了让一个非模板化的`mitk::Image`访问这个模板方法，你可以使用定义在[mitkImageAccessByItk.h](http://docs.mitk.org/2012.06/mitkImageAccessByItk_8h.html)的AccessByItk宏。这个宏可以检测`mitk::Image`实际的图像类型，并作必要的转换。如果了解，请参见"Modules / Adaptor classes"。

	AccessByItk(mitkImage, MyAccessMethod)

在本节我们访问的方法称作[RegionGrowing()](http://docs.mitk.org/2012.06/Step6RegionGrowing_8txx.html#aff8261268c6bb7f2e947845e4ea82d49)（它定义在[Step6RegionGrowing.txx](http://docs.mitk.org/2012.06/Step6RegionGrowing_8txx-example.html))

	template < typename TPixel, unsigned int VImageDimension >
	void RegionGrowing( itk::Image<TPixel, VImageDimension>* itkImage, Step6* step6 )
	{
	  typedef itk::Image< TPixel, VImageDimension > ImageType;
	  typedef float InternalPixelType;
	  typedef itk::Image< InternalPixelType, VImageDimension > InternalImageType;
	  mitk::Geometry3D* geometry = step6->m_FirstImage->GetGeometry();
	  // create itk::CurvatureFlowImageFilter for smoothing and set itkImage as input
	  typedef itk::CurvatureFlowImageFilter< ImageType, InternalImageType > 
	    CurvatureFlowFilter;
	  typename CurvatureFlowFilter::Pointer smoothingFilter = CurvatureFlowFilter::New();
	  
	  smoothingFilter->SetInput( itkImage );
	  smoothingFilter->SetNumberOfIterations( 4 );
	  smoothingFilter->SetTimeStep( 0.0625 );
	 
	  // create itk::ConnectedThresholdImageFilter and set filtered image as input
	  typedef itk::ConnectedThresholdImageFilter< InternalImageType, ImageType > RegionGrowingFilterType;
	  typedef typename RegionGrowingFilterType::IndexType IndexType;
	  typename RegionGrowingFilterType::Pointer regGrowFilter = RegionGrowingFilterType::New();
	  
	  regGrowFilter->SetInput( smoothingFilter->GetOutput() );
	  regGrowFilter->SetLower( step6->GetThresholdMin() );
	  regGrowFilter->SetUpper( step6->GetThresholdMax() );
	  
	  // convert the points in the PointSet m_Seeds (in world-coordinates) to
	  // "index" values, i.e. points in pixel coordinates, and add these as seeds
	  // to the RegionGrower
	  mitk::PointSet::PointsConstIterator pit, pend = step6->m_Seeds->GetPointSet()->GetPoints()->End();
	  IndexType seedIndex;
	  for (pit = step6->m_Seeds->GetPointSet()->GetPoints()->Begin(); pit != pend; ++pit)
	  {
	    geometry->WorldToIndex(pit.Value(), seedIndex);
	    regGrowFilter->AddSeed( seedIndex );
	  }
	  
	  regGrowFilter->GetOutput()->Update();
	  mitk::Image::Pointer mitkImage = mitk::Image::New();
	  mitk::CastToMitkImage(regGrowFilter->GetOutput(), mitkImage);
	  if (step6->m_ResultNode.IsNull())
	  {
	    step6->m_ResultNode = mitk::DataNode::New();
	    step6->m_DataStorage->Add(step6->m_ResultNode);
	  }

此外，访问函数要求为所有数据类型和2/3维实例化。由于对于一些编译器如果没有显示实例化会导致内存问题，因此，需要为2维和3维提供不同的实例化，在这里我们分开两个文件:
	
- 对于2维，定义在[Step6RegionGrowing1.cpp](http://docs.mitk.org/2012.06/Step6RegionGrowing1_8cpp-example.html)


- 对于3维，则定义在[Step6RegionGrowing2.cpp](http://docs.mitk.org/2012.06/Step6RegionGrowing2_8cpp-example.html)

StartRegionGrowing()方法在最后调用我们的访问方法[RegionGrowing()](http://docs.mitk.org/2012.06/Step6RegionGrowing_8txx.html#aff8261268c6bb7f2e947845e4ea82d49):

	void Step6::StartRegionGrowing()
	{
	  AccessByItk_1(m_FirstImage, RegionGrowing, this);
	  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
	}

ITK图像与MITK图像之间的相互转换
----------------------------
在某些情况，我们可以简单地在ITK和MITK图像类型间作转换。由ITK转MITK是十分简单的，因为[mitk::Image](http://docs.mitk.org/2012.06/classmitk_1_1Image.html)可以处理大部分数据类型。而由MITK转ITK则相对复杂一点，因为ITK图像不得不在编译时以一个确定的像素类型和确定的维数实例化。
	

- [mitk::Image](http://docs.mitk.org/2012.06/classmitk_1_1Image.html) [mitk::ImportItkImage](http://docs.mitk.org/2012.06/group__Adaptor.html#gab8fdb347545821af9a8c885098c339fe)(itk::Image<...>) 


- [mitk::CastToItkImage](http://docs.mitk.org/2012.06/group__Adaptor.html#ga2fa7e358c6738e35822e6e49d0b2c29d)(mitkImage, itk::Image<...>) 

把MITK图像类型连接至VTK
----------------------
> 因为处理都称作filter，图像处理和可视化看作基于流水线处理过程，因此这里采用了术语连接，即指把MITK图像类型传给VTK filter处理。 [VTK](http://www.vtk.org)是一个有名的医学图像处理可视化开源包。

图像不能转换或拷贝：图像数据通过一个封装在MITK图像类型的VTK对象访问。


- vtkImageData* [mitk::Image::GetVtkImageData](http://docs.mitk.org/2012.06/classmitk_1_1Image.html#abf3c711db109ca01dec15197d9809291)(int time = 0)

MITK曲面类型与VTK曲面类型的转换
----------------------------
同样：没有转换，只能访问：

- vtkPolyData* [mitk::Surface::GetVtkPolyData](http://docs.mitk.org/2012.06/classmitk_1_1Surface.html#af7613b0a4c91f5aa1ec3ed28b358a846)(int time = 0) 

- [mitk::Surface::SetVtkPolyData](http://docs.mitk.org/2012.06/classmitk_1_1Surface.html#ae925c94a03896caaecf3ada7f6816c52)(vtkPolyData*, int time = 0) 

[上一节](step5.md)[下一节](step7.md)[返回](../MITK-tutorial.md)