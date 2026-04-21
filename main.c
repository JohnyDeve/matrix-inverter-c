#include <stdio.h>
#include <stdlib.h>

#define SUCCESS 0
#define ERROR_CANNOT_OPEN_FILE 1
#define ERROR_DATA_READING 2
#define ERROR_OUT_OF_MEMORY 3
#define ERROR_INCORRECT_COMMAND_ARGS 4
#define WARNING_CANNOT_CLOSE_FILE 5
#define ERROR_DATA_WRITING 6

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
        fprintf(stderr, "Warning: %s\n", status_message_table[warning]);
}

static int clear_context(Context *context, int error_code)
{
        //TODO
        return 0;
}

static double *getloc(double *matrix, size_t i, size_t j, size_t W)
{
        //TODO
        return matrix;
}

static int get_inv_matrix(double *A, double *L, double *U, size_t H, size_t W)
{
        //TODO
        return 0;
}

int main(int argc, char *argv[])
{
        if (argc != 3)
        {
                fprintf(stderr, "Error: %s\n", status_message_table[ERROR_INCORRECT_COMMAND_ARGS]);
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

        for (size_t i = 0; i < H; i++)
        {
                for (size_t j = 0; j < W; j++)
                {
                        if (fscanf(context->input_file, "%lf", getloc(context->mat, i, j, W)) != 1)
                        {
                                return clear_context(context, ERROR_DATA_READING);
                        }
                }
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
                if (fprintf(context->output_file, "%zu %zu\n", W, H) < 0)
                {
                        return clear_context(context, ERROR_DATA_WRITING);
                }

                for (size_t i = 0; i < W; i++)
                {
                        for (size_t j = 0; j < W; j++)
                        {
                                if (fprintf(context->output_file, "%g ", *getloc(context->Lmat, i, j, W)) < 0)
                                {
                                        return clear_context(context, ERROR_DATA_WRITING);
                                }
                        }
                        if (fprintf(context->output_file, "\n") < 0)
                        {
                                return clear_context(context, ERROR_DATA_WRITING);
                        }
                }
        }

        return clear_context(context, SUCCESS);
}
