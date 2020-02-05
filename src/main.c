#include <stdlib.h>
#include "cgl.h"

int main(void)
{
  cgl_Line *line = calloc(1, sizeof(cgl_Line));
  cgl_plot(line);
}
