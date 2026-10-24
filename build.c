#define CHIMERA_IMPLEMENTATION
#define CHIMERA_STRIP_PREFIX
#include <chimera.h>

void parse_file(StringBuilder sb) {
  bool inCodeSnippet = false;
  bool inUse = false;
  StringBuilder nameBuffer = {0};
  StringBuilder textBuf = {0};
  StringBuilder codeBuf = {0};

#define flush_text_buf()                                                       \
  do {                                                                         \
    if (textBuf.count > 0) {                                                   \
      da_push(&textBuf, '\0');                                                 \
      if (textBuf.items[0] != '\0') {                                          \
        println("printf(\"%%s\", \"%s\");", textBuf.items);                    \
      }                                                                        \
      da_free(textBuf);                                                        \
      textBuf = (StringBuilder){0};                                            \
    }                                                                          \
  } while (0)

#define flush_code_buf()                                                       \
  do {                                                                         \
    if (codeBuf.count > 0) {                                                   \
      da_push(&codeBuf, '\0');                                                 \
      println("%s", codeBuf.items);                                            \
      da_free(codeBuf);                                                        \
      codeBuf = (StringBuilder){0};                                            \
    }                                                                          \
  } while (0)

  for (size_t i = 0; i < sb.count; ++i) {
    char c = sb.items[i];
    if (inCodeSnippet) {
      if (c == '%') {
        if (i + 1 < sb.count && sb.items[i + 1] == '%') {
          da_push(&codeBuf, '%');  // Changed
          i++;
          continue;
        }
        flush_code_buf();  // Changed
        inCodeSnippet = false;
        continue;
      }
      da_push(&codeBuf, c);  // Changed from printf
      continue;
    }
    if (inUse) {
      if (c == '~') {
        inUse = false;
        da_push(&nameBuffer, '\0');
        StringBuilder temp = {0};
        read_file(nameBuffer.items, &temp);
        parse_file(temp);
        da_free(nameBuffer);
        da_free(temp);
        continue;
      }
      da_push(&nameBuffer, c);
      continue;
    }
    switch (c) {
    case '%': {
      flush_text_buf();
      inCodeSnippet = true;
      break;
    }
    case '~': {
      flush_text_buf();
      inUse = true;
      break;
    }
    case '\n': {
      flush_text_buf();
      println("printf(\"\\n\");");
      break;
    }
    case '"': {
      da_push(&textBuf, '\\');
      da_push(&textBuf, '"');
      break;
    }
    case '\\': {
      da_push(&textBuf, '\\');
      da_push(&textBuf, '\\');
      break;
    }
    default:
      da_push(&textBuf, c);
    }
  }
  flush_text_buf();
  da_free(textBuf);
}

int main(int argc, char **argv) {
  rebuild_file(argv, argc);
  StringBuilder sb = {0};
  read_file("./index.html.tt", &sb);
  da_push(&sb, '\0');
  println("#include <stdio.h>");

  println("#define CHIMERA_IMPLEMENTATION");
  println("#define CHIMERA_STRIP_PREFIX");
  println("#include <chimera.h>");
  println("int main() {");
  parse_file(sb);
  println("}");
  return 0;
}
