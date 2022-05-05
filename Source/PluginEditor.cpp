#include "PluginProcessor.h"
#include "PluginEditor.h"


ToniVerb_AudioProcessorEditor::ToniVerb_AudioProcessorEditor (ToniVerb_AudioProcessor& p)
: AudioProcessorEditor (&p), audioProcessor (p)
{
    juce::LookAndFeel::setDefaultLookAndFeel( &myCustomLNF);
    setSize (800, 400);
    startTimerHz(18);
    
    setImageComponents();
    setMeteringComponents();
    setSliderComponents();
    getComboboxes();
    setButtonComponents();
    
    addChildComponent(doc);
    doc.setAlwaysOnTop(true);
    
    checkButtonState();
}
ToniVerb_AudioProcessorEditor::~ToniVerb_AudioProcessorEditor()
{
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
}
void ToniVerb_AudioProcessorEditor::timerCallback()
{
    float peakSampleLeftPre = audioProcessor.getMeteringValue(0);
    float peakSampleRightPre = audioProcessor.getMeteringValue(1);
    float peakSampleLeftPost = audioProcessor.getMeteringValue(2);
    float peakSampleRightPost = audioProcessor.getMeteringValue(3);
    
    verticalMeterInput.setLevel(peakSampleLeftPre, peakSampleRightPre); //erst werte von processor abfragen, dann weiterverarbeiten
    verticalMeterInput.repaint();
    peakLampLin.setLevel(peakSampleLeftPre);
    peakLampRin.setLevel(peakSampleRightPre);
    peakLampLin.repaint();
    peakLampRin.repaint();

    verticalMeterOutput.setLevel(peakSampleLeftPost, peakSampleRightPost);
    verticalMeterOutput.repaint();
    peakLampLout.setLevel(peakSampleLeftPost);
    peakLampRout.setLevel(peakSampleRightPost);
    peakLampLout.repaint();
    peakLampRout.repaint();
    
   
}
//==========================================Main Window Bounds=============================================//
void ToniVerb_AudioProcessorEditor::resized()
{
    GlobalSliderWidth=80; GlobalSliderHeight=100; GlobalSliderOffsetY=125; GlobalLabelHeight=20; BorderX=60; BorderY=40; GlobalSliderOffsetX=(getWidth()-(BorderX*2))/8;
    //Parameter für platzierung von Slidern auf Matrix
    sliderMatrix1by1(lowCutSlider, lowCutLabel, 1, 3, false);
    sliderMatrix1by1(highCutSlider, highCutLabel, 1, 2, false);
    sliderMatrix1by1(widthSlider, widthLabel, 8, 3, true);
    sliderMatrix1by1(preDelaySlider, preDelayLabel, 8, 1, true);
    sliderMatrix1by1(dryGainSlider, dryGainLabel, 4.6f, 3, false);
    sliderMatrix1by1(wetGainSlider, wetGainLabel, 5.6f, 3, true);
    sliderMatrix1by1(inputGainSlider, inputGainLabel, 1, 1, false);
    sliderMatrix1by1(characterSlider, characterLabel, 8, 2, true);
    

    backgroundImageComponent.setBounds(0, 0, getWidth(), getHeight());
    
    
    verticalMeterInput.setBounds(10, 20, 40, 375);
    verticalMeterOutput.setBounds(getWidth()-50, 20, 40, 375);
    peakLampLin.setBounds(9, 5, 12, 12);
    peakLampRin.setBounds(21, 5, 12, 12);

    peakLampLout.setBounds(getWidth()-33, 5, 12, 12);
    peakLampRout.setBounds(getWidth()-21, 5, 12, 12);
    micSelector.setBounds(100, 100, 100, 100);
 
    programDisplay.setBounds((BorderX*1.2f)+GlobalSliderOffsetX, 6 , getWidth() - (BorderX*2.4f) - (GlobalSliderOffsetX*2), getHeight()-130);
    
    roomSelector.setBounds((BorderX*1.3f)+GlobalSliderOffsetX, 10, getWidth() - (BorderX*2.6f) - (GlobalSliderOffsetX*2) - 80, 30);
    aboutToggleButton.setBounds(roomSelector.getX() + roomSelector.getWidth() + 2,roomSelector.getY() , 80, roomSelector.getHeight());
    
    abToggleButton.setBounds(roomSelector.getX(), 300, 100, 30);
    xyToggleButton.setBounds(abToggleButton.getX(), abToggleButton.getY() + 30, abToggleButton.getWidth(), abToggleButton.getHeight());
    msToggleButton.setBounds(abToggleButton.getX(), abToggleButton.getY() + 60, abToggleButton.getWidth(), abToggleButton.getHeight());
    
    micSelectionLabel.setBounds(abToggleButton.getX(), abToggleButton.getY() - 25, abToggleButton.getWidth(), abToggleButton.getHeight());
    
    nearToggleButton.setBounds(roomSelector.getX() + abToggleButton.getWidth() +2, abToggleButton.getY(), abToggleButton.getWidth(), abToggleButton.getHeight());
    mediumToggleButton.setBounds(nearToggleButton.getX(), nearToggleButton.getY()+30, abToggleButton.getWidth(), abToggleButton.getHeight());
    farToggleButton.setBounds(nearToggleButton.getX(), nearToggleButton.getY()+60, abToggleButton.getWidth(), abToggleButton.getHeight());
    
    distanceSelectionLabel.setBounds(nearToggleButton.getX(), nearToggleButton.getY()-25, nearToggleButton.getWidth(), nearToggleButton.getHeight());
    
    muteWetToggleButton.setBounds(540, abToggleButton.getY(), abToggleButton.getWidth(), abToggleButton.getHeight());
    muteDryToggleButton.setBounds(muteWetToggleButton.getX(), abToggleButton.getY()+30, abToggleButton.getWidth(), abToggleButton.getHeight());
    bypassToggleButton.setBounds(muteWetToggleButton.getX(), abToggleButton.getY()+60, abToggleButton.getWidth(), abToggleButton.getHeight());
    
    
    roomImageComponent.setBounds(roomSelector.getX(),roomSelector.getY()+roomSelector.getHeight()+4,getWidth() - (BorderX*2.6f) - (GlobalSliderOffsetX*2),227);
    doc.setBounds(roomImageComponent.getBounds());
    
}

