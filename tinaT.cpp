/* 
 * Tina Tafoya
 * cs335 project: Uberdude
 * creating platforms for level 1
 * 
 * 
 */
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
    box[0].center.x = 0 ;
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
    box[2].center.y = 70;
    number++;

    //second raised platform
    box[3].width = 100;
    box[3].height = 10;
    box[3].center.x = 700;// + 5*65;
    box[3].center.y = 90;
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
    //start-stairs
    box[8].width = 100;
    box[8].height = 10;
    box[8].center.x = 1500;
    box[8].center.y = 80;
    number++;
    
    box[9].width = 100;
    box[9].height = 10;
    box[9].center.x = 1500;
    box[9].center.y = 60;
    number++;
    
    box[10].width = 100;
    box[10].height = 10;
    box[10].center.x = 1500;
    box[10].center.y = 100;
    number++;
    //end-stairs
    box[11].width = 100;
    box[11].height = 10;
    box[11].center.x = 1800;
    box[11].center.y = 300;
    number++;
    //start-nonsense
    box[12].width = 100;
    box[12].height = 10;
    box[12].center.x = 1790;
    box[12].center.y = 900;
    number++;
    
    box[13].width = 100;
    box[13].height = 10;
    box[13].center.x = 1810;
    box[13].center.y = 920;
    number++;
    
    box[14].width = 100;
    box[14].height = 10;
    box[14].center.x = 1820;
    box[14].center.y = 940;
    number++;
    
    box[15].width = 100;
    box[15].height = 10;
    box[15].center.x = 1830;
    box[15].center.y = 960;
    number++;

    box[16].width = 100;
    box[16].height = 10;
    box[16].center.x = 1840;
    box[16].center.y = 980;
    number++;
    
    box[17].width = 100;
    box[17].height = 10;
    box[17].center.x = 1850;
    box[17].center.y = 900;
    number++;
    
    box[18].width = 100;
    box[18].height = 10;
    box[18].center.x = 1860;
    box[18].center.y = 920;
    number++;
   
    box[19].width = 100;
    box[19].height = 10;
    box[19].center.x = 1870;
    box[19].center.y = 940;
    number++;
  //end-nonsense 
    box[20].width = 100;
    box[20].height = 10;
    box[20].center.x = 1900;
    box[20].center.y = 280;
    number++;
   
    box[21].width = 100;
    box[21].height = 10;
    box[21].center.x = 2000;
    box[21].center.y = 300;
    number++;
   
    box[22].width = 100;
    box[22].height = 10;
    box[22].center.x = 2800;
    box[22].center.y = 200;
    number++;
   
    box[23].width = 100;
    box[23].height = 10;
    box[23].center.x = 2600;
    box[23].center.y = 100;
    number++;
   
    box[24].width = 100;
    box[24].height = 10;
    box[24].center.x = 2600;
    box[24].center.y = 440;
    number++;
   
    box[25].width = 100;
    box[25].height = 10;
    box[25].center.x = 2030;
    box[25].center.y = 150;
    number++;
   
    box[26].width = 100;
    box[26].height = 10;
    box[26].center.x = 2300;
    box[26].center.y = 400;
    number++;
   
}
///
