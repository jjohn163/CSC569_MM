# CSC 569 Matrix Multiply Lab
Ryan Solorzano, Kevin Label, Joseph Johnson

| Method             | Num Nodes = 1| Num Nodes = 2| Num Nodes = 4| Num Nodes = 8 | Num Nodes = 16| Num Nodes = 28|
| :----------------  | :----------: | :----------: | :----------: | :-----------: | :-----------: | :-----------: |
| Sequential         | 46.63 s      | 46.94 s      | 46.20 s      | 46.10 s       | 46.03 s       | 45.98 s       |
| OpenMP             | 46.07 s      | 22.45 s      | 11.47 s      | 5.42 s        | 2.93 s        | 1.71 s        |
| OpenMP Tiled - 64* | 14.28 s      | 7.12 s       | 3.67 s       | 2.14 s        | 1.27 s        | 0.66 s        |
| OpenMP Tiled - 128*| 13.57 s      | 7.19 s       | 3.82 s       | 2.38 s        | 1.25 s        | 0.63 s        |
| MPI (2 nodes)      | 32.20 s      | 32.25 s      | 32.29 s      | 31.98 s       | 32.01 s       | 31.88 s       |
| MPI (4 nodes)      | 22.18 s      | 22.44 s      | 22.24 s      | 22.29 s       | 22.11 s       | 22.29 s       |
| MPI + OpenMP       | 13.02 s      | 7.88 s       | 3.60 s       | 2.44 s        | 1.55 s        | 1.21 s        |

*:  These are for the extra credit
