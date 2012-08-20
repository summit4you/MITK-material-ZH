[MITK](http://www.mitk.org/wiki) 入门指南
========================================
版本：0.1  
作者：[丘文峰](mailto:809104518@qq.com)

本文档用于介绍MITK。我们将从下载完MITK的编译开始，介绍如何使用MITK显示及与图像进行基本的交互，直到最后说明如何创建一个简单的功能完整的应用模块。

本入门指的例子可以在`mitk/QApplications/Tutorial/`找到。程序使用到两个样例数据文件。

* Pic3D.nrrd

	本文件包含一个图像数据，它位于`bin/CMakeExternals/Source/MITK-Data/Pic3D.nrrd`

* lugs.vtk
	
	本文件包含一个曲面数据，它位于`src/MITK/Modules/MitkExt/Testing/Data/lungs.vtk`

**************************************************************************************

* 指南章节安排
	* [入门](tutorial/step0.md)
	* [显示图片](tutorial/step1.md)
	* [加载多个数据集](tutorial/step2.md)
	* [创建三维视图](tutorial/step3.md)
	* [使用多个视图浏览数据](tutorial/step4.md)
	* [种子点交互](tutorial/step5.md)
	* [交互式区域增长](tutorial/step6.md)
	* [把区域增长分割结果转为曲面](tutorial/step7.md)
	* [使用QmitkStdMultiWidget作为主窗口](tutorial/step8.md)
	* [完善功能](tutorial/step9.md)
	* [新的交互](tutorial/step10.md)