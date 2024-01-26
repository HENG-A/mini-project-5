import numpy as np
import matplotlib.pyplot as plt
from scipy import signal
import math

f = open("hL.txt", "r")
lines = f.readlines()
hL_data = []
for l in lines:
    hL_data.append(float(l))

n = np.arange(0,len(hL_data),1)

plt.stem(n,hL_data)
plt.grid()
plt.xlabel("n")
plt.ylabel("Impulse response")
plt.title("Left impulse response")
plt.savefig('Left impulse response.png')
plt.show()
plt.close()

hR_path = "hR.txt"
f = open("hR.txt", "r")
lines = f.readlines()
hR_data = []
for l in lines:
    hR_data.append(float(l))
f.close()

n = np.arange(0,len(hR_data),1)

plt.stem(n,hR_data)
plt.grid()
plt.xlabel("n")
plt.ylabel("Impulse response")
plt.title("Right impulse response")
plt.savefig('Right impulse response.png')
plt.show()
plt.close()

f = open("YL.txt", "r")
lines = f.readlines()
YL_data = []
for l in lines:
    value = float(l)
    if value > 0:
        YL_data.append(20 * math.log(value, 10))
    else:
        # Handle the case when value <= 0 (avoid math domain error)
        YL_data.append(0)

f.close()

n = np.arange(0,len(YL_data),1)

plt.plot(n,YL_data)
plt.grid()
plt.xlabel("n")
plt.ylabel("log spectrum")
plt.title("Left log spectrum")
plt.savefig('Left log spectrum.png')
plt.show()
plt.close()


f = open("YR.txt", "r")
lines = f.readlines()
YR_data = []
for l in lines:
    value = float(l)
    if value > 0:
        YR_data.append(20 * math.log(value, 10))
    else:
        # Handle the case when value <= 0 (avoid math domain error)
        YR_data.append(0)
f.close()

n = np.arange(0,len(YR_data),1)

plt.plot(n,YR_data)
plt.grid()
plt.xlabel("n")
plt.ylabel("log spectrum")
plt.title("Right log spectrum")
plt.savefig('Right log spectrum.png')
plt.show()
plt.close()
