#pragma once

#include <boost/python.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>

#include "ConfigSet.h"
#include "GameObject.h"
#include "ItemInfo.h"
#include "PyGame.h"
#include "PyImguiInterface.h"
#include "Spell.h"
#include "Utils.h"

using namespace boost::python;

/// Defines the mapping between the C++ and Python classes
BOOST_PYTHON_MODULE(lview) {
  class_<SpellInfo>("SpellInfo")
      .def_readonly("width", &SpellInfo::width)
      .def_readonly("cast_radius", &SpellInfo::castRadius)
      .def_readonly("speed", &SpellInfo::speed)
      .def_readonly("cast_range", &SpellInfo::castRange)
      .def_readonly("delay", &SpellInfo::delay)
      .def_readonly("height", &SpellInfo::height)
      .def_readonly("icon", &SpellInfo::icon)
      .def_readonly("travel_time", &SpellInfo::travelTime);

  class_<ItemSlot>("Item")
      .def_readonly("slot", &ItemSlot::slot)
      .def_readonly("id", &ItemSlot::GetId)
      .def_readonly("cost", &ItemSlot::GetCost)
      .def_readonly("movement_speed", &ItemSlot::GetMovementSpeed)
      .def_readonly("health", &ItemSlot::GetHealth)
      .def_readonly("crit", &ItemSlot::GetCrit)
      .def_readonly("ability_power", &ItemSlot::GetAbilityPower)
      .def_readonly("mana", &ItemSlot::GetMana)
      .def_readonly("armour", &ItemSlot::GetArmour)
      .def_readonly("magic_resist", &ItemSlot::GetMagicResist)
      .def_readonly("physical_damage", &ItemSlot::GetPhysicalDamage)
      .def_readonly("attack_speed", &ItemSlot::GetAttackSpeed)
      .def_readonly("life_steal", &ItemSlot::GetLifeSteal)
      .def_readonly("hp_regen", &ItemSlot::GetHpRegen)
      .def_readonly("movement_speed_percent",
                    &ItemSlot::GetMovementSpeedPercent);

  class_<Spell>("Spell", init<SpellSlot>())
      .def_readonly("name", &Spell::name)
      .def_readonly("slot", &Spell::slot)
      .def_readonly("summoner_spell_type", &Spell::summonerSpellType)
      .def_readonly("level", &Spell::level)
      .def_readonly("ready_at", &Spell::readyAt)
      .def_readonly("value", &Spell::value)

      .def("get_current_cooldown", &Spell::GetRemainingCooldown)
      .def("trigger", &Spell::Trigger)

      .def_readonly("width", &Spell::GetWidth)
      .def_readonly("cast_radius", &Spell::GetCastRadius)
      .def_readonly("speed", &Spell::GetSpeed)
      .def_readonly("cast_range", &Spell::GetCastRange)
      .def_readonly("delay", &Spell::GetDelay)
      .def_readonly("height", &Spell::GetHeight)
      .def_readonly("icon", &Spell::GetIcon)
      .def_readonly("travel_time", &Spell::GetTravelTime)
      .def("has_tags", &Spell::HasSpellFlags)
      .def("equal_tags", &Spell::EqualSpellFlags);

  class_<GameObject>("Obj")
      .def_readonly("address", &GameObject::address)
      .def_readonly("health", &GameObject::health)
      .def_readonly("max_health", &GameObject::maxHealth)
      .def_readonly("base_atk", &GameObject::baseAttack)
      .def_readonly("bonus_atk", &GameObject::bonusAttack)
      .def_readonly("armour", &GameObject::armour)
      .def_readonly("magic_resist", &GameObject::magicResist)
      .def_readonly("movement_speed", &GameObject::movementSpeed)
      .def_readonly("is_alive", &GameObject::isAlive)
      .def_readonly("name", &GameObject::name)
      .def_readonly("pos", &GameObject::position)
      .def_readonly("prev_pos", &GameObject::previousPosition)
      .def_readonly("duration", &GameObject::duration)
      .def_readonly("is_visible", &GameObject::isVisible)
      .def_readonly("last_visible_at", &GameObject::lastVisibleAt)
      .def_readonly("id", &GameObject::objectIndex)
      .def_readonly("net_id", &GameObject::networkId)
      .def_readonly("crit", &GameObject::crit)
      .def_readonly("crit_multi", &GameObject::critMulti)
      .def_readonly("ap", &GameObject::abilityPower)
      .def_readonly("atk_speed_multi", &GameObject::atkSpeedMulti)
      .def_readonly("team", &GameObject::team)

      .def_readonly("acquisition_radius", &GameObject::GetAcquisitionRadius)
      .def_readonly("selection_radius", &GameObject::GetSelectionRadius)
      .def_readonly("pathing_radius", &GameObject::GetPathingRadius)
      .def_readonly("gameplay_radius", &GameObject::GetGameplayRadius)

      .def_readonly("basic_missile_speed",
                    &GameObject::GetBasicAttackMissileSpeed)
      .def_readonly("basic_atk_windup", &GameObject::GetBasicAttackWindup)

      .def_readonly("atk_speed_ratio", &GameObject::GetAttackSpeedRatio)
      .def_readonly("base_ms", &GameObject::GetBaseMovementSpeed)
      .def_readonly("base_atk_speed", &GameObject::GetBaseAttackSpeed)
      .def_readonly("base_atk_range", &GameObject::GetBaseAttackRange)
      .def_readonly("atk_range", &GameObject::GetAttackRange)
      .def_readonly("is_ranged", &GameObject::IsRanged)

      .def("__eq__", &GameObject::IsEqualTo)
      .def("__ne__", &GameObject::IsNotEqualTo)
      .def("is_ally_to", &GameObject::IsAllyTo)
      .def("is_enemy_to", &GameObject::IsEnemyTo)
      .def("has_tags", &GameObject::HasUnitTags)

      // Champion
      .def_readonly("Q", &GameObject::Q)
      .def_readonly("W", &GameObject::W)
      .def_readonly("E", &GameObject::E)
      .def_readonly("R", &GameObject::R)
      .def_readonly("D", &GameObject::D)
      .def_readonly("F", &GameObject::F)
      .def_readonly("items", &GameObject::ItemsToPyList)
      .def_readonly("lvl", &GameObject::level)

      .def("get_summoner_spell", &GameObject::GetSummonerSpell,
           return_value_policy<reference_existing_object>())

      // Missile
      .def_readonly("src_id", &GameObject::srcIndex)
      .def_readonly("dest_id", &GameObject::destIndex)
      .def_readonly("start_pos", &GameObject::startPos)
      .def_readonly("end_pos", &GameObject::endPos)

      // Spell
      .def_readonly("width", &GameObject::GetWidth)
      .def_readonly("cast_radius", &GameObject::GetCastRadius)
      .def_readonly("speed", &GameObject::GetSpeed)
      .def_readonly("cast_range", &GameObject::GetCastRange)
      .def_readonly("delay", &GameObject::GetDelay)
      .def_readonly("height", &GameObject::GetHeight)
      .def_readonly("icon", &GameObject::GetIcon)
      .def_readonly("travel_time", &GameObject::GetTravelTime)
      .def("has_tags", &GameObject::HasSpellFlags)
      .def("equal_tags", &GameObject::EqualSpellFlags);

  enum_<SpellFlags>("SpellFlag")
      .value("AffectAllyChampion", SpellFlags::AffectAllyChampion)
      .value("AffectEnemyChampion", SpellFlags::AffectEnemyChampion)
      .value("AffectAllyLaneMinion", SpellFlags::AffectAllyLaneMinion)
      .value("AffectEnemyLaneMinion", SpellFlags::AffectEnemyLaneMinion)
      .value("AffectAllyWard", SpellFlags::AffectAllyWard)
      .value("AffectEnemyWard", SpellFlags::AffectEnemyWard)
      .value("AffectAllyTurret", SpellFlags::AffectAllyTurret)
      .value("AffectEnemyTurret", SpellFlags::AffectEnemyTurret)
      .value("AffectAllyInhibs", SpellFlags::AffectAllyInhibs)
      .value("AffectEnemyInhibs", SpellFlags::AffectEnemyInhibs)
      .value("AffectAllyNonLaneMinion", SpellFlags::AffectAllyNonLaneMinion)
      .value("AffectJungleMonster", SpellFlags::AffectJungleMonster)
      .value("AffectEnemyNonLaneMinion", SpellFlags::AffectEnemyNonLaneMinion)
      .value("AffectAlwaysSelf", SpellFlags::AffectAlwaysSelf)
      .value("AffectNeverSelf", SpellFlags::AffectNeverSelf)

      .value("ProjectDestination", SpellFlags::ProjectedDestination)

      .value("AffectAllyMob", SpellFlags::AffectAllyMob)
      .value("AffectEnemyMob", SpellFlags::AffectEnemyMob)
      .value("AffectAllyGeneric", SpellFlags::AffectAllyGeneric)
      .value("AffectEnemyGeneric", SpellFlags::AffectEnemyGeneric);

  class_<PyGame>("Game")
      .def_readonly("champs", &PyGame::champs)
      .def_readonly("minions", &PyGame::minions)
      .def_readonly("jungle", &PyGame::jungle)
      .def_readonly("turrets", &PyGame::turrets)
      .def_readonly("missiles", &PyGame::missiles)
      .def_readonly("others", &PyGame::others)
      .def_readonly("hovered_obj", &PyGame::GetHoveredObject)
      .def_readonly("player", &PyGame::GetLocalChampion)
      .def_readonly("time", &PyGame::gameTime)
      .def_readonly("map", &PyGame::GetMap)

      .def("get_obj_by_id", &PyGame::GetObjectByIndex,
           return_value_policy<reference_existing_object>())
      .def("get_obj_by_netid", &PyGame::GetObjectByNetId,
           return_value_policy<reference_existing_object>())

      .def("is_point_on_screen", &PyGame::IsScreenPointOnScreen,
           PyGame::IsScreenPointOnScreenOverloads())
      .def("is_point_on_screen", &PyGame::IsWorldPointOnScreen,
           PyGame::IsWorldPointOnScreenOverloads())
      .def("world_to_screen", &PyGame::WorldToScreen)
      .def("world_to_minimap", &PyGame::WorldToMinimap)
      .def("distance_to_minimap", &PyGame::DistanceToMinimap)
      .def("distance", &PyGame::Distance)

      .def("draw_line", &PyGame::DrawLine)
      .def("draw_circle", &PyGame::DrawCircle)
      .def("draw_circle_filled", &PyGame::DrawCircleFilled)
      .def("draw_circle_world", &PyGame::DrawCircleWorld)
      .def("draw_circle_world_filled", &PyGame::DrawCircleWorldFilled)
      .def("draw_text", &PyGame::DrawTxt)
      .def("draw_rect", &PyGame::DrawRect, PyGame::DrawRectOverloads())
      .def("draw_rect_filled", &PyGame::DrawRectFilled,
           PyGame::DrawRectFilledOverloads())
      .def("draw_rect_world", &PyGame::DrawRectWorld)
      .def("draw_triangle_world", &PyGame::DrawTriangleWorld)
      .def("draw_triangle_world_filled", &PyGame::DrawTriangleWorldFilled)
      .def("draw_button", &PyGame::DrawButton, PyGame::DrawButtonOverloads())
      .def("draw_image", &PyGame::DrawImage)
      .def("draw_image", &PyGame::DrawImageRounded)

      .def("get_spell_info", &PyGame::GetSpellInfo,
           return_value_policy<reference_existing_object>())
      .def("linear_collision", &PyGame::LinearCollision)
      .def("hp_bar_pos", &PyGame::HpBarPos)

      .def("was_key_pressed", &PyGame::WasKeyPressed)
      .def("is_key_down", &PyGame::IsKeyDown)
      .def("press_key", &PyGame::PressKey)
      .def("press_left_click", &PyGame::PressLeftClick)
      .def("press_right_click", &PyGame::PressRightClick)
      .def("click_at", &PyGame::ClickAt)
      .def("move_cursor", &PyGame::MoveCursor)
      .def("get_cursor", &PyGame::GetCursor);

  enum_<MapType>("MapType")
      .value("SummonersRift", MapType::SUMMONERS_RIFT)
      .value("HowlingAbyss", MapType::HOWLING_ABYSS);

  class_<MapObject>("Map")
      .def("height_at", &MapObject::GetHeightAt)
      .def_readonly("type", &MapObject::type);

  class_<PyImguiInterface>("UI")
      .def("begin", &PyImguiInterface::Begin)
      .def("end", &PyImguiInterface::End)

      .def("button", &PyImguiInterface::Button)
      .def("colorbutton", &PyImguiInterface::ColorButton)
      .def("colorpick", &PyImguiInterface::ColorPicker)
      .def("checkbox", &PyImguiInterface::Checkbox)
      .def("text", &PyImguiInterface::Text)
      .def("text", &PyImguiInterface::TextColored)
      .def("labeltext", &PyImguiInterface::LabelText)
      .def("labeltext", &PyImguiInterface::LabelTextColored)
      .def("separator", &PyImguiInterface::Separator)
      .def("dragint", &PyImguiInterface::DragInt,
           PyImguiInterface::DragIntOverloads())
      .def("dragfloat", &PyImguiInterface::DragFloat,
           PyImguiInterface::DragFloatOverloads())
      .def("keyselect", &PyImguiInterface::KeySelect)
      .def("sliderfloat", &PyImguiInterface::SliderFloat)

      .def("header", &PyImguiInterface::CollapsingHeader)
      .def("treenode", &PyImguiInterface::TreeNode)
      .def("treepop", &PyImguiInterface::TreePop)
      .def("opennext", &PyImguiInterface::SetNextItemOpen)

      .def("sameline", &PyImguiInterface::SameLine)
      .def("begingroup", &PyImguiInterface::BeginGroup)
      .def("endgroup", &PyImguiInterface::EndGroup)

      .def("listbox", &PyImguiInterface::ListBox);

  class_<ImVec4>("Color", init<float, float, float, float>())
      .def_readonly("BLACK", &Colors::BLACK)
      .def_readonly("WHITE", &Colors::WHITE)
      .def_readonly("RED", &Colors::RED)
      .def_readonly("DARK_RED", &Colors::DARK_RED)
      .def_readonly("GREEN", &Colors::GREEN)
      .def_readonly("DARK_GREEN", &Colors::DARK_GREEN)
      .def_readonly("YELLOW", &Colors::YELLOW)
      .def_readonly("DARK_YELLOW", &Colors::DARK_YELLOW)
      .def_readonly("CYAN", &Colors::CYAN)
      .def_readonly("PURPLE", &Colors::PURPLE)
      .def_readonly("GRAY", &Colors::GRAY)
      .def_readonly("ORANGE", &Colors::ORANGE)
      .def_readonly("BLUE", &Colors::BLUE)
      .def_readonly("BROWN", &Colors::BROWN)

      .def_readwrite("r", &ImVec4::x)
      .def_readwrite("g", &ImVec4::y)
      .def_readwrite("b", &ImVec4::z)
      .def_readwrite("a", &ImVec4::w);

  class_<Vector4>("Vec4", init<float, float, float, float>())
      .def_readwrite("x", &Vector4::x)
      .def_readwrite("y", &Vector4::y)
      .def_readwrite("z", &Vector4::z)
      .def_readwrite("w", &Vector4::w)
      .def("length", &Vector4::length)
      .def("normalize", &Vector4::normalize)
      .def("distance", &Vector4::distance)
      .def("scale", &Vector4::scale)
      .def("scale", &Vector4::vscale)
      .def("add", &Vector4::add)
      .def("sub", &Vector4::sub)
      .def("clone", &Vector4::clone);

  class_<Vector3>("Vec3", init<float, float, float>())
      .def_readwrite("x", &Vector3::x)
      .def_readwrite("y", &Vector3::y)
      .def_readwrite("z", &Vector3::z)
      .def("length", &Vector3::length)
      .def("normalize", &Vector3::normalize)
      .def("distance", &Vector3::distance)
      .def("scale", &Vector3::scale)
      .def("scale", &Vector3::vscale)
      .def("rotate_x", &Vector3::rotate_x)
      .def("rotate_y", &Vector3::rotate_y)
      .def("rotate_z", &Vector3::rotate_z)
      .def("add", &Vector3::add)
      .def("sub", &Vector3::sub)
      .def("clone", &Vector3::clone);

  class_<Vector2>("Vec2", init<float, float>())
      .def_readwrite("x", &Vector2::x)
      .def_readwrite("y", &Vector2::y)
      .def("length", &Vector2::length)
      .def("normalize", &Vector2::normalize)
      .def("distance", &Vector2::distance)
      .def("scale", &Vector2::scale)
      .def("scale", &Vector2::vscale)
      .def("add", &Vector2::add)
      .def("sub", &Vector2::sub)
      .def("clone", &Vector2::clone);

  class_<ConfigSet>("Config")
      .def("set_int", &ConfigSet::SetInt)
      .def("set_bool", &ConfigSet::SetBool)
      .def("set_float", &ConfigSet::SetFloat)
      .def("set_str", &ConfigSet::SetStr)
      .def("get_int", &ConfigSet::GetInt)
      .def("get_bool", &ConfigSet::GetBool)
      .def("get_float", &ConfigSet::GetFloat)
      .def("get_str", &ConfigSet::GetStr);

  enum_<SpellSlot>("SpellSlot")
      .value("Q", SpellSlot::Q)
      .value("W", SpellSlot::W)
      .value("E", SpellSlot::E)
      .value("R", SpellSlot::R)
      .value("D", SpellSlot::D)
      .value("F", SpellSlot::F);

  enum_<SummonerSpellType>("SummonerSpellType")
      .value("Ghost", SummonerSpellType::GHOST)
      .value("Heal", SummonerSpellType::HEAL)
      .value("Barrier", SummonerSpellType::BARRIER)
      .value("Exhaust", SummonerSpellType::EXHAUST)
      .value("Clarity", SummonerSpellType::CLARITY)
      .value("Snowball", SummonerSpellType::SNOWBALL)
      .value("Flash", SummonerSpellType::FLASH)
      .value("Teleport", SummonerSpellType::TELEPORT)
      .value("Cleanse", SummonerSpellType::CLEANSE)
      .value("Ignite", SummonerSpellType::IGNITE)
      .value("Smite", SummonerSpellType::SMITE)
      .value("None", SummonerSpellType::NONE);

  enum_<UnitTag>("UnitTag")
      .value("Unit_Champion", UnitTag::Unit_Champion)
      .value("Unit_Champion_Clone", UnitTag::Unit_Champion_Clone)
      .value("Unit_IsolationNonImpacting", UnitTag::Unit_IsolationNonImpacting)
      .value("Unit_KingPoro", UnitTag::Unit_KingPoro)
      .value("Unit_Minion", UnitTag::Unit_Minion)
      .value("Unit_Minion_Lane", UnitTag::Unit_Minion_Lane)
      .value("Unit_Minion_Lane_Melee", UnitTag::Unit_Minion_Lane_Melee)
      .value("Unit_Minion_Lane_Ranged", UnitTag::Unit_Minion_Lane_Ranged)
      .value("Unit_Minion_Lane_Siege", UnitTag::Unit_Minion_Lane_Siege)
      .value("Unit_Minion_Lane_Super", UnitTag::Unit_Minion_Lane_Super)
      .value("Unit_Minion_Summon", UnitTag::Unit_Minion_Summon)
      .value("Unit_Minion_Summon_Large", UnitTag::Unit_Minion_Summon_Large)
      .value("Unit_Monster", UnitTag::Unit_Monster)
      .value("Unit_Monster_Blue", UnitTag::Unit_Monster_Blue)
      .value("Unit_Monster_Buff", UnitTag::Unit_Monster_Buff)
      .value("Unit_Monster_Camp", UnitTag::Unit_Monster_Camp)
      .value("Unit_Monster_Crab", UnitTag::Unit_Monster_Crab)
      .value("Unit_Monster_Dragon", UnitTag::Unit_Monster_Dragon)
      .value("Unit_Monster_Epic", UnitTag::Unit_Monster_Epic)
      .value("Unit_Monster_Gromp", UnitTag::Unit_Monster_Gromp)
      .value("Unit_Monster_Krug", UnitTag::Unit_Monster_Krug)
      .value("Unit_Monster_Large", UnitTag::Unit_Monster_Large)
      .value("Unit_Monster_Medium", UnitTag::Unit_Monster_Medium)
      .value("Unit_Monster_Raptor", UnitTag::Unit_Monster_Raptor)
      .value("Unit_Monster_Red", UnitTag::Unit_Monster_Red)
      .value("Unit_Monster_Wolf", UnitTag::Unit_Monster_Wolf)
      .value("Unit_Plant", UnitTag::Unit_Plant)
      .value("Unit_Special", UnitTag::Unit_Special)
      .value("Unit_Special_AzirR", UnitTag::Unit_Special_AzirR)
      .value("Unit_Special_AzirW", UnitTag::Unit_Special_AzirW)
      .value("Unit_Special_CorkiBomb", UnitTag::Unit_Special_CorkiBomb)
      .value("Unit_Special_EpicMonsterIgnores",
             UnitTag::Unit_Special_EpicMonsterIgnores)
      .value("Unit_Special_KPMinion", UnitTag::Unit_Special_KPMinion)
      .value("Unit_Special_MonsterIgnores",
             UnitTag::Unit_Special_MonsterIgnores)
      .value("Unit_Special_Peaceful", UnitTag::Unit_Special_Peaceful)
      .value("Unit_Special_SyndraSphere", UnitTag::Unit_Special_SyndraSphere)
      .value("Unit_Special_TeleportTarget",
             UnitTag::Unit_Special_TeleportTarget)
      .value("Unit_Special_Trap", UnitTag::Unit_Special_Trap)
      .value("Unit_Special_Tunnel", UnitTag::Unit_Special_Tunnel)
      .value("Unit_Special_TurretIgnores", UnitTag::Unit_Special_TurretIgnores)
      .value("Unit_Special_UntargetableBySpells",
             UnitTag::Unit_Special_UntargetableBySpells)
      .value("Unit_Special_Void", UnitTag::Unit_Special_Void)
      .value("Unit_Special_YorickW", UnitTag::Unit_Special_YorickW)
      .value("Unit_Structure", UnitTag::Unit_Structure)
      .value("Unit_Structure_Inhibitor", UnitTag::Unit_Structure_Inhibitor)
      .value("Unit_Structure_Nexus", UnitTag::Unit_Structure_Nexus)
      .value("Unit_Structure_Turret", UnitTag::Unit_Structure_Turret)
      .value("Unit_Structure_Turret_Inhib",
             UnitTag::Unit_Structure_Turret_Inhib)
      .value("Unit_Structure_Turret_Inner",
             UnitTag::Unit_Structure_Turret_Inner)
      .value("Unit_Structure_Turret_Nexus",
             UnitTag::Unit_Structure_Turret_Nexus)
      .value("Unit_Structure_Turret_Outer",
             UnitTag::Unit_Structure_Turret_Outer)
      .value("Unit_Structure_Turret_Shrine",
             UnitTag::Unit_Structure_Turret_Shrine)
      .value("Unit_Ward", UnitTag::Unit_Ward);
}
