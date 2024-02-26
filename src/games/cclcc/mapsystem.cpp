#include "mapsystem.h"
#include <array>
#include <algorithm>
#include <vector>
#include "../../mem.h"
#include "../../inputsystem.h"
#include "../../vm/interface/input.h"
#include "../../game.h"
#include "../../profile/scriptvars.h"

#include "../../profile/games/cclcc/mapsystem.h"
#include "../../profile/ui/mapsystem.h"
#include <random>

namespace Impacto {
namespace UI {
namespace CCLCC {
using namespace Impacto::Vm::Interface;
using namespace Impacto::UI::CCLCC;
using namespace Impacto::UI::Widgets;
using namespace Impacto::Profile::CCLCC::MapSystem;
using namespace Impacto::Profile::ScriptVars;

inline float toFlt(double d) { return static_cast<float>(d); }
inline float toFlt(int d) { return static_cast<float>(d); }
inline int toInt(float f) { return static_cast<int>(f); }

std::array<MapSystemCCLCC::MapPositionTransitions, 4> MapPosTransitions;
enum { MapZoom1, MapZoom3, MapMove, MapMove2, MapPosTransitionsMax };

float MapZoomPosX;
float MapZoomPosY;

int MapZoomTo = 0;
float MapZoomGx = 0;
float MapZoomGy = 0;
int MapZoomNow = 0;

int MapZoomMode = 0;
int MapZoomCt = 0;
int MapZoomCtMax = 0;

constexpr int MapPhotoIdMap[11][4] = {
    // index 0 is map part for photos
    // index 3 is map part for article

    {0, 1, 2, 0x3e8},       {3, 4, 5, 0x3e9},       {6, 7, 8, 0x3ea},
    {9, 10, 11, 0x3eb},     {10, 0xff, 0xff, 0xff}, {12, 0xff, 0xff, 0xff},
    {13, 0xff, 0xff, 0xff}, {14, 0xff, 0xff, 0xff}, {15, 0xff, 0xff, 0xff},
    {16, 0xff, 0xff, 0xff}, {17, 0xff, 0xff, 0xff},
};

constexpr float MapPoolPosOffsets[][2] = {
    {2709.0, 200.0}, {2981.0, 214.0}, {3222.0, 203.0},  {2717.0, 332.0},
    {2984.0, 349.0}, {3236.0, 342.0}, {2713.0, 459.0},  {2974.0, 482.0},
    {3222.0, 485.0}, {2734.0, 596.0}, {2740.0, 784.0},  {3047.0, 787.0},
    {2754.0, 930.0}, {3066.0, 930.0}, {2755.0, 1072.0}, {3064.0, 1073.0},
};

constexpr float MapPoolArticleOffsets[][2] = {
    {2699.0, 200.0}, {3071.0, 214.0}, {2707.0, 482.0}, {3074.0, 499.0}};

constexpr float MapArticleOffsets[][12]{
    {1095.0, 813.0, 1143.0, 745.0, 1169.0, 676.0, 1048.0, 735.0, 898.0, 744.0,
     882.0, 849.0},
    {1095.0, 813.0, 1143.0, 745.0, 302.0, 1259.0, 390.0, 1093.0, 226.0, 982.0,
     218.0, 1094.0},
    {439.0, 1189.0, 481.0, 1113.0, 756.0, 1005.0, 607.0, 869.0, 478.0, 953.0,
     451.0, 843.0},
    {645.0, 975.0, 707.0, 881.0, 756.0, 1005.0, 607.0, 869.0, 478.0, 953.0,
     451.0, 843.0},
    {645.0, 975.0, 707.0, 881.0, 842.0, 621.0, 680.0, 564.0, 698.0, 432.0,
     864.0, 424.0},
    {680.0, 564.0, 779.0, 647.0, 842.0, 621.0, 680.0, 564.0, 698.0, 432.0,
     864.0, 424.0},
    {680.0, 564.0, 779.0, 647.0, 1361.0, 628.0, 1466.0, 484.0, 0.0, 0.0, 0.0,
     0.0},
    {0.0, 0.0, 0.0, 0.0, 155.0, 550.0, 308.0, 440.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, 1037.0, 1355.0, 1203.0, 1320.0, 0.0, 0.0, 0.0, 0.0},
    {1113.0, 1419.0, 1298.0, 1324.0, 1188.0, 1043.0, 1294.0, 934.0, 0.0, 0.0,
     0.0, 0.0},
    {1431.0, 980.0, 1390.0, 952.0, 515.0, 1412.0, 645.0, 1499.0, 0.0, 0.0, 0.0,
     0.0},
    {788.0, 1553.0, 750.0, 1511.0, 1132.0, 1139.0, 984.0, 1175.0, 0.0, 0.0, 0.0,
     0.0},
    {832.0, 1183.0, 1082.0, 1194.0, 1277.0, 52.0, 1430.0, 170.0, 0.0, 0.0, 0.0,
     0.0},
    {1589.0, 205.0, 1512.0, 183.0, 1339.0, 1243.0, 1536.0, 1264.0, 0.0, 0.0,
     0.0, 0.0},
    {1644.0, 1359.0, 1638.0, 1284.0, 1508.0, 1411.0, 1357.0, 1490.0, 0.0, 0.0,
     0.0, 0.0},
    {1306.0, 1596.0, 1453.0, 1509.0, 0.0, 0.0, 2658.0, 913.0, 2998.0, 915.0,
     3184.0, 921.0},
    {2803.0, 931.0, 2641.0, 866.0, 1298.0, 1257.0, 1458.0, 1131.0, 0.0, 0.0,
     0.0, 0.0},
    {1627.0, 1126.0, 1309.0, 1284.0, 0.0, 0.0, 205.0, 680.0, 205.0, 680.0,
     205.0, 680.0},
};

constexpr float PartsOffsetData[][3][4] = {{{1169.0, 676.0, 1048.0, 735.0},
                                            {898.0, 744.0, 882.0, 849.0},
                                            {1095.0, 813.0, 1143.0, 745.0}},

                                           {{302.0, 1259.0, 390.0, 1093.0},
                                            {226.0, 982.0, 218.0, 1094.0},
                                            {439.0, 1189.0, 481.0, 1113.0}},

                                           {{756.0, 1005.0, 607.0, 869.0},
                                            {478.0, 953.0, 451.0, 843.0},
                                            {645.0, 975.0, 707.0, 881.0}},

                                           {{842.0, 621.0, 680.0, 564.0},
                                            {698.0, 432.0, 864.0, 424.0},
                                            {620.0, 650.0, 779.0, 647.0}},

                                           {{756.0, 1005.0, 607.0, 869.0},
                                            {478.0, 953.0, 451.0, 843.0},
                                            {645.0, 975.0, 707.0, 881.0}},

                                           {{756.0, 1005.0, 607.0, 869.0},
                                            {478.0, 953.0, 451.0, 843.0},
                                            {645.0, 975.0, 707.0, 881.0}},

                                           {{756.0, 1005.0, 607.0, 869.0},
                                            {478.0, 953.0, 451.0, 843.0},
                                            {645.0, 975.0, 707.0, 881.0}},

                                           {{756.0, 1005.0, 607.0, 869.0},
                                            {478.0, 953.0, 451.0, 843.0},
                                            {645.0, 975.0, 707.0, 881.0}}};

constexpr int PartsIdMap[][4] = {{2, 0xff, 0xff, 0xff}};

constexpr uint32_t Tints[] = {
    0xB43C3C, 0xB43C3C, 0xCA2ECC, 0x387A21, 0xEFA918, 0xEFA918, 0x40D08A,
    0xABABAB, 0xE26E10, 0x1761F5, 0xABABAB, 0xE26E10, 0x1761F5, 0x387A21,
    0xB43C3C, 0xCA2ECC, 0xEFA0DE, 0x387A21, 0xCA2ECC, 0x40D08A, 0xEFA0DE,
    0xABABAB, 0xE26E10, 0xEFA918, 0x40D08A, 0xEFA0DE, 0x1761F5, 0x387A21,
    0x387A21, 0x1761F5, 0xEFA918, 0xEFA918, 0x40D08A, 0x40D08A, 0xEFA0DE,
    0xABABAB, 0x387A21, 0x1761F5, 0xEFA918, 0x40D08A, 0xEFA0DE, 0xABABAB,
    0xE26E10, 0xE26E10, 0xCA2ECC, 0xCA2ECC, 0x387A21, 0x1761F5, 0xEFA918,
    0x40D08A, 0xCA2ECC, 0xCA2ECC, 0xCA2ECC, 0xCA2ECC, 0xCA2ECC, 0xCA2ECC,
    0xCA2ECC, 0xCA2ECC, 0xCA2ECC, 0xCA2ECC, 0xCA2ECC, 0xCA2ECC, 0xCA2ECC,
    0xCA2ECC, 0xCA2ECC, 0xCA2ECC, 0xCA2ECC, 0xCA2ECC, 0xCA2ECC, 0xCA2ECC,
    0xCA2ECC, 0xCA2ECC, 0xE26E10, 0xE26E10, 0xE26E10, 0xE26E10, 0xE26E10,
    0xE26E10, 0xE26E10, 0xE26E10, 0xE26E10, 0x40D08A, 0xEFA0DE, 0xE26E10,
    0x40D08A, 0x40D08A};

constexpr uint32_t LineColors1[] = {
    0x10F050, 0x10F050, 0x10F050, 0x10F050, 0x10F050, 0x10F050, 0x10F050,
    0x10F050, 0x10F050, 0x10F050, 0x10F050, 0x10F050, 0x10F050, 0x10F050,
    0x10F050, 0x10F050, 0x0,      0xFDC000, 0xFDC000, 0x0,
};

constexpr uint32_t LineColors2[] = {
    0x1030F0, 0x1030F0, 0x1030F0, 0x1030F0, 0x1030F0, 0x1030F0, 0x1030F0,
    0x1030F0, 0x1030F0, 0xF01030, 0xF01030, 0xF01030, 0xF01030, 0xF01030,
    0xF01030, 0xF01030, 0x0,      0xFDC000, 0xFDC000, 0x0,      0x0,
    0x0,      0x0,      0xFDC000, 0x0,      0x0,      0x0,      0x0,
    0xDC0FA,  0xDC0FA,  0xDC0FA,  0xDC0FA,  0xDC0FA,  0xDC0FA,  0xDC0FA,
    0xDC0FA,  0xDC0FA,  0xDC0FA,  0xDC0FA,  0xDC0FA,  0xDC0FA,  0xDC0FA,
    0xDC0FA,  0xDC0FA,  0xDC0FA,
};

inline int CALCrnd(int arg1) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(0, 0x7FFF);
  return distr(gen) * arg1 >> 0xf;
}

void MapSystemCCLCC::MapInit() {
  ScrWork[6369] = 0;
  ScrWork[6370] = 0;
  ScrWork[6371] = 0;

  MapPartsMax = 0;

  MapResetPoolAll(0);
  for (auto itr = MapPartsDisp.begin(); itr != MapPartsDisp.end(); ++itr) {
    *itr = MapPartsDispStruct{0xff, 0, Animation{}, Hidden, 0, 0, 0};
    itr->fadeAnim.DurationIn = FadeAnimationDuration;
    itr->fadeAnim.DurationOut = FadeAnimationDuration;
  }
  for (int i = 0; i < MapGroup.size(); i++) {
    MapGroup[i] = MapGroupStruct{i, i, i};
  }
  SetFlag(2801, 0);
  SetFlag(2802, 0);
  SetFlag(2803, 0);
  SetFlag(2804, 0);
  SetFlag(2805, 0);
}
void MapSystemCCLCC::MapSetFadein(int arg1, int arg2) {
  if (MapPartsMax != 0) {
    for (int i = 0; i < MapPartsMax; ++i) {
      if (MapPartsDisp[i].partId == arg1 && MapPartsDisp[i].type == arg2) {
        return;
      }
    }
  }
  MapPartsDisp[MapPartsMax].partId = arg1;
  MapPartsDisp[MapPartsMax].type = arg2;
  MapPartsDisp[MapPartsMax].state = Showing;
  MapPartsDisp[MapPartsMax].fadeAnim.Progress = 0;
  MapPartsDisp[MapPartsMax].delay = 0;

  if (arg2 == 0) {
    MapPartsDisp[MapPartsMax].angle = CALCrnd(12);
    MapPartsMax++;
  } else if (arg2 >= 1 && arg2 <= 4) {
    MapSetFadeIn_PhotoArticle(arg1, arg2);
  } else if (arg2 == 5) {
    MapPartsDisp[MapPartsMax].angle = CALCrnd(1820) - 910;
    MapPartsMax++;
  } else if (arg2 == 6 || arg2 == 7 || arg2 == 12 || arg2 == 13 || arg2 == 14) {
    MapSetFadeIn_Line(arg1, arg2);
  }
}
void MapSystemCCLCC::MapSetFadeIn_PhotoArticle(int arg1, int arg2) {
  MapPartsDisp[MapPartsMax].angle = CALCrnd(3640) - 1820;
  MapPartsMax++;
  if (arg2 > 0 && arg2 < 4 && MapPhotoIdMap[arg1][arg2 - 1] > 999) {
    return;
  }
  MapPartsDisp[MapPartsMax].partId = arg1;
  MapPartsDisp[MapPartsMax].type = arg2 + 7;
  MapPartsDisp[MapPartsMax].state = Showing;
  MapPartsDisp[MapPartsMax].fadeAnim.Progress = 0;
  MapPartsDisp[MapPartsMax].delay = 16;
  MapPartsDisp[MapPartsMax].angle = CALCrnd(12);
  MapPartsMax++;
}
void MapSystemCCLCC::MapSetFadeIn_Line(int arg1, int arg2) {
  if (arg2 == 7) {
    float pos1 = PartsOffsetData[arg1][0][0] - PartsOffsetData[arg1][0][2];
    float pos2 = PartsOffsetData[arg1][0][1] - PartsOffsetData[arg1][0][3];
    MapPartsDisp[MapPartsMax].dist =
        toInt(sqrt(pos1 * pos1 + pos2 * pos2) / 200.0f * 16.0f);
    if (MapPartsDisp[MapPartsMax].dist < 16) {
      MapPartsDisp[MapPartsMax].dist = 16;
    }
    MapPartsDisp[MapPartsMax].fadeAnim.DurationIn =
        MapPartsDisp[MapPartsMax].dist / 60.0f;
    MapPartsMax++;
    for (int i = 0; i < MapPartsMax; ++i) {
      if (MapPartsDisp[i].partId == arg1 && MapPartsDisp[i].type == 0) {
        MapPartsDisp[i].delay = MapPartsDisp[MapPartsMax - 1].dist - 16;
        MapPartsDisp[i].state = Hiding;
        MapPartsDisp[i].fadeAnim.Progress = 1;
        break;
      }
    }

    MapPartsDisp[MapPartsMax].partId = arg1;
    MapPartsDisp[MapPartsMax].type = 0;
    MapPartsDisp[MapPartsMax].state = Showing;
    MapPartsDisp[MapPartsMax].fadeAnim.Progress = 0;
    MapPartsDisp[MapPartsMax].angle = CALCrnd(12);
    MapPartsDisp[MapPartsMax].delay = MapPartsDisp[MapPartsMax - 1].dist;
    MapPartsMax++;
    return;
  } else if (arg2 == 6 || arg2 > 11 && arg2 < 15) {
    int index = (arg2 == 6) ? 0 : arg2 - 11;
    int mappedId = (arg1 < sizeof(PartsIdMap) / sizeof(float[4]))
                       ? static_cast<int>(PartsIdMap[arg1][index])
                       : 0;
    if (mappedId == 0xff) return;
    float pos1 = PartsOffsetData[mappedId][0][2] - PartsOffsetData[arg1][0][2];
    float pos2 = PartsOffsetData[mappedId][0][3] - PartsOffsetData[arg1][0][3];
    MapPartsDisp[MapPartsMax].dist =
        toInt(sqrt(pos1 * pos1 + pos2 * pos2) / 200.0f * 16.0f);
    if (MapPartsDisp[MapPartsMax].dist < 16) {
      MapPartsDisp[MapPartsMax].dist = 16;
    }
    MapPartsDisp[MapPartsMax].fadeAnim.DurationIn =
        MapPartsDisp[MapPartsMax].dist / 60.0f;
  MapPartsMax++;
        for (int i = 0; i < MapPartsMax; i++) {
          if (MapPartsDisp[i].partId == PartsIdMap[arg1][mappedId] &&
              MapPartsDisp[i].type == 8) {
            MapPartsDisp[i].delay = MapPartsDisp[MapPartsMax - 1].dist - 16;
            MapPartsDisp[i].state = Hiding;
            MapPartsDisp[i].fadeAnim.Progress = 1;
            break;
          }
        }
        MapPartsDisp[MapPartsMax].partId = PartsIdMap[arg1][mappedId];
        MapPartsDisp[MapPartsMax].type = 8;
        MapPartsDisp[MapPartsMax].state = Showing;
        MapPartsDisp[MapPartsMax].fadeAnim.Progress = 0;
        MapPartsDisp[MapPartsMax].delay = MapPartsDisp[MapPartsMax - 1].dist;
        MapPartsDisp[MapPartsMax].angle = CALCrnd(12);
        MapPartsMax++;
        return;
  }
}
void MapSystemCCLCC::MapSetGroup(int arg1, int arg2, int arg3, int arg4) {
  if (arg1 >= 40) return;
  MapGroup[arg1].a = arg2;
  MapGroup[arg1].b = arg3;
  MapGroup[arg1].c = arg4;
}
void MapSystemCCLCC::MapSetFadeout(int arg1, int arg2) {
  bool flag = false;
  if (arg2 > 0 && arg2 <= 4) {
    if (MapPartsMax) {
      for (int i = 0; i < MapPartsMax; ++i) {
        if (MapPartsDisp[i].partId == arg1 &&
            MapPartsDisp[i].type == arg2 + 7) {
          MapPartsDisp[i].state = Hiding;
          MapPartsDisp[i].fadeAnim.Progress = 1;
          MapPartsDisp[i].delay = 0;
          flag = true;
          break;
        }
      }
    }
  } else if (arg2 == 7) {
    for (int i = 0; i < MapPartsMax; ++i) {
      if (MapPartsDisp[i].partId == arg1 && MapPartsDisp[i].type == 0) {
        MapPartsDisp[i].state = Hiding;
        MapPartsDisp[i].fadeAnim.Progress = 1;
        MapPartsDisp[i].delay = 0;
        flag = true;
        break;
      }
    }
  } else if (arg2 == 0) {
    for (int i = 0; i < MapPartsMax; ++i) {
      if (MapPartsDisp[i].partId == arg1 && MapPartsDisp[i].type == 7) {
        MapPartsDisp[i].state = Hiding;
        MapPartsDisp[i].fadeAnim.Progress = 1;
        MapPartsDisp[i].fadeAnim.DurationOut = MapPartsDisp[i].dist / 60.0f;
        MapPartsDisp[i].delay = 16;
        break;
      }
    }
  }

  for (int i = 0; i < MapPartsMax; ++i) {
    if (MapPartsDisp[i].partId == arg1 && MapPartsDisp[i].type == arg2) {
      MapPartsDisp[i].state = Hiding;
      MapPartsDisp[i].fadeAnim.Progress = 1;
      MapPartsDisp[i].delay = flag ? 16 : 0;
      MapPartsDisp[i].fadeAnim.DurationOut =
          (arg2 == 7) ? MapPartsDisp[i].dist / 60.0f : 16.0f / 60.0f;
      return;
    }
  }
}
void MapSystemCCLCC::MapSetDisp(int arg1, int arg2) {
  if (MapPartsMax != 0) {
    for (int i = 0; i < MapPartsMax; ++i) {
      if (MapPartsDisp[i].partId == arg1 && MapPartsDisp[i].type == arg2) {
        return;
      }
    }
  }
  MapPartsDisp[MapPartsMax].partId = arg1;
  MapPartsDisp[MapPartsMax].type = arg2;
  MapPartsDisp[MapPartsMax].state = Shown;
  MapPartsDisp[MapPartsMax].delay = 0;

  if (arg2 == 0) {
    MapPartsDisp[MapPartsMax].angle = CALCrnd(12);
    MapPartsMax++;
  } else if (arg2 >= 1 && arg2 <= 4) {
    MapPartsDisp[MapPartsMax].angle = CALCrnd(3640) - 1820;
    MapPartsMax++;
    if (MapPhotoIdMap[arg1][arg2 - 1] >= 1000) {
      return;
    }
    MapPartsDisp[MapPartsMax].partId = arg1;
    MapPartsDisp[MapPartsMax].type = arg2 + 7;
    MapPartsDisp[MapPartsMax].state = Showing;
    MapPartsDisp[MapPartsMax].fadeAnim.Progress = 0;
    MapPartsDisp[MapPartsMax].delay = 0;
    MapPartsDisp[MapPartsMax].angle = CALCrnd(12);
    MapPartsMax++;
  } else if (arg2 == 5) {
    MapPartsDisp[MapPartsMax].angle = CALCrnd(1820) - 910;
    MapPartsMax++;
  } else if (arg2 > 11 && arg2 < 15 || arg2 == 6) {
    int index = (arg2 == 6) ? 0 : arg2 - 11;
    int mappedId = (arg1 < sizeof(PartsIdMap) / sizeof(float[4]))
                       ? static_cast<int>(PartsIdMap[arg1][index])
                       : 0;
    if (mappedId != 0xff) {
      float pos1 =
          PartsOffsetData[mappedId][0][2] - PartsOffsetData[arg1][0][2];
      float pos2 =
          PartsOffsetData[mappedId][0][3] - PartsOffsetData[arg1][0][3];
    MapPartsDisp[MapPartsMax].dist =
        toInt(sqrt(pos1 * pos1 + pos2 * pos2) / 200.0f * 16.0f);
    }
    if (MapPartsDisp[MapPartsMax].dist < 16) {
      MapPartsDisp[MapPartsMax].dist = 16;
    }
    MapPartsDisp[MapPartsMax].fadeAnim.DurationOut =
        MapPartsDisp[MapPartsMax].dist / 60.0f;
  MapPartsMax++;
  } else if (arg2 == 7) {
    float pos1 = PartsOffsetData[arg1][0][0] - PartsOffsetData[arg1][0][2];
    float pos2 = PartsOffsetData[arg1][0][1] - PartsOffsetData[arg1][0][3];
    MapPartsDisp[MapPartsMax].dist =
        toInt(sqrt(pos1 * pos1 + pos2 * pos2) / 200.0f * 16.0f);
    if (MapPartsDisp[MapPartsMax].dist < 16) {
      MapPartsDisp[MapPartsMax].dist = 16;
    }
    MapPartsDisp[MapPartsMax].fadeAnim.DurationOut =
        MapPartsDisp[MapPartsMax].dist / 60.0f;
    MapPartsMax++;
    for (int i = 0; i < MapPartsMax; ++i) {
      if (MapPartsDisp[i].partId == arg1 && MapPartsDisp[i].type == 0) {
        return;
      }
    }

    MapPartsDisp[MapPartsMax].partId = arg1;
    MapPartsDisp[MapPartsMax].type = 0;
    MapPartsDisp[MapPartsMax].state = Shown;
    MapPartsDisp[MapPartsMax].angle = CALCrnd(12);
    MapPartsDisp[MapPartsMax].delay = 0;
    MapPartsMax++;
  }
}
void MapSystemCCLCC::MapSetHide(int arg1, int arg2) {}
bool MapSystemCCLCC::MapPoolFadeEndChk_Wait() {
  for (int i = 0; i < 20; ++i) {
    if (MapPool[i].id == 0xff) continue;
    if (MapPoolDisp[i * 2].state == Showing ||
        MapPoolDisp[i * 2].state == Hiding)
      return false;
    if (MapPoolDisp[i * 2 + 1].state == Showing ||
        MapPoolDisp[i * 2 + 1].state == Hiding)
      return false;
    if (MapPoolDisp[i * 2].delay != 0) return false;
    if (MapPoolDisp[i * 2 + 1].delay != 0) return false;
  }
  return true;
}
void MapSystemCCLCC::MapMoveAnimeInit(int arg1, int arg2, int arg3) {}
void MapSystemCCLCC::MapMoveAnimeMain() {}
void MapSystemCCLCC::MapGetPos(int arg1, int arg2, int& arg3, int& arg4) {
  switch (arg2) {
    case 0:
      arg3 = toInt(PartsOffsetData[arg1][0][0]);
      arg4 = toInt(PartsOffsetData[arg1][0][1]);
      break;
    case 1:
    case 8:
      arg3 = toInt(PartsOffsetData[arg1][0][2]);
      arg4 = toInt(PartsOffsetData[arg1][0][3]);
      break;
    case 2:
    case 9:
      arg3 = toInt(PartsOffsetData[arg1][1][0]);
      arg4 = toInt(PartsOffsetData[arg1][1][1]);
      break;
    case 3:
    case 10:
      arg3 = toInt(PartsOffsetData[arg1][1][2]);
      arg4 = toInt(PartsOffsetData[arg1][1][3]);
      break;
    case 5:
      arg3 = toInt(PartsOffsetData[arg1][2][2]);
      arg4 = toInt(PartsOffsetData[arg1][2][3]);
      break;
    case 11:
      arg3 = toInt(PartsOffsetData[arg1][2][0]);
      arg4 = toInt(PartsOffsetData[arg1][2][1]);
      break;
    default:
      break;
  }
}
void MapSystemCCLCC::MapSetPool(int arg1, int arg2, int arg3) {
  MapPool[arg1].id = arg2;
  MapPool[arg1].type = arg3;
}
void MapSystemCCLCC::MapResetPoolAll(int arg1) {
  for (int i = 0; i < 20; ++i) {
    MapPool[i].id = 0xff;
    MapPool[i].type = 0xff;
    MapPool[i].button.Enabled = false;
    MapPool[i].button.Id = 0xff;
    MapPool[i].button.HasFocus = false;
    MapPool[i].button.Bounds = {0, 0, 0, 0};
  }
  for (int i = 0; i < 40; ++i) {
    MapPoolDisp[i].state = Hidden;
    MapPoolDisp[i].fadeAnim.DurationIn = FadeAnimationDuration;
    MapPoolDisp[i].fadeAnim.DurationOut = FadeAnimationDuration;
    MapPoolDisp[i].angle = 0;
  }
}
bool MapSystemCCLCC::MapFadeEndChk_Wait() {
  for (int i = 0; i < MapPartsMax; ++i) {
    if (MapPartsDisp[i].state == Showing || MapPartsDisp[i].state == Hiding)
      return false;
    if (MapPartsDisp[i].delay != 0) return false;
  }
  return true;
}
void MapSystemCCLCC::MapPoolShuffle(int param_1) {
  auto start = MapPool.begin() + param_1 * 10;
  auto end = start;

  // Find the end of the range to shuffle
  while (end != MapPool.end() && end->id != 255) {
    ++end;
  }

  std::random_device rd;
  std::mt19937 g(rd());

  // Why reinvent the wheel
  std::shuffle(start, end, g);
}

void MapSystemCCLCC::MapPoolSetDisp(int arg1, int arg2) {}
void MapSystemCCLCC::MapPoolSetHide(int arg1, int arg2) {}
void MapSystemCCLCC::MapPoolSetFadein(int arg1, int arg2) {
  // arg1 always 0

  int id = MapPool[arg2].id;
  if (id != 0xff) {
    if (MapPoolDisp[arg2 * 2].state == Hidden) {
      MapPoolDisp[arg2 * 2].state = Showing;
      MapPoolDisp[arg2 * 2].fadeAnim.Progress = 0;
      MapPoolDisp[arg2 * 2].delay = 0;
      MapPoolDisp[arg2 * 2].angle = CALCrnd(3640) - 1820;
    }
    int poolPinIndex = arg2 * 2 + 1;
    if (MapPhotoIdMap[MapPool[arg2].id][MapPool[arg2].type] < 1000) {
      if (MapPoolDisp[poolPinIndex].state == Hidden) {
        MapPoolDisp[poolPinIndex].state = Showing;
        MapPoolDisp[poolPinIndex].fadeAnim.Progress = 0;
        MapPoolDisp[poolPinIndex].delay = 16;
        MapPoolDisp[poolPinIndex].pinId = CALCrnd(12);
      }
    } else {
      MapPoolDisp[poolPinIndex].state = Hidden;
      MapPoolDisp[poolPinIndex].delay = 0;
      MapPoolDisp[poolPinIndex].fadeAnim.Progress = 0;
    }
  }
}
void MapSystemCCLCC::MapPoolSetFadeout(int arg1, int arg2) {
  if (MapPoolDisp[arg2 * 2].state == Shown ||
      MapPoolDisp[arg2 * 2].state == Showing) {
    MapPoolDisp[arg2 * 2].delay = 0x10;
    MapPoolDisp[arg2 * 2].state = Hiding;
    MapPoolDisp[arg2 * 2].fadeAnim.Progress = 1;
  }
  int poolPinIndex = arg2 * 2 | 1;
  if (MapPoolDisp[poolPinIndex].state == Shown ||
      MapPoolDisp[poolPinIndex].state == Showing) {
    MapPoolDisp[poolPinIndex].delay = 0;
    MapPoolDisp[poolPinIndex].state = Hiding;
    MapPoolDisp[poolPinIndex].fadeAnim.Progress = 1;
  }
  return;
}

void MapSystemCCLCC::HandlePoolUpDownNav(int maxPoolRow, int poolType,
                                         bool isUp) {
  PhotoSelClick = false;
  if (hoverMapPoolIdx == 0xff) {
    for (int i = 0; i < 10; ++i) {
      if (MapPool[i].id != 0xff && MapPoolDisp[i * 2].state == Shown) {
        hoverMapPoolIdx = i;
        return;
      }
    }
  } else {
    int tmpPoolIdx;
    do {
      if (poolType == 3) {
        hoverMapPoolIdx =
            (hoverMapPoolIdx < 2) ? hoverMapPoolIdx + 2 : hoverMapPoolIdx - 2;
      } else {
        if (isUp)
          hoverMapPoolIdx =
              (hoverMapPoolIdx < 3) ? hoverMapPoolIdx + 6 : hoverMapPoolIdx - 3;
        else
          hoverMapPoolIdx =
              (hoverMapPoolIdx < 6) ? hoverMapPoolIdx + 3 : hoverMapPoolIdx - 6;
        if (MapPoolDisp[18].state == Shown && MapPool[9].id != 0xff) {
          hoverMapPoolIdx = 9;
          return;
        }
      }
      if (MapPool[hoverMapPoolIdx].id != 0xff &&
          MapPoolDisp[hoverMapPoolIdx * 2].state == Shown) {
        return;
      }
      int colNum = hoverMapPoolIdx % maxPoolRow;
      int rowNum = hoverMapPoolIdx / maxPoolRow;
      tmpPoolIdx = rowNum * maxPoolRow;

      if (colNum >= 0 && colNum <= 2) {
        constexpr int offsets[3][2] = {{1, 2}, {0, 2}, {1, 0}};

        for (int i = 0; i < 2; ++i) {
          if ((MapPool[tmpPoolIdx + offsets[colNum][i]].id != 0xff &&
               MapPoolDisp[(tmpPoolIdx + offsets[colNum][i]) * 2].state ==
                   Shown)) {
            tmpPoolIdx += offsets[colNum][i];
            break;
          }
          if (colNum < 2 && poolType == 3) break;
        }
      }
    } while (MapPool[tmpPoolIdx].id == 0xff ||
             MapPoolDisp[tmpPoolIdx * 2].state != Shown);
    hoverMapPoolIdx = tmpPoolIdx;
  }
}

void MapSystemCCLCC::HandlePoolLeftRightNav(int maxPoolRow, int poolType,
                                            bool isLeft) {
  PhotoSelClick = false;
  if (hoverMapPoolIdx == 0xff) {
    for (int i = 0; i < 10; ++i) {
      if (MapPool[i].id != 0xff && MapPoolDisp[i * 2].state == Shown) {
        hoverMapPoolIdx = i;
        return;
      }
    }
  } else {
    int mapId = hoverMapPoolIdx;
    do {
      if (poolType == 3) {
        if (hoverMapPoolIdx != 3)
          mapId = (hoverMapPoolIdx == 0 || hoverMapPoolIdx == 2)
                      ? hoverMapPoolIdx + 1
                      : hoverMapPoolIdx - 1;
        else
          mapId = 2;
      } else {
        if (hoverMapPoolIdx == 9) {
          mapId = (isLeft) ? 6 : 8;
        } else {
          if (isLeft) {
            if (hoverMapPoolIdx % 3 == 0) {
              mapId = hoverMapPoolIdx + 2;
            } else {
              mapId = hoverMapPoolIdx - 1;
            }
          } else {
            if (hoverMapPoolIdx % 3 == 2) {
              mapId = hoverMapPoolIdx - 2;
            } else {
              mapId = hoverMapPoolIdx + 1;
            }
          }
        }
      }
      hoverMapPoolIdx = mapId;
      if (MapPool[hoverMapPoolIdx].id != 0xff &&
          MapPoolDisp[hoverMapPoolIdx * 2].state == Shown)
        break;

      int rowNum = mapId / maxPoolRow;

      if (rowNum >= 0 && rowNum <= 2) {
        int offsets[3][2] = {{maxPoolRow, 2 * maxPoolRow},
                             {-maxPoolRow, maxPoolRow},
                             {-maxPoolRow, -2 * maxPoolRow}};

        for (int i = 0; i < 2; ++i) {
          mapId = hoverMapPoolIdx + offsets[rowNum][i];

          if (MapPool[mapId].id != 0xff &&
              MapPoolDisp[mapId * 2].state == Shown) {
            break;
          }
        }

        if ((rowNum == 2 && hoverMapPoolIdx == maxPoolRow * 2) ||
            (rowNum == 1 && hoverMapPoolIdx == 3) ||
            (rowNum == 0 && hoverMapPoolIdx == 0)) {
          if (MapPoolDisp[18].state == Shown && MapPool[9].id != 0xff) {
            mapId = 9;
            break;
          }
        }
      }
    } while (MapPool[mapId].id == 0xff ||
             MapPoolDisp[mapId * 2].state != Shown);
    hoverMapPoolIdx = mapId;
  }
}

bool MapSystemCCLCC::MapPlayerPhotoSelect(int arg1) {
  ScrWork[6500] = 2;
  int oldHover = hoverMapPoolIdx;
  if (arg1 == 0) {
    int poolType = MapPool[0].type;
    int maxPoolRow =
        (poolType == 3) ? 2 : 3;  // 3 per row for photo, 2 for articles

    if (PhotoSelClick) {
      for (int i = 0; i < MapPool.size(); i++) {
        if (MapPool[i].id != 0xff && MapPool[i].button.Enabled) {
          MapPool[i].button.UpdateInput();
          if (MapPool[i].button.Hovered) {
            hoverMapPoolIdx = i;
          }
        }
      }
    }

    if (PADinputButtonWentDown & PAD1UP) {
      HandlePoolUpDownNav(maxPoolRow, poolType, true);
    } else if (PADinputButtonWentDown & PAD1DOWN) {
      HandlePoolUpDownNav(maxPoolRow, poolType, false);
    } else if (PADinputButtonWentDown & PAD1LEFT) {
      HandlePoolLeftRightNav(maxPoolRow, poolType, true);
    } else if (PADinputButtonWentDown & PAD1RIGHT) {
      HandlePoolLeftRightNav(maxPoolRow, poolType, false);
    } else if (PADinputButtonWentDown & PAD1A && (hoverMapPoolIdx != 0xff) &&
               (MapPoolCurCt[hoverMapPoolIdx] == 16)) {
      if (hoverMapPoolIdx != 0xff) {
        selectedMapPoolIdx = hoverMapPoolIdx;
      }
      PhotoSelClick = false;
    } else if (!PhotoSelClick && PADinputMouseWentDown & PAD1A) {
      PhotoSelClick = true;
    }

    if (oldHover != hoverMapPoolIdx) {
      if (oldHover != 0xff) {
        MapPool[oldHover].button.HasFocus = false;
      }
      if (hoverMapPoolIdx != 0xff) {
        MapPool[hoverMapPoolIdx].button.HasFocus = true;
      }
    }

    if (selectedMapPoolIdx != 0xff) {
      ScrWork[6367] = selectedMapPoolIdx;
      ScrWork[6368] = MapPool[selectedMapPoolIdx].id;
      ScrWork[6381] = MapPool[selectedMapPoolIdx].type;
      ScrWork[6500] = 0;
      MapPool[selectedMapPoolIdx].button.HasFocus = false;
      hoverMapPoolIdx = 0xff;
      selectedMapPoolIdx = 0xff;
      return true;
    }
  } else {
    ImpLogSlow(LL_Debug, LC_VMStub, "MapPlayerPhotoSelect: arg1 != 0\n");
  }
  return false;
}

void MapSystemCCLCC::MapResetPool(int arg1) {
  MapPool[arg1].id = 0xff;
  MapPool[arg1].type = 0xff;
  MapPool[arg1].button.Enabled = false;
  MapPool[arg1].button.Id = 0xff;
  MapPool[arg1].button.HasFocus = false;
  MapPool[arg1].button.Bounds = {0, 0, 0, 0};
}

void getMapPos(float newSize, float newX, float newY, float& setX, float& setY,
               float& setSize, float scaleX, float scaleY) {
  setSize = (newSize < 100) ? 100 : (newSize > 800) ? 800 : newSize;
  float mapWidth = MapBgSprite.Sheet.DesignWidth * 100.0f / setSize;
  float mapHeight = MapBgSprite.Sheet.DesignHeight * 100.0f / setSize;
  float offsetMapX = newX - mapWidth * 0.5f;
  float offsetMapY = newY - mapHeight * 0.5f;

  if (!GetFlag(2805)) {
    if (offsetMapX > MapBgSprite.Sheet.DesignWidth - mapWidth)
      offsetMapX = MapBgSprite.Sheet.DesignWidth - mapWidth;

    if (offsetMapX < 0) offsetMapX = 0;

    if (offsetMapY > MapBgSprite.Sheet.DesignHeight - mapHeight)
      offsetMapY = MapBgSprite.Sheet.DesignHeight - mapHeight;

    if (offsetMapY < 0) offsetMapY = 0;
  }

  setX = mapWidth * scaleX + offsetMapX;
  setY = mapHeight * scaleY + offsetMapY;
}

void MapSystemCCLCC::MapSetGroupEx(int arg1, int arg2, int arg3) {
  if (arg1 >= 40) return;
  switch (arg2) {
    case 0:
      MapGroup[arg1].a = arg3;
      break;
    case 1:
      MapGroup[arg1].b = arg3;
      break;
    case 2:
      MapGroup[arg1].c = arg3;
      break;
  }
}
void MapSystemCCLCC::MapZoomInit(int arg1, int arg2, int arg3) {
  MapZoomTo = arg3 * 4;

  float scaledX = toFlt(arg1) * 1920.0f / 1280.0f;
  float scaledY = toFlt(arg2) * 1080.0f / 720.0f;
  MapZoomGx = scaledX;
  MapZoomGy = scaledY;

  float zoomSize = 0.0f;
  getMapPos(toFlt(ScrWork[6362]), toFlt(ScrWork[6363]), toFlt(ScrWork[6364]),
            MapZoomPosX, MapZoomPosY, zoomSize, scaledX, scaledY);

  MapZoomNow = zoomSize * 4;
  int ticks = MapZoomNow;
  int steps = 0;
  if (MapZoomNow < MapZoomTo) {
    for (; ticks < MapZoomTo; ticks = ticks / 400 + 1) {
      steps = steps + 1;
    }
  } else if (MapZoomNow > MapZoomTo) {
    for (; ticks < MapZoomTo; ticks = (ticks - ticks / 400) - 1) {
      steps = steps + 1;
    }
  }

  MapZoomCtAcc.fill(0);

  for (int i = 6; i > 0; --i) {
    int uVar10 = 0;
    if (i != 0) {
      uVar10 = 3 * i * (i + 1);  // Sum of first i multiples of 6
    }

    if (uVar10 <= steps) {
      std::fill(MapZoomCtAcc.begin(), MapZoomCtAcc.begin() + i, 6);
      int ivar9 = (steps - uVar10) / (i + 1);
      MapZoomCtAcc[i] = ivar9;
      steps = (steps - uVar10) - (ivar9 * i + ivar9);
      if (steps != 0) {
        MapZoomCtAcc[steps - 1]++;
      }
      return;
    };
  }
  MapZoomCtAcc[0] = steps;
  return;
}

bool MapSystemCCLCC::MapZoomMain() {
  int max = (GetFlag(SF_MESALLSKIP) == 0) ? 1 : 4;

  int zoomCounter = 0;
  for (int i = 0; i < max; i++) {
    int j = 0;
    while (j < 13 && MapZoomCtAcc[j] != 0) {
      j++;
    }
    if (j != 13) {
      MapZoomCtAcc[j] -= 1;
      int inc = (j + 1 < 8) ? j + 1 : 13 - j;
      zoomCounter += inc;
    }
  }
  if (MapZoomNow < MapZoomTo) {
    for (int i = 0; i < zoomCounter; ++i) {
      MapZoomNow = MapZoomNow + 1 + MapZoomNow / 400;
    }
    if (MapZoomNow > MapZoomTo) MapZoomNow = MapZoomTo;
  } else if (MapZoomNow > MapZoomTo) {
    for (int i = 0; i < zoomCounter; ++i) {
      MapZoomNow = (MapZoomNow - 1) - MapZoomNow / 400;
    }
    if (MapZoomTo > MapZoomNow) MapZoomNow = MapZoomTo;
  }
  ScrWork[6362] = (MapZoomNow + 5) >> 2;
  float clampedSize = (ScrWork[6362] < 100)   ? 100.0f
                      : (ScrWork[6362] > 800) ? 800.0f
                                              : ScrWork[6362];
  float newMapPosX =
      MapZoomPosX - (MapZoomGx / 1920.0f + -0.5f) *
                        (MapBgSprite.Sheet.DesignWidth * 100.0f) / clampedSize;

  float newMapPosY =
      MapZoomPosY - (MapZoomGy / 1080.0f + -0.5f) *
                        (MapBgSprite.Sheet.DesignHeight * 100.0f) / clampedSize;

  ScrWork[6363] = static_cast<int>(newMapPosX);
  ScrWork[6364] = static_cast<int>(newMapPosY);
  return MapZoomTo == MapZoomNow;
}
void MapSystemCCLCC::MapZoomInit2(int arg1, int arg2) {}
bool MapSystemCCLCC::MapZoomMain3() {
  int max = (GetFlag(SF_MESALLSKIP) == 0) ? 1 : 4;

  int zoomCounter = 0;
  for (int i = 0; i < max; i++) {
    int j =
        static_cast<int>(std::find_if(MapZoomCtAcc.begin(), MapZoomCtAcc.end(),
                                      [](int x) { return x != 0; }) -
                         MapZoomCtAcc.begin());
    if (j != 13) {
      MapZoomCtAcc[j] -= 1;
      int inc = (j + 1 < 8) ? j + 1 : 13 - j;
      zoomCounter += inc;
    }
  }
  int newZoomCt = MapZoomCt + zoomCounter;
  if (MapZoomMode == 2 && MapZoomCt < 40) {
    zoomCounter = (newZoomCt < 40) ? 0 : newZoomCt - 40;
  }

  MapZoomCt = newZoomCt;

  if (MapZoomCt > MapZoomCtMax) {
    MapZoomCt = MapZoomCtMax;
  }

  if (MapPosTransitions[MapZoom3].Start.size <
      MapPosTransitions[MapZoom3].End.size) {
    for (int i = 0; i < zoomCounter; ++i) {
      MapPosTransitions[MapZoom3].Current.size +=
          1 + MapPosTransitions[MapZoom3].Current.size / 1000;
    }
    if (MapPosTransitions[MapZoom3].Current.size >
        MapPosTransitions[MapZoom3].End.size)
      MapPosTransitions[MapZoom3].Current.size =
          MapPosTransitions[MapZoom3].End.size;
  } else if (MapPosTransitions[MapZoom3].Start.size >
             MapPosTransitions[MapZoom3].End.size) {
    for (int i = 0; i < zoomCounter; ++i) {
      MapPosTransitions[MapZoom3].Current.size -=
          1 + MapPosTransitions[MapZoom3].Current.size / 1000;
    }
    if (MapPosTransitions[MapZoom3].Current.size <
        MapPosTransitions[MapZoom3].End.size)
      MapPosTransitions[MapZoom3].Current.size =
          MapPosTransitions[MapZoom3].End.size;
  }

  MapPosTransitions[MapZoom3].Current.x =
      MapPosTransitions[MapZoom3].Start.x +
      (MapPosTransitions[MapZoom3].End.x -
       MapPosTransitions[MapZoom3].Start.x) *
          ((float)MapZoomCt / MapZoomCtMax);

  MapPosTransitions[MapZoom3].Current.y =
      MapPosTransitions[MapZoom3].Start.y +
      (MapPosTransitions[MapZoom3].End.y -
       MapPosTransitions[MapZoom3].Start.y) *
          ((float)MapZoomCt / MapZoomCtMax);

  ScrWork[6362] =
      static_cast<int>((MapPosTransitions[MapZoom3].Current.size + 5) / 10);
  ScrWork[6363] = static_cast<int>(MapPosTransitions[MapZoom3].Current.x);
  ScrWork[6364] = static_cast<int>(MapPosTransitions[MapZoom3].Current.y);
  return MapZoomCt == MapZoomCtMax;
}
bool MapSystemCCLCC::MapZoomInit3(int arg1, int arg2, int arg3, bool ex) {
  if (ex) {
    MapZoomMode = 1;
  } else {
    MapZoomMode = 0;
  }
  MapZoomCt = 0;
  float zoomSize = 0.0f;
  float mapX = 0.0f;
  float mapY = 0.0f;
  getMapPos(toFlt(ScrWork[6362]), toFlt(ScrWork[6363]), toFlt(ScrWork[6364]),
            mapX, mapY, zoomSize, 0.5, 0.5);
  MapPosTransitions[MapZoom3].Start.size = 10 * zoomSize;
  MapPosTransitions[MapZoom3].Start.x = mapX;
  MapPosTransitions[MapZoom3].Start.y = mapY;

  if (arg3 != 0) {
    getMapPos(toFlt(arg3), toFlt(arg1), toFlt(arg2), mapX, mapY, zoomSize, 0.5,
              0.5);
  } else {
    getMapPos(zoomSize, toFlt(arg1), toFlt(arg2), mapX, mapY, zoomSize, 0.5,
              0.5);
  }

  MapPosTransitions[MapZoom3].End.size = 10 * zoomSize;
  MapPosTransitions[MapZoom3].End.x = mapX;
  MapPosTransitions[MapZoom3].End.y = mapY;

  MapPosTransitions[MapZoom3].Current.size =
      MapPosTransitions[MapZoom3].Start.size;
  MapPosTransitions[MapZoom3].Current.x = MapPosTransitions[MapZoom3].Start.x;
  MapPosTransitions[MapZoom3].Current.y = MapPosTransitions[MapZoom3].Start.y;

  if (MapPosTransitions[MapZoom3].End.x ==
          MapPosTransitions[MapZoom3].Start.x &&
      MapPosTransitions[MapZoom3].End.y ==
          MapPosTransitions[MapZoom3].Start.y &&
      MapPosTransitions[MapZoom3].End.size ==
          MapPosTransitions[MapZoom3].Start.size) {
    return false;
  }

  int steps = 0;
  MapZoomCtMax = (ex) ? 40 : 0;
  if (arg3 == 0) {
    float mapPosDiag = sqrt((MapPosTransitions[MapZoom3].End.x -
                             MapPosTransitions[MapZoom3].Start.x) *
                                (MapPosTransitions[MapZoom3].End.x -
                                 MapPosTransitions[MapZoom3].Start.x) +
                            (MapPosTransitions[MapZoom3].End.y -
                             MapPosTransitions[MapZoom3].Start.y) *
                                (MapPosTransitions[MapZoom3].End.y -
                                 MapPosTransitions[MapZoom3].Start.y));
    float mapBGNewHeight = MapBgSprite.Sheet.DesignHeight * 100.0f / zoomSize;
    float mapBGNewWidth = MapBgSprite.Sheet.DesignWidth * 100.0f / zoomSize;
    float mapBGDiag =
        sqrt(mapBGNewHeight * mapBGNewHeight + mapBGNewWidth * mapBGNewWidth);

    steps = toInt(mapPosDiag / (mapBGDiag / 400.0f));
    if (steps == 0) {
      MapZoomCtMax = 1;
      MapZoomCtAcc[0] = 1;
      std::fill(MapZoomCtAcc.begin() + 1, MapZoomCtAcc.end(), 1);
      return true;
    }
    MapZoomCtMax += steps;
  } else if (MapPosTransitions[MapZoom3].Start.size <
             MapPosTransitions[MapZoom3].End.size) {
    for (int ticks = toInt(MapPosTransitions[MapZoom3].Start.size);
         ticks < MapPosTransitions[MapZoom3].End.size;
         ticks += ticks / 1000 + 1) {
      steps = steps + 1;
    }
  } else if (MapPosTransitions[MapZoom3].Start.size >
             MapPosTransitions[MapZoom3].End.size) {
    for (int ticks = toInt(MapPosTransitions[MapZoom3].Start.size);
         ticks > MapPosTransitions[MapZoom3].End.size;
         ticks -= ticks / 1000 + 1) {
      steps = steps + 1;
    }
  }

  MapZoomCtMax += steps;

  std::fill(MapZoomCtAcc.begin(), MapZoomCtAcc.end(), 0);

  for (int i = 6; i > 0; --i) {
    int sixSum = 0;
    if (i != 0) {
      sixSum = 3 * i * (i + 1);  // Sum of first i multiples of 6
    }
    if (!ex) sixSum *= 2;
    if (sixSum <= MapZoomCtMax) {
      std::fill(MapZoomCtAcc.begin(), MapZoomCtAcc.begin() + i, 6);
      if (!ex) std::fill(MapZoomCtAcc.end() - i - 1, MapZoomCtAcc.end(), 6);
      int peakAcc = (MapZoomCtMax - sixSum) / (i + 1);
      MapZoomCtAcc[i] = peakAcc;
      int index = (MapZoomCtMax - sixSum) - peakAcc * (i + 1);
      if (index != 0) {
        MapZoomCtAcc[index - 1]++;
      }
      return true;
    };
  }
  MapZoomCtAcc[0] = MapZoomCtMax;
  return true;
}

bool MapSystemCCLCC::MapMoveAnimeInit2(int arg1, int arg2, int arg3) {
  MapMoveMode = 0;
  if (!arg3 || ScrWork[6362] < arg3) {
    arg3 = ScrWork[6362];
  }
  getMapPos(toFlt(ScrWork[6362]), toFlt(ScrWork[6363]), toFlt(ScrWork[6364]),
            MapPosTransitions[MapMove2].Start.x,
            MapPosTransitions[MapMove2].Start.y,
            MapPosTransitions[MapMove2].Start.size, 0.5f, 0.5f);
  getMapPos(toFlt(ScrWork[6362]), toFlt(arg1), toFlt(arg2),
            MapPosTransitions[MapMove2].End.x,
            MapPosTransitions[MapMove2].End.y,
            MapPosTransitions[MapMove2].End.size, 0.5, 0.5);
  if (arg3 == ScrWork[6362]) {
    MapMoveMode = 99;
    return MapZoomInit3(static_cast<int>(MapPosTransitions[MapMove2].End.x),
                        static_cast<int>(MapPosTransitions[MapMove2].End.y), 0);
  } else {
    float MapMoveCenterPosX = (arg1 + ScrWork[6363]) * 0.5f;
    float MapMoveCenterPosY = (arg2 + ScrWork[6364]) * 0.5f;
    float MapMoveLimitSize = 0;
    getMapPos(toFlt(arg3), MapMoveCenterPosX, MapMoveCenterPosY,
              MapMoveCenterPosX, MapMoveCenterPosY, MapMoveLimitSize, 0.5, 0.5);
    return MapZoomInit3(static_cast<int>(MapMoveCenterPosX),
                        static_cast<int>(MapMoveCenterPosY),
                        static_cast<int>(MapMoveLimitSize), true);
  }
}

bool MapSystemCCLCC::MapMoveAnimeMain2() {
  bool ret = MapZoomMain3();
  if (MapMoveMode != 0) {
    return ret;
  }
  if (ret) {
    if (MapMoveMode == 99) {
      return true;
    }
    MapZoomInit3(static_cast<int>(MapPosTransitions[MapMove2].End.x),
                 static_cast<int>(MapPosTransitions[MapMove2].End.y),
                 static_cast<int>(MapPosTransitions[MapMove2].End.size), true);
    for (int i = 0; i < 6; ++i) {
      MapZoomCtAcc[MapZoomCtAcc.size() - i - 1] = MapZoomCtAcc[i];
    }
    std::fill(MapZoomCtAcc.begin(), MapZoomCtAcc.begin() + 6, 0);
    MapMoveMode++;
    MapZoomMode = 2;
  }
  return false;
}
void MapSystemCCLCC::MapPlayerPotalSelectInit() {}
bool MapSystemCCLCC::MapPlayerPotalSelect() { return false; }
void MapSystemCCLCC::MapSystem_28() {}
void MapSystemCCLCC::MapDispPhoto(int id, int arg2) {
  int alpha = 256;
  float shadowZoom = 1.0f, zoomMulti = 1.0f;
  if (MapPartsDisp[id].state == Hiding && !MapPartsDisp[id].delay) {
    zoomMulti = 16.0f - MapPartsDisp[id].fadeAnim.Progress * 60.0f *
                            FadeAnimationDuration;
    shadowZoom = (zoomMulti) / 24.0f + 1.0f;
    zoomMulti = zoomMulti / 32.0f + 1.0f;
    alpha =
        MapPartsDisp[id].fadeAnim.Progress * 60.0f * FadeAnimationDuration * 16;
  } else if (MapPartsDisp[id].state == Showing) {
    zoomMulti =
        -MapPartsDisp[id].fadeAnim.Progress * 60.0f * FadeAnimationDuration;
    shadowZoom = (zoomMulti) / 24.0f + 1.6666f;
    zoomMulti = zoomMulti / 32.0f + 1.5f;
    alpha =
        MapPartsDisp[id].fadeAnim.Progress * 60.0f * FadeAnimationDuration * 16;
  }

  float xOffset = id;
  float yOffset = id;
  int mappedId = id;
  int index = MapPartsDisp[id].partId;
  if (arg2 == 3) {
    index = (index < 40) ? MapGroup[index].c : index;
    xOffset = PartsOffsetData[MapPartsDisp[id].partId][1][2];
    yOffset = PartsOffsetData[MapPartsDisp[id].partId][1][3];
    mappedId = MapPhotoIdMap[index][2];
  } else if (arg2 == 2) {
    index = (index < 40) ? MapGroup[index].b : index;
    xOffset = PartsOffsetData[MapPartsDisp[id].partId][1][0];
    yOffset = PartsOffsetData[MapPartsDisp[id].partId][1][1];
    mappedId = MapPhotoIdMap[index][1];
  } else if (arg2 == 1) {
    index = (index < 40) ? MapGroup[index].a : index;
    xOffset = PartsOffsetData[MapPartsDisp[id].partId][0][2];
    yOffset = PartsOffsetData[MapPartsDisp[id].partId][0][3];
    mappedId = MapPhotoIdMap[index][0];
  }

  if (mappedId == 0xff) {
    return;
  }
  mappedId = (mappedId < 1000) ? mappedId : mappedId - 1000;

  const float scaledFactor = 1080.0f / MapBGHeight;
  zoomMulti *= scaledFactor;
  shadowZoom *= scaledFactor;

  Sprite displayedSprite = MapPartsPhotoSprites[mappedId];
  xOffset = xOffset - 16.0f - 82.0f;
  yOffset = yOffset - 10.0f;

  float xMapEdge = mapPosX + MapBGWidth;
  float yMapEdge = mapPosY + MapBGHeight;

  if (mapPosX <= xOffset + displayedSprite.Bounds.Width &&
      mapPosY <= yOffset + displayedSprite.Bounds.Height &&
      xOffset <= xMapEdge && yOffset <= yMapEdge) {
    float angle = toFlt(MapPartsDisp[mappedId].angle * 2.0f * M_PI / 65536.0f);

    xOffset = xOffset - mapPosX;
    yOffset = yOffset - mapPosY;

    float scaledPosOffsetX = (xOffset + 82) * scaledFactor;
    float scaledPosOffsetY = (yOffset + 10) * scaledFactor;

    // Shadow
    Renderer->DrawSpriteOffset(
        displayedSprite,
        glm::vec2((xOffset + 82 + 3) * scaledFactor - 82,
                  (yOffset + 10 + 3) * scaledFactor - 10),
        glm::vec2{82, 10}, glm::vec4{0.0f, 0.0f, 0.0f, (alpha >> 1) / 256.0f},
        glm::vec2{shadowZoom, shadowZoom}, angle);
    // Photo
    Renderer->DrawSpriteOffset(
        displayedSprite,
        glm::vec2(scaledPosOffsetX - 82, scaledPosOffsetY - 10),
        glm::vec2{82, 10}, glm::vec4{1.0f, 1.0f, 1.0f, alpha / 256.0f},
        glm::vec2{zoomMulti, zoomMulti}, angle);
  }
}

void MapSystemCCLCC::MapPoolDispPhoto(int poolId) {
  int mappedMapPoolId = MapPhotoIdMap[MapPool[poolId].id][MapPool[poolId].type];
  if (mappedMapPoolId == 0xff) return;

  Sprite displayedSprite;
  float xOffset;
  float yOffset;

  // Display Photo
  if (mappedMapPoolId < 1000) {
    displayedSprite = MapPartsPhotoSprites[mappedMapPoolId];
    xOffset = MapPoolPosOffsets[poolId][0];
    yOffset = MapPoolPosOffsets[poolId][1];
  }
  // Display Article
  else {
    displayedSprite = MapPartsArticleSprites[mappedMapPoolId - 1000];
    xOffset = MapPoolArticleOffsets[poolId][0];
    yOffset = MapPoolArticleOffsets[poolId][1];
  }

  xOffset = xOffset - 16.0f - 82.0f;
  yOffset = yOffset - 10.0f;

  int alpha = 256;
  float shadowZoom = 1.0f, zoomMulti = 1.0f;
  if (MapPoolDisp[poolId * 2].state == Hiding &&
      !MapPoolDisp[poolId * 2].delay) {
    zoomMulti = 16.0f - MapPoolDisp[poolId * 2].fadeAnim.Progress * 60.0f *
                            FadeAnimationDuration;
    shadowZoom = (zoomMulti) / 24.0f + 1.0f;
    zoomMulti = zoomMulti / 32.0f + 1.0f;
    alpha = MapPoolDisp[poolId * 2].fadeAnim.Progress * 60.0f *
            FadeAnimationDuration * 16;
  } else if (MapPoolDisp[poolId * 2].state == Showing) {
    zoomMulti = -MapPoolDisp[poolId * 2].fadeAnim.Progress * 60.0f *
                FadeAnimationDuration;
    shadowZoom = (zoomMulti) / 24.0f + 1.6666f;
    zoomMulti = zoomMulti / 32.0f + 1.5f;
    alpha = MapPoolDisp[poolId * 2].fadeAnim.Progress * 60.0f *
            FadeAnimationDuration * 16;
  }
  float scaledFactor = 1080.0f / MapBGHeight;
  zoomMulti *= scaledFactor;
  shadowZoom *= scaledFactor;
  float xMapEdge = mapPosX + MapBGWidth;
  float yMapEdge = mapPosY + MapBGHeight;

  if (mapPosX <= xOffset + displayedSprite.Bounds.Width &&
      mapPosY <= yOffset + displayedSprite.Bounds.Height &&
      xOffset <= xMapEdge && yOffset <= yMapEdge) {
    float angle = toFlt(MapPoolDisp[poolId * 2].angle * 2.0f * M_PI / 65536.0f);

    xOffset = xOffset - mapPosX;
    yOffset = yOffset - mapPosY;

    float scaledPosOffsetX = (xOffset + 82) * scaledFactor;
    float scaledPosOffsetY = (yOffset + 10) * scaledFactor;

    if (MapPoolCurCt[poolId] != 0) {
      float selectedTagXOffset =
          (MapPoolCurCt[poolId] * 50.0f / 16.0f + 78.0f) * 2;

      // HoverTag Tag
      Renderer->DrawSpriteOffset(
          SelectedMapPoolTagSprite,
          glm::vec2(scaledPosOffsetX - selectedTagXOffset,
                    scaledPosOffsetY - 18),
          glm::vec2{selectedTagXOffset, 11},
          glm::vec4{1.0f, 1.0f, 1.0f, alpha / 256.0f},
          glm::vec2{zoomMulti / 2.0, zoomMulti / 2.0}, angle);
    }
    // Shadow
    Renderer->DrawSpriteOffset(
        displayedSprite,
        glm::vec2((xOffset + 82 + 3) * scaledFactor - 82,
                  (yOffset + 10 + 3) * scaledFactor - 10),
        glm::vec2{82, 10}, glm::vec4{0.0f, 0.0f, 0.0f, (alpha >> 1) / 256.0f},
        glm::vec2{shadowZoom, shadowZoom}, angle);
    // Photo
    Renderer->DrawSpriteOffset(
        displayedSprite,
        glm::vec2(scaledPosOffsetX - 82, scaledPosOffsetY - 10),
        glm::vec2{82, 10}, glm::vec4{1.0f, 1.0f, 1.0f, alpha / 256.0f},
        glm::vec2{zoomMulti, zoomMulti}, angle);
    if (!MapPool[poolId].button.Enabled) {
      MapPool[poolId].button.Enabled = true;
      MapPool[poolId].button.Id = poolId;
      MapPool[poolId].button.OnClickHandler = [this](Button* button) {
        selectedMapPoolIdx = button->Id;
      };
    }

    MapPool[poolId].button.Bounds = {
        zoomMulti * displayedSprite.Bounds.Width * -0.25f +
            (scaledPosOffsetX - 82),
        scaledPosOffsetY - 10, zoomMulti * displayedSprite.Bounds.Width,
        zoomMulti * displayedSprite.Bounds.Height};
  }
}

void MapSystemCCLCC::MapPoolDispPin(int poolId) {
  int poolDispIndex = poolId * 2 + 1;
  Sprite displayedSprite = MapPartsPinSprites[MapPoolDisp[poolDispIndex].pinId];

  int alpha = 256;
  float zoomMulti = 1.0f;
  if (MapPoolDisp[poolDispIndex].state == Hiding &&
      !MapPoolDisp[poolDispIndex].delay) {
    zoomMulti = 16.0f - MapPoolDisp[poolDispIndex].fadeAnim.Progress * 60.0f *
                            FadeAnimationDuration;
    zoomMulti = zoomMulti / 32.0f + 1.0f;
    alpha = MapPoolDisp[poolDispIndex].fadeAnim.Progress * 60.0f *
            FadeAnimationDuration * 16;
  } else if (MapPoolDisp[poolDispIndex].state == Showing) {
    zoomMulti = -MapPoolDisp[poolDispIndex].fadeAnim.Progress * 60.0f *
                FadeAnimationDuration;
    zoomMulti = zoomMulti / 32.0f + 1.5f;
    alpha = MapPoolDisp[poolDispIndex].fadeAnim.Progress * 60.0f *
            FadeAnimationDuration * 16;
  }
  float scaledFactor = 1080.0f / MapBGHeight;
  zoomMulti *= scaledFactor;
  float xMapEdge = mapPosX + MapBGWidth;
  float yMapEdge = mapPosY + MapBGHeight;

  float xOffset = MapPoolPosOffsets[poolId][0] - 16.0f;
  float yOffset = MapPoolPosOffsets[poolId][1] - 29.0f;

  if (mapPosX <= xOffset + displayedSprite.Bounds.Width &&
      mapPosY <= yOffset + displayedSprite.Bounds.Height &&
      xOffset <= xMapEdge && yOffset <= yMapEdge) {
    xOffset = xOffset - mapPosX;
    yOffset = yOffset - mapPosY;

    float scaledPosOffsetX = (xOffset + 16.0f) * scaledFactor;
    float scaledPosOffsetY = (yOffset + 29.0f) * scaledFactor;

    glm::vec4 color = RgbIntToFloat(Tints[poolId]);
    color[3] = alpha / 256.0f;

    Renderer->DrawSpriteOffset(
        displayedSprite,
        glm::vec2(scaledPosOffsetX - 16.0f, scaledPosOffsetY - 29.0f),
        glm::vec2{16.0f, 29.0f}, color, glm::vec2{zoomMulti, zoomMulti}, 0);
  }
}

void MapSystemCCLCC::MapDispPin(int id) {
  Sprite displayedSprite = MapPartsPinSprites[MapPartsDisp[id].angle];

  int alpha = 256;
  float zoomMulti = 1.0f;
  if (MapPartsDisp[id].state == Hiding && !MapPartsDisp[id].delay) {
    zoomMulti = 16.0f - MapPartsDisp[id].fadeAnim.Progress * 60.0f *
                            FadeAnimationDuration;
    zoomMulti = zoomMulti / 32.0f + 1.0f;
    alpha =
        MapPartsDisp[id].fadeAnim.Progress * 60.0f * FadeAnimationDuration * 16;
  } else if (MapPartsDisp[id].state == Showing) {
    zoomMulti =
        -MapPartsDisp[id].fadeAnim.Progress * 60.0f * FadeAnimationDuration;
    zoomMulti = zoomMulti / 32.0f + 1.5f;
    alpha =
        MapPartsDisp[id].fadeAnim.Progress * 60.0f * FadeAnimationDuration * 16;
  }

  float scaledFactor = 1080.0f / MapBGHeight;
  zoomMulti *= scaledFactor;
  float xMapEdge = mapPosX + MapBGWidth;
  float yMapEdge = mapPosY + MapBGHeight;

  int mappedId = MapPartsDisp[id].partId;
  float xOffset, yOffset;
  switch (MapPartsDisp[id].type) {
    case 0:
      xOffset = PartsOffsetData[mappedId][0][0];
      yOffset = PartsOffsetData[mappedId][0][1];
      break;
    case 8:
      xOffset = PartsOffsetData[mappedId][0][2];
      yOffset = PartsOffsetData[mappedId][0][3];
      break;
    case 9:
      xOffset = PartsOffsetData[mappedId][1][0];
      yOffset = PartsOffsetData[mappedId][1][1];
      break;
    case 10:
      xOffset = PartsOffsetData[mappedId][1][2];
      yOffset = PartsOffsetData[mappedId][1][3];
      break;
    case 11:
      xOffset = PartsOffsetData[mappedId][2][0];
      yOffset = PartsOffsetData[mappedId][2][1];
      break;
    default:
      break;
  }
  xOffset = xOffset - 16.0f;
  yOffset = yOffset - 29.0f;

  if (mapPosX <= xOffset + displayedSprite.Bounds.Width &&
      mapPosY <= yOffset + displayedSprite.Bounds.Height &&
      xOffset <= xMapEdge && yOffset <= yMapEdge) {
    xOffset = xOffset - mapPosX;
    yOffset = yOffset - mapPosY;

    float scaledPosOffsetX = (xOffset + 16.0f) * scaledFactor;
    float scaledPosOffsetY = (yOffset + 29.0f) * scaledFactor;

    glm::vec4 color = RgbIntToFloat(Tints[mappedId + 27]);
    color[3] = alpha / 256.0f;

    Renderer->DrawSpriteOffset(
        displayedSprite,
        glm::vec2(scaledPosOffsetX - 16.0f, scaledPosOffsetY - 29.0f),
        glm::vec2{16.0f, 29.0f}, color, glm::vec2{zoomMulti, zoomMulti}, 0);
  }
}

void MapSystemCCLCC::MapDispArticle(int id) {
  int partId = MapPartsDisp[id].partId;

  int alpha = 256;
  float shadowZoom = 1.0f;
  float zoomMulti = 1.0f;
  if (MapPartsDisp[id].state == Hiding && !MapPartsDisp[id].delay) {
    zoomMulti = 16.0f - MapPartsDisp[id].fadeAnim.Progress * 60.0f *
                            FadeAnimationDuration;
    shadowZoom = (zoomMulti) / 24.0f + 1.0f;
    zoomMulti = zoomMulti / 32.0f + 1.0f;
    alpha =
        MapPartsDisp[id].fadeAnim.Progress * 60.0f * FadeAnimationDuration * 16;
  } else if (MapPartsDisp[id].state == Showing) {
    zoomMulti =
        -MapPartsDisp[id].fadeAnim.Progress * 60.0f * FadeAnimationDuration;
    shadowZoom = (zoomMulti) / 24.0f + 1.6666f;
    zoomMulti = zoomMulti / 32.0f + 1.5f;
    alpha =
        MapPartsDisp[id].fadeAnim.Progress * 60.0f * FadeAnimationDuration * 16;
  }

  float xPartOffset = PartsOffsetData[partId][2][0] - 166;
  float yPartOffset = PartsOffsetData[partId][2][1] - 16;

  if (mapPosX <= xPartOffset + 354.0 && mapPosY <= yPartOffset + 247.0) {
    float scaledFactor = 1080.0f / MapBGHeight;
    float angle = toFlt(MapPartsDisp[id].angle * 2 * M_PI / 65536.0f);

    xPartOffset = xPartOffset - mapPosX;
    yPartOffset = yPartOffset - mapPosY;

    float displayShadowPhotoX = (xPartOffset + 166 + 3) * scaledFactor - 166;
    float displayShadowPhotoY = (yPartOffset + 16 + 3) * scaledFactor - 16;

    float scaledPosOffsetX = (xPartOffset + 166) * scaledFactor;
    float scaledPosOffsetY = (yPartOffset + 16) * scaledFactor;
    float displayPhotoX = scaledPosOffsetX - 166;
    float displayPhotoY = scaledPosOffsetY - 16;

    // Shadow
    Renderer->DrawSpriteOffset(
        MapPartsArticleSprites[partId],
        glm::vec2(displayShadowPhotoX, displayShadowPhotoY), glm::vec2{166, 16},
        glm::vec4{0.0f, 0.0f, 0.0f, (alpha >> 1) / 256.0f},
        glm::vec2{shadowZoom * 0.5 * scaledFactor,
                  shadowZoom * scaledFactor * 0.5},
        angle);
    // Photo
    Renderer->DrawSpriteOffset(MapPartsArticleSprites[partId],
                                 glm::vec2(displayPhotoX, displayPhotoY),
                                 glm::vec2{166, 16}, glm::vec4{1.0f},
                                 glm::vec2{zoomMulti * 0.5 * scaledFactor,
                                           zoomMulti * 0.5 * scaledFactor},
                                 angle);
  }
}

void MapSystemCCLCC::MapDispTag(int id) {
  Sprite displayedSprite = MapPartsTagSprites[MapPartsDisp[id].partId];

  int alpha = 256;
  float zoomMulti = 1.0f;
  float shadowZoom = 1.0f;
  if (MapPartsDisp[id].state == Hiding && !MapPartsDisp[id].delay) {
    zoomMulti = 16.0f - MapPartsDisp[id].fadeAnim.Progress * 60.0f *
                            FadeAnimationDuration;
    shadowZoom = (zoomMulti) / 24.0f + 1.0f;
    zoomMulti = zoomMulti / 32.0f + 1.0f;
    alpha =
        MapPartsDisp[id].fadeAnim.Progress * 60.0f * FadeAnimationDuration * 16;
  } else if (MapPartsDisp[id].state == Showing) {
    zoomMulti =
        -MapPartsDisp[id].fadeAnim.Progress * 60.0f * FadeAnimationDuration;
    shadowZoom = (zoomMulti) / 24.0f + 1.6666f;
    zoomMulti = zoomMulti / 32.0f + 1.5f;
    alpha =
        MapPartsDisp[id].fadeAnim.Progress * 60.0f * FadeAnimationDuration * 16;
  }
  float scaledFactor = 1080.0f / MapBGHeight;
  zoomMulti *= scaledFactor;
  shadowZoom *= scaledFactor;
  float xMapEdge = mapPosX + MapBGWidth;
  float yMapEdge = mapPosY + MapBGHeight;

  float xOffset = PartsOffsetData[MapPartsDisp[id].partId][2][2] - 47.0f;
  float yOffset = PartsOffsetData[MapPartsDisp[id].partId][2][3] - 17.0f;

  if (mapPosX <= xOffset + displayedSprite.Bounds.Width &&
      mapPosY <= yOffset + displayedSprite.Bounds.Height &&
      xOffset <= xMapEdge && yOffset <= yMapEdge) {
    xOffset = xOffset - mapPosX;
    yOffset = yOffset - mapPosY;
    float angle = toFlt(MapPartsDisp[id].angle * 2 * M_PI / 65536.0f);

    float shadowScaledPosOffsetX = (xOffset + 2.0f + 46.0f) * scaledFactor;
    float shadowScaledPosOffsetY = (yOffset + 2.0f + 16.0f) * scaledFactor;
    // Shadow
    Renderer->DrawSpriteOffset(displayedSprite,
                                 glm::vec2(shadowScaledPosOffsetX - 46.0f,
                                           shadowScaledPosOffsetY - 16.0f),
                                 glm::vec2{46.0f, 16.0f},
                                 {0.0f, 0.0f, 0.0f, alpha / 2 / 256.0f},
                                 glm::vec2{shadowZoom, shadowZoom}, angle);
    float scaledPosOffsetX = (xOffset + 46.0f) * scaledFactor;
    float scaledPosOffsetY = (yOffset + 16.0f) * scaledFactor;

    // Tag
    Renderer->DrawSpriteOffset(
        displayedSprite,
        glm::vec2(scaledPosOffsetX - 46.0f, scaledPosOffsetY - 16.0f),
        glm::vec2{46.0f, 16.0f}, {1.0f, 1.0f, 1.0f, alpha / 256.0f},
        glm::vec2{zoomMulti, zoomMulti}, angle);
  }
}

void MapSystemCCLCC::MapDispLine(int id, int arg2) {
  int index = MapPartsDisp[id].partId;
  float xOffset1 = PartsOffsetData[index][0][2];
  float yOffset1 = PartsOffsetData[index][0][3];
  float xOffset2, yOffset2;
  glm::vec4 color;
  if (arg2 != 7 && index <= sizeof(PartsIdMap) / sizeof(int)) {
    index = (arg2 == 6)                ? PartsIdMap[index][0]
            : (arg2 > 11 && arg2 < 15) ? PartsIdMap[index][arg2 - 11]
                                       : index;
  } else if (index > sizeof(PartsIdMap) / sizeof(int)) {
    index = 0;
  }
  if (arg2 == 7) {
    xOffset2 = PartsOffsetData[index][0][0];
    yOffset2 = PartsOffsetData[index][0][1];
    color = RgbIntToFloat(LineColors2[MapPartsDisp[id].partId]);
  } else {
    xOffset2 = PartsOffsetData[index][0][2];
    yOffset2 = PartsOffsetData[index][0][3];
    color = RgbIntToFloat(LineColors1[MapPartsDisp[id].partId]);
  }

  float linePercent = 1.0f;
  if (MapPartsDisp[id].state == Hiding && !MapPartsDisp[id].delay) {
    linePercent = toFlt(MapPartsDisp[id].fadeAnim.Progress * 60.0f *
                        FadeAnimationDuration) /
                  MapPartsDisp[id].dist;
  } else if (MapPartsDisp[id].state == Showing) {
    linePercent = toFlt(MapPartsDisp[id].fadeAnim.Progress * 60.0f *
                        FadeAnimationDuration) /
                  MapPartsDisp[id].dist;
  }
  float scaledFactor = 1080.0f / MapBGHeight;

  float xDist = xOffset2 - xOffset1;
  float yDist = yOffset1 - yOffset2;
  float dist = sqrt(xDist * xDist + yDist * yDist);

  float xMapEdge = mapPosX + MapBGWidth;
  float yMapEdge = mapPosY + MapBGHeight;
  float angle = atan2(yDist, xDist);

  if (mapPosX <= xOffset2 + 20 && mapPosY <= yOffset2 + 20 &&
      xOffset2 <= xMapEdge && yOffset2 <= yMapEdge) {
    float lineWidth = abs(dist) * linePercent;
    glm::vec4 shadowTint = {0.0f, 0.0f, 0.0f, 0.5f};
    float lineX2 = xOffset1 + cosf(angle) * dist * linePercent;
    float lineY2 = yOffset1 - sinf(angle) * dist * linePercent;
    float xPos = 0.5f * (xOffset1 + lineX2 - dist * linePercent) - mapPosX;
    float yPos = (yOffset1 + lineY2) / 2.0f - mapPosY - 5.0f;
    float yPosShadow =
        (xDist < 0 && yDist < 0)
            ? (yOffset1 + lineY2 - 5.0f) / 2.0f - mapPosY - 10.0f
            : (yOffset1 + lineY2 + 5.0f) / 2.0f - mapPosY - 10.0f;

    Sprite shortenedLine = MapLine;
    shortenedLine.Bounds.Width = lineWidth;

    RectF lineShadowRect(xPos * scaledFactor, yPosShadow * scaledFactor,
                         lineWidth * scaledFactor, 20.0f * scaledFactor);
    Renderer->DrawSprite(shortenedLine, lineShadowRect, shadowTint, -angle);
    RectF lineRect(xPos * scaledFactor, yPos * scaledFactor,
                   lineWidth * scaledFactor, 10.0f * scaledFactor);
    Renderer->DrawSprite(shortenedLine, lineRect, color, -angle);
  }
}

void MapSystemCCLCC::MapPartsSort() {
  if (MapPartsMax == 0) return;
  auto partsEnd = std::remove_if(
      MapPartsDisp.begin(), MapPartsDisp.begin() + MapPartsMax,
      [](MapPartsDispStruct& part) { return part.state == Hidden; });
  for (auto it = partsEnd; it != MapPartsDisp.begin() + MapPartsMax; ++it) {
    it->partId = 0xff;
  }
  MapPartsMax = static_cast<int>(partsEnd - MapPartsDisp.begin());
}

void MapSystemCCLCC::MapSetPos(float dt) {
  float mapScaler = (ScrWork[6362] < 100)   ? 100.0f
                    : (ScrWork[6362] > 800) ? 800.0f
                                            : toFlt(ScrWork[6362]);
  float MapSheetWidth = MapBgSprite.Sheet.DesignWidth;
  float MapSheetHeight = MapBgSprite.Sheet.DesignHeight;

  MapBGWidth = MapBgSprite.Sheet.DesignWidth * 100.0f / mapScaler;
  MapBGHeight = MapBgSprite.Sheet.DesignHeight * 100.0f / mapScaler;

  mapPosX = ScrWork[0x18db] - MapBGWidth / 2;
  mapPosY = ScrWork[0x18dc] - MapBGHeight / 2;

  if (!GetFlag(2805)) {
    if (mapPosX > MapSheetWidth - MapBGWidth) {
      mapPosX = MapSheetWidth - MapBGWidth;
    }
    if (mapPosY > MapSheetHeight - MapBGHeight) {
      mapPosY = MapSheetHeight - MapBGHeight;
    }
    if (mapPosX < 0.0) {
      mapPosX = 0.0;
    }
    if (mapPosY < 0.0) {
      mapPosY = 0.0;
    }
  }

  // MapScaleMode use??
  MapFadeMain(dt);

  for (int i = 0; i < MapPoolCurCt.size(); ++i) {
    if (i == hoverMapPoolIdx) {
      MapPoolCurCt[i] += (MapPoolCurCt[i] >= 16) ? 0 : 2;
    } else if (MapPoolCurCt[i]) {
      MapPoolCurCt[i] -= 2;
    }
  }
}

void MapSystemCCLCC::MapFadeMain(float dt) {
  for (int i = 0; i < MapPartsMax; ++i) {
    auto& partsDispElem = MapPartsDisp[i];
    if (partsDispElem.state == Shown || partsDispElem.state == Hidden) {
      continue;
    }
    if (!GetFlag(SF_MESALLSKIP) &&
        (partsDispElem.delay == 0 || --partsDispElem.delay == 0)) {
      if (partsDispElem.state == Showing) {
        if (partsDispElem.fadeAnim.IsIn()) {
          partsDispElem.state = Shown;
        } else if (partsDispElem.fadeAnim.State == AS_Stopped) {
          if (partsDispElem.type == 0 ||
              partsDispElem.type >= 8 && partsDispElem.type <= 11) {
            Io::InputStream* stream;
            if (Io::VfsOpen("sysse", 7, &stream) == IoError_OK)
              Audio::Channels[Audio::AC_SSE].Play(
                  Audio::AudioStream::Create(stream), false, 0.0f);
          }
          partsDispElem.fadeAnim.StartIn(true);
        }
      } else if (partsDispElem.state == Hiding) {
        if (partsDispElem.fadeAnim.IsOut()) {
          partsDispElem.state = Hidden;
        } else if (partsDispElem.fadeAnim.State == AS_Stopped) {
          if (partsDispElem.type == 0 ||
              partsDispElem.type >= 8 && partsDispElem.type <= 11) {
            Io::InputStream* stream;
            if (Io::VfsOpen("sysse", 9, &stream) == IoError_OK)
              Audio::Channels[Audio::AC_SSE].Play(
                  Audio::AudioStream::Create(stream), false, 0.0f);
          }
          partsDispElem.fadeAnim.StartOut(true);
        }
      }
      partsDispElem.fadeAnim.Update(dt);

    } else if (GetFlag(SF_MESALLSKIP)) {
      partsDispElem.state = partsDispElem.state == Showing ? Shown : Hidden;
      partsDispElem.delay = 0;
    }
  }
  MapPartsSort();

  for (int i = 0; i < MapPoolDisp.size(); ++i) {
    auto& poolDispElem = MapPoolDisp[i];
    if (poolDispElem.state == Shown || poolDispElem.state == Hidden) {
      continue;
    }
    // MapFadeFl = 1;
    if (GetFlag(SF_MESALLSKIP)) {
      poolDispElem.state = poolDispElem.state == Showing ? Shown : Hidden;
      poolDispElem.delay = 0;
      continue;
    }
    if (poolDispElem.delay == 0 || --poolDispElem.delay == 0) {
      if (poolDispElem.state == Showing) {
        if (poolDispElem.fadeAnim.IsIn()) {
          poolDispElem.state = Shown;
        } else if (poolDispElem.fadeAnim.State == AS_Stopped) {
          if (i % 2 == 1) {
            Io::InputStream* stream;
            int64_t err = Io::VfsOpen("sysse", 7, &stream);
            if (err == IoError_OK) {
              Audio::Channels[Audio::AC_SSE].Play(
                  Audio::AudioStream::Create(stream), false, 0.0f);
            }
          }
          poolDispElem.fadeAnim.StartIn(true);
        }

      } else if (poolDispElem.state == Hiding) {
        if (poolDispElem.fadeAnim.IsOut()) {
          poolDispElem.state = Hidden;
        } else if (poolDispElem.fadeAnim.State == AS_Stopped) {
          if (i % 2 == 1) {
            Io::InputStream* stream;
            int64_t err = Io::VfsOpen("sysse", 9, &stream);
            if (err == IoError_OK) {
              Audio::Channels[Audio::AC_SSE].Play(
                  Audio::AudioStream::Create(stream), false, 0.0f);
            }
          }
          poolDispElem.fadeAnim.StartOut(true);
        }
      }
      poolDispElem.fadeAnim.Update(dt);
    }
  }
}

void MapSystemCCLCC::Update(float dt) {
  if (ScrWork[6360] && GetFlag(2800)) {
    MapSetPos(dt);
  }

  if (ScrWork[0x1964] == 0) {
    if (ScrWork[0x1965] == 0) {
      ScrWork[0x1966] = 0;
    } else {
      ScrWork[0x1965]--;
    }
  } else {
    ScrWork[0x1966] = ScrWork[0x1964];
    if (ScrWork[0x1965] < 0x20) {
      ScrWork[0x1965]++;
    }
  }
}

void MapSystemCCLCC::RenderButtonGuide() {
  if (!GetFlag(1244) && !GetFlag(2487)) {
    if (ScrWork[6501] != 0) {
      float guideXWidth = (ScrWork[6501] * 1920.0f) / 32.0f;
      MapButtonGuideSprite.Bounds.Width = guideXWidth;
      Renderer->DrawSprite(MapButtonGuideSprite, glm::vec2{0.0f, 988.0f});
      if (guideXWidth < 1920) {
        Sprite MapButtonGuideSprite2 = MapButtonGuideSprite;
        std::array<glm::vec4, 4> tints = {
            glm::vec4{1.0f, 1.0f, 1.0f, 0.0f},
            glm::vec4{1.0f, 1.0f, 1.0f, 1.0f},
            glm::vec4{1.0f, 1.0f, 1.0f, 1.0f},
            glm::vec4{1.0f, 1.0f, 1.0f, 0.0f},
        };
        MapButtonGuideSprite2.Bounds.X = guideXWidth;
        MapButtonGuideSprite2.Bounds.Width = 60;

        std::array<glm::vec2, 4> dest = {
            glm::vec2{guideXWidth, 1042.0f},       // bottom left
            glm::vec2{guideXWidth + 60, 1042.0f},  // bottom right
            glm::vec2{guideXWidth, 988.0f},        // top left
            glm::vec2{guideXWidth + 60, 988.0f},   // top right
        };
        Renderer->DrawSprite(MapButtonGuideSprite2, dest, tints, 0, false);
      }
    }
  }
}

void MapSystemCCLCC::Render() {
  if (!GetFlag(2800)) {
    return;
  }
  // Render map bg
  MapBgSprite.Bounds =
      Rect(static_cast<int>(mapPosX), static_cast<int>(mapPosY),
           static_cast<int>(MapBGWidth), static_cast<int>(MapBGHeight));
  Renderer->DrawSprite(MapBgSprite, Rect(0, 0, 1920, 1080), glm::vec4{1.0f},
                       0.0f);

  // Render map parts
  for (int i = 0; i < MapPartsMax; ++i) {
    if (MapPartsDisp[i].state == Shown ||
        MapPartsDisp[i].state == Showing && MapPartsDisp[i].delay == 0 ||
        MapPartsDisp[i].state == Hiding) {
      switch (MapPartsDisp[i].type) {
        case 1:
          MapDispPhoto(i, 1);
          break;
        case 2:
          if (PartsOffsetData[MapPartsDisp[i].partId][1][0] ||
              PartsOffsetData[MapPartsDisp[i].partId][1][1]) {
            MapDispPhoto(i, 2);
          }
          break;
        case 3:
          if (PartsOffsetData[MapPartsDisp[i].partId][1][2] ||
              PartsOffsetData[MapPartsDisp[i].partId][1][3]) {
            MapDispPhoto(i, 3);
          }
          break;
        case 4:
          MapDispArticle(i);
          break;
        case 5:
          MapDispTag(i);
          break;
        case 6:
          if (MapPartsDisp[i].partId > 0 ||
              PartsIdMap[MapPartsDisp[i].partId][0] != 0xff)
            MapDispLine(i, 6);
          break;
        case 7:
          MapDispLine(i, 7);
          break;
        case 12:
          if (MapPartsDisp[i].partId > 0 ||
              PartsIdMap[MapPartsDisp[i].partId][1] != 0xff)
            MapDispLine(i, 12);
          break;
        case 13:
          if (MapPartsDisp[i].partId > 0 ||
              PartsIdMap[MapPartsDisp[i].partId][2] != 0xff)
            MapDispLine(i, 13);
          break;
        case 14:
          if (MapPartsDisp[i].partId > 0 ||
              PartsIdMap[MapPartsDisp[i].partId][3] != 0xff)
            MapDispLine(i, 14);
          break;
        default:
          break;
      }
    }
  }
  for (int i = 0; i < MapPartsMax; ++i) {
    if (MapPartsDisp[i].state == Shown ||
        MapPartsDisp[i].state == Showing && MapPartsDisp[i].delay == 0 ||
        MapPartsDisp[i].state == Hiding) {
      if (MapPartsDisp[i].type == 0 || MapPartsDisp[i].type == 8 ||
          MapPartsDisp[i].type == 11 ||
          MapPartsDisp[i].type == 9 &&
              (PartsOffsetData[MapPartsDisp[i].partId][1][0] ||
               PartsOffsetData[MapPartsDisp[i].partId][1][1]) ||
          MapPartsDisp[i].type == 10 &&
              (PartsOffsetData[MapPartsDisp[i].partId][1][2] ||
               PartsOffsetData[MapPartsDisp[i].partId][1][3])) {
        MapDispPin(i);
      }
    }
  }
  // Render map pool
  for (int i = 0; i < 20; ++i) {
    if (MapPoolDisp[i * 2].state == Shown ||
        MapPoolDisp[i * 2].state == Hiding ||
        MapPoolDisp[i * 2].state == Showing && MapPoolDisp[i * 2].delay == 0) {
      MapPoolDispPhoto(i);
    }
    if (MapPoolDisp[i * 2 + 1].state == Shown ||
        MapPoolDisp[i * 2 + 1].state == Hiding ||
        MapPoolDisp[i * 2 + 1].state == Showing &&
            MapPoolDisp[i * 2 + 1].delay == 0) {
      MapPoolDispPin(i);
    }
  }
}
}  // namespace CCLCC
}  // namespace UI
}  // namespace Impacto