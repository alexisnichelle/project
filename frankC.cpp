/* Frank Clemente
 * CS335 Project
 * Group #4
 */

using namespace std;
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

#define USE_SOUND
#ifdef USE_SOUND
#include <FMOD/fmod.h>
#include <FMOD/wincompat.h>
#include "fmod.h"
#endif

bool songPlaying = false;

void music(int trackNo)
{
#ifdef USE_SOUND
    if (fmod_init()) {
	printf("Error - music\n\n");
	return;
    }


    switch(trackNo) {
	case 0:
	    if (!songPlaying) {
		if (fmod_createsound((const char *)"./sounds/TitleScreen.mp3", 0)) {
		    printf("Error - TitleScreen.mp3\n\n");
		    return;
		}
		songPlaying = true;
	    } else {
		fmod_stopsound();
		if (fmod_createsound((const char *)"./sounds/TitleScreen.mp3", 0)) {
		    printf("Error - TitleScreen.mp3\n\n");
		    return;
		}
	    }
	    break;
	case 1:
	    if (!songPlaying) {
		if (fmod_createsound((const char *)"./sounds/HeatMan.mp3", 0)) {
		    printf("Error - HeatMan music\n\n");
		    return;
		}
	    } else {
		fmod_stopsound();
		if (fmod_createsound((const char *)"./sounds/HeatMan.mp3", 0)) {
		    printf("Error - HeatMan music\n\n");
		    songPlaying = true;
		    return;
		}
	    }
	    break;
	case 2:
	    if (!songPlaying) {
		if (fmod_createsound((const char *)"./sounds/AirMan.mp3", 0)) {
		    printf("Error - AirMan music\n\n");
		    return;
		}
	    } else {
		fmod_stopsound();
		if (fmod_createsound((const char *)"./sounds/AirMan.mp3", 0)) {
		    printf("Error - AirMan music\n\n");
		    songPlaying = true;
		    return;
		}
	    }
	    break;
	case 3:
	    if (!songPlaying) {
		if (fmod_createsound((const char *)"./sounds/MetalMan.mp3", 0)) {
		    printf("Error - MetalMan music\n\n");
		    return;
		}
	    } else {
		fmod_stopsound();
		if (fmod_createsound((const char *)"./sounds/MetalMan.mp3", 0)) {
		    printf("Error - MetalMan music\n\n");
		    songPlaying = true;
		    return;
		}
	    }
	    break;
	case 4:
	    if (!songPlaying) {
		if (fmod_createsound((const char *)"./sounds/Boss.mp3", 0)) {
		    printf("Error - Boss music\n\n");
		    return;
		}
	    } else {
		fmod_stopsound();
		if (fmod_createsound((const char *)"./sounds/Boss.mp3", 0)) {
		    printf("Error - Boss music\n\n");
		    songPlaying = true;
		    return;
		}
	    }
	    break;
	case 5:
	    if (!songPlaying) {
		if (fmod_createsound((const char *)"./sounds/BossDefeated.mp3", 0)) {
		    printf("Error - BossDefeated music\n\n");
		    return;
		}
	    } else {
		fmod_stopsound();
		if (fmod_createsound((const char *)"./sounds/BossDefeated.mp3", 0)) {
		    printf("Error - BossDefeated music\n\n");
		    songPlaying = true;
		    return;
		}
	    }
	    break;
	case 6:
	    if (!songPlaying) {
		if (fmod_createsound((const char *)"./sounds/GameOver.mp3", 0)) {
		    printf("Error - GameOver music\n\n");
		    return;
		}
	    } else {
		fmod_stopsound();
		if (fmod_createsound((const char *)"./sounds/GameOver.mp3", 0)) {
		    printf("Error - GameOver music\n\n");
		    return;
		}
	    }
	    break;
    }

    if (trackNo != 6) {
	fmod_setmode(0, FMOD_LOOP_NORMAL);
    }
#endif
}


