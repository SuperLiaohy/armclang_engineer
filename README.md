## 这是一个吉大工程机器人的armclang项目

> 该项目既可以用于keil的mdk构建，也可以用于cmake构建（故可使用clion进行高效的代码编辑），

- 项目名称：armclang_engineer
- 工程大目录下有
  --------------

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
  ---------------------------------

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
  ---------------------------------------

  - Buzzer Buzzer是用户自己编写的蜂鸣器代码，可以用来通过改变频率模拟声音，要注意里面用的是默认osdelay延时，故在error_task里要使用他的haldelay版，否则就会出现error_task无法起到作用，其他任务会在osdelay时运行
  - ImageTrans 是用户编写的图传链路的代码 1. 自定义控制器发送和接收 2. 图传链路的键鼠控制
  - Imu Imu是用户自己编写的陀螺仪代码，分为陀螺仪驱动和，解算算法（四元数解算），和滤波算法（ekf）
  - Judge Judge是用户编写的裁判系统代码，里面将UI和其他信息分开
  - MicroTime MicroTime是用户自己编写的时间戳代码，是用systick实现的，可以进行微妙级延时和计时
  - Motor Motor是用户自己编写的电机代码，用模板实现的Motor类。现在已经实现的电机有1. dmMotor 2. lkMotor 3. lkMotorBoard 4.GM6020 5. M3508 6.M2006
  - Pump Pump是用户自己编写的气泵代码。目前只有一个大概的框架
  - RemoteControl RemoteControl是用户自己编写的遥控器代码，用hal库的串口接收不定长实现的遥控器类
  - RGBLED RGBLED是用户自己编写的RGBLED代码，要注意里面用的是默认osdelay延时，故在error_task里要使用他的haldelay版，否则就会出现error_task无法起到作用，其他任务会在osdelay时运行
  - W25Q64 W25Q64是用户自己编写的flash代码，移植官方的W25Q64的代码，用来实现读写flash，可以用于离线存储代码
- 在Bsp目录下(即用户编写的板级支持包代码如下)
  -------------------------------------------

  - CAN CAN是用户自己编写的can代码，封装HAL库实现的SuperCan类。
  - CDC CDC是用户自己编写的CDC代码，目前只是是实现了一个printf。因为HAL库的CDC已经很通俗了，所以这里就不太想封装。
  - GPIO GPIO是用户自己编写的gpio代码，通过模板封装HAL库实现的SuperGPIO类。
  - Spi Spi是用户自己编写的spi代码，封装HAL库实现的SuperSPI类。
  - Uart Uart是用户自己编写的uart代码，封装HAL库实现的SuperUart类。
  - WDG WDG是用户自己编写的wdg代码，封装HAL库实现的SuperWDG类。看门狗。
- 在CallBack目录下(即用户编写的回调函数代码如下)
  ----------------------------------------------

> 除了cdc的回调函数是套了一成函数，其他的回调函数都是直接调用的hal库的回调函数(覆写弱函数)

- CanCallback.cpp CanCallback.cpp是用户自己编写的can回调函数代码
- CDCCallback.cpp CDCCallback.cpp是用户自己编写的cdc回调函数代码
- GPIOCallback.cpp GPIOCallback.cpp是用户自己编写的gpio回调函数代码
- SPICallback.cpp SPICallback.cpp是用户自己编写的spi回调函数代码
- UartCallback.cpp UartCallback.cpp是用户自己编写的uart回调函数代码
- 在Moudle目录下(即用户编写的模块代码如下)
  ----------------------------------------

  - Crc Crc是用户自己编写的crc代码，封装大疆的Crc校验。
  - CustomList CustomList是用户自己编写的一个链表的数据结构，可以用来实现动态可边长的数组。
  - Detect Detect是用户自己编写的一个检测离线和恢复的模块，可以用来检测其他各个模块的状态，然后执行对应的回调
  - DoubleBuffer DoubleBuffer是用户自己编写的一个双缓冲区的数据结构，本来是用来实现各种通信的双缓存的接发，实现读写的分离，但是实际效果没有明显提升，且对通信的稳定性似乎有影响，（现已不用）。
  - Filter Filter是用户自己编写的一个滤波器的模块，目前只有一个迭代的均值滤波器。
  - Key Key是用户自己的一个按键模块，检测机制是每隔一段时间，对按键进行扫描，这套key被遥控器和图传链路共用。
  - Matrix Matrix是用户自己编写的一个矩阵的数据结构，通过模板实现用arm_math库来实现矩阵的加减乘除，求逆。
  - MyMath MyMath是用户自己编写的一个数学库，里面大部分是用模板实现的一些数学函数和常量。
  - PID PID是用户自己编写的一个pid模块，实现了单级pid和串级pid。
  - Slope Slope是用户自己编写的一个斜率模块，可以用来实现各种斜率控制器。
  - Vofa Vofa是用户自己编写的一个vofa模块，用模板实现的一个很方便就可实现vofa的justfloat协议。
- 在Obj目录下(即用户编写的物体代码如下)
  -------------------------------------

  - Chassis Chassis是用户自己编写的一个底盘类，用来实现底盘的运动。
  - RoboArm RoboArm是用户自己编写的一个机械臂类，用来实现机械臂的运动。
  - Interact Interact是用户自己编写的一个交互类，用来实现和各路通道的交互通信的统一。
