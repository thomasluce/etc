#include <stdio.h>
#include <stdlib.h>


FILE *input_file;

int parse_double_dot() {
  unsigned char input = getc(input_file);
  if(input == '.') return 1;
  else {
    ungetc(input, input_file);
    return 0;
  }
}

int parse_elipsis() {
  unsigned char input = getc(input_file);
  if(input == '.') return parse_double_dot();
  else {
    ungetc(input, input_file);
    return 0;
  }
}

int main(int argc, char **argv) {
  if(argc > 1) {
    input_file = fopen(argv[1], "r");
  } else {
    input_file = input_file;
  }

  char input;
  unsigned char temp;
  unsigned char memory[65535];
  int  pointer = 0;
  for(pointer =0; pointer < 65535; pointer++) memory[pointer] = 0;
  pointer = 0;

  while((input = getc(input_file)) != EOF) {
    switch(input) {
      case '.':
        if(parse_elipsis() == 1) {
          pointer++;
        } else if(parse_double_dot() == 1) {
          pointer--;
        } else {
          memory[pointer] = getc(stdin);
        }
        break;
      case '\'':
        putc(memory[pointer], stdout);
        break;
      case ' ':
        temp = getc(input_file);
        if(temp == ' ') {
          memory[pointer] += 1;
          break;
        } else {
          ungetc(temp, input_file);
          memory[pointer] -= 1;
        }
        break;
      case '\n':
        if(memory[pointer] == 0) getc(input_file);
        break;
      case '\t':
        if(memory[pointer] > 0) getc(input_file);
        break;
      default:
        fprintf(stderr, "Unknown command: %c(%d)\n", input, input);
        return 1;
    }
  }

  return 0;
}