int moreLives(XEvent *e, bool &up, bool &down, bool &left, bool &right, bool &b, bool &a) 
{
    //  for debugging purposes
    //	cout << XLookupKeysym(&e->xkey, 0) << endl;

    if(XLookupKeysym(&e->xkey, 0) == 65362)
	    up = true;
    
    if(up)
	if(XLookupKeysym(&e->xkey, 0) == 65364)
	    down = true;

    if(down)
	if(XLookupKeysym(&e->xkey, 0) == 65361)
	    left = true;

    if(left)
	if(XLookupKeysym(&e->xkey, 0) == 65363)
	    right = true;

    if(right)
	if(XLookupKeysym(&e->xkey, 0) == 98)
	    b = true;

    if(b)
	if(XLookupKeysym(&e->xkey, 0) == 97)
	    a = true;

    if(a)
	return 30;

    return 3;
}
void level(Shape *platform, int &num) 
{

	platform[27].width = 100;
	platform[27].height = 10;
        platform[27].center.x = 3000;
	platform[27].center.y = 200;
	num++;

	platform[28].width = 100;
	platform[28].height = 10;
        platform[28].center.x = 3200;
	platform[28].center.y = 200;
	num++;
	
	platform[29].width = 100;
	platform[29].height = 10;
        platform[29].center.x = 3200;
	platform[29].center.y = 180;
	num++;


	platform[30].width = 100;
	platform[30].height = 10;
        platform[30].center.x = 3200;
	platform[30].center.y = 160;
	num++;


	platform[31].width = 100;
	platform[31].height = 10;
        platform[31].center.x = 3200;
	platform[31].center.y = 140;
	num++;

	platform[32].width = 100;
	platform[32].height = 10;
        platform[32].center.x = 3200;
	platform[32].center.y = 120;
	num++;


	platform[33].width = 100;
	platform[33].height = 10;
        platform[33].center.x = 3200;
	platform[33].center.y = 100;
	num++;

	platform[34].width = 100;
	platform[34].height = 10;
        platform[34].center.x = 3200;
	platform[34].center.y = 80;
	num++;

	platform[35].width = 100;
	platform[35].height = 10;
        platform[35].center.x = 3200;
	platform[35].center.y = 60;
	num++;

	platform[36].width = 100;
	platform[36].height = 10;
        platform[36].center.x = 3200;
	platform[36].center.y = 40;
	num++;

	platform[37].width = 100;
	platform[37].height = 10;
        platform[37].center.x = 3200;
	platform[37].center.y = 20;
	num++;
	

	platform[38].width = 100;
	platform[38].height = 10;
        platform[38].center.x = 3200;
	platform[38].center.y = 300;
	num++;

	platform[39].width = 100;
	platform[39].height = 10;
        platform[39].center.x = 3200;
	platform[39].center.y = 320;
	num++;

	platform[40].width = 100;
	platform[40].height = 10;
        platform[40].center.x = 3200;
	platform[40].center.y = 340;
	num++;

	platform[41].width = 100;
	platform[41].height = 10;
        platform[41].center.x = 3200;
	platform[41].center.y = 360;
	num++;
	platform[42].width = 100;
	platform[42].height = 10;
        platform[42].center.x = 3200;
	platform[42].center.y = 380;
	num++;
	platform[43].width = 100;
	platform[43].height = 10;
        platform[43].center.x = 3200;
	platform[43].center.y = 400;
	num++;
	platform[44].width = 100;
	platform[44].height = 10;
        platform[44].center.x = 3200;
	platform[44].center.y = 420;
	num++;
	platform[45].width = 100;
	platform[45].height = 10;
        platform[45].center.x = 3200;
	platform[45].center.y = 440;
	num++;
	platform[46].width = 100;
	platform[46].height = 10;
        platform[46].center.x = 3200;
	platform[46].center.y = 460;
	num++;
	platform[47].width = 100;
	platform[47].height = 10;
        platform[47].center.x = 3200;
	platform[47].center.y = 480;
	num++;

	platform[48].width = 100;
	platform[48].height = 10;
        platform[48].center.x = 3200;
	platform[48].center.y = 500;
	num++;

	platform[49].width = 100;
	platform[49].height = 10;
        platform[49].center.x = 3200;
	platform[49].center.y = 520;
	num++;

	platform[50].width = 100;
	platform[50].height = 10;
        platform[50].center.x = 3200;
	platform[50].center.y = 540;
	num++;

	platform[51].width = 100;
	platform[51].height = 10;
        platform[51].center.x = 3200;
	platform[51].center.y = 560;
	num++;

	platform[52].width = 100;
	platform[52].height = 10;
        platform[52].center.x = 3200;
	platform[52].center.y = 580;
	num++;

	platform[53].width = 100;
	platform[53].height = 10;
        platform[53].center.x = 3400;
	platform[53].center.y = 0;
	num++;

	platform[54].width = 100;
	platform[54].height = 10;
        platform[54].center.x = 3400;
	platform[54].center.y = 20;
	num++;

	platform[55].width = 100;
	platform[55].height = 10;
        platform[55].center.x = 3400;
	platform[55].center.y = 40;
	num++;

	platform[56].width = 100;
	platform[56].height = 10;
        platform[56].center.x = 3400;
	platform[56].center.y = 60;
	num++;

	platform[57].width = 100;
	platform[57].height = 10;
        platform[57].center.x = 3400;
	platform[57].center.y = 80;
	num++;

	platform[58].width = 100;
	platform[58].height = 10;
        platform[58].center.x = 3400;
	platform[58].center.y = 100;
	num++;

	platform[59].width = 100;
	platform[59].height = 10;
        platform[59].center.x = 3400;
	platform[59].center.y = 500;
	num++;

	platform[60].width = 100;
	platform[60].height = 10;
        platform[60].center.x = 3400;
	platform[60].center.y = 520;
	num++;

	platform[61].width = 100;
	platform[61].height = 10;
        platform[61].center.x = 3200;
	platform[61].center.y = 0;
	num++;

	platform[62].width = 100;
	platform[62].height = 10;
        platform[62].center.x = 3400;
	platform[62].center.y = 540;
	num++;

	platform[63].width = 100;
	platform[63].height = 10;
        platform[63].center.x = 3400;
	platform[63].center.y = 560;
	num++;

	platform[64].width = 100;
	platform[64].height = 10;
        platform[64].center.x = 3400;
	platform[64].center.y = 580;
	num++;

	platform[65].width = 100;
	platform[65].height = 10;
        platform[65].center.x = 3400;
	platform[65].center.y = 600;
	num++;

	platform[66].width = 100;
	platform[66].height = 10;
        platform[66].center.x = 3200;
	platform[66].center.y = 600;
	num++;

	platform[67].width = 100;
	platform[67].height = 10;
        platform[67].center.x = 3400;
	platform[67].center.y = 0;
	num++;

	platform[68].width = 100;
	platform[68].height = 10;
        platform[68].center.x = 3600;
	platform[68].center.y = 20;
	num++;

	platform[69].width = 100;
	platform[69].height = 10;
        platform[69].center.x = 3600;
	platform[69].center.y = 40;
	num++;

	platform[70].width = 100;
	platform[70].height = 10;
        platform[70].center.x = 3600;
	platform[70].center.y = 60;
	num++;

	platform[71].width = 100;
	platform[71].height = 10;
        platform[71].center.x = 3600;
	platform[71].center.y = 80;
	num++;

	platform[72].width = 100;
	platform[72].height = 10;
        platform[72].center.x = 3600;
	platform[72].center.y = 100;
	num++;

	platform[73].width = 100;
	platform[73].height = 10;
        platform[73].center.x = 3600;
	platform[73].center.y = 500;
	num++;

	platform[74].width = 100;
	platform[74].height = 10;
        platform[74].center.x = 3600;
	platform[74].center.y = 520;
	num++;

	platform[75].width = 100;
	platform[75].height = 10;
        platform[75].center.x = 3600;
	platform[75].center.y = 540;
	num++;

	platform[76].width = 100;
	platform[76].height = 10;
        platform[76].center.x = 3600;
	platform[76].center.y = 560;
	num++;

	platform[77].width = 100;
	platform[77].height = 10;
        platform[77].center.x = 3600;
	platform[77].center.y = 580;
	num++;

	platform[78].width = 100;
	platform[78].height = 10;
        platform[78].center.x = 3600;
	platform[78].center.y = 600;
	num++;

	platform[79].width = 100;
	platform[79].height = 10;
        platform[79].center.x = 3600;
	platform[79].center.y = 0;
	num++;

	platform[80].width = 100;
	platform[80].height = 10;
        platform[80].center.x = 3800;
	platform[80].center.y = 0;
	num++;

	platform[81].width = 100;
	platform[81].height = 10;
        platform[81].center.x = 3800;
	platform[81].center.y = 20;
	num++;

	platform[82].width = 100;
	platform[82].height = 10;
        platform[82].center.x = 3800;
	platform[82].center.y = 40;
	num++;

	platform[83].width = 100;
	platform[83].height = 10;
        platform[83].center.x = 3800;
	platform[83].center.y = 60;
	num++;

	platform[84].width = 100;
	platform[84].height = 10;
        platform[84].center.x = 3800;
	platform[84].center.y = 80;
	num++;

	platform[85].width = 100;
	platform[85].height = 10;
        platform[85].center.x = 3800;
	platform[85].center.y = 100;
	num++;

	platform[86].width = 100;
	platform[86].height = 10;
        platform[86].center.x = 3800;
	platform[86].center.y = 500;
	num++;

	platform[87].width = 100;
	platform[87].height = 10;
        platform[87].center.x = 3800;
	platform[87].center.y = 520;
	num++;

	platform[88].width = 100;
	platform[88].height = 10;
        platform[88].center.x = 3800;
	platform[88].center.y = 540;
	num++;

	platform[89].width = 100;
	platform[89].height = 10;
        platform[89].center.x = 3800;
	platform[89].center.y = 560;
	num++;

	platform[90].width = 100;
	platform[90].height = 10;
        platform[90].center.x = 3800;
	platform[90].center.y = 580;
	num++;

	platform[91].width = 100;
	platform[91].height = 10;
        platform[91].center.x = 3800;
	platform[91].center.y = 600;
	num++;

	platform[92].width = 100;
	platform[92].height = 10;
        platform[92].center.x = 4000;
	platform[92].center.y = 0;
	num++;

	platform[93].width = 100;
	platform[93].height = 10;
        platform[93].center.x = 4000;
	platform[93].center.y = 20;
	num++;

	platform[94].width = 100;
	platform[94].height = 10;
        platform[94].center.x = 4000;
	platform[94].center.y = 40;
	num++;

	platform[95].width = 100;
	platform[95].height = 10;
        platform[95].center.x = 4000;
	platform[95].center.y = 60;
	num++;

	platform[96].width = 100;
	platform[96].height = 10;
        platform[96].center.x = 4000;
	platform[96].center.y = 80;
	num++;

	platform[97].width = 100;
	platform[97].height = 10;
        platform[97].center.x = 4000;
	platform[97].center.y = 100;
	num++;

	platform[98].width = 100;
	platform[98].height = 10;
        platform[98].center.x = 4000;
	platform[98].center.y = 500;
	num++;

	platform[99].width = 100;
	platform[99].height = 10;
        platform[99].center.x = 4000;
	platform[99].center.y = 520;
	num++;

	platform[100].width = 100;
	platform[100].height = 10;
        platform[100].center.x = 4000;
	platform[100].center.y = 540;
	num++;

	platform[101].width = 100;
	platform[101].height = 10;
        platform[101].center.x = 4000;
	platform[101].center.y = 560;
	num++;

	platform[102].width = 100;
	platform[102].height = 10;
        platform[102].center.x = 4000;
	platform[102].center.y = 580;
	num++;

	platform[103].width = 100;
	platform[103].height = 10;
        platform[103].center.x = 4000;
	platform[103].center.y = 600;
	num++;

	platform[104].width = 100;
	platform[104].height = 10;
	platform[104].center.x = 4200;
	platform[104].center.y = 0;
	num++;

	platform[105].width = 100;
	platform[105].height = 10;
        platform[105].center.x = 4200;
	platform[105].center.y = 0;
	num++;

	platform[106].width = 100;
	platform[106].height = 10;
        platform[106].center.x = 4200;
	platform[106].center.y = 20;
	num++;

	platform[107].width = 100;
	platform[107].height = 10;
        platform[107].center.x = 4200;
	platform[107].center.y = 40;
	num++;

	platform[108].width = 100;
	platform[108].height = 10;
        platform[108].center.x = 4200;
	platform[108].center.y = 60;
	num++;

	platform[109].width = 100;
	platform[109].height = 10;
        platform[109].center.x = 4200;
	platform[109].center.y = 80;
	num++;

	platform[110].width = 100;
	platform[110].height = 10;
        platform[110].center.x = 4200;
	platform[110].center.y = 100;
	num++;

	platform[111].width = 100;
	platform[111].height = 10;
        platform[111].center.x = 4200;
	platform[111].center.y = 120;
	num++;

	platform[112].width = 100;
	platform[112].height = 10;
        platform[112].center.x = 4200;
	platform[112].center.y = 140;
	num++;

	platform[113].width = 100;
	platform[113].height = 10;
        platform[113].center.x = 4200;
	platform[113].center.y = 160;
	num++;

	platform[114].width = 100;
	platform[114].height = 10;
        platform[114].center.x = 4200;
	platform[114].center.y = 180;
	num++;

	platform[115].width = 100;
	platform[115].height = 10;
        platform[115].center.x = 4200;
	platform[115].center.y = 200;
	num++;

	platform[116].width = 100;
	platform[116].height = 10;
        platform[116].center.x = 4200;
	platform[116].center.y = 220;
	num++;

	platform[117].width = 100;
	platform[117].height = 10;
	platform[117].center.x = 4200;
	platform[117].center.y = 240;
	num++;

	platform[118].width = 100;
	platform[118].height = 10;
        platform[118].center.x = 4200;
	platform[118].center.y = 260;
	num++;

	platform[119].width = 100;
	platform[119].height = 10;
        platform[119].center.x = 4200;
	platform[119].center.y = 280;
	num++;

	platform[120].width = 100;
	platform[120].height = 10;
        platform[120].center.x = 4200;
	platform[120].center.y = 300;
	num++;

	platform[121].width = 100;
	platform[121].height = 10;
        platform[121].center.x = 4200;
	platform[121].center.y = 320;
	num++;

	platform[122].width = 100;
	platform[122].height = 10;
        platform[122].center.x = 4200;
	platform[122].center.y = 340;
	num++;

	platform[123].width = 100;
	platform[123].height = 10;
        platform[123].center.x = 4200;
	platform[123].center.y = 360;
	num++;

	platform[124].width = 100;
	platform[124].height = 10;
        platform[124].center.x = 4200;
	platform[124].center.y = 380;
	num++;

	platform[125].width = 100;
	platform[125].height = 10;
        platform[125].center.x = 4200;
	platform[125].center.y = 400;
	num++;

	platform[126].width = 100;
	platform[126].height = 10;
        platform[126].center.x = 4200;
	platform[126].center.y = 420;
	num++;

	platform[127].width = 100;
	platform[127].height = 10;
        platform[127].center.x = 4200;
	platform[127].center.y = 440;
	num++;

	platform[128].width = 100;
	platform[128].height = 10;
        platform[128].center.x = 4200;
	platform[128].center.y = 460;
	num++;

	platform[129].width = 100;
	platform[129].height = 10;
        platform[129].center.x = 4200;
	platform[129].center.y = 480;
	num++;

	platform[130].width = 100;
	platform[130].height = 10;
        platform[130].center.x = 4200;
	platform[130].center.y = 500;
	num++;

	platform[131].width = 100;
	platform[131].height = 10;
        platform[131].center.x = 4200;
	platform[131].center.y = 520;
	num++;

	platform[132].width = 100;
	platform[132].height = 10;
        platform[132].center.x = 4200;
	platform[132].center.y = 540;
	num++;

	platform[133].width = 100;
	platform[133].height = 10;
        platform[133].center.x = 4200;
	platform[133].center.y = 560;
	num++;

	platform[134].width = 100;
	platform[134].height = 10;
        platform[134].center.x = 4200;
	platform[134].center.y = 580;
	num++;

	platform[135].width = 100;
	platform[135].height = 10;
        platform[135].center.x = 4200;
	platform[135].center.y = 600;
	num++;
}

