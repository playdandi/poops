#include "Sound.h"
#include "SimpleAudioEngine.h"

//using namespace cocos2d;
using namespace CocosDenshion;

void Sound::SetFileExtension()
{
    int platform = CC_TARGET_PLATFORM;
    switch (platform)
    {
        case CC_PLATFORM_IOS:
            ext = "wav"; break;
        case CC_PLATFORM_ANDROID:
            ext = "ogg"; break;
    }
}

void Sound::PreLoadSound()
{
    SetFileExtension();

    if (ext == "wav")
    {
        SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("sounds/background.mp3");
        SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("sounds/background_crush.mp3");
        SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/explosion.mp3");
        SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/click.wav");
        SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/combo_double.wav");
        SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/combo_triple.wav");
        SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/combo_quadruple.wav");
        SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/combo_pentuple.wav");
        SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/crush3.wav");
    }
    else
    {
        SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("sounds/background.ogg");
        SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/explosion.ogg");
        SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/click.wav");
        SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/combo_double.ogg");
        SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/combo_triple.ogg");
        SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/combo_quadruple.ogg");
        SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/combo_pentuple.ogg");
        SimpleAudioEngine::sharedEngine()->preloadEffect("sounds/crush3.ogg");
    }

    SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.7);
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);
}

void Sound::playTouchSound()
{
    SimpleAudioEngine::sharedEngine()->playEffect("sounds/click.wav");
}

void Sound::playBombSound()
{
    if (ext == "wav")
        SimpleAudioEngine::sharedEngine()->playEffect("sounds/explosion.mp3");
    else
        SimpleAudioEngine::sharedEngine()->playEffect("sounds/explosion.ogg");
}

void Sound::playBackgroundSound(bool isCrushing)
{
    if (ext == "wav")
    {
        if (isCrushing)
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sounds/background_crush.mp3", true);
        else
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sounds/background.mp3", true);
    }
    else
    {
        if (isCrushing)
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sounds/background_crush.mp3", true);
        else
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sounds/background.ogg", true);
    }
    
}

void Sound::StopBackgroundSound()
{
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void Sound::playComboSound(int combo)
{
    if (ext == "wav")
    {
        switch(combo)
        {
            case 2:
                SimpleAudioEngine::sharedEngine()->playEffect("sounds/combo_double.wav"); break;
            case 3:
                SimpleAudioEngine::sharedEngine()->playEffect("sounds/combo_triple.wav"); break;
            case 4:
                SimpleAudioEngine::sharedEngine()->playEffect("sounds/combo_quadruple.wav"); break;
            case 5:
                SimpleAudioEngine::sharedEngine()->playEffect("sounds/combo_pentuple.wav"); break;
            case 6:
                SimpleAudioEngine::sharedEngine()->playEffect("sounds/crush3.wav"); break;
        }
    }
    else
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
}
