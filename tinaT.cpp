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
    box[4].center.x = 1300;// + 5*65;
    box[4].center.y = 20;
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
    
    box[7].width = 100;
    box[7].height = 10;
    box[7].center.x = 1300;
    box[7].center.y = 40;
    number++;
    
    box[8].width = 100;
    box[8].height = 10;
    box[8].center.x = 1500;
    box[8].center.y = 250;
    number++;
    
    box[9].width = 100;
    box[9].height = 10;
    box[9].center.x = 1500;
    box[9].center.y = 150;
    number++;
    
    box[10].width = 100;
    box[10].height = 10;
    box[10].center.x = 1500;
    box[10].center.y = 140;
    number++;
    
    box[11].width = 100;
    box[11].height = 10;
    box[11].center.x = 1500;
    box[11].center.y = 130;
    number++;
    
    box[12].width = 100;
    box[12].height = 10;
    box[12].center.x = 1500;
    box[12].center.y = 120;
    number++;
    
    box[13].width = 100;
    box[13].height = 10;
    box[13].center.x = 1500;
    box[13].center.y = 110;
    number++;
    
    box[14].width = 100;
    box[14].height = 10;
    box[14].center.x = 1500;
    box[14].center.y = 100;
    number++;
    
    box[15].width = 100;
    box[15].height = 10;
    box[15].center.x = 1500;
    box[15].center.y = 90;
    number++;

    box[16].width = 100;
    box[16].height = 10;
    box[16].center.x = 1500;
    box[16].center.y = 80;
    number++;
    
    box[17].width = 100;
    box[17].height = 10;
    box[17].center.x = 1500;
    box[17].center.y = 70;
    number++;
    
    box[18].width = 100;
    box[18].height = 10;
    box[18].center.x = 1500;
    box[18].center.y = 60;
    number++;
    
}
///
