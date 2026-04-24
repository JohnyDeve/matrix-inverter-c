#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define ERROR_CANNOT_OPEN_FILE 1
#define ERROR_DATA_READING 2
#define ERROR_OUT_OF_MEMORY 3
#define ERROR_INCORRECT_COMMAND_ARGS 4
#define WARNING_CANNOT_CLOSE_FILE 5
#define ERROR_DATA_WRITING 6
#define GETLOC(matrix, i, j, W) ((matrix) + ((W) * (i) + (j)))

typedef struct
{
        FILE *input_file;
        FILE *output_file;
        double *mat;
        double *Lmat;
        double *Umat;
} Context;

static const char *status_message_table[] = {
        [SUCCESS] = "",
        [ERROR_CANNOT_OPEN_FILE] = "Executor cannot open the file",
        [ERROR_DATA_READING] = "Executor cannot read data in the file",
        [ERROR_OUT_OF_MEMORY] = "Executor can't allocate memory",
        [ERROR_INCORRECT_COMMAND_ARGS] = "Executor get incorrect command line arguments",
        [WARNING_CANNOT_CLOSE_FILE] = "Executor cannot close the file, implementation continued...",
        [ERROR_DATA_WRITING] = "Executor cannot write data in the file"
};

static void drop_warning(int warning)
{
        fprintf(stderr, "[WARNING]: %s\n", status_message_table[warning]);
}

static int clear_context(Context *context, int error_code)
{
        if (context->mat)
        {
                free(context->mat);
                context->mat = NULL;
        }
        if (context->Lmat)
        {
                free(context->Lmat);
                context->Lmat = NULL;
        }
        if (context->Umat)
        {
                free(context->Umat);
                context->Umat = NULL;
        }

        if (context->input_file)
        {
                if (fclose(context->input_file))
                {
                        drop_warning(WARNING_CANNOT_CLOSE_FILE);
                }
                context->input_file = NULL;
        }
        if (context->output_file)
        {
                if (fclose(context->output_file))
                {
                        drop_warning(WARNING_CANNOT_CLOSE_FILE);
                }
                context->output_file = NULL;
        }
        if (error_code != SUCCESS)
        {
                fprintf(stderr, "[ERROR]: %s\n", status_message_table[error_code]);
        }
        return error_code != SUCCESS;
}

static int get_inv_matrix(double *A, double *L, double *U, size_t H, size_t W)
{
        if (H != W)
        {
                return 1;
        }

        for (size_t i = 0; i < W; i++)
        {
                *GETLOC(L, i, i, W) = 1.0;
        }

        double weights_sum = 0.0;
        for (size_t i = 0; i < W; i++)
        {
                for (size_t k = i; k < W; k++)
                {
                        weights_sum = 0.0;
                        for (size_t j = 0; j < i; j++)
                        {
                                weights_sum += (*GETLOC(L, i, j, W) * *GETLOC(U, j, k, W));
                        }
                        *GETLOC(U, i, k, W) = *GETLOC(A, i, k, W) - weights_sum;
                }

                for (size_t k = i; k < W; k++)
                {
                        weights_sum = 0.0;
                        for (size_t j = 0; j < i; j++)
                        {
                                weights_sum += (*GETLOC(L, k, j, W) * *GETLOC(U, j, i, W));
                        }
                        *GETLOC(L, k, i, W) = (*GETLOC(A, k, i, W) - weights_sum) / *GETLOC(U, i, i, W);
                }
        }

        for (size_t i = 0; i < W; i++)
        {
                if (*GETLOC(U, i, i, W) == 0.0)
                {
                        return 1;
                }
        }

        for (size_t i = 0; i < W; i++)
        {
                for (size_t j = 0; j < W; j++)
                {
                        *GETLOC(A, i, j, W) = (i == j) ? 1.0 : 0.0;

                        if (i == 0)
                        {
                                continue;
                        }
                        weights_sum = 0.0;
                        for (size_t k = 0; k < i; k++)
                        {
                                weights_sum += (*GETLOC(L, i, k, W) * *GETLOC(A, k, j, W));
                        }
                        *GETLOC(A, i, j, W) -= weights_sum;
                }
        }

        for (size_t i = W; i-- > 0;)
        {
                for (size_t j = 0; j < W; j++)
                {
                        *GETLOC(L, i, j, W) = *GETLOC(A, i, j, W);
                        if (i != W - 1)
                        {
                                weights_sum = 0.0;
                                for (size_t k = i + 1; k < W; k++)
                                {
                                        weights_sum += (*GETLOC(U, i, k, W) * *GETLOC(L, k, j, W));
                                }
                        }
                        *GETLOC(L, i, j, W) = (*GETLOC(L, i, j, W) - weights_sum) / (*GETLOC(U, i, i, W));
                }
        }

        return 0;
}

