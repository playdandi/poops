#ifndef __poops__Sound__
#define __poops__Sound__

#include "Common.h"

class Sound
{
public:
    void SetFileExtension();
    void PreLoadSound();
    void playTouchSound();
    void playBombSound();
    void playBackgroundSound(bool isCrushing);
    void StopBackgroundSound();
    void playComboSound(int combo);
    void playItemSound();

private:
    std::string ext;
};

#endif /* defined(__poops__Sound__) */
