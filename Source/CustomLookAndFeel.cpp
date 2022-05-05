#include "CustomLookAndFeel.h"

void CustomLNF::drawRotarySlider(juce::Graphics &g, int x, int y , int width, int height, float SliderPosProportional, float RotaryStartAngle, float RotaryEndAngle, juce::Slider &slider)
{
    auto rotY = (height/2)+1.f;
    g.setColour(juce::Colours::white.withAlpha(0.3f));
    //g.fillRoundedRectangle(1, 1, width-2, height-2, 3.f);
    g.setColour(juce::Colours::white);
    //g.drawRoundedRectangle(1, 1, width-2, height-2, 3.f, 2.f);
    g.setOpacity(0.5f);
    g.drawImage(imageShadow,0, 0, width, width, 0, 0, imageShadow.getWidth(), imageShadow.getHeight());
    g.setOpacity(1.f);
    g.drawImage(imageKnobDark,0, 0, width, height, 0, 0, imageKnobDark.getWidth(), imageKnobDark.getHeight());
    if (slider.getName() == "hiCutSlider")
    {
        g.setOpacity(1 - SliderPosProportional);
        g.drawImage(imageKnob,0, 0, width, height, 0, 0, imageKnob.getWidth(), imageKnob.getHeight());
    }
    else
    {
        g.setOpacity(SliderPosProportional);
        g.drawImage(imageKnob,0, 0, width, height, 0, 0, imageKnob.getWidth(), imageKnob.getHeight());
    }
    
    g.addTransform(juce::AffineTransform::rotation(RotaryStartAngle + SliderPosProportional * (RotaryEndAngle - RotaryStartAngle), width/2, rotY));
    g.setOpacity(1.f);
    g.drawImage(imageKnobDot, 0, 0, width, height, 0, 0, imageKnobDot.getWidth(), imageKnobDot.getHeight());
    
    
}
void CustomLNF::drawToggleButton(juce::Graphics &g, juce::ToggleButton &button, bool highlighted, bool buttonDown)
{
    buttonName = button.getButtonText().toStdString();
    
    if (button.getToggleState()==true)
    {
        if (button.getName() == "mute")
        {
            g.drawImage(imageButtonRed,0,0, button.getWidth(), button.getHeight(), 0, 0, 400, 200);
        }
        else
        {
            g.drawImage(imageButtonDown,0,0, button.getWidth(), button.getHeight(), 0, 0, 400, 200);
        }
       
    }
    else
    {
        g.drawImage(imageButtonUp,0,0, button.getWidth(), button.getHeight(), 0, 0, 400, 200);
    }
    g.setColour(juce::Colours::white.withAlpha(0.8f));
    g.setFont(12.f);
    g.drawText(buttonName, 0, 0, button.getWidth(), button.getHeight(), juce::Justification::centred);
    
   
}
