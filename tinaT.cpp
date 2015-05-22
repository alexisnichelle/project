#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include "ppm.h"
extern "C" {
#include "fonts.h"
}
#include "mystruct.h"

void buildlevel(Shape *box, int &number)
{
    //manually assign individual boxes
    box[0].width = 100;
    box[0].height = 10;
    box[0].center.x = 100 ;
    box[0].center.y = 10;
    number++;
    box[1].width = 100;
    box[1].height = 10;
    box[1].center.x = 200 ;
    box[1].center.y = 10;
    number++;

    //first raised platform 
    box[2].width = 100;
    box[2].height = 10;
    box[2].center.x = 500;// + 5*65;
    box[2].center.y = 40;
    number++;

    //second raised platform
    box[3].width = 100;
    box[3].height = 10;
    box[3].center.x = 700;// + 5*65;
    box[3].center.y = 60;
    number++;
    
    box[4].width = 100;
    box[4].height = 10;
    box[4].center.x = 950;// + 5*65;
    box[4].center.y = 10;
    number++;
    
    box[5].width = 100;
    box[5].height = 10;
    box[5].center.x = 550;
    box[5].center.y = 280;
    number++;
    
    //top box
    box[6].width = 100;
    box[6].height = 10;
    box[6].center.x = 900;
    box[6].center.y = 400;
    number++;
    
}
///
