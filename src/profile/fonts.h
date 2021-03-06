#pragma once

#include "../font.h"
#include <flat_hash_map.hpp>

namespace Impacto {
namespace Profile {

extern ska::flat_hash_map<std::string, Font*> Fonts;

void LoadFonts();

}  // namespace Profile
}  // namespace Impacto