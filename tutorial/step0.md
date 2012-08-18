MITK 入门指南 -- 入门
====================
如果想编译入门示例，请按照下面编译步骤操作并确保`MITK_BUILD_EXAMPLES`在CMake中的设置为`ON`

> 译注：你也可以自己进行单独编译，在本节结束。我将提供一个独立的CMake示例文件。若你对CMake不太了解，
请浏览[CMake](http://www.cmake.org/)的官方网站。

<center>![CMake配置选项](http://docs.mitk.org/2012.06/configureCMake.PNG)</center>

入门指南的示例文件可以在MITK源代码的子目录`Applications\Tutorial\`中找到. 在成功编译之后，你会在相应子目录下的` bin\Release\` 或 `bin\Debug\` 找到编译文件。

> 以下是作者自己写的CMake文件，在入门示例中，你只需要参考下述模版，把代码和包含下述内容的CMakeLists.txt文件放在同一目录下即可以由CMake生成一个工程文件。

	cmake_minimum_required(VERSION 2.8.4)
	project(Step1) # 这里改成你的项目名
	
	find_package(MITK REQUIRED)
	
	# Check that MITK has been build with Qt support
	if(NOT MITK_USE_QT)
	  message(SEND_ERROR "MITK needs to be built with MITK_USE_QT set to ON")
	endif()
	
	# Check prerequisites for this application.
	# We need the Qmitk module.
	MITK_CHECK_MODULE(result Qmitk)
	if(result)
	  message(SEND_ERROR "MITK module(s) \"${result}\" not available from the MITK build at ${MITK_DIR}")
	endif()
	
	# Set-up the build system to use the Qmitk module
	MITK_USE_MODULE(Qmitk)
	include_directories(${ALL_INCLUDE_DIRECTORIES})
	link_directories(${ALL_LIBRARY_DIRS})
	
	add_executable(${PROJECT_NAME} Step1.cpp) # 这里改成你的源文件名
	target_link_libraries(${PROJECT_NAME} ${ALL_LIBRARIES})

[下一节](step1.md)[返回](../MITK-tutorial.md)