void ToniVerb_AudioProcessorEditor::buttonClicked(juce::Button *button)
{
    if (button == &bypassToggleButton)
    {
    }
    else if (button == &aboutToggleButton)
    {
        if(aboutToggleButton.getToggleStateValue()==true){doc.setVisible(true);}
        if(aboutToggleButton.getToggleStateValue()==false){doc.setVisible(false);}
    }
    else if (button ==&nearToggleButton)
    {
        nearToggleButton.setToggleState(true, juce::NotificationType::dontSendNotification);
        
        mediumToggleButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        farToggleButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        DistID = 1;
        distanceSelector.setSelectedId(1);
        updateConvolution();
    }
    else if (button ==&mediumToggleButton)
    {
        mediumToggleButton.setToggleState(true, juce::NotificationType::dontSendNotification);
        
        nearToggleButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        farToggleButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        DistID = 2;
        distanceSelector.setSelectedId(2);
        updateConvolution();
    }
    else if (button ==& farToggleButton)
    {
        farToggleButton.setToggleState(true, juce::NotificationType::dontSendNotification);
        
        nearToggleButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        mediumToggleButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        DistID = 3;
        distanceSelector.setSelectedId(3);
        updateConvolution();
    }
    else if (button ==&abToggleButton)
    {
        abToggleButton.setToggleState(true, juce::NotificationType::dontSendNotification);
        
        xyToggleButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        msToggleButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        MicID =1;
        micSelector.setSelectedId(1);
        updateConvolution();
    }
    else if (button ==&xyToggleButton)
    {
        xyToggleButton.setToggleState(true, juce::NotificationType::dontSendNotification);
        
        abToggleButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        msToggleButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        MicID =2;
        micSelector.setSelectedId(2);
        updateConvolution();
    }
    else if (button ==& msToggleButton)
    {
        msToggleButton.setToggleState(true, juce::NotificationType::dontSendNotification);
        
        abToggleButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        xyToggleButton.setToggleState(false, juce::NotificationType::dontSendNotification);
        MicID =3;
        micSelector.setSelectedId(3);
        updateConvolution();
    }
    
};

