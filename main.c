#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"

int main( int argc, char **argv ) {

  screen s;
  struct matrix *edges;
  struct matrix *transform;

  edges = new_matrix(4, 4);
  transform = new_matrix(4, 4);
  clear_screen(s);
  color c;
  c.red = 0;
  c.green = 0;
  c.blue = 255;
  //add_circle(edges,200,200,100,0.001);
  add_edge(edges,50,50,0,200,50,0);
  add_edge(edges,50,50,0,100,100,0);
  add_edge(edges,100,100,0,150,100,0);
  add_edge(edges,150,100,0,200,50,0);
  add_curve(edges,50,50,100,100,150,100,200,50,0.01,0);
  draw_lines(edges,s,c);
  display(s);

  save_ppm(s,"curves.ppm");
  
  /*
  if ( argc == 2 )
    parse_file( argv[1], transform, edges, s );
  else
    parse_file( "stdin", transform, edges, s );
  */
  
  free_matrix( transform );
  free_matrix( edges );
}  
