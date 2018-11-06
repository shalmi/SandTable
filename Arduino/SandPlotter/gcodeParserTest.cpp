/* strtok example */
#include <stdio.h>
#include <string.h>
#include <cstdlib>


int main ()
{
  char str[] =" G01 X255.34 Y372.41";
  char * pch;
  printf ("Splitting string \"%s\" into tokens:\n",str);
  pch = strtok (str,"G XY");
  float inputs[3];
  int index = 0;
  while (pch != NULL)
  {
    printf ("%s\n",pch);
    inputs[index] = std::strtof(pch, NULL);
    index++;
    if (index>3){
        printf("THERE HAS BEEN AN ERROR");
        while(1){
            int x=3;
        }
    }
    pch = strtok (NULL, "G XY");
    
  }
  for( int x =0; x<3;x++){
      printf("%f\n",inputs[x]);
  }
  return 0;
}