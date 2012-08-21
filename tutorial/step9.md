MITK 入门指南 -- 插件
===================

MITK使用模块化概念以最大化重用和移植。你可以启动一个MITK应用程序看看（例如ExtApp，由MITK提供的样例程序）。一个应用程序由多个插件组成。一个插件可以是包含完整功能的，也就是说它可以是一个视图，插件包含一组确定的行为和属性。

通过使用MITK BundleGenerator可以很方便创建一个MITK插件。具体用法在[如何使用BundleGenerator创建MITK插件](../NewPluginPage.md)中有描述。

本节我们使用BundleGenerator工具创建一个QmitkRegionGrowing插件。首先我们看看由BundleGenerator创建的文件有哪些：

	documentation\doxygen\
	  modules.dox............................. Doxygen file 用于描述你的插件
	
	resources\
	  icon.xpm................................ 插件的icon. GIMP or other programs (including your text editor)
	                                           can be used to change this
	
	src\internal\
	  QmitkMITKRegionGrowingView.cpp.......... 最重要的文件实现行为
	  QmitkMITKRegionGrowingView.h............ 头文件
	  QmitkMITKRegionGrowingViewControls.ui...  Qt  Designer 生成的XML文件, 它描述了你定义的按扭，文本框等控件
	
	CMakeLists.txt \.......................... CMake配置文件
	files.cmake    /
	manifest_headers.cmake.................... 你插件的相关信息
	plugin.xml ............................... BlueBerry 集成

