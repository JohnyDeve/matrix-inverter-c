![](https://img.shields.io/badge/TEXT-HEX-COLOR?style=for-the-badge&logo=с&logoColor=#A8B9CC)
![](https://img.shields.io/badge/github-repo-blue?logo=c)
[![C Language](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)](https://wikipedia.org)
![License](https://shields.io)
![Build](https://shields.io)
![Repo Size](https://shields.io)
[![C Language](https://shields.io)](https://wikipedia.org)
# Matrix Inverter (C implementation)

Console utility in C language designed to calculate the inverse HxW matrix using the **LU decomposition method**. The program focuses on reliability, strict error handling, and efficient memory management.

## Core Features

*   **Algorithm**: Utilizes LU Decomposition instead of the standard Gauss-Jordan elimination, providing an efficient approach for dense matrices.
*   **Resource Management**: Implements the `Context` pattern (an approach similar to RAII). All dynamic allocations and file descriptors are encapsulated within a single structure, ensuring no memory leaks even when a critical error occurs.
*   **Error Handling**: Built on a centralized status code table. In the event of a failure, the program gracefully shuts down via `clear_context`, reporting a specific error message and detailed comments to `stderr`.

## Data Format

The program processes plain text files.

**Input File (`input.txt`):**
The first line must contain two integers: `height` and `width`. These are followed by the matrix elements.
```text
3 3
1 2 3
0 1 4
5 6 0
```

**Output File (`output.txt`):**
Contains the matrix dimensions and resulting inverse matrix or the message `no_solution` if the matrix is singular (non-invertible).
```text
3 3
-24 18 5 
20 -15 -4 
-5 4 1
```

## Build and Usage

To build the project, use any standard C compiler (GCC/Clang):

```bash
gcc main.c -o matrix_inverter
```

**Execution:**
```bash
./matrix_inverter input.txt output.txt
```

## Technical Overview

### Error Reporting
The program uses a structured error-handling system. Each state corresponds to an entry in the `status_message_table`.
Example `stderr` output:
> `[ERROR]: Executor cannot allocate memory`

### The Context Structure
```c
typedef struct
{
        FILE *input_file;
        FILE *output_file;
        double *mat;
        double *Lmat;
        double *Umat;
} Context;
```
This architecture allows for centralized resource cleanup in the `clear_context(Context *ctx)` function. 
This keeps the business logic clean and avoids "if-else nests" when managing pointers and file handles.

## Future Improvements
While the current version is fully functional and stable, the following features are planned for future releases:
- [ ] Multithreading support for large-scale matrices using POSIX threads.
- [ ] SIMD optimization for the LU decomposition core.

## Contributing
Contributions are welcome! If you find a bug or have a suggestion for optimization:
1. Fork the repository.
2. Create a new branch (`git checkout -b feature/improvement`).
3. Commit your changes.
4. Push to the branch and open a Pull Request.

Please ensure that any new code follows the existing `Context`-based error handling pattern to maintain memory safety.

## License
This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for more details. 

Developed with focus on efficiency and reliability by [Kuzyakin Ivan/JohnyDeve].
