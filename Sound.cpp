#include "Sound.h"
#include "SimpleAudioEngine.h"

//using namespace cocos2d;
using namespace CocosDenshion;


void Sound::PreLoadSound()
{
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("sounds/background.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/explosion.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/click.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/combo_double.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/combo_triple.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/combo_quadruple.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/combo_pentuple.ogg");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/crush3.ogg");

    SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.7);
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);
}

void Sound::playTouchSound()
{
    SimpleAudioEngine::sharedEngine()->playEffect("sounds/click.wav");
}

void Sound::playBombSound()
{
    SimpleAudioEngine::sharedEngine()->playEffect("sounds/explosion.ogg");
}

void Sound::playBackgroundSound()
{
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sounds/background.ogg", true);
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
            SimpleAudioEngine::sharedEngine()->playEffect("sounds/combo_double.ogg"); break;
        case 3:
            SimpleAudioEngine::sharedEngine()->playEffect("sounds/combo_triple.ogg"); break;
        case 4:
            SimpleAudioEngine::sharedEngine()->playEffect("sounds/combo_quadruple.ogg"); break;
        case 5:
            SimpleAudioEngine::sharedEngine()->playEffect("sounds/combo_pentuple.ogg"); break;
        case 6:
            SimpleAudioEngine::sharedEngine()->playEffect("sounds/crush3.ogg"); break;
    }
}
