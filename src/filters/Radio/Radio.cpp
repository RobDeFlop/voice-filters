#include "Radio.h"

using namespace alt;

void Radio::SetParam(const size_t &key, bool value) {
  if (key == hash("noSettings")) {
    isDefault = value;
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

void Radio::GetParam(const size_t &key, bool &value) {
  if (key == hash("noSettings"))
    value = isDefault;
}

void Radio::Process(pcmSample *data, uint32_t size) {
  Init();
  UpdateFilterFreq(m_settings.freq_low, m_settings.freq_hi, m_last_eq_in, f);
  UpdateFilterFreq(m_settings.o_freq_lo, m_settings.freq_hi, m_last_eq_out,
                   f_o);

  const auto fudge = static_cast<float>(m_settings.fudge);
  constexpr auto frameCount = 256;

  // TODO:: audiodata needs to be data->sample
  auto audioData = static_cast<float>(data->sample);

  std::cout << "data->sample" << data->sample << std::endl;
  std::cout << "audioData: " << audioData << std::endl;

  // TODO:: channel with data->sample. Create AudioSampleBuffer or directly pass
  // the values thorught the process method
  //  float* channels[2];
  //  channels[0] = bufferToFill.buffer->getArrayOfChannels()[0] +
  //  bufferToFill.startSample;
  //  channels[1] = bufferToFill.buffer->getArrayOfChannels()[1] + bufferToFill.startSample;
  //
  //  AudioSampleBuffer buffer (channels,
  //                           bufferToFill.buffer->getNumChannels(),
  //                           bufferToFill.numSamples);
  //
  //  m_filter->process (buffer.getNumSamples(), buffer.getArrayOfChannels());
  //  f->process()

  // f->process((int)size, &audioData);
  //
  //   for (int i = 0; i < sizeof(audioData); i++) {
  //     DoProcessRingMod(&audioData[i], frameCount, m_rm_mod_angel[i]);
  //     if (fudge > 0.0f) {
  //       DoProcess(&audioData[i], frameCount, m_vol_follow[i]);
  //     }
  //   }
  //
  //   f_o->process(frameCount, reinterpret_cast<float *const *>(audioData));

  // data->channels.left = static_cast<int16_t>(audioData[0]);
  // data->channels.right = static_cast<int16_t>(audioData[1]);
}

bool Radio::Init() {
  Dsp::Params params{};
  params[0] = 44100;
  params[1] = 4;
  params[2] = 1600;
  params[3] = 1300;

  f->setParams(params);
  f_o->setParams(params);

  memset(m_rm_mod_angel, 0, sizeof(m_rm_mod_angel));
  memset(m_vol_follow, 0, sizeof(m_vol_follow));

  return true;
}

void Radio::UpdateFilterFreq(double lowFreq, double highFreq,
                             std::pair<double, double> &last,
                             const std::unique_ptr<Dsp::Filter> &filter) {
  if (last.first == lowFreq && last.second == highFreq) {
    return;
  }

  const auto new_center_freq = CalculateCenterFreq(lowFreq, highFreq);
  const auto new_bandwidth = CalculateBandwidth(lowFreq, highFreq);

  if (filter) {
    filter->setParam(2, new_center_freq);
    filter->setParam(3, new_bandwidth);
  }

  last.first = lowFreq;
  last.second = highFreq;
}

double Radio::CalculateCenterFreq(double lowFreq, double highFreq) {
  return lowFreq + (CalculateBandwidth(lowFreq, highFreq) / 2.0);
}

double Radio::CalculateBandwidth(double lowFreq, double highFreq) {
  return highFreq - lowFreq;
}
void Radio::DoProcess(float *samples, int frame_count, float &volNext) {
  const auto fudge = static_cast<float>(m_settings.fudge);
  float vol = 0.0f;

  for (int i = 0; i < frame_count; i++) {
    vol += (samples[i] * samples[i]);
  }

  vol /= (float)frame_count;
  vol *= fudge;

  volNext = volNext * 0.5f + vol * 0.5f;

  float randomNumber = (((float)(rand() & 32767)) / 16384.0f) - 1.0f;
  int count = (rand() & 127) + 1;

  float _temp;
  for (int i = 0; i < frame_count; i++) {
    if (!count--) {
      randomNumber = (((float)(rand() & 32767)) / 16384.0f) - 1.0f;
      count = (rand() & 127) + 1;
    }
    // Add random to inputs * by current volume;
    _temp = samples[i] + randomNumber * volNext;

    // Make it an integer between -60 and 60
    _temp = static_cast<int>(_temp * 40.0f);

    // Drop it back down but massively quantised and too high
    _temp = (_temp / 40.0f);
    _temp *= 0.05 * (float)m_settings.fudge;
    _temp += samples[i] * (1 - (0.05 * fudge));
    samples[i] = std::clamp(-1.0f, _temp, 1.0f);
  }
}

void Radio::DoProcessRingMod(float *samples, int frame_count, double &angle) {
  const auto rm_mod_freq = m_settings.rm_mod_freq;
  const auto rm_mix = m_settings.rm_mix;

  if ((rm_mod_freq != 0.0f) && (rm_mix != 0.0f)) {
    for (int i = 0; i < frame_count; ++i) {
      float sample = samples[i];
      sample = (sample * (1 - rm_mix)) + (rm_mix * (sample * sin(angle)));
      samples[i] = std::clamp(-1.0f, sample, 1.0f);
      angle += rm_mod_freq * TWO_PI_OVER_SAMPLE_RATE;
    }
  }
}
