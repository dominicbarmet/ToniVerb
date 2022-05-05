#include "PluginProcessor.h"
#include "PluginEditor.h"
//=================================================================
ToniVerb_AudioProcessor::ToniVerb_AudioProcessor():

        valueTreeState(*this, nullptr, "Parameters", createParameters()),
        lowCutFilter(juce::dsp::IIR::Coefficients<float>::makeHighPass(48000, 20.f)),
        hiCutFilter(juce::dsp::IIR::Coefficients<float>::makeLowPass(48000, 20000.f))

#ifndef JucePlugin_PreferredChannelConfigurations

     : AudioProcessor
        
(BusProperties()
                #if ! JucePlugin_IsMidiEffect
                #if ! JucePlugin_IsSynth
                       .withInput  ("Input",
                                    juce::AudioChannelSet::stereo(),
                                    true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                      ),
#endif
{
}

ToniVerb_AudioProcessor::~ToniVerb_AudioProcessor(){}
//=================================================================
const juce::String ToniVerb_AudioProcessor::getName() const{return JucePlugin_Name;}
bool ToniVerb_AudioProcessor::acceptsMidi() const{return false;}
bool ToniVerb_AudioProcessor::producesMidi() const{return false;}
bool ToniVerb_AudioProcessor::isMidiEffect() const{return false;}
double ToniVerb_AudioProcessor::getTailLengthSeconds() const {return 0.0;}
//==========================Programs=======================//
int ToniVerb_AudioProcessor::getNumPrograms(){return 1;}
int ToniVerb_AudioProcessor::getCurrentProgram(){return 0;}
void ToniVerb_AudioProcessor::setCurrentProgram (int index){}
const juce::String ToniVerb_AudioProcessor::getProgramName (int index){return "programName";}
void ToniVerb_AudioProcessor::changeProgramName (int index, const juce::String& newName){}
//==============================================================================

void ToniVerb_AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{DBG("Prepare to play has been called");
    lastSampleRate = sampleRate;
    //Startparameter
    lastFreqLowCut = 0.f;
    lastFreqHiCut = 0.f;
    
    lastMixProportion =0.f;
    initialDel.reset(sampleRate, 0.6f);
    //Metering-Setup
    LeftPre.reset(sampleRate, 0.6f);
    RightPre.reset(sampleRate, 0.6f);
    LeftPost.reset(sampleRate, 0.6f);
    RightPost.reset(sampleRate, 0.6f); //Smoothingzeit in S
    
    LeftPre.setCurrentAndTargetValue(-60.f);
    RightPre.setCurrentAndTargetValue(-60.f);
    LeftPost.setCurrentAndTargetValue(-60.f);
    RightPost.setCurrentAndTargetValue(-60.f); //Zielwert -60dB
    
    //dsp-Setup
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    Faltung.prepare(spec);
    Faltung.reset();
    FaltungOmni.prepare(spec);
    FaltungOmni.reset();
    
   
    lowCutFilter.prepare(spec);
    lowCutFilter.reset();
    hiCutFilter.prepare(spec);
    hiCutFilter.reset();
    
    initialDelay.setMaximumDelayInSamples(10000.f);
    initialDelay.prepare(spec);
    initialDelay.reset();
}

void ToniVerb_AudioProcessor::releaseResources()
{
    DBG("Release Ressources has been called");
    // When playback stops, you can use this as an opportunity to free up any spare memory, etc.
}
#ifndef JucePlugin_PreferredChannelConfigurations
bool ToniVerbV3AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported. In this template code we only support mono or stereo. Some plugin hosts, such as certain GarageBand versions, will only load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif
        return true;
  #endif
}
#endif
//=================================================================================
//=================================================================================
//=================================================================================
void ToniVerb_AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
    buffer.clear (i, 0, buffer.getNumSamples());
    omniSignal.clear(i, 0, omniSignal.getNumSamples());
    drySignal.clear(i, 0, drySignal.getNumSamples());
    }
    //==================================================================================================//
    auto inputVolumeValue = valueTreeState.getRawParameterValue("INPUT_VOLUME");
    buffer.applyGain(pow(10,inputVolumeValue->load()/20));
    juce::dsp::AudioBlock<float> block (buffer);
    drySignal.makeCopyOf(buffer);
    juce::dsp::AudioBlock<float> dryBlock (drySignal);

    updateFilter();
    //=====================================INPUT METERING=====================================================//
       
    LeftPre.skip(buffer.getNumSamples());
    RightPre.skip(buffer.getNumSamples());
        {
       const auto value = juce::Decibels::gainToDecibels(buffer.getMagnitude(0, 0, buffer.getNumSamples()));
            if (value < LeftPre.getCurrentValue()) LeftPre.setTargetValue(value);
            else LeftPre.setCurrentAndTargetValue(value);}
        {
        const auto value = juce::Decibels::gainToDecibels(buffer.getMagnitude(1, 0, buffer.getNumSamples()));
            if (value < RightPre.getCurrentValue()) RightPre.setTargetValue(value);
            else RightPre.setCurrentAndTargetValue(value);}
    
    //======================================================================================================//

    //=====================================================================================================//
    
    auto muteWetValue = valueTreeState.getRawParameterValue("MUTE_WET");
    
    if (muteWetValue->load()== true)
    {
        buffer.applyGain(0.f);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            if (initialDelay.getDelay() != initialDel.getNextValue())
            {
            initialDelay.setDelay(initialDel.getNextValue());
            }
        }
    }
    else
        {
            lowCutFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
            hiCutFilter.process(juce::dsp::ProcessContextReplacing<float>(block));
        
            omniSignal.makeCopyOf(buffer);
            juce::dsp::AudioBlock<float> omniBlock (omniSignal);

        //=====================================================================================================//
             Faltung.process(juce::dsp::ProcessContextReplacing<float>(block));
             FaltungOmni.process(juce::dsp::ProcessContextReplacing<float> (omniBlock));
            float* channelDataL = buffer.getWritePointer(0); float* channelDataR = buffer.getWritePointer(1);
            
            auto characterValue = valueTreeState.getRawParameterValue("CHARACTER");
            omniBlock.multiplyBy((characterValue->load())/100.f);

            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                auto widthValue = valueTreeState.getRawParameterValue("WIDTH");
                
                //M=(L+R)/2
                float mid = ((buffer.getSample(0, sample)+omniSignal.getSample(0, sample))+(buffer.getSample(1, sample)+omniSignal.getSample(1, sample)))/2;
                //S=(L-R)/2
                float side =(((buffer.getSample(0, sample)+omniSignal.getSample(0, sample))-(buffer.getSample(1, sample)+omniSignal.getSample(1, sample)))/2)*((widthValue->load()/100.f));
                
                float left = mid + side; float right = mid - side;
                
                auto wetVolumeValue = valueTreeState.getRawParameterValue("WET_VOLUME");
                float rightsum = right*(pow(10,wetVolumeValue->load()/20));//*(1-(characterValue->load()/300.f));
                float leftsum = left*(pow(10,wetVolumeValue->load()/20));//*(1-(characterValue->load()/300.f));
               
                auto preDelayValue = valueTreeState.getRawParameterValue("PREDELAY");
                initialDel.setTargetValue(preDelayValue->load());
                
                if (initialDelay.getDelay() != initialDel.getNextValue())
                {
                initialDelay.setDelay(initialDel.getNextValue());
                }
                
                initialDelay.pushSample(0, leftsum);
                initialDelay.pushSample(1, rightsum);
                channelDataL[sample]= initialDelay.popSample(0);
                channelDataR[sample]= initialDelay.popSample(1);
            }
        }
    //=====================================================================================================//
    
    //=====================================================================================================//
    
    
    {
        auto dryVolumeValue = valueTreeState.getRawParameterValue("DRY_VOLUME");
        dryBlock.multiplyBy(pow(10,dryVolumeValue->load()/20));
        
        auto muteDryValue = valueTreeState.getRawParameterValue("MUTE_DRY");
        
        if (muteDryValue->load() == true)
        {
            dryBlock.multiplyBy(0.f);
        }
            
        float* channelDataL = buffer.getWritePointer(0); float* channelDataR = buffer.getWritePointer(1);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                channelDataL[sample]= (buffer.getSample(0, sample)+ drySignal.getSample(0, sample));
                channelDataR[sample]= (buffer.getSample(1, sample) + drySignal.getSample(1,sample));
            }
    }
    //=====================================================================================================//
       
    LeftPost.skip(buffer.getNumSamples()); RightPost.skip(buffer.getNumSamples());
        {
       const auto value = juce::Decibels::gainToDecibels(buffer.getMagnitude(0, 0, buffer.getNumSamples()));
            if (value < LeftPost.getCurrentValue())LeftPost.setTargetValue(value);
            else LeftPost.setCurrentAndTargetValue(value);}
        {
        const auto value = juce::Decibels::gainToDecibels(buffer.getMagnitude(1, 0, buffer.getNumSamples()));
            if (value < RightPost.getCurrentValue()) RightPost.setTargetValue(value);
            else RightPost.setCurrentAndTargetValue(value);}
}
//=======================================END PROCESS BLOCK========================================//

