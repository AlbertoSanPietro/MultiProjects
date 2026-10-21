#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FOREVER for (;;)

typedef struct {
  char *buffer;
  size_t buf_len;
  ssize_t input_len;
} InputBuffer;

typedef enum {
  META_COMMAND_ACCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum { PREPARE_SUCESSS, PREPARE_UNRECOGNIZED_STATEMENT } PrepareResult;

InputBuffer *new_input_buffer() {
  InputBuffer *input_buffer = malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->buf_len = 0;
  input_buffer->input_len = 0;

  return input_buffer;
}

void print_prompt() { printf("db > "); }

void read_input(InputBuffer *input_buffer) {
  ssize_t bytes_read =
      getline(&(input_buffer->buffer), &(input_buffer->buf_len), stdin);

  if (bytes_read <= 0) {
    fprintf(stderr, "Error reading input\n");
    exit(EXIT_FAILURE);
  }

  // remove newline
  input_buffer->input_len = bytes_read - 1;
  input_buffer->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(InputBuffer *input_buffer) {
  free(input_buffer->buffer);
  free(input_buffer);
}

int main(int argc, char **argv) {
  InputBuffer *input_buffer = new_input_buffer();
  FOREVER {
    print_prompt();

    read_input(input_buffer);

    if (strcmp(input_buffer->buffer, ".exit") == 0) {
      close_input_buffer(input_buffer);
      exit(EXIT_SUCCESS);
    } else {
      printf("Unrecognized command '%s' .\n", input_buffer->buffer);
    }
  }
}
