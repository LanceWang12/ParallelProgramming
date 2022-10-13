---

tags: Parallel Programming

---

[toc]

# HW1
310552021 王睿成
## Part 1
### Q1-1: 
- Does the vector utilization increase, decrease or stay the same as VECTOR_WIDTH changes? Why?
    ```
    CLAMPED EXPONENT (required)
    Results matched with answer!
    ****************** Printing Vector Unit Statistics *******************
    Vector Width:              4
    Total Vector Instructions: 2381
    Vector Utilization:        84.5%
    Utilized Vector Lanes:     8048
    Total Vector Lanes:        9524
    ************************ Result Verification *************************
    ClampedExp Passed!!!
    ```

    ```
    CLAMPED EXPONENT (required)
    Results matched with answer!
    ****************** Printing Vector Unit Statistics *******************
    Vector Width:              8
    Total Vector Instructions: 1291
    Vector Utilization:        83.2%
    Utilized Vector Lanes:     8588
    Total Vector Lanes:        10328
    ************************ Result Verification *************************
    ClampedExp Passed!!!
    ```

    ```
    CLAMPED EXPONENT (required)
    Results matched with answer!
    ****************** Printing Vector Unit Statistics *******************
    Vector Width:              16
    Total Vector Instructions: 656
    Vector Utilization:        83.0%
    Utilized Vector Lanes:     8708
    Total Vector Lanes:        10496
    ************************ Result Verification *************************
    ClampedExp Passed!!!
    ```


- 以例題一來說 VECTOR_WIDTH 變大，vector utilization 下降
- Masked lane 數量越低時， vector utilization 會越高。在程式中進行指數運算時會使用到大量的 masked lane，因為指數部分大小差距較大，大部分 lane 已停止運算，只有少數幾個指數較大的項還在做運算，此時隨著 VECTOR_WIDTH 變大會有更多閒置的通道，故隨著 VECTOR_WIDTH 變高，vector utilization 會變低

## Part 2
### Q2-1:
送分

### Q2-2
1. What speedup does the vectorized code achieve over the unvectorized code?
    Execute 10 times in average.  Speed up 3.X by using vector.
    使用以下 python code 進行平均值計算：
    ```python=
    import os
    import numpy

    size = 10

    for ID in range(2, 4):

        lst = ['make', 'make VECTORIZE=1']
        output = {key: np.zeros(size) for key in lst}
        for ins in lst:
            for i in range(size):
                output[ins][i] = os.popen(f'make clean && {ins} && ./test_auto_vectorize -t {ID} | grep -o "[0-9]*\.[0-9]*sec" | grep -o "[0-9*\.[0-9]*"').read()

            #break

        # report
        print(f'Execute test{ID}.cpp')
        for key in output.keys():
            mean = np.mean(output[key])
            if key == 'make':
                tmp = 'No Vector'
            elif key == 'make VECTORIZE=1':
                tmp = 'Vector'
            print(f'{tmp}: {mean:.4f}sec in average (execute 10 times).')
        break
    ```
    | File Name | Using Vector | Time (sec) |
    |:---------:|:------------:|:----------:|
    | test1.cpp |      No      |   7.6075   |
    | test1.cpp |     Yes      |   2.4113   |

2. What additional speedup does using -mavx2 give (AVX2=1 in the Makefile)?
    - 送分
3. What can you infer about the bit width of the default vector registers on the PP machines?
    - 以下為 vector 程式 inner loop 的部分，mov, add 指令存取資料時間間隔寬度為 16 bytes，可推測出 vector register 寬度應為 128bits(16 * 8)。
    ```
    .LBB0_3:                            #    Parent Loop BB0_2 Depth=1
                                        # => This Inner Loop Header: Depth=2
	movaps	(%rbx,%rcx,4), %xmm0
	movaps	16(%rbx,%rcx,4), %xmm1
	addps	(%rbp,%rcx,4), %xmm0
	addps	16(%rbp,%rcx,4), %xmm1
	movaps	%xmm0, (%r15,%rcx,4)
	movaps	%xmm1, 16(%r15,%rcx,4)
	movaps	32(%rbx,%rcx,4), %xmm0
	movaps	48(%rbx,%rcx,4), %xmm1
	addps	32(%rbp,%rcx,4), %xmm0
	addps	48(%rbp,%rcx,4), %xmm1
	movaps	%xmm0, 32(%r15,%rcx,4)
	movaps	%xmm1, 48(%r15,%rcx,4)
	addq	$16, %rcx
	cmpq	$1024, %rcx                     # imm = 0x400
    ```
4. What about the bit width of the AVX2 vector registers.
    - 送分

### Q2-3: 
- Provide a theory for why the compiler is generating dramatically different assembly.
- 因為透過告知編譯器程式訊息，如：陣列記憶體的對齊, 指標是否存取共同資源區塊等，使編譯器可以更優化的方式來進行編譯，進而起到加速的效果
