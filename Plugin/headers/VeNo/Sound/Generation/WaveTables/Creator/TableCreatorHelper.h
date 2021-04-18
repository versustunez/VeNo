#pragma once

#include <VeNo/Sound/Generation/WaveTables/RawTable.h>
#include <cmath>
#include <cstdlib>

namespace VeNo::Audio::Waves
{
struct TableCreatorHelper
{
    static void fft (unsigned int N, RawTable* table);
    static double makeWaveTable (RawTable* table, double scale, double topFreq);
    static size_t findTableLen();
    static double getNextRand();
    static int fillTables (RawTable* table);
    static void createTable (RawTable* table);
    static void createTableRaw (RawTable* table);
    static double getScale (RawTable* table, double scale);
    static void createWaveTableOnIndex (RawTable* table, int index, double topFreq);
    static void findCutoff (double topFreq, RawTable* table);
};
} // namespace VeNo::Audio::Waves
