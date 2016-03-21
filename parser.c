#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
  Inputs:   char * filename 
  struct matrix * transform, 
  struct matrix * pm,
  screen s
  Returns: 

  Goes through the file named filename and performs all of the actions listed in that file.
  The file follows the following format:
  Every command is a single character that takes up a line
  Any command that requires arguments must have those arguments in the second line.
  The commands are as follows:
  line: add a line to the edge matrix - 
  takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
  circle: add a circle to the edge matrix - 
  takes 3 arguments (cx, cy, r)
  hermite: add a hermite curve to the edge matrix -
  takes 8 arguments (x0, y0, x1, y1, x2, y2, x3, y3)
  bezier: add a bezier curve to the edge matrix -
  takes 8 arguments (x0, y0, x1, y1, x2, y2, x3, y3)
  ident: set the transform matrix to the identity matrix - 
  scale: create a scale matrix, 
  then multiply the transform matrix by the scale matrix - 
  takes 3 arguments (sx, sy, sz)
  translate: create a translation matrix, 
  then multiply the transform matrix by the translation matrix - 
  takes 3 arguments (tx, ty, tz)
  xrotate: create an x-axis rotation matrix,
  then multiply the transform matrix by the rotation matrix -
  takes 1 argument (theta)
  yrotate: create an y-axis rotation matrix,
  then multiply the transform matrix by the rotation matrix -
  takes 1 argument (theta)
  zrotate: create an z-axis rotation matrix,
  then multiply the transform matrix by the rotation matrix -
  takes 1 argument (theta)
  apply: apply the current transformation matrix to the 
  edge matrix
  display: draw the lines of the edge matrix to the screen
  display the screen
  save: draw the lines of the edge matrix to the screen
  save the screen to a file -
  takes 1 argument (file name)
  quit: end parsing

  See the file script for an example of the file format


  IMPORTANT MATH NOTE:
  the trig functions int math.h use radian mesure, but us normal
  humans use degrees, so the file will contain degrees for rotations,
  be sure to conver those degrees to radians (M_PI is the constant
  for PI)
  ====================*/
void parse_file ( char * filename, 
                  struct matrix * transform, 
                  struct matrix * pm,
                  screen s) {

  FILE *f;
  char line[256];
  char* command = (char*)malloc(256);
  //command = " ";
  double args[8];
  
  clear_screen(s);

  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");
  
  while ( (fgets(line, 256, f) != NULL) && (strcmp(command,"quit")!=0)) {
    if( (line[strlen(line)-1]==' ') || (line[strlen(line)-1]=='\n')){
      line[strlen(line)-1]='\0';
    }
    printf(":%s: command:%s:\n",line,command);
    //printf("starting command :%s:\n",command);
    char first = line[0];
    if(isalpha(first)==0){
      //printf("HERE!!!\n");
      /* get the first token */
      char* arg = strtok(line," ");
      args[0] = atof(arg);
      //printf("args: %s",arg);
      int i = 1;
      while(arg!=NULL){
	//printf("AND HERE!!!\n");
	arg = strtok(NULL," ");
	if(arg!=NULL){
	  args[i] = atof(arg);
	  //printf(" %s",arg);
	}
	i++;
      }
      //printf("\n");
      if(strcmp(command,"line")==0){
	add_edge(pm,args[0],args[1],args[2],args[3],args[4],args[5]);
	printf("add_edge(pm,%G,%G,%G,%G,%G,%G)\n",args[0],args[1],args[2],args[3],args[4],args[5]);
      }
      else if(strcmp(command,"circle")==0){
	add_circle(pm,args[0],args[1],args[2],0.001);
	printf("add_circle(pm,%G,%G,%G,0.001)\n",args[0],args[1],args[2]);
      }
      else if(strcmp(command,"hermite")==0){
	add_curve(pm,args[0],args[1],args[2],args[3],args[4],args[5],args[6],args[7],0.001,0);
	printf("add_curve(pm,%G,%G,%G,%G,%G,%G,%G,%G,0.001,0)\n",args[0],args[1],args[2],args[3],args[4],args[5],args[6],args[7]);
      }
      else if(strcmp(command,"bezier")==0){
	add_curve(pm,args[0],args[1],args[2],args[3],args[4],args[5],args[6],args[7],0.001,1);
	printf("add_curve(pm,%G,%G,%G,%G,%G,%G,%G,%G,0.001,1)\n",args[0],args[1],args[2],args[3],args[4],args[5],args[6],args[7]);
      }
      else if(strcmp(command,"ident")==0){
	ident(pm);
	printf("ident(pm)\n");
      }
      else if(strcmp(command,"scale")==0){
	struct matrix *scale = make_scale(args[0],args[1],args[2]);
	matrix_mult(scale,transform);
      }
      else if(strcmp(command,"translate")==0){
	struct matrix *translate = make_translate(args[0],args[1],args[2]);
	matrix_mult(translate,transform);
      }
      else if(strcmp(command,"xrotate")==0){
	struct matrix *rotate = make_rotX(args[0] * (M_PI/180));
	matrix_mult(rotate,transform);
      }
      else if(strcmp(command,"yrotate")==0){
	struct matrix *rotate = make_rotY(args[0] * (M_PI/180));
	matrix_mult(rotate,transform);
      }
      else if(strcmp(command,"zrotate")==0){
	struct matrix *rotate = make_rotZ(args[0] * (M_PI/180));
	matrix_mult(rotate,transform);
      }
      else if(strcmp(command,"apply")==0){
	matrix_mult(transform,pm);
      }
      else if(strcmp(command,"display")==0){
	color c;
	c.red = MAX_COLOR;
	c.blue = 0;
	c.green = 0;
	draw_lines(pm,s,c);
	printf("display\n");
	//display(s);
      }
    }else{
      if(strcmp(command,"save")==0){
	printf("saving...\n");
	save_extension(s,line);
      }
      strcpy(command,line);
    }

    /*
      else if(strcmp(command,"save")==0){
      char* arg = strtok(line," ");
      arg = strtok(NULL," ");
      printf("arg: %s\n",arg);
      save_extension(s,arg);
      }
    */
    //command = line;
    //printf("command: %s\n",command);
  }
}
  