//===================COMPONENTS=================================//
//==========Slider + Slidermethod + PLatzierung Slider==========//
void ToniVerb_AudioProcessorEditor::setSliderComponents()
{
    inputVolumeSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.valueTreeState, "INPUT_VOLUME", inputGainSlider);
    dryVolumeSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.valueTreeState, "DRY_VOLUME", dryGainSlider);
    wetVolumeSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.valueTreeState, "WET_VOLUME", wetGainSlider);
    highCutFrequencySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.valueTreeState, "HIGH_CUT_FREQ", highCutSlider);
    lowCutFrequencySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.valueTreeState, "LOW_CUT_FREQ", lowCutSlider);
    widthSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.valueTreeState, "WIDTH", widthSlider);
    characterSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.valueTreeState, "CHARACTER", characterSlider);
    preDelaySliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.valueTreeState, "PREDELAY", preDelaySlider);
    
    
    setPluginSliderStyle(preDelaySlider, juce::Slider::SliderStyle::Rotary, 0, 0.f, "samples", preDelayLabel, "Pre-Delay"); //get ändern auf set
    setPluginSliderStyle(lowCutSlider, juce::Slider::SliderStyle::Rotary, 0, 20.f, " Hz", lowCutLabel, "Low Cut");
    lowCutSlider.setSkewFactor(0.5);
    setPluginSliderStyle(highCutSlider, juce::Slider::SliderStyle::Rotary, 0, 20000.f," Hz", highCutLabel, "High Cut");
    highCutSlider.setName("hiCutSlider");
    highCutSlider.setSkewFactor(0.5);
    wetGainSlider.setName("wetGainSlider");
    setPluginSliderStyle(wetGainSlider, juce::Slider::SliderStyle::Rotary, 0, 0.f, " dBFS", wetGainLabel, "Wet");
    dryGainSlider.setName("dryGainSlider");
    setPluginSliderStyle(dryGainSlider, juce::Slider::SliderStyle::Rotary, 0, 0.f, " dBFS", dryGainLabel, "Dry");
    setPluginSliderStyle(inputGainSlider, juce::Slider::SliderStyle::Rotary, 0, 0.f," dBFS", inputGainLabel, "Input Gain");
    
    setPluginSliderStyle(widthSlider, juce::Slider::SliderStyle::Rotary, 0, 100.f, " %", widthLabel, "Width");
    setPluginSliderStyle(characterSlider, juce::Slider::SliderStyle::Rotary, 0, 0.f, "", characterLabel, "Character");
}
void ToniVerb_AudioProcessorEditor::setPluginSliderStyle(juce::Slider &slider,juce::Slider::SliderStyle style, int decimalPlaces, float defaultValue, std::string valueSuffix, juce::Label &label, std::string labelText)
{
    addAndMakeVisible(slider); slider.setSliderStyle(style);
    slider.setTextValueSuffix(valueSuffix);
    slider.setNumDecimalPlacesToDisplay(decimalPlaces);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow , true, 80, 20);
    slider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    slider.setDoubleClickReturnValue(true, defaultValue);
    addAndMakeVisible(label);
    label.setText(labelText, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(juce::Label::textColourId, juce::Colours::white.withAlpha(0.8f));
    slider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white.withAlpha(0.8f));
}
void ToniVerb_AudioProcessorEditor::sliderMatrix1by1 (juce::Slider &slider, juce::Label &label, float MatrixX, float MatrixY, bool textRight)
{
    slider.setBounds(BorderX+((MatrixX-1)*GlobalSliderOffsetX), BorderY+((MatrixY-1)*GlobalSliderOffsetY), GlobalSliderWidth, GlobalSliderHeight);
    label.setBounds (BorderX+((MatrixX-1)*GlobalSliderOffsetX)-10, BorderY+((MatrixY-1)*GlobalSliderOffsetY)-(GlobalLabelHeight/2),   GlobalSliderWidth+20, GlobalLabelHeight);
    
}
//===================================Buttons=================================//
void ToniVerb_AudioProcessorEditor::setButtonComponents()
{
    
    //===================Mute & Bypass Section================================//
    addAndMakeVisible(muteDryToggleButton);
    muteDryToggleButton.setButtonText("Mute Dry");
    muteDryToggleButton.setName("mute");
    
    muteDryButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.valueTreeState, "MUTE_DRY", muteDryToggleButton);
    
    addAndMakeVisible(muteWetToggleButton);
    muteWetToggleButton.setButtonText("Mute Wet");
    muteWetToggleButton.setName("mute");
    
    muteWetButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.valueTreeState, "MUTE_WET", muteWetToggleButton);
    
    //addAndMakeVisible(bypassToggleButton);
    bypassToggleButton.setButtonText("Bypass");
    
    //=========================================About Button======================//
    
    addAndMakeVisible(aboutToggleButton);
    aboutToggleButton.setButtonText("About");
    aboutToggleButton.addListener(this);
    
    //================================Microphone Selection========================//
    
    addAndMakeVisible(abToggleButton);
    abToggleButton.setButtonText("AB");
    abToggleButton.addListener(this);
    
    addAndMakeVisible(xyToggleButton);
    xyToggleButton.setButtonText("XY");
    xyToggleButton.addListener(this);
    
    addAndMakeVisible(msToggleButton);
    msToggleButton.setButtonText("MS");
    msToggleButton.addListener(this);
    //================================Distance Selection========================//
    
    
    
    addAndMakeVisible(nearToggleButton);
    nearToggleButton.setButtonText("Near");
    nearToggleButton.addListener(this);
    
    addAndMakeVisible(mediumToggleButton);
    mediumToggleButton.setButtonText("Medium");
    mediumToggleButton.addListener(this);
    
    addAndMakeVisible(farToggleButton);
    farToggleButton.setButtonText("Far");
    farToggleButton.addListener(this);
    //===========================Selection Button Labels===================//
    
    addAndMakeVisible(micSelectionLabel);
    micSelectionLabel.setText("Microphones", juce::NotificationType::dontSendNotification);
    micSelectionLabel.setJustificationType(juce::Justification::centred);
    micSelectionLabel.setColour(juce::Label::textColourId, juce::Colours::white.withAlpha(0.8f));
    addAndMakeVisible(distanceSelectionLabel);
    
    distanceSelectionLabel.setText("Distance", juce::NotificationType::dontSendNotification);
    distanceSelectionLabel.setJustificationType(juce::Justification::centred);
    distanceSelectionLabel.setColour(juce::Label::textColourId, juce::Colours::white.withAlpha(0.8f));
    
}
//==================================Images========================================//
void ToniVerb_AudioProcessorEditor::setImageComponents()
{
    auto background = juce::ImageCache::getFromMemory(BinaryData::Background_png, BinaryData::Background_pngSize);
    if (! background.isNull())
    {
    backgroundImageComponent.setImage(background, juce::RectanglePlacement::stretchToFit);
    backgroundImageComponent.toBack();
    }
    else
        jassert (! background.isNull());
    addAndMakeVisible(roomImageComponent);
    addAndMakeVisible(backgroundImageComponent);
    roomImageComponent.setAlwaysOnTop(true);
}
//===============================Auswahllisten==================================//
void ToniVerb_AudioProcessorEditor::getComboboxes()
{
    
    
    addAndMakeVisible(roomSelector);
    
    roomSelector.addListener(this);
    roomAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.valueTreeState, "ROOM", roomSelector);
    
    roomSelector.setColour(juce::ComboBox::ColourIds::backgroundColourId, juce::Colours::black.withAlpha(0.6f));
    roomSelector.setColour(juce::ComboBox::ColourIds::focusedOutlineColourId , juce::Colours::orange.withAlpha(0.7f));
    roomSelector.setColour(juce::PopupMenu::ColourIds::backgroundColourId , juce::Colours::white.withAlpha(0.6f));
    roomSelector.setJustificationType(juce::Justification::left);
    //roomSelector.addItem("DEBUG", -1);
    //roomSelector.addSeparator();
    roomSelector.addItem("Konzertsaal 2", 1);
    roomSelector.addItem("Konzertsaal 3", 2);
    roomSelector.addItem("Kaskadenhalle", 3);
    roomSelector.addItem("Turm", 4);
    
    addChildComponent(micSelector);
    micSelector.addItem("AB", 1);
    micSelector.addItem("XY", 2);
    micSelector.addItem("MS", 3);
    
    micIDAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.valueTreeState, "MIC_ID", micSelector);
    
    
    addChildComponent(distanceSelector);
    distanceSelector.addItem("near", 1);
    distanceSelector.addItem("medium", 2);
    distanceSelector.addItem("far", 3);
    
    distanceIDAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.valueTreeState, "DISTANCE_ID", distanceSelector);
}
//===========================Listener Auswahllisten======================//
void ToniVerb_AudioProcessorEditor::comboBoxChanged(juce::ComboBox *box)
{
    RoomID = roomSelector.getSelectedId();
    
    if(box ==&roomSelector)
    {
      
        if (RoomID == 1)
        {
            roomImageComponent.setImage(imageKonzertsaal2, juce::RectanglePlacement::fillDestination);
        }
        else if (RoomID == 2)
        {
            roomImageComponent.setImage(imageKonzertsaal3, juce::RectanglePlacement::fillDestination);
            
        }
        
        else if (RoomID == 3)
        {
            roomImageComponent.setImage(imageKaskadenhalle, juce::RectanglePlacement::fillDestination);
        }
        else if (RoomID == 4)
        {
            roomImageComponent.setImage(imageTurm, juce::RectanglePlacement::fillDestination);
        }
        updateConvolution();
    }
    
};
//===================================Metering==================================//
void ToniVerb_AudioProcessorEditor::setMeteringComponents()
{
    addAndMakeVisible(verticalMeterInput);
    addAndMakeVisible(verticalMeterOutput);
    addAndMakeVisible(peakLampLin);
    addAndMakeVisible(peakLampRin);
    addAndMakeVisible(peakLampLout);
    addAndMakeVisible(peakLampRout);
    addAndMakeVisible(programDisplay);
}
//==================================Paint==================================//
void ToniVerb_AudioProcessorEditor::paint (juce::Graphics& g)
{
    
}
//=============================END COMPONENTS=========================//

