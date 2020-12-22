# HarrisCornerDetection
> CV course homework2

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

#### I: **Finding the difference in intensity for a displacement of $(u, v)$ in all directions.**

$$
E(u,v)=\sum_{x,y}w(x,y)[I(x+u,y+v)-I(x,y)]^2
$$

which

-  $w(x, y)$ is the **window function** 

  - include **Rectangular window** or **Gaussian window**

    ![]() 

    ![]()

- $I(x+u,y+v)$ is the **Shifted intensity**

- $I(x,y)$ is the **Intensity**

we want $I(x+u,y+v)-I(x,y)$ to be near $0$ for nearly constant patches and be $Large$ for distinctive patches. Typically, we will apply **Taylor Expansion** to the above equation
$$
\begin{aligned}
f(x+u,y+v)= & f(x,y) &+& uf_x(x,y) &+& vf_y(u,v) \\ 
+ & \frac{1}{2!}[u^2f_{xx}(x,y) &+& uvf_{xy}(u,v) &+&v^2f_{yy}(u,v)] \\
+ & \frac{1}{3!}[u^3f_{xxx}(x,y) &+& u^2vf_{xxy}(u,v) &+&uv^2f_{xyy}(x,y) &+&v^3f_{yyy}(u,v) ] \\
+ & \, ...
\end{aligned}
$$
Use the **First Order Approx **: $f(x+u,y+v)\approx f(x,y) + uf_x(x,y) + vf_y(x,y)$
$$
\begin{aligned}
\sum & [I(x+u,y+v)-I(x,y)]^2 \\
\approx\sum &[I(x,y) + uI_x + vI_y -I(x,y)]^2 \\
=\sum &[u^2I_x^2+2uvI_xI_y+v^2I_y^2] \\ 
=\sum & 
\begin{bmatrix}
u & v
\end{bmatrix}
\begin{bmatrix}
I_x^2 & I_xI_y \\
I_xI_y & I_y^2
\end{bmatrix}
\begin{bmatrix}
u \\ v
\end{bmatrix} 
= \begin{bmatrix}
u & v
\end{bmatrix}
(\sum
\begin{bmatrix}
I_x^2 & I_xI_y \\
I_xI_y & I_y^2
\end{bmatrix}
)
\begin{bmatrix}
u \\ v
\end{bmatrix}
\end{aligned}
$$
denote that:
$$
E(u,v) \approx 
\begin{bmatrix}
u & v
\end{bmatrix}
M
\begin{bmatrix}
u \\ v
\end{bmatrix}
$$
where :
$$
M = \sum_{x,y}w(x,y)
\begin{bmatrix}
I_x^2 & I_xI_y \\
I_xI_y & I_y^2
\end{bmatrix}
$$


#### ii: **Use a equation to determine if a window can contain a corner or not**

$$
R=det(M)-k(trace(M))^2
$$

where

- $det(M)=\lambda_1\lambda_2$
- $trace(M)=\lambda_1+\lambda_2$
- $\lambda_1\lambda_2$ are the eigen values of $M$
- $k$ is an empirically determined constant; $k = 0.04-0.06$

When $|R|$ is small, the region is flat;

When $R<0$, the region is edge

When $R$ is large, the region is a corner