如果你不太熟悉QT开发，请看看[奇趣科技公司(Trolltech)有关.ui文件的描述](http://doc.trolltech.com/4.6/designer-manual.html)(Oh,不应该用请，你应该看看！)

> Trolltech(奇趣科技公司)是由Haavard Nord (执行总裁) 和 Eirik Chambe-Eng (总裁)于1994年创立的，2008年6月被NOKIA收购。Trolltech是一家拥有两个主线产品（Qt和Qtopia）的软件公司。Qt 是一个跨平台C++应用程序开发框架。程序开发员利用其可以编写单一代码的应用程序，并可在Windows, Linux, Unix, Mac OS X和嵌入式Linux等不同平台上进行本地化运行。目前，Qt已被成功地应用于全球数以千计的商业应用程序。此外，Qt还是开放源代码KDE桌面环境的基础。[更多](http://baike.baidu.com/view/1815949.htm)

C++文件实现了一个QmitkAbstractView的子类。在QmitkRegionGrowing这个例子中，我们加入能力设置一些种子点并执行区域增长。你必定对如何把新生成的QmitkRegionGrowing插件整合至一个完整的应用ExtApp中感兴趣：

因为需要访问StdMultiWidget，因此mainifest_headers.cmake需要修改：

	set(Require-Plugin org.mitk.gui.qt.common org.mitk.gui.qt.stdmultiwidgeteditor)

接着，增加PointSet用于种子点。它需要对QmitkRegionGrowingView.h进行修改。在QmitkRegionGrowingView加入以下头文件：

	#include "QmitkPointListWidget.h"
	#include "QmitkStdMultiWidget.h"
	#include "QmitkStdMultiWidgetEditor.h"

声明PointSet为保护成员并增加一个QmitkPointListWidget和一个QmitkStdMultiWidget的指针变量：

	/// \brief This is the actual seed point data object
	mitk::PointSet::Pointer m_PointSet;
	
	QmitkPointListWidget* lstPoints;
	QmitkStdMultiWidget* m_MultiWidget;

对于实现文件 QmitkRegionGrowingView.cpp，为上述控件和PointSet进行初始化，实现代码在CreateQtPartControl():

	// create a QmitkPointListWidget and add it to the widget created from .ui file
	lstPoints = new QmitkPointListWidget();
	m_Controls.verticalLayout->addWidget(lstPoints);
	
	// get access to StdMultiWidget by using RenderWindowPart
	QmitkStdMultiWidgetEditor* qSMWE = dynamic_cast<QmitkStdMultiWidgetEditor*>(GetRenderWindowPart());
	m_MultiWidget = qSMWE->GetStdMultiWidget();
	
	// let the point set widget know about the multi widget (crosshair updates)
	lstPoints->SetMultiWidget( m_MultiWidget );
	      
	// create a new DataTreeNode containing a PointSet with some interaction
	m_PointSet = mitk::PointSet::New();
	
	mitk::DataNode::Pointer pointSetNode = mitk::DataNode::New();
	pointSetNode->SetData( m_PointSet );
	pointSetNode->SetName("seed points for region growing");
	pointSetNode->SetProperty("helper object", mitk::BoolProperty::New(true) );
	pointSetNode->SetProperty("layer", mitk::IntProperty::New(1024) );
	
	// add the pointset to the data tree (for rendering and access by other modules)
	GetDataStorage()->Add( pointSetNode );
	
	// tell the GUI widget about out point set
	lstPoints->SetPointSetNode( pointSetNode );

为了使用ITK区域增长算法，在 QmitkRegionGrowingView.h增加保护成员函数：

	/*!
	\brief ITK image processing function
	  This function is templated like an ITK image. The MITK-Macro AccessByItk determines the actual pixel type and dimensionality of
	  a given MITK image and calls this function for further processing (in our case region growing)
	*/
	template < typename TPixel, unsigned int VImageDimension >
	  void ItkImageProcessing( itk::Image< TPixel, VImageDimension >* itkImage, mitk::Geometry3D* imageGeometry );
	
在QmitkRegionGrowingView.cpp 增加头文件：

	// MITK
	#include "mitkImageAccessByItk.h"
	#include "mitkITKImageImport.h"
	#include "mitkProperties.h"
	#include "mitkColorProperty.h"
	
	// ITK
	#include <itkConnectedThresholdImageFilter.h>

并实现处理函数DoImageProcessing():
	// So we have an image. Let's see if the user has set some seed points already
	if ( m_PointSet->GetSize() == 0 )
	{
	  // no points there. Not good for region growing
	  QMessageBox::information( NULL, "Region growing functionality", 
	                                  "Please set some seed points inside the image first.\n"
	                                  "(hold Shift key and click left mouse button inside the image.)"
	                                );
	  return;
	}
	
	// actually perform region growing. Here we have both an image and some seed points
	AccessByItk_1( image, ItkImageProcessing, image->GetGeometry() ); // some magic to call the correctly templated function

并增加一个新的模板函数：
	template < typename TPixel, unsigned int VImageDimension >
	void QmitkRegionGrowingView::ItkImageProcessing( itk::Image< TPixel, VImageDimension >* itkImage, mitk::Geometry3D* imageGeometry )
	{
	  typedef itk::Image< TPixel, VImageDimension > InputImageType;
	  typedef typename InputImageType::IndexType    IndexType;
	  
	  // instantiate an ITK region growing filter, set its parameters
	  typedef itk::ConnectedThresholdImageFilter<InputImageType, InputImageType> RegionGrowingFilterType;
	  typename RegionGrowingFilterType::Pointer regionGrower = RegionGrowingFilterType::New();
	  regionGrower->SetInput( itkImage ); // don't forget this
	
	  // determine a thresholding interval
	  IndexType seedIndex;
	  TPixel min( std::numeric_limits<TPixel>::max() );
	  TPixel max( std::numeric_limits<TPixel>::min() );
	  mitk::PointSet::PointsContainer* points = m_PointSet->GetPointSet()->GetPoints();
	  for ( mitk::PointSet::PointsConstIterator pointsIterator = points->Begin(); 
	        pointsIterator != points->End();
	        ++pointsIterator ) 
	  {
	    // first test if this point is inside the image at all
	    if ( !imageGeometry->IsInside( pointsIterator.Value()) ) 
	    {
	      continue;
	    }
	
	    // convert world coordinates to image indices
	    imageGeometry->WorldToIndex( pointsIterator.Value(), seedIndex);
	
	    // get the pixel value at this point
	    TPixel currentPixelValue = itkImage->GetPixel( seedIndex );
	
	    // adjust minimum and maximum values
	    if (currentPixelValue > max)
	      max = currentPixelValue;
	
	    if (currentPixelValue < min)
	      min = currentPixelValue;
	
	    regionGrower->AddSeed( seedIndex );
	  }
	
	  std::cout << "Values between " << min << " and " << max << std::endl;
	
	  min -= 30;
	  max += 30;
	
	  // set thresholds and execute filter
	  regionGrower->SetLower( min );
	  regionGrower->SetUpper( max );
	
	  regionGrower->Update();
	
	  mitk::Image::Pointer resultImage = mitk::ImportItkImage( regionGrower->GetOutput() );
	  mitk::DataTreeNode::Pointer newNode = mitk::DataTreeNode::New();
	  newNode->SetData( resultImage );
	
	  // set some properties
	  newNode->SetProperty("binary", mitk::BoolProperty::New(true));
	  newNode->SetProperty("name", mitk::StringProperty::New("dumb segmentation"));
	  newNode->SetProperty("color", mitk::ColorProperty::New(1.0,0.0,0.0));
	  newNode->SetProperty("volumerendering", mitk::BoolProperty::New(true));
	  newNode->SetProperty("layer", mitk::IntProperty::New(1));
	  newNode->SetProperty("opacity", mitk::FloatProperty::New(0.5));
	
	  // add result to data tree
	  this->GetDefaultDataStorage()->Add( newNode );
	  mitk::RenderingManager::GetInstance()->RequestUpdateAll();
	}
	
希望MITK让你玩得开心！

如果你在实践中遇到任何困难，不要犹豫把你的问题反馈至MITK mailling list [mitk-users@lists.sourceforge.net](mitk-users@lists.sourceforge.net)!，那里的朋友都会很热心地去帮助你。

[上一节](step8.md)[下一节](step10.md)[返回](../MITK-tutorial.md)
