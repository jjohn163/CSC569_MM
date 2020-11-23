# CSC 569 Matrix Multiply Lab

| Method             | Num Nodes = 1| Num Nodes = 4| Num Nodes = 8| Num Nodes = 16| Num Nodes = 28|
| :----------------  | :----------: | :----------: | :----------: | :-----------: | :-----------: |
| Sequential         | 25.26 s      | 45.56 s      | 45.05 s      | 45.66 s       | 45.83 s       |
| OpenMP             | 87.05 s      | 43.43 s      | 21.73 s      | 5.85 s        | 3.54 s        |
| OpenMP Tiled*      | 14.24 s      | 7.11 s       | 3.70 s       | 1.23 s        | 0.66 s        |
| MPI                | 69.420 s     | 69.420 s     | 69.420 s     | 69.420 s      | 69.420 s      |
| MPI + OpenMP       | 69.420 s     | 69.420 s     | 69.420 s     | 69.420 s      | 69.420 s      |
| MPI + OpenMP Tiled*| 69.420 s     | 69.420 s     | 69.420 s     | 69.420 s      | 69.420 s      |

*: These rows are for the extra credit
