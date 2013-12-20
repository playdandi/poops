#include "Sound.h"
#include "SimpleAudioEngine.h"

//using namespace cocos2d;
using namespace CocosDenshion;


void Sound::PreLoadSound()
{
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("sounds/background.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/explosion.mp3");
    SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/click.wav");

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
