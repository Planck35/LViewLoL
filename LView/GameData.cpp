#include "GameData.h"

#include <json/json.h>
#include <json/value.h>

#include <filesystem>
#include <fstream>

#include "Overlay.h"
#include "Utils.h"

using namespace std;

namespace {

Json::Value ParseFromJsonFile(const std::string& path) {
  std::ifstream input_data(path);

  Json::Reader reader;
  Json::Value result;
  if (!reader.parse(input_data, result)) {
    throw std::runtime_error("Failed to parse JSON file");
  }
  return result;
}

}  // namespace

UnitInfo* GameData::UnknownUnit = new UnitInfo();
SpellInfo* GameData::UnknownSpell = new SpellInfo();
ItemInfo* GameData::UnknownItem = new ItemInfo();
std::map<std::string, UnitInfo*> GameData::Units = {};
std::map<std::string, SpellInfo*> GameData::Spells = {};
std::map<std::string, Texture2D*> GameData::Images = {};
std::map<int, ItemInfo*> GameData::Items = {};

void GameData::Load(const std::string& dataFolder) {
  std::string unitData = dataFolder + "/UnitData.json";
  std::string spellData = dataFolder + "/SpellData.json";
  std::string spellDataCustom = dataFolder + "/SpellDataCustom.json";
  std::string itemData = dataFolder + "/ItemData.json";
  std::string spellIcons = dataFolder + "/icons_spells";
  std::string champIcons = dataFolder + "/icons_champs";
  std::string extraIcons = dataFolder + "/icons_extra";

  printf("\r	Loading item data    \n");
  LoadItemData(itemData);

  printf("\r	Loading unit data    \n");
  LoadUnitData(unitData);

  printf("\r	Loading spell data   \n");
  LoadSpellData(spellData);
  LoadSpellData(spellDataCustom);

  printf("\r	Loading images      \n");
  LoadIcons(spellIcons);
  LoadIcons(champIcons);
  LoadIcons(extraIcons);

  printf("\r	Loading complete                             \n");
}

UnitInfo* GameData::GetUnitInfoByName(const std::string& name) {
  auto it = Units.find(name);
  if (it != Units.end()) return it->second;
  return UnknownUnit;
}

SpellInfo* GameData::GetSpellInfoByName(const std::string& name) {
  auto it = Spells.find(name);
  if (it != Spells.end()) return it->second;
  return UnknownSpell;
}

ItemInfo* GameData::GetItemInfoById(int id) {
  auto it = Items.find(id);
  if (it != Items.end()) return it->second;
  return UnknownItem;
}

void GameData::LoadUnitData(const std::string& path) {
  Json::Value json_value = ParseFromJsonFile(path);

  if (!json_value.isArray()) return;

  for (int i = 0; i < json_value.size(); ++i) {
    const auto& unit_json = json_value[i];
    if (unit_json.isObject()) {
      UnitInfo* unit = new UnitInfo();
      unit->name = Character::ToLower(unit_json["name"].asCString());
      unit->acquisitionRange = unit_json["acquisitionRange"].asFloat();
      unit->attackSpeedRatio = unit_json["attackSpeedRatio"].asFloat();
      unit->baseAttackRange = unit_json["attackRange"].asFloat();
      unit->baseAttackSpeed = unit_json["attackSpeed"].asFloat();
      unit->baseMovementSpeed = unit_json["baseMoveSpeed"].asFloat();
      unit->basicAttackMissileSpeed =
          unit_json["basicAtkMissileSpeed"].asFloat();
      unit->basicAttackWindup = unit_json["basicAtkWindup"].asFloat();
      unit->gameplayRadius = unit_json["gameplayRadius"].asFloat();
      unit->pathRadius = unit_json["pathingRadius"].asFloat();
      unit->selectionRadius = unit_json["selectionRadius"].asFloat();

      if (unit_json["tags"].isArray()) {
        for (int j = 0; j < unit_json["tag"].size(); ++j) {
          unit->SetTag(unit_json["tag"][j].asCString());
        }
      }
      Units[unit->name] = unit;
    }
  }
}

