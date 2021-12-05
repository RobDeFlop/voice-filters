#include "Radio.h"


using namespace alt;

void Radio::SetParam(const size_t &key, bool value) {
    if(key == hash("default")){
        m_settings.enabled = true;
        m_settings.freq_low = 389.0;
        m_settings.freq_hi = 3248.0;
        m_settings.fudge = 0.0;
        m_settings.rm_mod_freq = 0.0;
        m_settings.rm_mix = 0.16;
        m_settings.o_freq_lo = 348.0;
        m_settings.o_freq_hi = 4900.0;
    }
}

void Radio::Process(pcmSample *data, uint32_t size) {
    updateFilterFreq(m_settings.freq_low, m_settings.freq_hi, m_last_eq_in, f);
    updateFilterFreq(m_settings.o_freq_lo, m_settings.freq_hi, m_last_eq_out, f_o);

    const auto fudge = static_cast<float>(m_settings.fudge);
    constexpr auto frameCount = 256;

    //audiodata may needs to be data->sample
    float audioData[2];
    audioData[0] = static_cast<float>(data->channels.left);
    audioData[1] = static_cast<float>(data->channels.right);

    // process the audiodata per frame : f->process(frameCount, audioData), f_o->process(frameCount, audioData)


}