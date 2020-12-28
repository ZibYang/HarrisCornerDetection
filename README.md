# HarrisCornerDetection
> CV course homework2
## 0. How to run this project

1. Clone: `https://github.com/ZibYang/HarrisCornerDetection.git`

2. Do the following steps:
```shell
mkdir build && cd build
cmake ..
make
./src/start windowSize sobelKernelSize k # recommend 5 5 0.05
```

**Make sure we can find the camera!**

## 1. What is feature

![]()

1. **Flat** region: no change in all directions

![]()

2. **Edge** region: no change along the edge direction

![]()

3. **Corner** region: significant change in all directions

![]()

## 2. Basic Idea

> The early attempt to find **corners** was done by Chris Harris 7 Mike Stephens in *A Combined Corner and Edge Detector* in 1988 ----from [Wiki](https://docs.opencv.org/3.0-beta/doc/py_tutorials/py_feature2d/py_features_harris/py_features_harris.html)

### 1. Theory

#### I: **Finding the difference in intensity for a displacement of <img src="https://latex.codecogs.com/gif.latex?(u,v)" title="(u,v))"/> in all directions.**

<img src="https://latex.codecogs.com/gif.latex?E(u,v)=\sum_{x,y}w(x,y)[I(x&plus;u,y&plus;v)-I(x,y)]^2" title="E(u,v)=\sum_{x,y}w(x,y)[I(x+u,y+v)-I(x,y)]^2" />

which

-  <img src="https://latex.codecogs.com/gif.latex?w(x,y)" title="w(x,y)" /> is the **window function** 

    - include **Rectangular window** or **Gaussian window**

    ![]() 

    ![]()

- <img src="https://latex.codecogs.com/gif.latex?I(x+u,y+v)" title="I(x+u,y+v)"/> is the **Shifted intensity**

- <img src="https://latex.codecogs.com/gif.latex?I(x,y)" title="I(x,y)"/> is the **Intensity**

we want <img src="https://latex.codecogs.com/gif.latex?I(x+u,y+v)-I(x,y)" title="I(x+u,y+v)-I(x,y)"/> to be near 0 for nearly constant patches and be $Large$ for distinctive patches. Typically, we will apply **Taylor Expansion** to the above equation

<img src="https://latex.codecogs.com/gif.latex?\begin{aligned}&space;f(x&plus;u,y&plus;v)=&space;&&space;f(x,y)&space;&&plus;&&space;uf_x(x,y)&space;&&plus;&&space;vf_y(u,v)&space;\\&space;&plus;&space;&&space;\frac{1}{2!}[u^2f_{xx}(x,y)&space;&&plus;&&space;uvf_{xy}(u,v)&space;&&plus;&v^2f_{yy}(u,v)]&space;\\&space;&plus;&space;&&space;\frac{1}{3!}[u^3f_{xxx}(x,y)&space;&&plus;&&space;u^2vf_{xxy}(u,v)&space;&&plus;&uv^2f_{xyy}(x,y)&space;&&plus;&v^3f_{yyy}(u,v)&space;]&space;\\&space;&plus;&space;&&space;\,&space;...&space;\end{aligned}" title="\begin{aligned} f(x+u,y+v)= & f(x,y) &+& uf_x(x,y) &+& vf_y(u,v) \\ + & \frac{1}{2!}[u^2f_{xx}(x,y) &+& uvf_{xy}(u,v) &+&v^2f_{yy}(u,v)] \\ + & \frac{1}{3!}[u^3f_{xxx}(x,y) &+& u^2vf_{xxy}(u,v) &+&uv^2f_{xyy}(x,y) &+&v^3f_{yyy}(u,v) ] \\ + & \, ... \end{aligned}" />  

Use the **First Order Approx**:  

<img src="https://latex.codecogs.com/gif.latex?f(x&plus;u,y&plus;v)\approx&space;f(x,y)&space;&plus;&space;uf_x(x,y)&space;&plus;&space;vf_y(x,y)" title="f(x+u,y+v)\approx f(x,y) + uf_x(x,y) + vf_y(x,y)" />

<img src="https://latex.codecogs.com/gif.latex?\begin{aligned}&space;\sum&space;&&space;[I(x&plus;u,y&plus;v)-I(x,y)]^2&space;\\&space;\approx\sum&space;&[I(x,y)&space;&plus;&space;uI_x&space;&plus;&space;vI_y&space;-I(x,y)]^2&space;\\&space;=\sum&space;&[u^2I_x^2&plus;2uvI_xI_y&plus;v^2I_y^2]&space;\\&space;=\sum&space;&&space;\begin{bmatrix}&space;u&space;&&space;v&space;\end{bmatrix}&space;\begin{bmatrix}&space;I_x^2&space;&&space;I_xI_y&space;\\&space;I_xI_y&space;&&space;I_y^2&space;\end{bmatrix}&space;\begin{bmatrix}&space;u&space;\\&space;v&space;\end{bmatrix}&space;=&space;\begin{bmatrix}&space;u&space;&&space;v&space;\end{bmatrix}&space;(\sum&space;\begin{bmatrix}&space;I_x^2&space;&&space;I_xI_y&space;\\&space;I_xI_y&space;&&space;I_y^2&space;\end{bmatrix}&space;)&space;\begin{bmatrix}&space;u&space;\\&space;v&space;\end{bmatrix}&space;\end{aligned}" title="\begin{aligned} \sum & [I(x+u,y+v)-I(x,y)]^2 \\ \approx\sum &[I(x,y) + uI_x + vI_y -I(x,y)]^2 \\ =\sum &[u^2I_x^2+2uvI_xI_y+v^2I_y^2] \\ =\sum & \begin{bmatrix} u & v \end{bmatrix} \begin{bmatrix} I_x^2 & I_xI_y \\ I_xI_y & I_y^2 \end{bmatrix} \begin{bmatrix} u \\ v \end{bmatrix} = \begin{bmatrix} u & v \end{bmatrix} (\sum \begin{bmatrix} I_x^2 & I_xI_y \\ I_xI_y & I_y^2 \end{bmatrix} ) \begin{bmatrix} u \\ v \end{bmatrix} \end{aligned}" />

#### ii: **Use a equation to determine if a window can contain a corner or not**

<img src="https://latex.codecogs.com/gif.latex?R=det(M)-k(trace(M))^2" title="R=det(M)-k(trace(M))^2" />  
  
where

<img src="https://latex.codecogs.com/gif.latex?det(M)=\lambda_1\lambda_2\\&space;trace(M)=\lambda_1&plus;\lambda_2\\&space;\lambda_1\lambda_2&space;\,&space;are&space;\,&space;the&space;\,&space;eigen&space;\,&space;values&space;\,of&space;\,&space;M\\&space;k&space;\,is&space;\,&space;an\,&space;empirically\,&space;determined\,constant\,&space;k&space;=&space;0.04-0.06" title="det(M)=\lambda_1\lambda_2\\ trace(M)=\lambda_1+\lambda_2\\ \lambda_1\lambda_2 \, are \, the \, eigen \, values \,of \, M\\ k \,is \, an\, empirically\, determined\,constant\, k = 0.04-0.06" />

When <img src="https://latex.codecogs.com/gif.latex?|R|" title="|R|" /> is small, the region is flat;

When<img src="https://latex.codecogs.com/gif.latex?R<0" title="R<0" /> the region is edge

When <img src="https://latex.codecogs.com/gif.latex?R" title="R" /> is large, the region is a corner

### 2. How to calculate the Ix, Iy and Ixy

> Use Sobel kernel to converte
1. How to calculate sobel kernel due to **kernel size**   

    First, using YangHui trainagle to calculate the **smooth()** and the **diff()**

  <img src="https://latex.codecogs.com/gif.latex?YangHui(k,n)=\left\{\begin{matrix}\frac{n!}{(n-k)!k!} \, k\geq0,k\leq n\\ 0 \, otherwise\end{matrix}\right." title="YangHui" /> 

  <img src="https://latex.codecogs.com/gif.latex?smooth(x, windowSize)=\frac{(windowSize-1)!}{(windowSize-1-x)!x!}" title="smooth"/>

  <img src="https://latex.codecogs.com/gif.latex?diff(x, windowSize)=YangHui(x,\,windowSize-2)-YangHui(x-1,\,YangHui-2)" title=diff/>  

  Then, use **smooth()** and the **diff()** to get the **sobel()**

  <img src="https://latex.codecogs.com/gif.latex?sobel(windowSize)=diff(0...windowSize,\,windowSize)*smooth(0...windowSize,\,windowSize)" title="sobel"/>

  2. How to get the **WindowFunction()**  
     I use Guass function here:  
     <img src="https://latex.codecogs.com/gif.latex?w(u,v)=\frac{1}{2\pi \sigma ^2}e^{-\frac{u^2+v^2}{2\sigma ^2}}" title="w"/>

### 3. Result
1. compare with **OpenCV**
<center class="pics"> 
   <img src="https://github.com/ZibYang/HarrisCornerDetection/raw/main/pics/vspic.png" width="200" alt="vs"/>
</center> 
2. Max, Min and R
Max:  
<center class="pics"> 
   <img src="https://github.com/ZibYang/HarrisCornerDetection/raw/main/pics/slambda1.jpg" width="200" alt="max"/>
</center> 
Min:  
<center class="pics"> 
   <img src="https://github.com/ZibYang/HarrisCornerDetection/raw/main/pics/slambda2.jpg" width="200" alt="min"/>
</center> 
R:  
<center class="pics"> 
   <img src="https://github.com/ZibYang/HarrisCornerDetection/raw/main/pics/sR.jpg" width="200" alt="R"/>
</center> 



