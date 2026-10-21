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
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum { PREPARE_SUCESS, PREPARE_UNRECOGNIZED_STATEMENT } PrepareResult;

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
  StatementType type;
} Statement;

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

MetaCommandResult do_meta_command(InputBuffer *input_buffer) {
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    exit(EXIT_SUCCESS);
  } else {
    return META_COMMAND_UNRECOGNIZED_COMMAND;
  }
}

PrepareResult prepare_statement(InputBuffer *input_buffer,
                                Statement *statement) {
  if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
    statement->type = STATEMENT_INSERT;
    return PREPARE_SUCESS;
  }

  if (strcmp(input_buffer->buffer, "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCESS;
  }

  return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement *statement) {
  switch (statement->type) {
  case (STATEMENT_INSERT):
    puts("This is where we'd do INSERT");
    break;
  case (STATEMENT_SELECT):
    puts("This is were we'd do SELECT");
    break;
  }
}

int main(int argc, char **argv) {
  InputBuffer *input_buffer = new_input_buffer();
  FOREVER {
    print_prompt();

    read_input(input_buffer);

    if (input_buffer->buffer[0] == '.') {
      switch (do_meta_command(input_buffer)) {
      case (META_COMMAND_SUCCESS):
        continue;
      case (META_COMMAND_UNRECOGNIZED_COMMAND):
        printf("Unrecognized command '.%s'\n", input_buffer->buffer);
        continue;
      }
    }

    Statement statement;
    switch (prepare_statement(input_buffer, &statement)) {
    case (PREPARE_SUCESS):
      break;
    case (PREPARE_UNRECOGNIZED_STATEMENT):
      printf("Unrecognized word at start of '.%s'\n", input_buffer->buffer);
      continue;
    }
    execute_statement(&statement);
    printf("Executed.\n");
  }
}
