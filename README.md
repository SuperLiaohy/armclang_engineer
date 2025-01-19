## 这是一个吉大工程机器人的armclang项目  

- 项目名称：armclang_engineer
- 工程大目录下有 
  - 
  - build build目录时用来存放cmake构建生成的文件
  - Core Core目录是cubemx生成的源文件和头文件(只有少部分用户自己编写的代码)
  - Drivers Drivers目录时cubemx生成的源文件和头文件
  - libs libs目录是在cmake系下构建keil的cmsis-dsp库生成的静态库
  - MDK-ARM MDK-ARM目录是cubemx生成的keil工程文件
  - Middlewares Middlewares目录是cubemx生成的源文件和头文件
  - Own Own目录是用户自己编写的代码部分
  - USB_DEVICE USB_DEVICE目录是cubemx生成的有关USB的源文件和头文件
  - .mxproject .mxproject是cubemx生成的文件
  - arm-none-eabi-toolchain.cmake arm-none-eabi-toolchain.cmake是cmake是用户自己定义的编译器工具链，主要存放一些编译器的选择和路径和一些编译选项mcu架构
  - CMakeLists.txt CMakeLists.txt是用户自己编写的一个用于用cmake构建用armclang下keil的mdk项目的文件。
  - armclang_engineer.ioc armclang_engineer.ioc是cubemx的配置文件
  - calcelf.exe 是一个可视化看编译后所占用的内存大小的工具(类似于armgcc编译完后生成的图表)  
- 在Own目录下(即用户编写的代码如下)
  - 
> 代码的目录分类逻辑主要依据高内聚低耦合的原则，  
> 即将代码分为抽象最高层的对象代码Obj，抽象中层的对象代码App，抽象底层的对象代码Bsp，  
> 然后将依赖相对独立的代码封装为Moudle，这样降低耦合性，提高可移植性。  
> 最后在CallBack回调函数中实现部分逻辑，在OperationSystem中的Thread中实现大部分逻辑。  
  - App App目录是用户自己编写的应用层代码，为抽象中层的对象
  - Bsp Bsp目录是用户自己封装hal库实现的板级支持包，为抽抽象底层的对象
  - CallBack CallBack是Hal库的一些回调函数
  - Moudle Moudle是用户自己编写的模块代码
  - Obj Obj是用户自己编写的物体代码，为抽象最高层的对象代码
  - OperationSystem OperationSystem主要是用户自己实现的一些和rtos有关的代码，其中的Thread为主体逻辑部分  
- 在App目录下(即用户编写的应用层代码如下)
  -
  - 