static int read_filedata(double *mat, FILE *input_file, size_t W, size_t H)
{
        for (size_t i = 0; i < H; i++)
        {
                for (size_t j = 0; j < W; j++)
                {
                        if (fscanf(input_file, "%lf", GETLOC(mat, i, j, W)) != 1)
                        {
                                return 1;
                        }
                }
        }
        return SUCCESS;
}

static int write_filedata(double *mat, FILE *output_file, size_t W, size_t H)
{
        if (fprintf(output_file, "%zu %zu\n", W, H) < 0)
        {
                return 1;
        }

        for (size_t i = 0; i < W; i++)
        {
                for (size_t j = 0; j < W; j++)
                {
                        if (fprintf(output_file, "%g ", *GETLOC(mat, i, j, W)) < 0)
                        {
                                return 1;
                        }
                }
                if (fprintf(output_file, "\n") < 0)
                {
                        return 1;
                }
        }
        return SUCCESS;
}

int main(int argc, char *argv[])
{
        if (argc != 3)
        {
                fprintf(stderr, "[ERROR]: %s\n", status_message_table[ERROR_INCORRECT_COMMAND_ARGS]);
                return 1;
        }

        Context context[1] = { NULL };

        context->input_file = fopen(argv[1], "r");
        if (!context->input_file)
        {
                return clear_context(context, ERROR_CANNOT_OPEN_FILE);
        }

        size_t H, W;
        if (fscanf(context->input_file, "%zu %zu", &W, &H) != 2)
        {
                return clear_context(context, ERROR_DATA_READING);
        }

        context->mat = (double *)malloc(sizeof(double) * W * H);
        if (!context->mat)
        {
                return clear_context(context, ERROR_OUT_OF_MEMORY);
        }

        context->Lmat = (double *)calloc(W * H, sizeof(double));
        if (!context->Lmat)
        {
                return clear_context(context, ERROR_OUT_OF_MEMORY);
        }

        context->Umat = (double *)calloc(W * H, sizeof(double));
        if (!context->Umat)
        {
                return clear_context(context, ERROR_OUT_OF_MEMORY);
        }

        if (read_filedata(context->mat, context->input_file, W, H))
        {
                return clear_context(context, ERROR_DATA_READING);
        }

        if (fclose(context->input_file))
        {
                drop_warning(WARNING_CANNOT_CLOSE_FILE);
        }
        context->input_file = NULL;

        int solution_not_exists = get_inv_matrix(context->mat, context->Lmat, context->Umat, H, W);

        context->output_file = fopen(argv[2], "w");
        if (!context->output_file)
        {
                return clear_context(context, ERROR_CANNOT_OPEN_FILE);
        }

        if (solution_not_exists)
        {
                if (fprintf(context->output_file, "no_solution\n") < 0)
                {
                        return clear_context(context, ERROR_DATA_WRITING);
                }
        }
        else
        {
                if (write_filedata(context->Lmat, context->output_file, W, H))
                {
                        return clear_context(context, ERROR_DATA_WRITING);
                }
        }

        return clear_context(context, SUCCESS);
}
