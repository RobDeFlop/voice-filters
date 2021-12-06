#pragma once

#include "main.h"
#include <memory>

#include "DspFilters/Dsp.h"
#include "RadioSettings.h"


class Radio : public alt::IAudioFilter {
public:
    Radio(){};
    ~Radio(){};

    void SetParam(const size_t &key, bool value) override;
    void GetParam(const size_t &key, bool &value) override;
    void Process(pcmSample *data, uint32_t size) override;

private:
    RadioSettings m_settings;
    bool isDefault = false;
    const double TWO_PI_OVER_SAMPLE_RATE = 2 * 3.141592653 / 441000;

    std::unique_ptr<Dsp::Filter> f = std::make_unique<Dsp::SmoothedFilterDesign<Dsp::Butterworth::Design::BandPass<4>, 2, Dsp::DirectFormII>>(1024);
    std::unique_ptr<Dsp::Filter> f_o = std::make_unique<Dsp::SmoothedFilterDesign<Dsp::Butterworth::Design::BandPass<4>, 2, Dsp::DirectFormII>>(1024);

    std::pair<double, double> m_last_eq_in;
    std::pair<double, double> m_last_eq_out;

    float m_vol_follow[2];
    double m_rm_mod_angel[2];

    bool Init();
    void UpdateFilterFreq(double lowFreq, double highFreq, std::pair<double, double>& last, const std::unique_ptr<Dsp::Filter>& filter);
    double CalculateCenterFreq(double lowFreq, double highFreq);
    double CalculateBandwidth(double lowFreq, double highFreq);
    void DoProcess(float* samples, int frame_count, float& volNext);
    void DoProcessRingMod(float* samples, int frame_count, double& angle);
};
