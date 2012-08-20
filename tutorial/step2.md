MITK 入门指南 -- 加载多个数据集
============================

<center>![样例运行结果](http://docs.mitk.org/2012.06/step2_result.png)</center>

* [step2.cpp](http://docs.mitk.org/2012.06/Step2_8cpp-example.html) 本示例的源代码

本示例程序说明如何加载多个数据集。

DataNodeFactory用于产生包含数据集的结点。
> 译注：MITK的数据管理是基于树状结构的，所以每个数据集称作数据结点

本图显示当程序读入图片数据和曲面数据时的显示效果。为了得到上述结果，在运行程序时需要传入两个参数，图像文件`bin/CMakeExternals/Source/MITK-Data/Pic3D.nrrd`及曲面文件`src/MITK/Modules/MitkExt/Testing/Data/lungs.vtk`。若你的MITK源文件中没有包括此测试数据，你可以通过[在线下载](http://mitk.org/git/?p=MITK-Data.git;a=tree)。

本示例代码仅在上一节的代码基础上修改了第II和III部分：

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
	      // Part III: Put the data into the datastorage
	      //*********************************************************************
	      // Since the DataNodeFactory directly creates a node,
	      // use the datastorage to add the read node
	      storage->Add(nodeReader->GetOutput());
	    }
	    catch(...)
	    {
	      fprintf( stderr, "Could not open file %s \n\n", filename );
	      exit(2);
	    }
	  }

[上一节](step1.md)[下一节](step3.md)[返回](../MITK-tutorial.md)