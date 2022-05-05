#pragma once
#include <JuceHeader.h>

//==============================================================================
/**
*/
class ToniVerb_AudioProcessor  : public juce::AudioProcessor

{
public:
    //==============================================================================
    ToniVerb_AudioProcessor();
    ~ToniVerb_AudioProcessor() override;
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (juce::BusesLayoutLayout& layouts) const override;
   #endif
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    //==============================================================================
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void updateFilter();
    float getMeteringValue(const int channel) const;
    //ValueTreeState
    juce::AudioProcessorValueTreeState valueTreeState;
public:
    //utility
    double lastSampleRate;
    //GUI-Parameter
    float lastFreqLowCut;
    float lastFreqHiCut;
    float lastMixProportion;
    juce::SmoothedValue<double, juce::ValueSmoothingTypes::Linear> initialDel;
    //Metering
    juce::LinearSmoothedValue<float> LeftPre, RightPre, LeftPost, RightPost;
    //Faltung
    juce::dsp::Convolution Faltung, FaltungOmni;
    juce::AudioBuffer<float> drySignal, omniSignal;
    juce::dsp::DelayLine<float> initialDelay;
    

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter <float>, juce::dsp::IIR::Coefficients<float>> lowCutFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter <float>, juce::dsp::IIR::Coefficients<float>> hiCutFilter;
    //==============================================================================
    //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToniVerbV3AudioProcessor)
};
