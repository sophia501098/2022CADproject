### <center>Readme</center>

##### 1、使用说明

`project`文件夹中是vs工程，直接打开CADproject.sln即可

`exe`文件夹中是可执行文件，直接运行CADproject.exe即可，`WASD`可以控制视角，可以得到如下结果：

<img src="pic\result.png" alt="result" style="zoom:67%;" />

注：红色是底面，绿色是sweep出的顶点

##### 2、实验平台

win10 c++14

Visual Studio 2022

OpenGL 4.6.0 

##### 3、实体构造

1）使用欧拉操作构造带2个孔的六边形平面

2）sweep

##### 4、实现

* `Draw.h`：Draw类中定义了各个绘制函数，包括`DrawSolid()`、`DrawFace()`和`DrawFaceWithHole()`
* `Element.h`：定义了几何和拓扑元素，包括类`Point`、`Vertex`、 `HalfEdge`、`Edge`、 `Loop`、 `Face`、`Solid`;
* `Operation.h`：Operation类定义了五个欧拉操作`mvsf()`、`mev()`、`mef()`、`kemrh()`、`kfmrh()`以及`sweep()`操作
* `main.cpp`中构造了实体并使用OpenGL创建窗口绘制