#ifndef __poops__Sound__
#define __poops__Sound__

#include <iostream>

class Sound
{
public:
    void PreLoadSound();
    void playTouchSound();
    void playBombSound();
    void playBackgroundSound();
    void StopBackgroundSound();
};

#endif /* defined(__poops__Sound__) */
