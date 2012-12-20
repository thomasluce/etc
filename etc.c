#include <stdio.h>
#include <stdlib.h>

FILE *input_file;

#define INC    0
#define DEC    1
#define IN     2
#define OUT    3
#define INC_B  4
#define DEC_B  5
#define SKIP   6
#define SKIP_G 7
#define GO     8

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

void read_program(unsigned char *instruction_buffer) {
  char input;
  unsigned char temp;
  int ip = 0;
  while((input = getc(input_file)) != EOF) {
    switch(input) {
      case '.':
        if(parse_elipsis() == 1) {
          instruction_buffer[ip] = INC;
        } else if(parse_double_dot() == 1) {
          instruction_buffer[ip] = DEC;
        } else {
          instruction_buffer[ip] = IN;
        }
        break;
      case '\'':
        instruction_buffer[ip] = OUT;
        break;
      case ' ':
        temp = getc(input_file);
        if(temp == ' ') {
          instruction_buffer[ip] = INC_B;
        } else {
          ungetc(temp, input_file);
          instruction_buffer[ip] = DEC_B;
        }
        break;
      case '\n':
        instruction_buffer[ip] = SKIP;
        break;
      case '\t':
        instruction_buffer[ip] = SKIP_G;
        break;
      case '/':
        instruction_buffer[ip] = GO;
        break;
      default:
        fprintf(stderr, "Unknown command: %c(%d)\n", input, input);
        exit(1);
    }

    ip++;
  }
}

int main(int argc, char **argv) {
  if(argc > 1) {
    input_file = fopen(argv[1], "r");
  } else {
    input_file = input_file;
  }

  unsigned char memory[65535];
  unsigned char instructions[65535];

  int pointer = 0;
  int ip = 0;
  int instruction = 0;
  for(pointer =0; pointer < 65535; pointer++) memory[pointer] = 0;
  for(pointer =0; pointer < 65535; pointer++) instructions[pointer] = 99;
  read_program(instructions);
  pointer = 0;

  while((instruction = instructions[ip]) != 99) {
    switch(instruction) {
      case INC:
        pointer++;
        break;
      case DEC:
        pointer--;
        break;
      case IN:
        memory[pointer] = getc(stdin);
        break;
      case OUT:
        putc(memory[pointer], stdout);
        break;
      case INC_B:
        memory[pointer] += 1;
        break;
      case DEC_B:
        memory[pointer] -= 1;
        break;
      case SKIP:
        if(memory[pointer] == 0) ip++;
        break;
      case SKIP_G:
        if(memory[pointer] > 0) ip++;
        break;
      case GO:
        ip = memory[pointer]-1;
        break;
      default:
        fprintf(stderr, "Should not be here\n");
        return 1;
    }
    ip++;
  }

  return 0;
}
