# Hw2: Multi-thread Programming

Please some detail in [PP website](https://nycu-sslab.github.io/PP-f22/assignments/HW2/)

1. Compute the top half of the image in thread 0, and the bottom half of the image in thread 1
2. Extend the code to use 2, 3, 4 threads, partitioning the image generation work accordingly

## Q1
In your write-up, produce a graph of speedup compared to the reference sequential implementation as a function of the number of threads used FOR VIEW 1. Is speedup linear in the number of threads used? In your writeup hypothesize why this is (or is not) the case?
1. view1
    ![](https://i.imgur.com/XPqxfUi.png)
2. view2
    ![](https://i.imgur.com/gkiLvvz.png)

**Ans:** view1, view2 的加速皆不是線性成長，且可以觀察到在 view1 中，3線程比2, 4線程都還要差。view1, view2 差別只在繪製區塊不同，故可以發現圖片中不同區塊所需要的運算量或許是不同的，使每個線程之間的工作量不同，最大工作量的線程拖慢了其他線程的速度。

## Q2
How do your measurements explain the speedup graph you previously created?
1. view1
    ![](https://i.imgur.com/pN1nkU3.png)
2. view2
    ![](https://i.imgur.com/xteqfJN.png)

**Ans:** 以 2, 3, 4, 5, 32 線程分別執行 view1, view2，並且分別量測每個線程的執行時間，可以發現 view1 呈現鐘形分佈，view2 會隨著線程 ID 遞減