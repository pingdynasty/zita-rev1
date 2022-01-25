#ifndef __ZitaRev1Patch_h__
#define __ZitaRev1Patch_h__

#include "Patch.h"
#include "ScaledValue.h"
#include "ZitaRev1StereoProcessor.h"

class ZitaRev1Patch : public Patch {
  ZitaRev1StereoProcessor* reverb;
  AudioBuffer* input;
  //   min,    max,  init, param
  //  0.02,  0.100,  0.04, R_DELAY
  //  50.0, 1000.0, 200.0, R_XOVER
  //   1.0,    8.0,   3.0, R_RTLOW
  //   1.0,    8.0,   2.0, R_RTMID
  // 1.5e3, 24.0e3, 6.0e3, R_FDAMP
  //  40.0,  2.5e3, 160.0, R_EQ1FR
  // -15.0,   15.0,   0.0, R_EQ1GN
  // 160.0,   10e3, 2.5e3, R_EQ2FR
  // -15.0,   15.0,   0.0, R_EQ2GN
  //   0.0,    1.0,   0.5, R_OPMIX
  //  -9.0,    9.0,   0.0, R_RGXYZ
  LinearFloat delay = LinearFloat(0.02, 0.100, 0.04);
  ExponentialFloat xover = ExponentialFloat(50, 1000, 200);
  LinearFloat rtlow = LinearFloat(1, 8, 3);
  LinearFloat rtmid = LinearFloat(1, 8, 2);
  LinearFloat fdamp = LinearFloat(1.5e3, 24.0e3, 6.0e3);
  ExponentialFloat eq1fr = ExponentialFloat(40.0, 2.5e3, 160.0);
  LinearFloat eq1gn = LinearFloat(-15.0, 15.0, 0.0);
  ExponentialFloat eq2fr = ExponentialFloat(160.0, 10e3, 2.5e3);
  LinearFloat eq2gn = LinearFloat(-15.0, 15.0, 0.0);
public:
  ZitaRev1Patch(){
    reverb = ZitaRev1StereoProcessor::create(getSampleRate());
    registerParameter(PARAMETER_A, "RT60 Low");
    registerParameter(PARAMETER_B, "RT60 Mid");
    registerParameter(PARAMETER_C, "HF Damping");
    registerParameter(PARAMETER_D, "Mix");
    registerParameter(PARAMETER_AA, "Delay");
    registerParameter(PARAMETER_AB, "Crossover Freq");
    registerParameter(PARAMETER_AC, "EQ1 Freq");
    registerParameter(PARAMETER_AD, "EQ1 Gain");
    registerParameter(PARAMETER_AE, "EQ2 Freq");
    registerParameter(PARAMETER_AF, "EQ2 Gain");
    setParameterValue(PARAMETER_AA, delay.getControl());
    setParameterValue(PARAMETER_AB, xover.getControl());
    setParameterValue(PARAMETER_AC, eq1fr.getControl());
    setParameterValue(PARAMETER_AD, eq1gn.getControl());
    setParameterValue(PARAMETER_AE, eq2fr.getControl());
    setParameterValue(PARAMETER_AF, eq2gn.getControl());
    input = AudioBuffer::create(2, getBlockSize());
  }
  ~ZitaRev1Patch(){
    ZitaRev1StereoProcessor::destroy(reverb);
    AudioBuffer::destroy(input);
  }
  void processAudio(AudioBuffer &buffer){
    input->copyFrom(buffer); // need to make a copy for dry/wet mix to work
    delay = getParameterValue(PARAMETER_AA);
    xover = getParameterValue(PARAMETER_AB);
    rtlow = getParameterValue(PARAMETER_A);
    rtmid = getParameterValue(PARAMETER_B);
    fdamp = getParameterValue(PARAMETER_C);
    eq1fr = getParameterValue(PARAMETER_AC);
    eq1gn = getParameterValue(PARAMETER_AD);
    eq2fr = getParameterValue(PARAMETER_AE);
    eq2gn = getParameterValue(PARAMETER_AF);
    reverb->setDelay(delay);
    reverb->setCrossover(xover);
    reverb->setRT60Low(rtlow);
    reverb->setRT60Mid(rtmid);
    reverb->setHFDamping(fdamp);
    reverb->setEq1(eq1fr, eq1gn);
    reverb->setEq2(eq2fr, eq2gn);
    reverb->setOutputMix(getParameterValue(PARAMETER_D));
    reverb->process(*input, buffer);
    debugMessage("delay/xover", delay, xover);
  }
};

#endif // __ZitaRev1Patch_h__
