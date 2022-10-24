#pragma once
#include <map>

#include "ItemInfo.h"
#include "SpellInfo.h"
#include "Texture2D.h"
#include "UnitInfo.h"

/// Data that cant be read from memory or it is too inefficient to do so can be
/// accessed with this class.
class GameData {
 public:
  static void Load(const std::string& dataFolder);
  static UnitInfo* GetUnitInfoByName(const std::string& name);
  static SpellInfo* GetSpellInfoByName(const std::string& name);
  static ItemInfo* GetItemInfoById(int id);

 private:
  static void LoadUnitData(const std::string& path);
  static void LoadSpellData(const std::string& path);
  static void LoadIcons(const std::string& path);
  static void LoadItemData(const std::string& path);

 public:
  static UnitInfo* UnknownUnit;
  static SpellInfo* UnknownSpell;
  static ItemInfo* UnknownItem;

  static std::map<std::string, UnitInfo*> Units;
  static std::map<std::string, SpellInfo*> Spells;
  static std::map<std::string, Texture2D*> Images;
  static std::map<int, ItemInfo*> Items;
};