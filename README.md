# Mini Project - 5: Digital Filters

## 執行結果

#### M=8

![](.\result\8\Left%20impulse%20response.png)

![](.\result\8\Right%20impulse%20response.png)

![](.\result\8\Left%20log%20spectrum.png)

![](.\result\8\Right%20log%20spectrum.png)

#### M=32

![](.\result\32\Left%20impulse%20response.png)

![](.\result\32\Right%20impulse%20response.png)

![](.\result\32\Left%20log%20spectrum.png)

![](.\result\32\Right%20log%20spectrum.png)

#### M=1024

![](.\result\1024\Left%20impulse%20response.png)

![](.\result\1024\Right%20impulse%20response.png)

![](.\result\1024\Left%20log%20spectrum.png)

![](.\result\1024\Right%20log%20spectrum.png)

## 綜合討論

由上面幾張圖可得，當M值越大時，其濾波器的impulse response就越多，且可以分的越細

而從圖上的log spectrum可以發現，當M越大時，其Left log spectrum，也就是band-pass filter，可以濾出更短波長的波型，也符合M值的設定

而Right log spectrum，也就是band-stop filter，輸出結果大致相同，也可以猜測我們的band-stop filter是有正常且正確的運作的。
