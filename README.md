# CSC 569 Matrix Multiply Lab

| Method             | Num Nodes = 1| Num Nodes = 2| Num Nodes = 4| Num Nodes = 8| Num Nodes = 16 | Num Nodes = 28|
| :----------------  | :----------: | :----------: | :----------: | :-----------: | :-----------: | :-----------: |
| Sequential         | 46.63 s      | 46.94 s      | 46.20 s      | 46.10 s       | 46.03 s       | 45.98 s       |
| OpenMP             | 46.07 s      | 22.45 s      | 11.47 s      | 5.42 s        | 2.93 s        | 1.71 s        |
| OpenMP Tiled - 64* | 14.35 s      | 7.12 s       | 3.67 s       | 2.14 s        | 1.27 s        | 1.53 s        |
| OpenMP Tiled - 128*| 13.94 s      | 7.20 s       | 3.88 s       | 2.38 s        | 1.28 s        | 1.51 s        |
| MPI                | 69.420 s     | 69.420 s     | 69.420 s     | 69.420 s      | 69.420 s      | 45.83 s       |
| MPI + OpenMP       | 69.420 s     | 69.420 s     | 69.420 s     | 69.420 s      | 69.420 s      | 45.83 s       |
| MPI + OpenMP Tiled*| 69.420 s     | 69.420 s     | 69.420 s     | 69.420 s      | 69.420 s      | 45.83 s       |

*: These rows are for the extra credit
