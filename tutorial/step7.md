MITK 入门指南 -- 把区域增长分割结果转为曲面
=======================================

在本节，我们通过一个VTK filter把上一节的分割结果转为曲面模型。其拳代码如下：

- [Step7.cpp](http://docs.mitk.org/2012.06/Step7_8cpp-example.html)
- [Step7.h](http://docs.mitk.org/2012.06/Step7_8h-example.html)
- [Step7main.cpp](http://docs.mitk.org/2012.06/Step7main_8cpp-example.html)

[Step7](http://docs.mitk.org/2012.06/classStep7.html)在[Step6](http://docs.mitk.org/2012.06/classStep6.html)基础上修改。修改只是在StartRegionGrowing()方法内增加对结果图像的处理，使其产生曲面文件。代码如下：

	if(m_ResultImage.IsNotNull())
	  {
	    m_ResultNode->SetProperty("volumerendering", mitk::BoolProperty::New(false));
	    vtkMarchingCubes* surfaceCreator = vtkMarchingCubes::New();
	    surfaceCreator->SetInput(m_ResultImage->GetVtkImageData());
	    surfaceCreator->SetValue(0, 1);
	    mitk::Surface::Pointer surface = mitk::Surface::New();
	    surface->SetVtkPolyData(surfaceCreator->GetOutput());
	    mitk::DataNode::Pointer surfaceNode = mitk::DataNode::New();
	    surfaceNode->SetData(surface);
	    m_DataStorage->Add(surfaceNode);
	    mitk::RenderingManager::GetInstance()->RequestUpdateAll();
	    std::cout << "8";
	    surfaceCreator->Delete();
	  }

[上一节](step6.md)[下一节](step8.md)[返回](../MITK-tutorial.md)