#include "TableCreator.h"

#include "VUtils/Math.h"
#include "VeNo/Core/Config.h"
#include "VeNo/Utils/Math.h"
#include "WaveLib.h"

#include <cassert>

namespace VeNo::Audio {

WaveTable *TableCreator::CreateWaves(RawTable &rawTable) {
  auto *table = new WaveTable();
  if (rawTable.IsNonImpulse) {
    CreateTableRaw(rawTable, table);
    return table;
  }
  FillTables(rawTable, table);
  return table;
}


void TableCreator::CreateTable(
    int waveTableIndex, const std::function<void(RawTable *)> &generateWave) {
  RawTable table;
  generateWave(&table);
  auto &waveLib = WaveLib::Get();
  waveLib.SetWaveTable(waveTableIndex, CreateWaves(table));
}

void TableCreator::FillTables(RawTable &rawTable, WaveTable *waveTable) {
  auto &freqWaveRe = rawTable.Data;
  unsigned int maxHarmonic = RawTable::TableLength >> 1u;
  const double minValue = 0.000001;
  while (std::fabs(freqWaveRe[maxHarmonic]) < minValue && maxHarmonic)
    --maxHarmonic;
  double topFreq = 2.0 / 3.0 / maxHarmonic;
  size_t size = 0;
  size_t maxHarmonicTemps = maxHarmonic;
  // This should create enough Headroom...
  while (maxHarmonicTemps > 4) {
    size++;
    maxHarmonicTemps >>= 1u;
  }
  size = std::max(size, static_cast<size_t>(1));
  waveTable->Waves = new Wave[size]{};
  waveTable->Length = size;
  for (size_t i = 0; i < size; ++i) {
    auto &wave = waveTable->Waves[i];
    wave.TopFreq = topFreq;
    auto len = RawTable::TableLength;
    auto tempData = std::array<double, RawTable::TableLength+1>{};
    size_t numSamples = len;
    tempData[0] = 0;
    tempData[numSamples] = 0;
    for (size_t x = 1; x <= maxHarmonic; ++x) {
      tempData[x] = rawTable.Data[x];
      tempData[numSamples - x] = rawTable.Data[numSamples - x];
    }
    MakeWaveTable(wave, topFreq, tempData.data());
    topFreq *= 2.0;
    maxHarmonic >>= 1u;
  }
}

void TableCreator::CreateTableRaw(RawTable &rawTable, WaveTable *waveTable) {
  auto &config = Core::Config::get();
  waveTable->Waves = new Wave[1];
  waveTable->Length = 1;
  auto &wave = waveTable->Waves[0];
  wave.TopFreq = config.sampleRate / 2.0;
  for (size_t x = 0; x < RawTable::TableLength; x++)
    wave.Data[x] = static_cast<float>(rawTable.Data[x]);
  wave.Data[RawTable::TableLength] = wave.Data[0];
}

void TableCreator::MakeWaveTable(Wave &wave, double topFreq, double *tempData) {
  FFT(RawTable::TableLength, tempData, wave.Data.data());
  //ApplyCutoff(wave, topFreq);
  auto scale = static_cast<float>(FindScale(wave));
  for (size_t idx = 0; idx < RawTable::TableLength; idx++)
    wave.Data[idx] = wave.Data[idx] * scale;
  wave.Data[RawTable::TableLength] = wave.Data[0];
}

void TableCreator::FFT(int N, double *ar, float *ai) {
  int NV2 = N >> 1;
  int NM1 = N - 1;
  int TEMP = N;
  int M = 0;
  while (TEMP >>= 1)
    ++M;

  int j = 1;
  int i;
  double t;
  for (i = 1; i <= NM1; i++) {
    if (i < j) {
      t = ar[j - 1];
      ar[j - 1] = ar[i - 1];
      ar[i - 1] = t;
      t = ai[j - 1];
      ai[j - 1] = ai[i - 1];
      ai[i - 1] = t;
    }

    int k = NV2;
    while (k < j) {
      j -= k;
      k /= 2;
    }

    j += k;
  }

  int LE = 1;
  for (int L = 1; L <= M; L++) {
    int LE1 = LE;
    LE *= 2;
    double Ur = 1.0;
    double Ui = 0.;
    double Wr = std::cos(V_PI / LE1);
    double Wi = -std::sin(V_PI / LE1);
    for (j = 1; j <= LE1; j++) {
      for (i = j; i <= N; i += LE) {
        int ip = i + LE1;
        double Tr = ar[ip - 1] * Ur - ai[ip - 1] * Ui;
        double Ti = ar[ip - 1] * Ui + ai[ip - 1] * Ur;
        ar[ip - 1] = ar[i - 1] - Tr;
        ai[ip - 1] = ai[i - 1] - Ti;
        ar[i - 1] = ar[i - 1] + Tr;
        ai[i - 1] = ai[i - 1] + Ti;
      }
      double Ur_old = Ur;
      Ur = Ur_old * Wr - Ui * Wi;
      Ui = Ur_old * Wi + Ui * Wr;
    }
  }
}

double TableCreator::FindScale(Wave &wave) {
  double max = 0;
  for (size_t idx = 0; idx < RawTable::TableLength; idx++) {
    if (const double temp = std::fabs(wave.Data[idx]); max < temp)
      max = temp;
  }
  return 1.0 / max * .999;
}

void TableCreator::ApplyCutoff(Wave &wave, double topFreq) {
  const double sRate = Core::Config::get().sampleRate;
  const double cutOff = topFreq * (sRate / 2) * 1.2;
  const double RC = 0.9 / (cutOff * 2 * V_PI);
  const double dt = 1.0 / sRate;
  const auto alpha = static_cast<float>(dt / (RC + dt));

  float prev_sample = 0.0;
  constexpr auto length = RawTable::TableLength;
  for (size_t i = 0; i < length; ++i) {
    const float tmp = wave.Data[i];
    wave.Data[i] = prev_sample + (alpha * (wave.Data[i] - prev_sample));
    prev_sample = tmp;
  }
}
} // namespace VeNo::Audio