void level2Cont(Shape *platform, int &num) 
{

	platform[170].width = 25;
	platform[170].height = 10;
        platform[170].center.x = 8600;
	platform[170].center.y = 500;
	num++;

	platform[171].width = 30;
	platform[171].height = 10;
        platform[171].center.x = 8800;
	platform[171].center.y = 520;
	num++;

	platform[172].width = 25;
	platform[172].height = 10;
        platform[172].center.x = 9000;
	platform[172].center.y = 540;
	num++;

	platform[173].width = 100;
	platform[173].height = 20;
        platform[173].center.x = 8550;
	platform[173].center.y = 300;
	num++;

	platform[174].width = 30;
	platform[174].height = 10;
        platform[174].center.x = 9150;
	platform[174].center.y = 540;
	num++;

	platform[175].width = 25;
	platform[175].height = 10;
        platform[175].center.x = 9250;
	platform[175].center.y = 400;
	num++;
	
	platform[176].width = 30;
	platform[176].height = 10;
        platform[176].center.x = 9400;
	platform[176].center.y = 500;
	num++;

	platform[177].width = 100;
	platform[177].height = 10;
        platform[177].center.x = 9000;
	platform[177].center.y = 50;
	num++;

	platform[178].width = 100;
	platform[178].height = 10;
        platform[178].center.x = 9200;
	platform[178].center.y = 50;
	num++;

	platform[179].width = 100;
	platform[179].height = 10;
        platform[179].center.x = 9400;
	platform[179].center.y = 50;
	num++;

	platform[180].width = 30;
	platform[180].height = 10;
        platform[180].center.x = 9500;
	platform[180].center.y = 350;
	num++;

	platform[181].width = 40;
	platform[181].height = 10;
        platform[181].center.x = 9600;
	platform[181].center.y = 450;
	num++;

	platform[182].width = 50;
	platform[182].height = 10;
	platform[182].center.x = 8750;
	platform[182].center.y = 200;
	num++;

	platform[183].width = 100;
	platform[183].height = 20;
        platform[183].center.x = 8550;
	platform[183].center.y = 300;
	num++;

	platform[184].width = 30;
	platform[184].height = 10;
        platform[184].center.x = 9150;
	platform[184].center.y = 540;
	num++;

	platform[185].width = 25;
	platform[185].height = 10;
        platform[185].center.x = 9250;
	platform[185].center.y = 400;
	num++;
	
	platform[186].width = 30;
	platform[186].height = 10;
        platform[186].center.x = 9400;
	platform[186].center.y = 500;
	num++;

	platform[187].width = 100;
	platform[187].height = 10;
        platform[187].center.x = 9000;
	platform[187].center.y = 50;
	num++;

	platform[188].width = 100;
	platform[188].height = 10;
        platform[188].center.x = 9200;
	platform[188].center.y = 50;
	num++;

	platform[189].width = 100;
	platform[189].height = 10;
        platform[189].center.x = 9400;
	platform[189].center.y = 50;
	num++;

	platform[190].width = 30;
	platform[190].height = 10;
        platform[190].center.x = 9700;
	platform[190].center.y = 350;
	num++;

	platform[191].width = 40;
	platform[191].height = 10;
        platform[191].center.x = 9950;
	platform[191].center.y = 350;
	num++;

	platform[192].width = 100;
	platform[192].height = 50;
	platform[192].center.x = 10325;
	platform[192].center.y = 200;
	num++;

	platform[193].width = 100;
	platform[193].height = 50;
        platform[193].center.x = 10325;
	platform[193].center.y = 100;
	num++;

	platform[194].width = 100;
	platform[194].height = 50;
        platform[194].center.x = 10325;
	platform[194].center.y = 0;
	num++;

	platform[195].width = 100;
	platform[195].height = 10;
        platform[195].center.x = 9600;
	platform[195].center.y = 50;
	num++;
	
	platform[196].width = 100;
	platform[196].height = 10;
        platform[196].center.x = 9800;
	platform[196].center.y = 50;
	num++;

	platform[197].width = 100;
	platform[197].height = 50;
        platform[197].center.x = 10425;
	platform[197].center.y = 0;
	num++;

	platform[198].width = 100;
	platform[198].height = 50;
        platform[198].center.x = 10425;
	platform[198].center.y = 100;
	num++;

	platform[199].width = 100;
	platform[199].height = 50;
        platform[199].center.x = 10425;
	platform[199].center.y = 200;
	num++;

	platform[200].width = 100;
	platform[200].height = 50;
	platform[200].center.x = 10625;
	platform[200].center.y = 0;
	num++;

	platform[201].width = 100;
	platform[201].height = 50;
	platform[201].center.x = 10625;
	platform[201].center.y = 100;
	num++;

	platform[202].width = 100;
	platform[202].height = 50;
	platform[202].center.x = 10625;
	platform[202].center.y = 200;
	num++;

	platform[203].width = 100;
	platform[203].height = 50;
	platform[203].center.x = 10625;
	platform[203].center.y = 300;
	num++;

	platform[204].width = 100;
	platform[204].height = 50;
	platform[204].center.x = 10625;
	platform[204].center.y = 400;
	num++;

	platform[205].width = 100;
	platform[205].height = 50;
	platform[205].center.x = 10825;
	platform[205].center.y = 0;
	num++;

	platform[206].width = 100;
	platform[206].height = 50;
	platform[206].center.x = 10825;
	platform[206].center.y = 100;
	num++;

	platform[207].width = 100;
	platform[207].height = 50;
	platform[207].center.x = 10825;
	platform[207].center.y = 200;
	num++;

	platform[208].width = 100;
	platform[208].height = 50;
	platform[208].center.x = 10825;
	platform[208].center.y = 300;
	num++;

	platform[209].width = 100;
	platform[209].height = 50;
	platform[209].center.x = 10825;
	platform[209].center.y = 400;
	num++;

	platform[210].width = 100;
	platform[210].height = 50;
	platform[210].center.x = 11025;
	platform[210].center.y = 0;
	num++;

	platform[211].width = 100;
	platform[211].height = 50;
	platform[211].center.x = 11025;
	platform[211].center.y = 100;
	num++;

	platform[212].width = 100;
	platform[212].height = 50;
	platform[212].center.x = 11025;
	platform[212].center.y = 200;
	num++;

	platform[213].width = 100;
	platform[213].height = 50;
	platform[213].center.x = 11025;
	platform[213].center.y = 300;
	num++;

	platform[214].width = 100;
	platform[214].height = 50;
	platform[214].center.x = 11025;
	platform[214].center.y = 400;
	num++;

	platform[215].width = 100;
	platform[215].height = 50;
	platform[215].center.x = 11225;
	platform[215].center.y = 0;
	num++;

	platform[216].width = 100;
	platform[216].height = 50;
	platform[216].center.x = 11425;
	platform[216].center.y = 0;
	num++;

	platform[217].width = 100;
	platform[217].height = 50;
	platform[217].center.x = 11625;
	platform[217].center.y = 0;
	num++;

	platform[218].width = 100;
	platform[218].height = 50;
	platform[218].center.x = 11825;
	platform[218].center.y = 0;
	num++;

	platform[219].width = 100;
	platform[219].height = 50;
	platform[219].center.x = 11825;
	platform[219].center.y = 100;
	num++;

	platform[220].width = 100;
	platform[220].height = 50;
	platform[220].center.x = 11825;
	platform[220].center.y = 200;
	num++;

	platform[221].width = 100;
	platform[221].height = 50;
	platform[221].center.x = 11825;
	platform[221].center.y = 300;
	num++;

	platform[222].width = 100;
	platform[222].height = 50;
	platform[222].center.x = 11825;
	platform[222].center.y = 400;
	num++;

	platform[223].width = 100;
	platform[223].height = 50;
	platform[223].center.x = 12025;
	platform[223].center.y = 0;
	num++;

	platform[224].width = 100;
	platform[224].height = 50;
	platform[224].center.x = 12025;
	platform[224].center.y = 100;
	num++;

	platform[225].width = 100;
	platform[225].height = 50;
	platform[225].center.x = 12025;
	platform[225].center.y = 200;
	num++;

	platform[226].width = 100;
	platform[226].height = 50;
	platform[226].center.x = 12025;
	platform[226].center.y = 300;
	num++;

	platform[227].width = 100;
	platform[227].height = 50;
	platform[227].center.x = 12025;
	platform[227].center.y = 400;
	num++;

	platform[228].width = 30;
	platform[228].height = 15;
	platform[228].center.x = 11250;
	platform[228].center.y = 200;
	num++;

	platform[229].width = 30;
	platform[229].height = 15;
	platform[229].center.x = 11610;
	platform[229].center.y = 200;
	num++;

	platform[230].width = 100;
	platform[230].height = 50;
	platform[230].center.x = 11825;
	platform[230].center.y = 450;
	num++;

	platform[231].width = 100;
	platform[231].height = 50;
	platform[231].center.x = 12025;
	platform[231].center.y = 450;
	num++;

	platform[232].width = 100;
	platform[232].height = 50;
	platform[232].center.x = 11025;
	platform[232].center.y = 450;
	num++;

	platform[233].width = 100;
	platform[233].height = 50;
	platform[233].center.x = 10825;
	platform[233].center.y = 450;
	num++;
}
/*void makeEnemy(Boss *boss, Projectile *proj) 
{
	Boss enemy;
	*boss = enemy;
//	Projectile p = new Projectile[10];


}*/