void GameData::LoadSpellData(const std::string& path) {
  Json::Value json_value = ParseFromJsonFile(path);
  if (!json_value.isArray()) return;

  for (int i = 0; i < json_value.size(); ++i) {
    const auto& spell_json = json_value[i];
    if (spell_json.isObject()) {
      SpellInfo* spell_info = new SpellInfo();
      spell_info->name = Character::ToLower(spell_json["name"].asCString());
      spell_info->flags = (SpellFlags)spell_json["flags"].asInt64();
      spell_info->delay = spell_json["delay"].asFloat();
      spell_info->icon = Character::ToLower(spell_json["icon"].asCString());
      spell_info->height = spell_json["height"].asFloat();
      spell_info->width = spell_json["width"].asFloat();
      spell_info->castRange = spell_json["castRange"].asFloat();
      spell_info->castRadius = spell_json["castRadius"].asFloat();
      spell_info->speed = spell_json["speed"].asFloat();
      spell_info->travelTime = spell_json["travelTime"].asFloat();
      spell_info->flags =
          (SpellFlags)(spell_info->flags |
                       (spell_json["projectDestination"].asBool()
                            ? ProjectedDestination
                            : 0));
      Spells[spell_info->name] = spell_info;
    }
  }
}

void GameData::LoadIcons(const std::string& path) {
  std::string folder(path);
  WIN32_FIND_DATAA findData;
  HANDLE hFind;

  int nrFiles = std::distance(std::filesystem::directory_iterator(path),
                              std::filesystem::directory_iterator());
  int nrFile = 0;
  hFind = FindFirstFileA((folder + "\\*.png").c_str(), &findData);
  do {
    if (hFind != INVALID_HANDLE_VALUE) {
      if (nrFile % 100 == 0)
        printf("\r	Loading %d/%d      ", nrFile, nrFiles);

      std::string filePath = folder + "/" + findData.cFileName;
      Texture2D* image =
          Texture2D::LoadFromFile(Overlay::GetDxDevice(), filePath);
      if (image == nullptr)
        printf("Failed to load: %s\n", filePath.c_str());
      else {
        std::string fileName(findData.cFileName);
        fileName.erase(fileName.find(".png"), 4);
        Images[Character::ToLower(fileName)] = image;
      }
    }
    nrFile++;
  } while (FindNextFileA(hFind, &findData));
}

void GameData::LoadItemData(const std::string& path) {
  Json::Value json_value = ParseFromJsonFile(path);
  if (!json_value.isArray()) return;

  for (int i = 0; i < json_value.size(); ++i) {
    const auto& item_json = json_value[i];
    if (item_json.isObject()) {
      ItemInfo* item_info = new ItemInfo();

      item_info->movementSpeed = item_json["movementSpeed"].asFloat();
      item_info->health = item_json["health"].asFloat();
      item_info->crit = item_json["crit"].asFloat();
      item_info->abilityPower = item_json["abilityPower"].asFloat();
      item_info->mana = item_json["mana"].asFloat();
      item_info->armour = item_json["armour"].asFloat();
      item_info->magicResist = item_json["magicResist"].asFloat();
      item_info->physicalDamage = item_json["physicalDamage"].asFloat();
      item_info->attackSpeed = item_json["attackSpeed"].asFloat();
      item_info->lifeSteal = item_json["lifeSteal"].asFloat();
      item_info->hpRegen = item_json["hpRegen"].asFloat();
      item_info->movementSpeedPercent =
          item_json["movementSpeedPercent"].asFloat();
      item_info->cost = item_json["cost"].asFloat();
      item_info->id = item_json["id"].asInt64();

      Items[item_info->id] = item_info;
    }
  }
}
