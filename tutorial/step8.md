MITK 入门指南 -- 使用QmitkStdMultiWidget作为主窗口
===============================================

在本节，我们使用[QmitkStdMultiWidget](http://docs.mitk.org/2012.06/classQmitkStdMultiWidget.html)。它可以对同一套数据提供四个视图。从左上方到下方视图依次初始化为横向视图、矢状视图和冠状视图。在右下方的则初始化为3维视图。

![运行结果图](http://docs.mitk.org/2012.06/step8_result.png)

本节源代码包括：

- [Step8.cpp](http://docs.mitk.org/2012.06/Step8_8cpp-example.html)
- [Step8.h](http://docs.mitk.org/2012.06/Step8_8h-example.html)
- [Step8main.cpp](http://docs.mitk.org/2012.06/Step8main_8cpp-example.html)

[Step8](http://docs.mitk.org/2012.06/classStep8.html)修改于[Step6](http://docs.mitk.org/2012.06/classStep6.html).修改仅是对`SetupWidgets()`修改：使用一个QmitkStdMultiWidget替换一个QmitkRenderWidnow和两个QmitkSliceWidget.

	 // Part Ia: create and initialize QmitkStdMultiWidget
 	 //*************************************************************************
	  QmitkStdMultiWidget* multiWidget = new QmitkStdMultiWidget(viewParent);
	  hlayout->addWidget(multiWidget);
	  // Tell the multiWidget which DataStorage to render
	  multiWidget->SetDataStorage(m_DataStorage);
	  // Initialize views as transversal, sagittal, coronar (from
	  // top-left to bottom)
	  mitk::TimeSlicedGeometry::Pointer geo = m_DataStorage->ComputeBoundingGeometry3D(
	      m_DataStorage->GetAll());
	  mitk::RenderingManager::GetInstance()->InitializeViews(geo);
	  // Initialize bottom-right view as 3D view
	  multiWidget->GetRenderWindow4()->GetRenderer()->SetMapperID(
	      mitk::BaseRenderer::Standard3D);
	  // Enable standard handler for levelwindow-slider
	  multiWidget->EnableStandardLevelWindow();
	  // Add the displayed views to the DataStorage to see their positions in 2D and 3D
	  multiWidget->AddDisplayPlaneSubTree();
	  multiWidget->AddPlanesToDataStorage();
	  multiWidget->SetWidgetPlanesVisibility(true);
	  //*************************************************************************
	  // Part II: Setup standard interaction with the mouse
	  //*************************************************************************
	  // Moving the cut-planes to click-point
	  multiWidget->EnableNavigationControllerEventListening();

[上一节](step7.md)[下一节](step9.md)[返回](../MITK-tutorial.md)