MITK 入门指南 -- 显示图片
=======================

<center>![示例结果](http://docs.mitk.org/2012.06/step1_result.png)</center>

通过执行StartVS_release.bat可以找到指南示例。每一节都在工程项目列表中找到。第一个示例程序说明如何在一个2维视图中显示一张图。上面是程序运行的截图

程序运行读入图像文件，文件位于`bin/CMakeExternals/Source/MITK-Data/Pic3D.nrrd`. 你若使用Visual Studio，可以右键"Step1"-project 然后选择 'Properties -> Configuration Properties -> Debugging'. 把图像文件的路径填入"Command Arguments"文本框.后续的小节都使用相同的步骤.

<center>![配置程序运行命令行参数](http://docs.mitk.org/2012.06/configureVisualStudioProperties.png)</center>

源代码分为I - V部分. 

* 创建一个`DataTree`对象
* 从文件中读取数据
* 把数据对象插入至`DataTree`对象
* 创建一个窗口，并把`DataTree`对象传给窗口
* 最后完成一些QT特定的初始化

	/*===================================================================
	The Medical Imaging Interaction Toolkit (MITK)
	Copyright (c) German Cancer Research Center, 
	Division of Medical and Biological Informatics.
	All rights reserved.
	This software is distributed WITHOUT ANY WARRANTY; without 
	even the implied warranty of MERCHANTABILITY or FITNESS FOR 
	A PARTICULAR PURPOSE.
	See LICENSE.txt or http://www.mitk.org for details.
	===================================================================*/
	#include "QmitkRegisterClasses.h"
	#include "QmitkRenderWindow.h"
	#include <mitkDataNodeFactory.h>
	#include <mitkStandaloneDataStorage.h>
	#include <itksys/SystemTools.hxx>
	#include <QApplication>
	//##Documentation
	//## @brief Load image (nrrd format) and display it in a 2D view
	int main(int argc, char* argv[])
	{
	  QApplication qtapplication( argc, argv );
	  if (argc < 2)
	  {
	    fprintf( stderr, "Usage:   %s [filename] \n\n", itksys::SystemTools::GetFilenameName(argv[0]).c_str() );
	    return 1;
	  }
	  // Register Qmitk-dependent global instances
	  QmitkRegisterClasses();
	  //*************************************************************************
	  // Part I: Basic initialization
	  //*************************************************************************
	  // Create a DataStorage
	  // The DataStorage manages all data objects. It is used by the 
	  // rendering mechanism to render all data objects
	  // We use the standard implementation mitk::StandaloneDataStorage.
	  mitk::StandaloneDataStorage::Pointer ds = mitk::StandaloneDataStorage::New();
	  //*************************************************************************
	  // Part II: Create some data by reading a file
	  //*************************************************************************
	  // Create a DataNodeFactory to read a data format supported
	  // by the DataNodeFactory (many image formats, surface formats, etc.)
	  mitk::DataNodeFactory::Pointer reader=mitk::DataNodeFactory::New();
	  const char * filename = argv[1];
	  try
	  {
	    reader->SetFileName(filename);
	    reader->Update();
	    //*************************************************************************
	    // Part III: Put the data into the datastorage
	    //*************************************************************************
	    // Add the node to the DataStorage
	    ds->Add(reader->GetOutput());
	  }
	  catch(...)
	  {
	    fprintf( stderr, "Could not open file %s \n\n", filename );
	    exit(2);
	  }
	  //*************************************************************************
	  // Part IV: Create window and pass the datastorage to it
	  //*************************************************************************
	  // Create a RenderWindow
	  QmitkRenderWindow renderWindow;
	  // Tell the RenderWindow which (part of) the datastorage to render
	  renderWindow.GetRenderer()->SetDataStorage(ds);
	  // Initialize the RenderWindow
	  mitk::TimeSlicedGeometry::Pointer geo = ds->ComputeBoundingGeometry3D(ds->GetAll());
	  mitk::RenderingManager::GetInstance()->InitializeViews( geo );
	  //mitk::RenderingManager::GetInstance()->InitializeViews();
	  // Select a slice
	  mitk::SliceNavigationController::Pointer sliceNaviController = renderWindow.GetSliceNavigationController();
	  if (sliceNaviController)
	    sliceNaviController->GetSlice()->SetPos( 0 );
	  //*************************************************************************
	  // Part V: Qt-specific initialization
	  //*************************************************************************
	  renderWindow.show();
	  renderWindow.resize( 256, 256 );
	  // for testing
	  #include "QtTesting.h"
	  if (strcmp(argv[argc-1], "-testing") != 0)
	    return qtapplication.exec();
	  else
	    return QtTesting();
	  // cleanup: Remove References to DataStorage. This will delete the object
	  ds = NULL;
	}

[上一节](step0.md)[下一节](step2.md)[返回](../MITK-tutorial.md)