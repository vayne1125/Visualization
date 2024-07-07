# Visualization

1122 NTOU Visualization class <br>
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
|:---:|:---:|:---:|
|<img src="/readmeimg/hw1_3.png" alt="hw1_3.png" title="hw1_3.png" height="250"/>|<img src="/readmeimg/hw1_4.png" alt="hw1_4.png" title="hw1_4.png" height="250"/>| <img src="/readmeimg/hw1_5.png" alt="hw1_5.png" title="hw1_5.png" height="250"/>|

#### cross-section
| original  | cross-section |
|:---:|:---:|
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
|:---:|:---:|:---:|
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

#### Different gap between two sample point with phong shading
|| big gap | small gap |
|:---:|:---:|:---:|
|dont<br>use<br>phong|<img src="/readmeimg/hw2_2_1.png" height="250"/>|<img src="/readmeimg/hw2_2_2.png" height="250"/>|
|use<br>phong|<img src="/readmeimg/hw2_2_1_ph.png" height="250"/>|<img src="/readmeimg/hw2_2_2_ph.png" height="250"/>|

#### 在相同 1d texture及取樣比率接近的情況下兩種方法之比較
- teddybear resolution: 128
- slice: 512 (= 0.255)

| slice | raycasting |
|:---:|:---:|
|<img src="/readmeimg/hw2_3_slice.png" height="250"/>|<img src="/readmeimg/hw2_3_ray.png" height="250"/>|

### Implement RK2 Method(Streamline)
- 計算當前點的方向和預計位置的方向取平均作為前進的方向
- 利用內插計算格子點中的準確速度
- 紀錄流線的遍歷位置，避免重複走到或是交錯
- 使用幾何著色器，將原本的線條繪製成梯形，達到頭粗尾細的效果
- 將速度分布標準化到0-1之間，並根據速度大小決定顏色
- 可以透過GUI調整前進距離、線密度、兩線的間距、至多至少取樣點數量、線的粗細參數以及速度分布的顏色圖
<img src="/readmeimg/hw3_1.png"/>

#### 全白
| 1.vec | 8.vec |14.vec |
|:---:|:---:|:---:|
|<img src="/readmeimg/hw3_2_1.png" height="250"/>|<img src="/readmeimg/hw3_2_8.png" height="250"/>|<img src="/readmeimg/hw3_2_10.png" height="250"/>|

#### 考慮速度(紅快藍慢)
| 7.vec | 19.vec | rect2.vec|
|:---:|:---:|:---:|
|<img src="/readmeimg/hw3_3_7.png" height="250"/>|<img src="/readmeimg/hw3_3_19.png" height="250"/>|<img src="/readmeimg/hw3_3_rect2.png" height="250"/>|

#### 較大的圖(512 x 512)
| test_not_unit.vec | test_unit.vec |
|:---:|:---:|
|<img src="/readmeimg/hw3_4_test_not_unit.png" height="320"/>|<img src="/readmeimg/hw3_4_test_unit.png" height="320"/>|

### Implement Sammon Mapping & PCA
- 隨機在二維平面佈點
- 計算高維度(原本資料)和二維度的距離比例以及兩點的向量
- 如果太近，就往反向移動；太遠，則靠近，持續迭代至收斂為止
- 計算並利用同群點的斜方差矩陣、特徵值及特徵向量繪製橢圓
- 使用幾何著色器，將原本的點繪製成圓型
- 根據螢幕比例繪製圓型，以達到拉伸畫面不會造成變形
- 可以透過GUI調整資料點個數、繪製橢圓的sigma、大小、顏色

<img src="/readmeimg/hw4.png"/>

#### 使用 900 筆資料(15維度)
| 原始點分布(隨機分布) | Apply Sammon Mapping|
|:---:|:---:|
|<img src="/readmeimg/hw4_1.png" height="320"/>|<img src="/readmeimg/hw4_2.png" height="320"/>|


#### 利用特徵值找出圈出點的橢圓形
| sigma = 1 (68.27%) | sigma = 2 (95.45%)|
|:---:|:---:|
|<img src="/readmeimg/hw4_2_1.png" height="320"/>|<img src="/readmeimg/hw4_2_2.png" height="320"/>|
| **sigma = (99.73%)** | **sigma = 1,2,3** |
|<img src="/readmeimg/hw4_2_3.png" height="320"/>|<img src="/readmeimg/hw4_2_4.png" height="320"/>|