- 在OperationSystem目录下(即用户编写的操作系统代码如下)
  -----------------------------------------------------

  - Heap Heap是用户自己编写的一个堆内存管理器，用于在H7的RAM上实现对各种SRAM。
  - LetterShell LetterShell是移植的一个开源的lettershell模块，用来实现一个简单的shell，可以用来调试程序。
  - Port Port是用户自己编写的一个的接口文件，用于将cpp文件里的方法进行包装用到c文件里，
  - Thread Thread是用户自己编写的一个线程类，里面编写了各种各样的Task任务主要逻辑。
    - ChassisTask ChassisTask是用户自己编写的一个底盘任务，用来实现底盘的运动。
      - 任务优先级osPriorityNormal，栈大小256 words，循环为每次逻辑后延时1ms一次
      - 调用chassis类，轮询更新底盘6个电机的斜坡，计算6个电机pid，然后发送can。
    - DetectTask DetectTask是用户自己编写的一个检测任务，用来实现各种检测任务。
      - 任务优先级osPriorityBelowNormal，栈大小256 words，循环为每次逻辑后延时1ms一次
      - 调用了Detect类，轮询判断各个模块的更新时间，以此来判断是否离线，然后调用对应的离线和恢复函数。
    - DJITask DJITask是用户自己编写的一个大疆任务，用来实现大疆的各种任务。
      - 任务优先级osPriorityNormal，栈大小512 words，循环为1ms一次
      - 调用了roboArm类，不断计算机械臂的差分器的diff的角度pid，然后使用can发送。
    - ImageTransTask ImageTransTask是用户自己编写的一个图传任务，用来实现图传的各种任务。
      - 任务优先级osPriorityHigh，栈大小512 words，循环为每次逻辑后延时1ms一次
      - 调用了roboArm类，目前只实现了图传链路的自定义接收
    - ImuTask ImuTask是用户自己编写的一个陀螺仪任务，用来实现陀螺仪的各种任务。
      - 任务优先级osPriorityHigh，栈大小528 words，循环为每次逻辑后延时1ms一次
      - 调用了imu类
      - 通过控制宏来选择
      - 1.是否使用温控阻塞
      - 2.是否使用温控
      - 3.是否使用自检零漂
      - 判断逻辑为先判断是否到达温控目标范围，然后进行零漂自检，然后就是不断轮询读取陀螺仪roll，yaw，pitch，并不断进行温控。
    - JudgeTask JudgeTask是用户自己编写的一个裁判系统任务，用来实现裁判系统的各种任务。
      - 任务优先级osPriorityNormal，栈大小512 words，循环为每次逻辑后延时1ms一次
      - 逻辑部分暂未实现
    - LedTask LedTask是用户自己编写的一个led任务，用来实现led的各种任务。
      - 任务优先级osPriorityNormal，栈大小256 words，循环为每次逻辑后延时1ms一次
      - 调用了RGBLED类，不断更新RGBLED的颜色，用来判断这是正常状态，若为急停则只该任务将不会被执行，急停时为不断闪烁红灯。
    - LKTask LKTask是用户自己编写的一个lk任务，用来实现lk的各种任务。
      - 任务优先级osPriorityNormal，栈大小384 words，循环为10ms一次
      - 调用了RGBLED类，循环向各个LKMotor发送电机多圈控制角度的指定，读取总角度的指定。（中间有延时是因为防止同一时间过度占用总线）
    - PCTask PCTask是用户自己编写的一个用于实现和上位机通信的任务。
      - 任务优先级osPriorityNormal，栈大小384 words，循环为1ms一次
      - 调用了Interact类，循环向上位机发送当前各个电机的关节的数据
    - PumpTask PumpTask是用户自己编写的一个pump任务，用来实现pump的各种任务。
      - 任务目前未实现。
    - RemoteControlTask RemoteControlTask是用户自己编写的一个遥控器任务。
      - 任务优先级osPriorityBelowNormal，栈大小256 words，循环为14ms一次
      - 调用interact类，任务循环为首先判断遥控器当前状态是否为lost若为lost则一直等到恢复后再进行解析数据，在数据判断里首先判断是否为急停，急停则唤醒error_task进入急停。
      - 然后便是遥控器的控制逻辑，首先滑轮通道是来设定是键鼠控制还是遥控器的控制，默认是遥控器控制，
      - 再之后便是对根据遥控器上面两个对应的小拨杆的位置，来设定机械臂的控制模式，和底盘的控制模式。
      - 最后便是对遥控器中间两个大拨杆的数据解析。根据上面小拨杆解析出对应的模式，然后将大拨杆获得的数据作用到对应的机械臂和底盘上。
    - StartTask StartTask是用户自己编写的一个启动任务。
      - 任务优先级osPriorityRealtime，栈大小256 words，无循环
      - 调用各种类，各种类再这个任务里实现任务初始化。当初始化完毕后，对任务进行删除回收。![img.png](img.png)
  - Thread.cpp Thread.cpp是所有的线程的函数文件，主要可以用来实现事件集的逻辑，让各个任务可以按顺序初始化，再执行。
  - Thread.h 是Thread.cpp对应的头文件
  - ThreadConfig.h 是用户事件集的头文件
  - Vari.cpp Vari.cpp是用户自己编写变量，是用来按顺序初始化各个变量，防止因为变量初始化顺序不统一导致的问题。
