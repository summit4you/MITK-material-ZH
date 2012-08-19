MITK 入门指南 -- 种子点交互
=========================

在上一节我们已经创建3个不同的视图显示数据，现在我们想引入种子点交互。

一个包含`PointSet`的数据结点加入`data tree`中，且在这个数据结点上关联一个用作交互的`PointSetInteractor`。交互模式以状态机定义，并存储在一个外部的XML文件。因此，我们需要加载一个状态机。

交互模式定义了：

	* 事件 -- 用作交互器响应（例如，指定鼠标按钮设置一个点）
	* 状态迁移 -- 跳转至下一个状态（例如，初始状态为“点集为空”）
	* 关联行为 -- 执行相应的代码（例如，当鼠标点击时加入一个点）

有关MITK更多的交互信息可以在[这里](http://docs.mitk.org/2012.06/InteractionPage.html)找到。

为了增加一个点，必须按住shift键并同时通过鼠标点击来标记点。

* [Step5.cpp](http://docs.mitk.org/2012.06/Step5_8cpp-example.html) 包含本节的完整代码

![运行结果](http://docs.mitk.org/2012.06/step5_result.png)

种子点交互必须创建一个点集和一个与之相符的数据结点，代码如下：

	mitk::PointSet::Pointer pointSet = mitk::PointSet::New();
	mitk::DataNode::Pointer pointSetNode = mitk::DataNode::New();
	pointSetNode->SetData(pointSet);
	// Add the node to the tree
	ds->Add(pointSetNode);
	// Create PointSetInteractor, associate to pointSetNode and add as
	// interactor to GlobalInteraction
	mitk::GlobalInteraction::GetInstance()->AddInteractor(
	mitk::PointSetInteractor::New("pointsetinteractor", pointSetNode)

[上一节](step4.md)[下一节](step6.md)[返回](../MITK-tutorial.md)