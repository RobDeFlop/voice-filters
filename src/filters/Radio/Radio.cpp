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
    update_filter_freq(m_settings.freq_low, m_settings.freq_hi, m_last_eq_in, f);
    update_filter_freq(m_settings.o_freq_lo, m_settings.freq_hi, m_last_eq_out, f_o);

    const auto fudge = static_cast<float>(m_settings.fudge);
    constexpr auto frame_count = 256;

    float* audioData[6];
    for(int i = 0; i<6; i++){
        if(data->channels.left[i])
    }
}