//====================================== Editor Loader ===================================//
bool ToniVerb_AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ToniVerb_AudioProcessor::createEditor()
{
    
    return new ToniVerb_AudioProcessorEditor(*this);
}
//==================================== Parameter Store =======================================//
void ToniVerb_AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block. You could do that either as raw data, or use the XML or ValueTree classes as intermediaries to make it easy to save and load complex data.
   DBG("getStateInformation has been called");
    auto state = valueTreeState.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary(*xml, destData);
}

void ToniVerb_AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block, whose contents will have been created by the getStateInformation() call.
    DBG("setStateInformation has been called");
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr) {
        if (xmlState->hasTagName(valueTreeState.state.getType())) {
            valueTreeState.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}
//======================================================================================//
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ToniVerb_AudioProcessor();
}
//=========================================UPDATE FILTER===========================//
void ToniVerb_AudioProcessor::updateFilter()
{
    auto freqLowCutValue = valueTreeState.getRawParameterValue("LOW_CUT_FREQ");
    float freqLowCut = (freqLowCutValue->load()); //abfrage nur wenn nötig? Old Value vergleichen und wenn nötig filter anpassen
 
    if (freqLowCut !=lastFreqLowCut)
    {
        *lowCutFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(lastSampleRate, freqLowCut);
        lastFreqLowCut = freqLowCut;
    }
    auto freqHighCutValue = valueTreeState.getRawParameterValue("HIGH_CUT_FREQ");
    float freqHiCut = (freqHighCutValue->load());
    
    if (freqHiCut != lastFreqHiCut) {
        *hiCutFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, freqHiCut);
        lastFreqHiCut =freqHiCut;
    }
}

