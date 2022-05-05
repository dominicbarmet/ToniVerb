/*
  ==============================================================================

    Documentation.h
    Created: 7 Apr 2022 4:29:21pm
    Author:  Dominic

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace Gui
{
class Documentation : public juce::Component

{
public:
    juce::HyperlinkButton button;

    void paint(juce::Graphics& g) override //calloutbox 
    {
       
        g.setColour(juce::Colours::darkgrey.withAlpha(0.8f));
        g.fillAll();
        g.setColour(juce::Colours::white);
        g.setFont(16.f);
        getLocalBounds().reduce(10, 10);
        g.drawMultiLineText(("TONIVERB originated as my bachelors project at ZHdK. It aims to give you access to new spaces and wants to inspire you to create something of your own with it. \r \r Find out more about how it has been made and stay updated on: \r \r \r github.com/dominicbarmet/ToniVerb"), 10, 25, getWidth()-20, getHeight()-60, juce::Justification::left);
        button.setURL(juce::URL("https://github.com/dominicbarmet/ToniVerb"));
        button.setTitle("gitHub");
        button.setBounds(10, 125, getWidth()-20, 30);
        addAndMakeVisible(button);
    }
};
}
