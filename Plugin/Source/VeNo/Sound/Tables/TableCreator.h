#pragma once
#include "WaveTable.h"

#include <functional>

namespace VeNo::Audio {
class TableCreator {
public:
  static void CreateAll();

protected:
  static WaveTable *CreateWaves(RawTable &table);
  static void CreateTable(int waveTableIndex,
                          const std::function<void(RawTable *)> &generateWave);
  static void FillTables(RawTable &, WaveTable *);
  static void CreateTableRaw(RawTable &, WaveTable *);
  static void MakeWaveTable(Wave &, double topFreq, double *tempData);
  static void FFT(int i, double *ar, double *ai);
  static double FindScale(Wave &wave);
  static void ApplyCutoff(Wave &wave, double topFreq);
};
} // namespace VeNo::Audio