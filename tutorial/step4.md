MITK 入门指南 -- 使用多个视图浏览数据
==================================

和前面两节一样，在这节中我们仍需要加载多套数据。不过现在，我们创建三个视图进行数据显示。在本节，我们使用`QmitkRenderWindow`作为3D视图，视图不进行体绘制。此外，两个视图用于数据的切片显示。这里用到`QmitkSliceWidget`,它的父类是`QmitkRenderWindow`，但是它提供滑动条用于浏览切片数据。我们创建两个`QmitkSliceWidget`的实例，一个用于横向显示(transversal)，一个用于矢状(sagittal)显示。在本节B部分，我们增加程序使得2个视图以3维空间正确的位置上显示，并且在2维视图中显示交叉线，用于反映当前的空间位置。

和上一节一样，要得到结果，程序运行时需要使用两个数据，位于`bin/CMakeExternals/Source/MITK-Data/Pic3D.nrrd`的图像文件和`src/MITK/Modules/MitkExt/Testing/Data/lungs.vtk`的曲面文件。若你的MITK源文件中没有包括此测试数据，你可以通过[在线下载](http://mitk.org/git/?p=MITK-Data.git;a=tree)。

* [Step4.cpp](http://docs.mitk.org/2012.06/Step4_8cpp-example.html) 包含本节A部分与B部分的代码

A -- 创建横向与矢状视图
---------------------

![A运行结果](http://docs.mitk.org/2012.06/step4a_result.png)

使用QT水平布局：

	#include <QHBoxLayout>

然后创建一个渲染窗口

	//## The QmitkRenderWindow is used for displaying a 3D view as in Step3,
	//## but without volume-rendering.
	//## Furthermore, we create two 2D views for slicing through the data.
	//## We use the class QmitkSliceWidget, which is based on the class
	//## QmitkRenderWindow, but additionally provides sliders
	//## to slice through the data. We create two instances of
	//## QmitkSliceWidget, one for transversal and one for sagittal slicing.
	//## The two slices are also shown at their correct position in 3D as
	//## well as intersection-line, each in the other 2D view.
	int main(int argc, char* argv[])
	{
	  QApplication qtapplication( argc, argv );
	  if(argc<2)
	  {
	    fprintf( stderr, "Usage:   %s [filename1] [filename2] ...\n\n", itksys::SystemTools::GetFilenameName(argv[0]).c_str() );
	    return 1;
	  }
	  // Register Qmitk-dependent global instances
	  QmitkRegisterClasses();
	  //*************************************************************************
	  // Part I: Basic initialization
	  //*************************************************************************
	  // Create a DataStorage
	  mitk::StandaloneDataStorage::Pointer ds = mitk::StandaloneDataStorage::New();
	  //*************************************************************************
	  // Part II: Create some data by reading files
	  //*************************************************************************
	  int i;
	  for(i=1; i<argc; ++i)
	  {
	    // For testing
	    if(strcmp(argv[i], "-testing")==0) continue;
	    // Create a DataNodeFactory to read a data format supported
	    // by the DataNodeFactory (many image formats, surface formats, etc.)
	    mitk::DataNodeFactory::Pointer nodeReader=mitk::DataNodeFactory::New();
	    const char * filename = argv[i];
	    try
	    {
	      nodeReader->SetFileName(filename);
	      nodeReader->Update();
	      //*********************************************************************
	      //Part III: Put the data into the datastorage
	      //*********************************************************************
	      // Since the DataNodeFactory directly creates a node,
	      // use the datastorage to add the read node
	      mitk::DataNode::Pointer node = nodeReader->GetOutput();
	      ds->Add(node);
	    }
	    catch(...)
	    {
	      fprintf( stderr, "Could not open file %s \n\n", filename );
	      exit(2);
	    }
	  }
	  //*************************************************************************
	  // Part IV: Create windows and pass the tree to it
	  //*************************************************************************
	  // Create toplevel widget with horizontal layout
	  QWidget toplevelWidget;
	  QHBoxLayout layout;
	  layout.setSpacing(2);
	  layout.setMargin(0);
	  toplevelWidget.setLayout(&layout);
	  //*************************************************************************
	  // Part IVa: 3D view
	  //*************************************************************************
	  // Create a renderwindow
	  QmitkRenderWindow renderWindow(&toplevelWidget);
	  layout.addWidget(&renderWindow);
	  // Tell the renderwindow which (part of) the datastorage to render
	  renderWindow.GetRenderer()->SetDataStorage(ds);
	  // Use it as a 3D view
	  renderWindow.GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);

创建用作模向切片显示的2维视图：

	QmitkSliceWidget view2(&toplevelWidget);
	layout.addWidget(&view2);
	view2.SetLevelWindowEnabled(true);
	// Tell the QmitkSliceWidget which (part of) the tree to render.
	// By default, it slices the data transversally
	view2.SetDataStorage(ds);

创建一个用于矢状切片的2维视图：

	QmitkSliceWidget view3(&toplevelWidget);
	layout.addWidget(&view3);
	view3.SetDataStorage(ds);

`toplevelWidget`现在是新的主`Widget`:

	return qtapplication.exec();

B -- 显示切片位置
----------------

![B运行结果](http://docs.mitk.org/2012.06/step4b_result.png)

我们现在想在2维看到位置，并能在3D视图中看到切片。因此需要在DataTree中加入：

	ds->Add(view2.GetRenderer()->GetCurrentWorldGeometry2DNode());
	ds->Add(view3.GetRenderer()->GetCurrentWorldGeometry2DNode());

现在重新编译和运行，将看到切片的位置如上图所示。

[上一节](step3.md)[下一节](step5.md)[返回](../MITK-tutorial.md)