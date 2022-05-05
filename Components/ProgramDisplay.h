#pragma once

#include <JuceHeader.h>
#include "../Source/PluginProcessor.h"
#include "../Source/PluginEditor.h"

namespace Gui
{
class ProgramDisplay : public juce::Component
    {
    public:
        void paint(juce::Graphics& g) override
        {
            g.setColour(juce::Colours::white.withAlpha(0.2f));
            g.fillRoundedRectangle(0, 0, getWidth(), getHeight(), 4.f);
            g.setColour(juce::Colours::black.withAlpha(0.8f));
        }
    
private:
    };
}
