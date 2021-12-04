struct RadioSettings {
    RadioSettings() = default;

    bool enabled = false;
    double freq_low = 0.0;
    double freq_hi = 0.0;
    double fudge = 0.0;
    double rm_mod_freq = 0.0;
    double rm_mix = 0.0;
    double o_freq_lo = 0.0;
    double o_freq_hi = 0.0;
};