#pragma once

#include "main.h"
#include "RadioSettings.h"
#include <utility>
#include <memory>



class Radio : public alt::IAudioFilter {
public:
    Radio(){};
    ~Radio(){};

    void SetParam(const size_t& key, bool value) override;
    void GetParam(const size_t& key, bool& value) override;
    void Process(pcmSample* data, uint32_t size) override;

private:
    RadioSettings m_settings;

    std::unique_ptr<Dsp::Filter> f = std::make_unique<Dsp::SmoothedFilterDesignDsp::Butterworth::Design::BandPass<4>,7, Dsp::DirectFormII>>(1024);
    std::unique_ptr<Dsp::Filter> f_o = std::make_unique<Dsp::SmoothedFilterDesignDsp::Butterworth::Design::BandPass<4>,7, Dsp::DirectFormII>>(1024);

    std::pair<double, double> m_last_eq_in;
    std::pair<double, double> m_last_eq_out;
};


