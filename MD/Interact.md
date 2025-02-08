# Interact类介绍

> 想法
>
> Interact下有三种交互大类
>
> 1. 遥控器链路$\rightarrow$RemoteControl类$\xrightarrow{调用}$对应的串口类SuperUart类
> 2. 图传链路$\rightarrow$ImageTrans类$\xrightarrow{调用}$对应的串口类SuperUart类
> 3. PC链路$\rightarrow$SuperCDC类$\xrightarrow{调用}$对应的USB虚拟串口类SuperCDC类
>
> 在遥控器链路下又可细分为
>
> 1. 遥控器的拨杆模式
>
>    拨杆下又细分机械臂的控制模式
>
>    1. 机械臂单独的各个关节的操作$\rightarrow$Normal模式
>    2. 机械臂的末端xyz坐标移动的操作$\rightarrow$XYZ模式
>    3. 机械臂的复位操作$\rightarrow$Reset模式
>
> 2. 遥控器的键鼠模式
>
> 在图传链路下又可细分为
>
> 1. 自定义控制器模式的下行
>
>    此模式为机械臂的自定义控制模式，图传将会向mcu发送30字节数据，各个关节一一映射$\rightarrow$custom模式
>
> 2. 自定义控制器模式的上行
>
>    此模式为机械臂的自定义控制模式，mcu会向mcu发送30字节数据$\rightarrow$custom模式
>
> 3. 自定义控制器模拟的键鼠
>
> 4. 图传的键鼠模式
>
> 在PC链路下又可细分为
>
> 1. PC链路的下行
>
>    此模式是把机械臂的控制交给上位机PC，PC给mcu发送数据$\rightarrow$vision模式
>
> 2. PC链路的上行
>
>    此模式是把机械臂的控制交给上位机PC，mcu给PC发送数据$\rightarrow$vision模式
>
>  
>
> 总结交互分为3种链路模式
>
> 1. remote_control
> 2. image_trans
> 3. PC
>
> 在此基础下机械臂的控制模式分为
>
> 1. Normal模式
> 2. XYZ模式
> 3. Reset模式
> 4. custom模式
> 5. vision模式
> 6. actions模式（按键触发，执行动作组模式）
>
> 所有模式的都应该有自己的数据缓冲区，然后将数据缓冲区里的内容在对应到receive_data和transmit_data里面中。做到各自都有自己的私有的备份数据，并且都转换到了统一的结构体中，做到对机械臂提供统一的接口。然后再由机械臂统一根据receive_data里的数据执行对应的动作。