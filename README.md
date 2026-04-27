<p align="center">
  <b>English</b> | <a href="./README.ru.md">Русский</a>
</p>

<p align="center">
  <img src="./readme_assets/inverse-matrix-x-banner.png" alt="Matrix Inverter Banner" width="700">
</p>

[![C Language Version 11](https://img.shields.io/badge/C11-00599C?style=for-the-badge&logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C11_(C_standard_revision))
[![Code Style](https://img.shields.io/badge/Code%20Style-clang--format-673DE6?style=for-the-badge)](https://clang.llvm.org/docs/ClangFormat.html)
[![License: MIT](https://img.shields.io/badge/MIT-green?style=for-the-badge)](https://opensource.org/license/mit)

[![GitHub Action](https://img.shields.io/badge/GitHub_Actions-2088FF?style=for-the-badge&logo=github-actions&logoColor=white)](https://github.com/JohnyDeve/matrix-inverter-c/actions)
![Repo Size](https://img.shields.io/github/repo-size/JohnyDeve/matrix-inverter-c?style=for-the-badge)
![Last Commit](https://img.shields.io/github/last-commit/JohnyDeve/matrix-inverter-c?style=for-the-badge)
[![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/JohnyDeve/matrix-inverter-c/clang-format-check.yml?event=push&style=for-the-badge&logo=githubactions&logoColor=white&label=clang%20format&labelColor=673DE6)](https://github.com/JohnyDeve/matrix-inverter-c/actions/workflows/clang-format-check.yml)

---

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

---
## Contacts & Support

### Author
**Your Name** — C Developer & System Architect  
Feel free to reach out for collaboration or questions regarding the implementation.

[![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white)](https://github.com/JohnyDeve)
[![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white)](www.linkedin.com/in/ivan-kuzyakin-1010011100000001010000110000101011011)
[![Telegram](https://img.shields.io/badge/Telegram-2CA5E0?style=for-the-badge&logo=telegram&logoColor=white)](https://t.me/kuzak1n)

### Support the Project
If this tool helped you or saved you some time, you can support its further development:

[![DonationAlerts](https://img.shields.io/badge/DonationAlerts-F57607?style=for-the-badge)](https://www.donationalerts.com/r/kuzakinc)

---
*Maintained with precision and care.*
