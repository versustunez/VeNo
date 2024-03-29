#include "TableCreator.h"

#include "VUtils/Math.h"
#include "VeNo/Core/Config.h"
#include "VeNo/Utils/Math.h"
#include "WaveLib.h"

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
  auto *rawTable = new RawTable{};
  const auto &config = Core::Config::get();
  rawTable->Length = size_t(
      VUtils::Math::nextPowerOfTwo(config.sampleRate / (3.0 * 20)) * 2.0);
  rawTable->Data = new double[rawTable->Length + 1]{};
  generateWave(rawTable);
  auto &waveLib = WaveLib::Get();
  waveLib.SetWaveTable(waveTableIndex, CreateWaves(*rawTable));
  delete rawTable; // done! ;) free me pls :>
}

void TableCreator::FillTables(RawTable &rawTable, WaveTable *waveTable) {
  auto &freqWaveRe = rawTable.Data;
  unsigned int maxHarmonic = rawTable.Length >> 1u;
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
  waveTable->Waves = new Wave[size]{};
  waveTable->Length = size;
  for (size_t i = 0; i < size; ++i) {
    auto &wave = waveTable->Waves[i];
    wave.Length = (double)rawTable.Length;
    wave.TopFreq = topFreq;
    auto len = (size_t)wave.Length;
    auto *tempData = new double[len + 1]{};
    size_t numSamples = len;
    tempData[0] = 0;
    tempData[numSamples] = 0;
    for (size_t x = 1; x <= maxHarmonic; ++x) {
      tempData[x] = rawTable.Data[x];
      tempData[numSamples - x] = rawTable.Data[numSamples - x];
    }
    MakeWaveTable(wave, topFreq, tempData);
    topFreq *= 2.0;
    maxHarmonic >>= 1u;
    delete[] tempData;
  }
}

void TableCreator::CreateTableRaw(RawTable &rawTable, WaveTable *waveTable) {
  auto &config = Core::Config::get();
  waveTable->Waves = new Wave[1];
  waveTable->Length = 1;
  auto &wave = waveTable->Waves[0];
  wave.TopFreq = config.sampleRate / 2.0;
  wave.Length = (double)rawTable.Length;
  wave.Data = new double[rawTable.Length + 1];
  for (size_t x = 0; x < rawTable.Length; x++)
    wave.Data[x] = rawTable.Data[x];
  wave.Data[rawTable.Length] = wave.Data[0];
}

void TableCreator::MakeWaveTable(Wave &wave, double topFreq, double *tempData) {
  auto length = (size_t)wave.Length;
  wave.Data = new double[length + 1]{};
  FFT((int)wave.Length, tempData, wave.Data);
  ApplyCutoff(wave, topFreq);
  double scale = FindScale(wave);
  for (size_t idx = 0; idx < length; idx++)
    wave.Data[idx] = wave.Data[idx] * scale;
  wave.Data[length] = wave.Data[0];
}

void TableCreator::FFT(int N, double *ar, double *ai) {
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
  for (size_t idx = 0; idx < wave.Length; idx++) {
    double temp = std::fabs(wave.Data[idx]);
    if (max < temp)
      max = temp;
  }
  return 1.0 / max * .999;
}

void TableCreator::ApplyCutoff(Wave &wave, double topFreq) {
  double sRate = Core::Config::get().sampleRate;
  double cutOff = topFreq * (sRate / 2) * 1.2;
  double RC = 0.9 / (cutOff * 2 * V_PI);
  double dt = 1.0 / sRate;
  double alpha = dt / (RC + dt);

  double prev_sample = 0.0;
  auto length = (size_t)wave.Length;
  for (size_t i = 0; i < length; ++i) {
    double tmp = wave.Data[i];
    wave.Data[i] = prev_sample + (alpha * (wave.Data[i] - prev_sample));
    prev_sample = tmp;
  }
}
} // namespace VeNo::Audio