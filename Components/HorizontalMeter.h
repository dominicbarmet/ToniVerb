#pragma once

#include <JuceHeader.h>


namespace Gui
{
class VerticalMeterLeft : public juce::Component
{
public:
    void paint(juce::Graphics& g) override
    {   auto boundsL = getLocalBounds().toFloat();
        boundsL.removeFromLeft(1.f);
        boundsL.removeFromRight(31.f);
        auto boundsR = getLocalBounds().toFloat();
        boundsR.removeFromLeft(13.f);
        boundsR.removeFromRight(19.f);
        g.setColour(juce::Colours::white.withAlpha(0.15f));
        g.fillRoundedRectangle(1.f, 0.f, 8.f, getHeight(), 6.f);
        g.fillRoundedRectangle(13.f, 0.f, 8.f, getHeight(), 6.f);
        g.setColour(juce::Colours::white.withAlpha(0.6f));
        g.drawRoundedRectangle(1.f, 0.f, 8.f, getHeight(), 6.f, 1.f);
        g.setColour(juce::Colours::greenyellow.withAlpha(0.8f));
        const auto scaledYl = juce::jmap(levelL, -60.f, 0.f, 0.f, static_cast<float>((getHeight())));
        g.fillRoundedRectangle(boundsL.removeFromBottom(scaledYl),4.f);
        
        g.setColour(juce::Colours::white.withAlpha(0.6f));
        g.drawRoundedRectangle(13.f, 0.f, 8.f, getHeight(), 6.f, 1.f);
        g.setColour(juce::Colours::greenyellow.withAlpha(0.8f));
        const auto scaledYr = juce::jmap(levelR, -60.f, 0.f, 0.f, static_cast<float>((getHeight())));
        g.fillRoundedRectangle(boundsR.removeFromBottom(scaledYr),4.f);
        g.setColour(juce::Colours::white);
        if (levelL > -3.f){
            if(levelL < 0.f){
            g.setColour(juce::Colours::yellow);
            }
        };
        if (levelL > 0.f){
            g.setColour(juce::Colours::red);
        };
        g.drawLine(1.f, getHeight()-scaledYl, 9.f, getHeight()-scaledYl,3.f);
        g.setColour(juce::Colours::white);
        if (levelR > -3.f){
            if(levelR < 0.f){
            g.setColour(juce::Colours::yellow);
            }
        };
        if (levelR > 0.f){
            g.setColour(juce::Colours::red);
        };
        g.drawLine(13.f, getHeight()-scaledYr, 21.f, getHeight()-scaledYr,2.f);
        
        sixdBlines = (getHeight()/10);
        threesBlines = sixdBlines/2;
        
        g.setColour(juce::Colours::white.withAlpha(0.45f));
        for(int pixel=1;
            pixel < 20;
            ++pixel)
        {
            lineheight = pixel*(threesBlines);
            g.drawLine(1.f, lineheight, 9.f, lineheight, 1.f);
            
            g.drawLine(13.f, lineheight, 21.f, lineheight, 1.f);
        }
        g.setFont(10.f);
        g.setColour(juce::Colours::white.withAlpha(0.6f));
       
        g.drawFittedText("-06", 21.f, sixdBlines-4.f,20.f, 8.f, juce::Justification::centred , 1);
        g.drawFittedText("-12", 21.f, 2*sixdBlines-4.f,20.f, 8.f, juce::Justification::centred, 1);
        g.drawFittedText("-18", 21.f, 3*sixdBlines-4.f,20.f, 8.f, juce::Justification::centred, 1);
        g.drawFittedText("-24", 21.f, 4*sixdBlines-4.f,20.f, 8.f, juce::Justification::centred, 1);
        g.drawFittedText("-30", 21.f, 5*sixdBlines-4.f,20.f, 8.f, juce::Justification::centred, 1);
        g.drawFittedText("-36", 21.f, 6*sixdBlines-4.f,20.f, 8.f, juce::Justification::centred, 1);
        g.drawFittedText("-42", 21.f, 7*sixdBlines-4.f,20.f, 8.f, juce::Justification::centred, 1);
        g.drawFittedText("-48", 21.f, 8*sixdBlines-4.f,20.f, 8.f, juce::Justification::centred, 1);
        g.drawFittedText("-54", 21.f, 9*sixdBlines-4.f,20.f, 8.f, juce::Justification::centred, 1);
 

        /*
        g.drawFittedText("-30",6/12*getHeight(), 0.f,(6/12*getHeight())-4.f, 30.f, 8.f, juce::Justification::centred, 1);
        g.drawFittedText("-40",8/12*getHeight(), 0.f,(8/12*getHeight())-4.f, 30.f, 8.f, juce::Justification::centred, 1);
        g.drawFittedText("-60",10/12*getHeight(), 0.f,(10/12*getHeight())-4.f, 30.f, 8.f, juce::Justification::centred, 1);*/
    }
    void setLevel(const float valueL, const float valueR) {levelL = valueL; levelR =valueR;}
private:
    float levelL = -60.f;
    float levelR = -60.f;
    float lineheight;
    float sixdBlines;
    float threesBlines;
    int pixel;
};
class VerticalMeterRight : public juce::Component
{
public:
    void paint(juce::Graphics& g) override
    {   auto boundsL = getLocalBounds().toFloat();
        boundsL.removeFromLeft(19.f);
        boundsL.removeFromRight(13.f);
        auto boundsR = getLocalBounds().toFloat();
        boundsR.removeFromLeft(31.f);
        boundsR.removeFromRight(1.f);
        g.setColour(juce::Colours::white.withAlpha(0.15f));
        g.fillRoundedRectangle(19.f, 0.f, 8.f, getHeight(), 6.f);
        g.fillRoundedRectangle(31.f, 0.f, 8.f, getHeight(), 6.f);
        g.setColour(juce::Colours::white.withAlpha(0.6f));
        g.drawRoundedRectangle(19.f, 0.f, 8.f, getHeight(), 6.f, 1.f);
        g.setColour(juce::Colours::greenyellow.withAlpha(0.8f));
        const auto scaledYl = juce::jmap(levelL, -60.f, 0.f, 0.f, static_cast<float>((getHeight())));
        g.fillRoundedRectangle(boundsL.removeFromBottom(scaledYl),4.f);
        
        g.setColour(juce::Colours::white.withAlpha(0.6f));
        g.drawRoundedRectangle(31.f, 0.f, 8.f, getHeight(), 6.f, 1.f);
        g.setColour(juce::Colours::greenyellow.withAlpha(0.8f));
        const auto scaledYr = juce::jmap(levelR, -60.f, 0.f, 0.f, static_cast<float>((getHeight())));
        g.fillRoundedRectangle(boundsR.removeFromBottom(scaledYr),4.f);
        
        sixdBlines = (getHeight()/10);
        threedBlines =sixdBlines/2;
        
        g.setColour(juce::Colours::white.withAlpha(0.45f));
        for(int pixel=1;
            pixel < 20;
            ++pixel)
        {
            lineheight = pixel*(threedBlines);
            g.drawLine(19.f, lineheight, 27.f, lineheight, 1.f);
            
            g.drawLine(31.f, lineheight, 39.f, lineheight, 1.f);
        }
        
        
        g.setColour(juce::Colours::white);
        if (levelL > -3.f){
            if(levelL < 0.f){
            g.setColour(juce::Colours::yellow);
            }
        };
        if (levelL > 0.f){
            g.setColour(juce::Colours::red);
        };
        g.drawLine(19.f, getHeight()-scaledYl, 27.f, getHeight()-scaledYl,3.f);
        g.setColour(juce::Colours::white);
        if (levelR > -3.f){
            if(levelR < 0.f){
            g.setColour(juce::Colours::yellow);
            }
        };
        if (levelR > 0.f){
            g.setColour(juce::Colours::red);
        };
        g.drawLine(31.f, getHeight()-scaledYr, 39.f, getHeight()-scaledYr,2.f);
    
        
        g.setFont(10.f);
        g.setColour(juce::Colours::white.withAlpha(0.6f));
       
        g.drawFittedText("-06", 0.f, sixdBlines-4.f,17.f, 8.f, juce::Justification::centred , 1);
        g.drawFittedText("-12", 0.f, 2*sixdBlines-4.f,17.f, 8.f, juce::Justification::centred, 1);
        g.drawFittedText("-18", 0.f, 3*sixdBlines-4.f,17.f, 8.f, juce::Justification::centred, 1);
        g.drawFittedText("-24", 0.f, 4*sixdBlines-4.f,17.f, 8.f, juce::Justification::centred, 1);
        g.drawFittedText("-30", 0.f, 5*sixdBlines-4.f,17.f, 8.f, juce::Justification::centred, 1);
        g.drawFittedText("-36", 0.f, 6*sixdBlines-4.f,17.f, 8.f, juce::Justification::centred, 1);
        g.drawFittedText("-42", 0.f, 7*sixdBlines-4.f,17.f, 8.f, juce::Justification::centred, 1);
        g.drawFittedText("-48", 0.f, 8*sixdBlines-4.f,17.f, 8.f, juce::Justification::centred, 1);
        g.drawFittedText("-54", 0.f, 9*sixdBlines-4.f,17.f, 8.f, juce::Justification::centred, 1);
    }
    void setLevel(const float valueL, const float valueR) {levelL = valueL; levelR =valueR;}
private:
    float levelL = -60.f;
    float levelR = -60.f;
    float lineheight;
    float sixdBlines;
    float threedBlines;
    int pixel;
};
}