//==========================Metering Parameterübergabe an Editor======================//
float ToniVerb_AudioProcessor::getMeteringValue(const int channel) const
{
    jassert(channel == 0|| channel == 1 || channel == 2 || channel == 3);
    if (channel == 0) return LeftPre.getCurrentValue();
    if (channel == 1) return RightPre.getCurrentValue();
    if (channel == 2) return LeftPost.getCurrentValue();
    if (channel == 3) return RightPost.getCurrentValue();
    return 0.f;
}

juce::AudioProcessorValueTreeState::ParameterLayout ToniVerb_AudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    params.push_back(std::make_unique<juce::AudioParameterFloat>("INPUT_VOLUME","Input Volume", -48.f, 6.f, 0.f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRY_VOLUME", "Dry Volume", -60.f, 12.f, -60.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("WET_VOLUME", "Wet Volume", -60.f, 12.f, 0.f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("HIGH_CUT_FREQ", "High Cut Frequency", 500.f, 20000.f, 20000.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("LOW_CUT_FREQ", "Low Cut Frequency", 20.f, 2000.f, 20.f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("WIDTH", "Width", 0.f, 200.f, 100.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CHARACTER", "Character", 0.f, 100.f, 0.f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PREDELAY", "Pre-Delay", 0.f, 10000.f, 0.f));
    
    params.push_back(std::make_unique<juce::AudioParameterBool>("MUTE_WET", "Mute Wet", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("MUTE_DRY", "Mute Dry", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("BYPASS", "Bypass", true));
    
    params.push_back(std::make_unique<juce::AudioParameterChoice>("ROOM","Room Selection", juce::StringArray("Konzertsaal 2", "Konzertsaal 3", "Kaskadenhalle", "Turm"), 2));
    
    params.push_back(std::make_unique<juce::AudioParameterChoice>("MIC_ID", "Microphone Selection", juce::StringArray("AB","XY","MS"), 3));
    
    params.push_back(std::make_unique<juce::AudioParameterChoice>("DISTANCE_ID", "Distance Selection", juce::StringArray("near","medium","far"), 1));
    
    return {params.begin(),params.end()};
}