//==============================STARTUP LOGIC=======================//
void ToniVerb_AudioProcessorEditor::checkButtonState()
{
    auto notificationType = juce::NotificationType::dontSendNotification;
    auto micIDState = audioProcessor.valueTreeState.getRawParameterValue("MIC_ID");
    auto distanceIDState = audioProcessor.valueTreeState.getRawParameterValue("DISTANCE_ID");
    auto roomIDValue = audioProcessor.valueTreeState.getRawParameterValue("ROOM");
    
    if (micIDState->load() == 0)
    {
        abToggleButton.setToggleState(true, notificationType);
    }
    else if (micIDState->load() == 1)
    {
        xyToggleButton.setToggleState(true, notificationType);
    }
    else if (micIDState->load() == 2)
    {
        msToggleButton.setToggleState(true, notificationType);
    }
    
    if (distanceIDState->load() == 0) {
        nearToggleButton.setToggleState(true, notificationType);
    }
    else if (distanceIDState->load() == 1) {
        mediumToggleButton.setToggleState(true, notificationType);
    }
    else if (distanceIDState->load() == 2) {
        farToggleButton.setToggleState(true, notificationType);
    }
    
    roomSelector.setSelectedId(roomIDValue->load()+1);
    
    RoomID = roomIDValue->load()+1;
    MicID = micIDState->load()+1;
    DistID = distanceIDState->load()+1;
    
    updateConvolution();
}

