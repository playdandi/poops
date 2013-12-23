#include "Sound.h"
#include "SimpleAudioEngine.h"

//using namespace cocos2d;
using namespace CocosDenshion;


void Sound::PreLoadSound()
{
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("sounds/background.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/explosion.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/click.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/combo_double.m4a");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/combo_triple.m4a");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/combo_quadruple.m4a");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/combo_pentuple.m4a");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/crush3.m4a");

    SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.7);
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);
}

void Sound::playTouchSound()
{
    SimpleAudioEngine::sharedEngine()->playEffect("sounds/click.wav");
}

void Sound::playBombSound()
{
    SimpleAudioEngine::sharedEngine()->playEffect("sounds/explosion.mp3");
}

void Sound::playBackgroundSound()
{
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sounds/background.mp3", true);
}

void Sound::StopBackgroundSound()
{
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void Sound::playComboSound(int combo)
{
    switch(combo)
    {
        case 2:
            SimpleAudioEngine::sharedEngine()->playEffect("sounds/combo_double.m4a"); break;
        case 3:
            SimpleAudioEngine::sharedEngine()->playEffect("sounds/combo_triple.m4a"); break;
        case 4:
            SimpleAudioEngine::sharedEngine()->playEffect("sounds/combo_quadruple.m4a"); break;
        case 5:
            SimpleAudioEngine::sharedEngine()->playEffect("sounds/combo_pentuple.m4a"); break;
        case 6:
            SimpleAudioEngine::sharedEngine()->playEffect("sounds/crush3.m4a"); break;
    }
}
