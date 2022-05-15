#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"
#include "../Components/HorizontalMeter.h"
#include "../Components/PeakLamp.h"
#include "../Components/ProgramDisplay.h"
#include "../Components/Documentation.h"
//==============================================================================
class ToniVerb_AudioProcessorEditor  : public juce::AudioProcessorEditor,
juce::ToggleButton::Listener, juce::Timer, juce::ComboBox::Listener
{
public:
    ToniVerb_AudioProcessorEditor (ToniVerb_AudioProcessor&);
    ~ToniVerb_AudioProcessorEditor() override;
    //=============================================================================
    void timerCallback() override;
    //=============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
public:
    Gui::ProgramDisplay programDisplay;
    //Slider und Label
    juce::Slider dryGainSlider, widthSlider, lowCutSlider, highCutSlider, preDelaySlider, wetGainSlider, inputGainSlider, characterSlider;
    juce::Label dryGainLabel, widthLabel, lowCutLabel, highCutLabel, preDelayLabel, wetGainLabel, inputGainLabel, characterLabel, micSelectionLabel, distanceSelectionLabel;
    //Bilder
    
    juce::ImageComponent backgroundImageComponent, roomImageComponent;
    
    juce::Image imageKonzertsaal2 = juce::ImageCache::getFromMemory(BinaryData::Photo_Konzertsaal2_jpg, BinaryData::Photo_Konzertsaal2_jpgSize);
    juce::Image imageKonzertsaal3 = juce::ImageCache::getFromMemory(BinaryData::Photo_Konzertsaal3_jpg , BinaryData::Photo_Konzertsaal3_jpgSize);
    juce::Image imageTurm = juce::ImageCache::getFromMemory(BinaryData::Photo_Turm_jpg, BinaryData::Photo_Turm_jpgSize);
    juce::Image imageKaskadenhalle = juce::ImageCache::getFromMemory(BinaryData::Photo_Kaskadenhalle_jpg, BinaryData::Photo_Kaskadenhalle_jpgSize);
 
    //Togglebuttons
    juce::ToggleButton muteWetToggleButton, muteDryToggleButton, bypassToggleButton, aboutToggleButton, nearToggleButton, mediumToggleButton, farToggleButton, abToggleButton, xyToggleButton, msToggleButton;
    //Parameter für GUI-Bounds
    double GlobalSliderWidth, GlobalSliderHeight, GlobalSliderOffsetX, GlobalSliderOffsetY, GlobalLabelHeight, BorderX, BorderY;
    //Metering-Instanzen
    Gui::VerticalMeterLeft verticalMeterInput;
    Gui::VerticalMeterRight verticalMeterOutput;
    Gui::PeakLamp peakLampLin, peakLampRin, peakLampLout, peakLampRout;
    
    int RoomID, MicID, DistID;
    CustomLNF myCustomLNF;
    Gui::Documentation doc;
public:
    juce::ComboBox roomSelector, micSelector, distanceSelector;
    void buttonClicked (juce::Button *button) override;
    void comboBoxChanged(juce::ComboBox *box) override;
    void getComboboxes();
    void setSliderComponents();
    void setImageComponents();
    void setMeteringComponents();
    void setButtonComponents();
    void updateConvolution();
    void getDocumentationWindow(juce::Graphics &g, juce::ResizableWindow);
    void checkButtonState();
    void setPluginSliderStyle(juce::Slider &slider,juce::Slider::SliderStyle style, int decimalPlaces, float defaultValue, std::string valueSuffix, juce::Label &label, std::string labelText);
    void sliderMatrix1by1 (juce::Slider &slider, juce::Label &label, float MatrixX, float MatrixY, bool textRight);
    ToniVerb_AudioProcessor& audioProcessor;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputVolumeSliderAttachment, dryVolumeSliderAttachment, wetVolumeSliderAttachment, highCutFrequencySliderAttachment, lowCutFrequencySliderAttachment, widthSliderAttachment, characterSliderAttachment, preDelaySliderAttachment;
    
    juce::StringArray attachmentArray;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> muteWetButtonAttachment, muteDryButtonAttachment, bypassButtonAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> roomAttachment, micIDAttachment, distanceIDAttachment;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToniVerb_AudioProcessorEditor)
};