//==========================CONVOLUTION SELECTION====================//
void ToniVerb_AudioProcessorEditor::updateConvolution()
{
    if (RoomID == 1)
    {
        if ((MicID == 1 ) &&(DistID ==1))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KS2_AB_NEAR_0_EDITED_wav, BinaryData::IR_KS2_AB_NEAR_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KS2_AB_NEAR_180_EDITED_wav, BinaryData::IR_KS2_AB_NEAR_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 1 ) &&(DistID ==2))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KS2_AB_MED_0_EDITED_wav, BinaryData::IR_KS2_AB_MED_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KS2_AB_MED_180_EDITED_wav, BinaryData::IR_KS2_AB_MED_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            
        }
        else if ((MicID == 1 ) &&(DistID ==3))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KS2_AB_FAR_0_EDITED_wav, BinaryData::IR_KS2_AB_FAR_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KS2_AB_FAR_180_EDITED_wav, BinaryData::IR_KS2_AB_FAR_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 2 ) &&(DistID ==1))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KS2_XY_NEAR_0_EDITED_wav, BinaryData::IR_KS2_XY_NEAR_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KS2_XY_NEAR_180_EDITED_wav, BinaryData::IR_KS2_XY_NEAR_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 2 ) &&(DistID ==2))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KS2_XY_MED_0_EDITED_wav, BinaryData::IR_KS2_XY_MED_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KS2_XY_MED_180_EDITED_wav, BinaryData::IR_KS2_XY_MED_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 2 ) &&(DistID ==3))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KS2_XY_FAR_0_EDITED_wav, BinaryData::IR_KS2_XY_FAR_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KS2_XY_FAR_180_EDITED_wav, BinaryData::IR_KS2_XY_FAR_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 3 ) &&(DistID ==1))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KS2_MS_NEAR_0_EDITED_wav, BinaryData::IR_KS2_MS_NEAR_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KS2_MS_NEAR_180_EDITED_wav, BinaryData::IR_KS2_MS_NEAR_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 3 ) &&(DistID ==2))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KS2_MS_MED_0_EDITED_wav, BinaryData::IR_KS2_MS_MED_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KS2_MS_MED_180_EDITED_wav, BinaryData::IR_KS2_MS_MED_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 3 ) &&(DistID ==3))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KS2_MS_FAR_0_EDITED_wav, BinaryData::IR_KS2_MS_FAR_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KS2_MS_FAR_180_EDITED_wav, BinaryData::IR_KS2_MS_FAR_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
    }
    else if (RoomID == 2)
    {
        if ((MicID == 1 ) &&(DistID ==1))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KS3_AB_NEAR_0_EDITED_wav, BinaryData::IR_KS3_AB_NEAR_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KS3_AB_NEAR_180_EDITED_wav, BinaryData::IR_KS3_AB_NEAR_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 1 ) &&(DistID ==2))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KS3_AB_MED_0_EDITED_wav, BinaryData::IR_KS3_AB_MED_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KS3_AB_MED_180_EDITED_wav, BinaryData::IR_KS3_AB_MED_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            
        }
        else if ((MicID == 1 ) &&(DistID ==3))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KS3_AB_FAR_0_EDITED_wav, BinaryData::IR_KS3_AB_FAR_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KS3_AB_FAR_180_EDITED_wav, BinaryData::IR_KS3_AB_FAR_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 2 ) &&(DistID ==1))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KS3_XY_NEAR_0_EDITED_wav, BinaryData::IR_KS3_XY_NEAR_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KS3_XY_NEAR_180_EDITED_wav, BinaryData::IR_KS3_XY_NEAR_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 2 ) &&(DistID ==2))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KS3_XY_MED_0_EDITED_wav, BinaryData::IR_KS3_XY_MED_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KS3_XY_MED_180_EDITED_wav, BinaryData::IR_KS3_XY_MED_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 2 ) &&(DistID ==3))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KS3_XY_FAR_0_EDITED_wav, BinaryData::IR_KS3_XY_FAR_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KS3_XY_FAR_180_EDITED_wav, BinaryData::IR_KS3_XY_FAR_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 3 ) &&(DistID ==1))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KS3_MS_NEAR_0_EDITED_wav, BinaryData::IR_KS3_MS_NEAR_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KS3_MS_NEAR_180_EDITED_wav, BinaryData::IR_KS3_MS_NEAR_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 3 ) &&(DistID ==2))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KS3_MS_MED_0_EDITED_wav, BinaryData::IR_KS3_MS_MED_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KS3_MS_MED_180_EDITED_wav, BinaryData::IR_KS3_MS_MED_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 3 ) &&(DistID ==3))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KS3_MS_FAR_0_EDITED_wav, BinaryData::IR_KS3_MS_FAR_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KS3_MS_FAR_180_EDITED_wav, BinaryData::IR_KS3_MS_FAR_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
    }
    else if (RoomID == 3)
    {
        if ((MicID == 1 ) &&(DistID ==1))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KASKADE_AB_NEAR_0_EDITED_wav, BinaryData::IR_KASKADE_AB_NEAR_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KASKADE_AB_NEAR_180_EDITED_wav, BinaryData::IR_KASKADE_AB_NEAR_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 1 ) &&(DistID ==2))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KASKADE_AB_MED_0_EDITED_wav, BinaryData::IR_KASKADE_AB_MED_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KASKADE_AB_MED_180_EDITED_wav, BinaryData::IR_KASKADE_AB_MED_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            
        }
        else if ((MicID == 1 ) &&(DistID ==3))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KASKADE_AB_FAR_0_EDITED_wav, BinaryData::IR_KASKADE_AB_FAR_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KASKADE_AB_FAR_180_EDITED_wav, BinaryData::IR_KASKADE_AB_FAR_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 2 ) &&(DistID ==1))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KASKADE_XY_NEAR_0_EDITED_wav, BinaryData::IR_KASKADE_XY_NEAR_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KASKADE_XY_NEAR_180_EDITED_wav, BinaryData::IR_KASKADE_XY_NEAR_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 2 ) &&(DistID ==2))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KASKADE_XY_MED_0_EDITED_wav, BinaryData::IR_KASKADE_XY_MED_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KASKADE_XY_MED_180_EDITED_wav, BinaryData::IR_KASKADE_XY_MED_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 2 ) &&(DistID ==3))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KASKADE_XY_FAR_0_EDITED_wav, BinaryData::IR_KASKADE_XY_FAR_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KASKADE_XY_FAR_180_EDITED_wav, BinaryData::IR_KASKADE_XY_FAR_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 3 ) &&(DistID ==1))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KASKADE_MS_NEAR_0_EDITED_wav, BinaryData::IR_KASKADE_MS_NEAR_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KASKADE_MS_NEAR_180_EDITED_wav, BinaryData::IR_KASKADE_MS_NEAR_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 3 ) &&(DistID ==2))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KASKADE_MS_MED_0_EDITED_wav, BinaryData::IR_KASKADE_MS_MED_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KASKADE_MS_MED_180_EDITED_wav, BinaryData::IR_KASKADE_MS_MED_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 3 ) &&(DistID ==3))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_KASKADE_MS_FAR_0_EDITED_wav, BinaryData::IR_KASKADE_MS_FAR_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_KASKADE_MS_FAR_180_EDITED_wav, BinaryData::IR_KASKADE_MS_FAR_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
    }
    else if (RoomID == 4)
    {
        if ((MicID == 1 ) &&(DistID ==1))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_TURM_AB_NEAR_0_EDITED_wav, BinaryData::IR_TURM_AB_NEAR_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_TURM_AB_NEAR_180_EDITED_wav, BinaryData::IR_TURM_AB_NEAR_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 1 ) &&(DistID ==2))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_TURM_AB_MED_0_EDITED_wav, BinaryData::IR_TURM_AB_MED_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_TURM_AB_MED_180_EDITED_wav, BinaryData::IR_TURM_AB_MED_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            
        }
        else if ((MicID == 1 ) &&(DistID ==3))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_TURM_AB_FAR_0_EDITED_wav, BinaryData::IR_TURM_AB_FAR_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_TURM_AB_FAR_180_EDITED_wav, BinaryData::IR_TURM_AB_FAR_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 2 ) &&(DistID ==1))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_TURM_XY_NEAR_0_EDITED_wav, BinaryData::IR_TURM_XY_NEAR_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_TURM_XY_NEAR_180_EDITED_wav, BinaryData::IR_TURM_XY_NEAR_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 2 ) &&(DistID ==2))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_TURM_XY_MED_0_EDITED_wav, BinaryData::IR_TURM_XY_MED_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_TURM_XY_MED_180_EDITED_wav, BinaryData::IR_TURM_XY_MED_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 2 ) &&(DistID ==3))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_TURM_XY_FAR_0_EDITED_wav, BinaryData::IR_TURM_XY_FAR_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_TURM_XY_FAR_180_EDITED_wav, BinaryData::IR_TURM_XY_FAR_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 3 ) &&(DistID ==1))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_TURM_MS_NEAR_0_EDITED_wav, BinaryData::IR_TURM_MS_NEAR_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_TURM_MS_NEAR_180_EDITED_wav, BinaryData::IR_TURM_MS_NEAR_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 3 ) &&(DistID ==2))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_TURM_MS_MED_0_EDITED_wav, BinaryData::IR_TURM_MS_MED_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_TURM_MS_MED_180_EDITED_wav, BinaryData::IR_TURM_MS_MED_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
        else if ((MicID == 3 ) &&(DistID ==3))
        {
            audioProcessor.Faltung.loadImpulseResponse(BinaryData::IR_TURM_MS_FAR_0_EDITED_wav, BinaryData::IR_TURM_MS_FAR_0_EDITED_wavSize , juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
            audioProcessor.FaltungOmni.loadImpulseResponse(BinaryData::IR_TURM_MS_FAR_180_EDITED_wav, BinaryData::IR_TURM_MS_FAR_180_EDITED_wavSize, juce::dsp::Convolution::Stereo::yes, juce::dsp::Convolution::Trim::no, 0);
        }
    }
   
}
