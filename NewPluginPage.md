如何使用BundleGenerator创建MITK插件
=================================
版本：0.1  
作者：[丘文峰](mailto:809104518@qq.com)

MITK插件生成器（BundleGenerator）是一个用于简化创建你的MITK项目（可选）或是插件的命令行工具。它可以在[这里](http://www.mitk.org/wiki/Download)下载或是从已编译的MITK工程中找到。

插件生成器的命令行参数描述如下：

	./MITKPluginGenerator -h
	A CTK plug-in generator for MITK (version 1.2.0)
	
	  -h, --help                      Show this help text
	  -o, --out-dir                   Output directory (default: /tmp)
	  -l, --license                   Path to a file containing license information (default: :/MITKLicense.txt)
	  -v, --vendor                    The vendor of the generated code (default: DKFZ, Medical and Biological Informatics)
	  -q, --quiet                     Do not print additional information
	  -y, --confirm-all               Answer all questions with 'yes'
	  -u, --check-update              Check for updates and exit
	  -n, --no-networking             Disable all network requests
	
	Plug-in View options
	  -vc, --view-class               The View's' class name
	  -vn, --view-name              * The View's human readable name
	
	Plug-in options
	  -ps, --plugin-symbolic-name   * The plugin's symbolic name
	  -pn, --plugin-name              The plug-in's human readable name
	
	Project options
	  --project-copyright             Path to a file containing copyright information (default: :/LICENSE.txt)
	  --project-name                  The project name
	  --project-app-name              The application name
	
	[* - options are required]

> 当然，你也可以给-h命令行参数，在你的控制台输出上述帮助信息

如果通过`-project-name`参数提供一个项目名，那么新的插件将作为新项目的一部分生成。

创建一个新的MITK创建
------------------
这里是一个使用插件生成器的例子，创建一个插件，它的符号名为`com.mycompany.myplugin`而视图名为`My View`:

	./MITKPluginGenerator --plugin-symbolic-name org.mycompany.myplugin --view-name "My View"

> 下图为译者在本机上执行上述命令的结果图![运行结果图](image/newpluginpage0.jpg?raw=true)

如果你没有通过`-out-dir`命令行参数指定插件的输出位置，例如在我们的例子里面。它输出至`C:/Users/summit/AppData/Local/Temp/MyProject`。那么不要忘记把插件加入至你的项目编译（通常你们把插件放至 <your-project>/Plugins/Plugins.cmake).

创建一个新的MITK项目
------------------

先决条件
-------
由插件生成器创建基于MITK的项目首先要满足与MITK本身的先决条件。评见[MITK先决条件](http://docs.mitk.org/2012.06/BuildInstructionsPage.html)。

这里是使用插件生成器创建一个项目，与上面创建插件类似，不过这次只是创建插件集成至新的项目中。命令行如下：

	./MITKPluginGenerator --plugin-symbolic-name org.mycompany.myplugin --view-name "My View" --project-name "MyProject" --project-app-name "MyApp"

> 下图为译者在本机上执行上述命令的结果图![运行结果图](image/newpluginpage1.jpg?raw=true)

生成的项目是完整的可以立即通过[CMake](http://docs.mitk.org/2012.06/BlueBerry/reference/api/html/namespaceCMake.html)配置。当编译生成项目时，首先会下载所有相关的程序库（例如MITK本身）。有关项目目录的组织及使用[CMake](http://docs.mitk.org/2012.06/BlueBerry/reference/api/html/namespaceCMake.html)配置你的MITK项目，可以阅读[SetupAMITKBasedProject](http://www.mitk.org/wiki/SetupAMitkBasedProject)

> 译注：为了避免重复下载编译MITK，在CMake里，把EXTERNAL_MITK_DIR指向已编译成功的MITK目录。生成并编译成功后，运行MyApp如下图所示：![运行结果图](image/newpluginpage2.jpg?raw=true)

局限性
-----

插件生成器只支持一组十分有限的配置选项。为了让你的项目或插件有更多的定制功能，你需要熟悉[CMake](http://docs.mitk.org/2012.06/BlueBerry/reference/api/html/namespaceCMake.html)和编译系统。

插件的编译系统
------------
你仅需要把插件新的文件放入你的插件目录src文件夹或是src/internal文件夹，并编辑在目录下面的files.cmake文件。如果你还有一些花哨的东西，例如外部库或外部头文件夹，那么你应该看看[CMake](http://docs.mitk.org/2012.06/BlueBerry/reference/api/html/namespaceCMake.html)的用户手册和MTIK的编译系统文档。

> 放心，这部分内容译者会第一时间把使用心得加上，敬请留意

排错
----

当运行项目的执行文件时，我得到**“could not find libary"**错误信息。
> 这大部分是因为错误的环境变量设置。在Windows，确保你使用提供的指处理文件启动Visual Studio或你的项目可执行文件。如果仍提示错误 ，认真检查你的批处理文件中PATH变量值（它必须包含MITK、ITK、VTK和QT的BIN文件夹)。而在Linux，按上述要求设置LD_LIBRARY_PATH变量。

