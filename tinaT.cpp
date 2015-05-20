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
    box[0].width = 400;
    box[0].height = 70;
    box[0].center.x = 120 + 5*65;
    box[0].center.y = 70;
    number++;

    //first raised platform 
    box[1].width = 100;
    box[1].height = 5;
    box[1].center.x = 120 + 5*65;
    box[1].center.y = 240;
    number++;

    //second raised platform
    box[2].width = 130;
    box[2].height = 5;
    box[2].center.x = 300 + 5*65;
    box[2].center.y = 340;
    number++;
    
    //second raised platform
    box[3].width = 130;
    box[3].height = 5;
    box[3].center.x = 900 + 5*65;
    box[3].center.y = 340;
    number++;
}
///
