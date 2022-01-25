#ifndef __ZitaRev1StereoProcessor_h__
#define __ZitaRev1StereoProcessor_h__

#include "SignalProcessor.h"
#include "reverb.h"

/**
zita-rev1 is a digital reverb audio effect developed by Fons Adriaensen at Kokkini Zita.
Ref: https://kokkinizita.linuxaudio.org/linuxaudio/zita-rev1-doc/quickguide.html

REV1 is a reworked version of the reverb originally developed for Aeolus. Its character is more 'hall' than 'plate', but it can be used on a wide variety of instruments or voices. It is not a spatialiser - the early reflections are different for the L and R inputs, but do not correspond to any real room. They have been tuned to match left and right sources to some extent.

* Delay. A delay of 20 to 100 ms operating on the 'wet' signal. Large values will provide the impression of a larger room.

* Reverb time controls. The reverb time (RT60) can be set at low and mid frequencies in the range of 1 to 8 seconds. The range affected by the 'Low' control is determined by the frequency setting to the left of it. At high frequencies the reverb time will decrease, this is controlled by the 'HF damping' control. The value set is the frequency at which the reverb time will be half the mid-frequency value.

* Equaliser. Two parametric filter sections can be used to change the character of the reverb. The operate on the 'wet' signal only of course. The bandwidths are fixed, and somewhat higher than the medium setting of a typical equaliser.

* Dry/wet mix. This is provided in stereo mode only. When the reverb is used in send/return mode this should be set to the full 'wet' position.

* XYZ gain. This is provided in ambisonic mode only, and controls the relative gain of the first order signals. This can be used to change the 'spatiality', and also to adapt the output to the normalisation standard used downstream.

The EQ and output controls are 'dezippered'. The others are not and may (but usually don't) cause side effects when modified.
*/
class ZitaRev1StereoProcessor : public MultiSignalProcessor {
protected:
  Reverb reverb;
public:
  ZitaRev1StereoProcessor(float sr){
    reverb.init(sr, false);
  }
  void setDelay(float value){
    reverb.set_delay(value);
  }
  void setCrossover(float value){
    reverb.set_xover(value);
  }
  void setRT60Low(float value){  
    reverb.set_rtlow(value);
  }
  void setRT60Mid(float value){
    reverb.set_rtmid(value);
  }
  void setHFDamping(float value){
    reverb.set_fdamp(value);
  }
  void setOutputMix(float mix){
    reverb.set_opmix(mix);
  }
  void setEq1(float freq, float gain){
    reverb.set_eq1(freq, gain);    
  }
  void setEq2(float freq, float gain){
    reverb.set_eq2(freq, gain);    
  }
  /**
   * Process a stereo or mono input into a stereo output.
   * Note that if the input buffer is also the output (process replacing), then
   * the 'output mix' works as an output gain control instead of dry/wet mix.
   */
  void process(AudioBuffer& input, AudioBuffer& output){
    size_t len = output.getSize();
    float* in[2] = {input.getSamples(0), input.getSamples(1)};
    float* out[2] = {output.getSamples(0), output.getSamples(1)};
    reverb.prepare(len);
    reverb.process(len, in, out);
  }
  static ZitaRev1StereoProcessor* create(float sr){
    return new ZitaRev1StereoProcessor(sr);
  }
  static void destroy(ZitaRev1StereoProcessor* obj){
    return delete obj;
  }
};

#endif /* __ZitaRev1StereoProcessor_h__ */
