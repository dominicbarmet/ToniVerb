#pragma once

#include <JuceHeader.h>


namespace Gui
{
class PeakLamp : public juce::Component
    {
    public:
        void paint(juce::Graphics& g) override
        {
            g.setColour(juce::Colours::black.withAlpha(0.6f));
            g.fillEllipse(1.f, 1.f, getWidth()-2.f, getHeight()-2.f);
            g.setColour(juce::Colours::white.withAlpha(0.6f));
            g.drawEllipse(1.f, 1.f, getWidth()-2.f, getHeight()-2.f, 1.f);
        if (level > -3.f){
            if(level < 0.f){
            g.setColour(juce::Colours::yellow.withAlpha(0.15f));
            g.fillEllipse(0.f, 0.f, getWidth(), getWidth());
            g.setColour(juce::Colours::yellow.withAlpha(0.9f));
            g.fillEllipse(4.f, 4.f, getWidth()-8.f, getWidth()-8.f);
            }
        };
        if (level > 0.f){
            g.setColour(juce::Colours::red.withAlpha(0.15f));
            g.fillEllipse(0.f, 0.f, getWidth(), getWidth());
            g.setColour(juce::Colours::red.withAlpha(0.9f));
            g.fillEllipse(4.f, 4.f, getWidth()-8.f, getWidth()-8.f);
        };
    }
    void setLevel(const float value) {level = value;}
    
private:
    float level = 0.f;
    };
}