### Implement SOM Method
Demo Video: https://youtu.be/8l7O3fyZHcQ
- 建立大小為k*k的網格點，並隨機給網格點n個值(n為訓練資料的維度)
- 每次迭代，隨機選一筆資料，並找出最像的網格點(BMU)
- 利用歐幾里得距離計算網格點間的位置，並更新自己和鄰居
- 隨著迭代次數更新學習率及鄰居範圍
- 利用外積計算三角形的法向量
- 利用幾何著色器將法向量視覺化
- 可以透過GUI調整lattice及渲染設定

<img src="/readmeimg/hw5.png"/>

#### 參數
| 參數 | 值 |
| ---  | --- |
| Start Learning Rate |  0.01 |
| Start Neighbourhood Radius | k/4 |
|Iterations | 500'000|
|Learning Rate| Start Learning Rate * exp(當前迭代次數/iterations)|
|Neighbourhood Radius | Start Neighbourhood Radius * ((Iterations - 當前迭代次數)/ Iterations)^2 (min = 1) |

#### 不同的 model 渲染成不同的樣子

|                       | line | surface |
|:--------------------: | :---:| :---:   |
| vase   <br> (32 * 32) |<img src="/readmeimg/hw5_1_vase_line.png" width="300"/>     | <img src="/readmeimg/hw5_1_vase_surface.png"  width="300"/>      |
| teapot <br> (32 * 32) |<img src="/readmeimg/hw5_1_teapot_line.png" width="300"/>   | <img src="/readmeimg/hw5_1_teapot_surface.png" width="300"/>   |
| bunny  <br> (32 * 32) |<img src="/readmeimg/hw5_1_bunny_line.png" width="300"/>    | <img src="/readmeimg/hw5_1_bunny_surface.png" width="300"/>    |
|fountain<br> (32 * 32) |<img src="/readmeimg/hw5_1_fountain_line.png" width="300"/> | <img src="/readmeimg/hw5_1_fountain_surface.png" width="300"/> |
| cloud  <br> (64 * 64) |<img src="/readmeimg/hw5_1_cloud_line.png" width="300"/>    | <img src="/readmeimg/hw5_1_cloud_surface.png" width="300"/>    |
|  cat   <br> (64 * 64) |<img src="/readmeimg/hw5_1_cat_line.png" width="300"/>      | <img src="/readmeimg/hw5_1_cat_surface.png" width="300"/>      |

#### 不同的 lattice size

|         | 16 * 16 | 32 * 32 | 64 * 64 |
|:------: | :------:| :------:| :------:|
|  line   |<img src="/readmeimg/hw5_2_1_1.png" height="400"/>|<img src="/readmeimg/hw5_2_1_2.png" height="400"/>|<img src="/readmeimg/hw5_2_1_3.png" height="400"/>|
| surface |<img src="/readmeimg/hw5_2_2_1.png" height="400"/>|<img src="/readmeimg/hw5_2_2_2.png" height="400"/>|<img src="/readmeimg/hw5_2_2_3.png" height="400"/>|

#### 不同的 fitting mesh(皆是使用 32 * 32 的網格)
|         | cylinder | plane |
|:------: | :-------:| :----:|
|  初始   |<img src="/readmeimg/hw5_3_1_1.png" height="400"/>|<img src="/readmeimg/hw5_3_1_2.png" height="400"/>|
| line    |<img src="/readmeimg/hw5_3_2_1.png" height="400"/>|<img src="/readmeimg/hw5_3_2_2.png" height="400"/>|
| surface |<img src="/readmeimg/hw5_3_3_1.png" height="400"/>|<img src="/readmeimg/hw5_3_3_2.png" height="400"/>|

#### 貼上紋理(皆是使用64 * 64的網格)
|     rainbow cloud     |  yellow marble | cheese |
|:--------------------: | :-------------:| :-----:|
|<img src="/readmeimg/hw5_4_1.png" height="400"/> |<img src="/readmeimg/hw5_4_2.png" height="400"/>|<img src="/readmeimg/hw5_4_3.png" height="400"/>|

#### 法向量視覺化(皆是使用64 * 64的網格)
|     vase     |  teapot | Bunny |
|:----------:  | :-----: |:-----:|
|<img src="/readmeimg/hw5_5_1.png" height="320"/>|<img src="/readmeimg/hw5_5_2.png" height="320"/>|<img src="/readmeimg/hw5_5_3.png" height="320"/>|
