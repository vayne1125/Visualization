# Visualization

1132 NTOU Visualization class <br>
You can read pdf in ./hw_doc to know my project. <br>
This project implements various visualization methods, including the marching cube method (iso-surface method), slice method, and raycasting method. Additionally, it utilizes imgui and implot to create GUI interfaces.

## Environment
You should have CMake and a C++ compilation environment installed.<br>
*Note: I used https://github.com/skeeto/w64devkit/releases for C++. Using an older version of Mingw may cause errors with glm and implot.*

## Usage
```
1. git clone https://github.com/vayne1125/Visualization.git
2. cd extern
3. Clone imgui, glfw, and glm from their respective GitHub repositories.
4. cd lib
5. Unzip the include and put the include folder and CMakeLists.txt into the imgui folder.
```
## Introduction and Show
### Implement Marching Cube Method
- 使用 Marching-cube method 畫出 iso surface
- 使用梯度(gradient)計算點的法向量來呈現光影
- 利用計算平面(ax + by + cz + d = 0)到點的距離，畫出 cross-section
- 利用 HSV 色彩坐標系顯示不同硬度的顏色，越硬越紅
- 利用資料結構維護不同硬度的顯示與渲染
- 畫出長條圖(histogram)顯示硬度分布
<img src="/readmeimg/hw1_1.png" alt="hw1_1" title="hw1_1"/>

#### other model
| engine  | teddybear | golf |
|---|---|---|
|<img src="/readmeimg/hw1_3_2.png" alt="hw1_3" title="hw1" height="250"/>|<img src="/readmeimg/hw1_4_2.png" alt="hw1_3" title="hw1" height="250"/>| <img src="/readmeimg/hw1_5_2.png" alt="hw1_3" title="hw1" height="250"/>|


