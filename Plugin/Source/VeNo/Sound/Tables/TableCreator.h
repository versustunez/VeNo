#pragma once
#include "WaveTable.h"

#include <functional>

namespace VeNo::Audio {
class TableCreator {
public:
  static void CreateAll();
protected:
  static WaveTable* CreateWaves(RawTable & table);
  static void CreateTable(int waveTableIndex, const std::function<void(RawTable*)>& generateWave);
  static void FillTables(RawTable&, WaveTable*);
  static void CreateTableRaw(RawTable&, WaveTable*);
  static void MakeWaveTable(Wave &, double topFreq, float *tempData);
  static void FFT(size_t i, float *ar, float *ai);
  static double FindScale(Wave& wave);
  static void ApplyCutoff(Wave& wave, double topFreq);

};
} // namespace VeNo::Audio