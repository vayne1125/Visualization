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
5. Unzip the include and put the include folder, CMakeLists.txt and 5 implot file into the imgui folder.
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
|<img src="/readmeimg/hw1_3.png" alt="hw1_3.png" title="hw1_3.png" height="250"/>|<img src="/readmeimg/hw1_4.png" alt="hw1_4.png" title="hw1_4.png" height="250"/>| <img src="/readmeimg/hw1_5.png" alt="hw1_5.png" title="hw1_5.png" height="250"/>|

#### cross-section
| original  | cross-section |
|---|---|
|<img src="/readmeimg/hw1_6.png" alt="hw1_6.png" title="hw1_6.png" height="250"/>|<img src="/readmeimg/hw1_6_cs.png" alt="hw1_6_cs.png" title="hw1_6_cs.png" height="250"/>|
|<img src="/readmeimg/hw1_7.png" alt="hw1_7.png" title="hw1_7.png" height="250"/>|<img src="/readmeimg/hw1_7_cs.png" alt="hw1_7_cs.png" title="hw1_7_cs.png" height="250"/>|

### Implement Slice Method
- 建立六個方向的切片(x/y/z + back-to-front/ front-to-back)
- 利用內插計算每一片的texture coordinate
- 計算正方體六個面到相機的距離，決定從哪個方向切片，以達到物體或相機旋轉的正常顯示
- 用 gradient 和 iso-value 建立 3d texture
- 利用 implot 建立自定義的 1d-texture 編輯器
- 可以透過 GUI 選擇切片的數量

<img src="/readmeimg/hw2_slice.png" alt="hw2_slice.png" title="hw2_slice.png"/>

#### different slice num with phong shading
|| 512 | 1024 |
|---|---|---|
|dont<br>use<br>phong|<img src="/readmeimg/hw2_1_512.png" height="250"/>|<img src="/readmeimg/hw2_1_1024.png" height="250"/>|
|use<br>phong|<img src="/readmeimg/hw2_1_512_ph.png" height="250"/>|<img src="/readmeimg/hw2_1_1024_ph.png" height="250"/>|


### Implement Raycasting Method
- 建立正方體六個面(12個三角形)的VAO
- 計算對應的texture coordinate (xmin,ymin,zmin) -> (0,0,0)
- Ray的方向為眼睛看向物體的方向
- 用 gradient 和 iso-value 建立 3d texture
- 利用 implot 建立自定義的 1d-texture 編輯器
- 可以透過 GUI 選擇切片的數量

<img src="/readmeimg/hw2_ray.png" alt="hw2_ray.png" title="hw2_ray.png"/>

#### different gap between two sample point with phong shading
|| big gap | small gap |
|---|---|---|
|dont<br>use<br>phong|<img src="/readmeimg/hw2_2_1.png" height="250"/>|<img src="/readmeimg/hw2_2_2.png" height="250"/>|
|use<br>phong|<img src="/readmeimg/hw2_2_1_ph.png" height="250"/>|<img src="/readmeimg/hw2_2_2_ph.png" height="250"/>|

#### 在相同 1d texture及取樣比率接近的情況下兩種方法之比較
- teddybear resolution: 128
- slice: 512 (= 0.255)

| slice | raycasting |
|---|---|
|<img src="/readmeimg/hw2_3_slice.png" height="250"/>|<img src="/readmeimg/hw2_3_ray.png" height="250"/>|
