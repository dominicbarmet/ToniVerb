/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 14 Feb 2022 11:33:54pm
    Author:  Dominic

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class CustomLNF : public juce::LookAndFeel_V4
{
    

//const mit allen getfrommemory
public:
    void drawRotarySlider(juce::Graphics &, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider &) override;
    void drawToggleButton(juce::Graphics &g, juce::ToggleButton &button, bool highlighted, bool buttonDown) override;
    //void drawButtonText(juce::Graphics &g, juce::TextButton &button , bool highlighted, bool buttonDown) override;
    //void drawButtonBackground(juce::Graphics &g, juce::Button &button, const juce::Colour &backgroundColour, bool highlighted, bool buttonDown) override;
    //void drawDocumentWindowTitleBar(juce::DocumentWindow &window, juce::Graphics &g, int posx, int posy,int  height,int width,const juce::Image *image, bool highlighted) override;
    //void paintToolbarBackground(juce::Graphics &g, int width, int height, juce::Toolbar &Toolbar) override;
    //void drawComboBox(juce::Graphics &g, int width, int height, bool buttonDown, int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox &combobox) override;
    //void drawPopupMenuBackground(juce::Graphics &g, int width, int height) override;
    //juce::Button* createDocumentWindowButton(int buttonType) override;
    juce::Image imageKnob = juce::ImageCache::getFromMemory(BinaryData::Knob_png, BinaryData::Knob_pngSize);
    juce::Image imageKnobDark = juce::ImageCache::getFromMemory(BinaryData::KnobDark_png , BinaryData::KnobDark_pngSize);
    juce::Image imageShadow = juce::ImageCache::getFromMemory(BinaryData::Knob_Shadow_png, BinaryData::Knob_Shadow_pngSize);
    juce::Image imageKnobDot = juce::ImageCache::getFromMemory(BinaryData::Knobdot_png , BinaryData::Knobdot_pngSize );
    juce::Image imageButtonUp = juce::ImageCache::getFromMemory(BinaryData::ButtonUp_png , BinaryData::ButtonUp_pngSize);
    juce::Image imageButtonDown = juce::ImageCache::getFromMemory(BinaryData::ButtonDown_png, BinaryData::ButtonDown_pngSize);
    juce::Image imageButtonRed = juce::ImageCache::getFromMemory(BinaryData::ButtonRed_png, BinaryData::ButtonRed_pngSize);
    int stateChange;
    std::string buttonName;
};

