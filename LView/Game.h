#pragma once
#include "MemSnapshot.h"
#include "Utils.h"
#include "Vector.h"
#include "imgui.h"
#include <map>
#include <memory>
#include <vector>

/// Interface for game related stuff
class Game {
 public:
  std::map<int, float> distanceCache;
  MemSnapshot* ms;
  ImDrawList* overlay;

 public:
  Game() {}

  // Exposed Fields
  std::vector<GameObject> champs, minions, turrets, jungle, missiles, others;
  float gameTime;

  std::shared_ptr<MapObject> map;
  std::shared_ptr<GameObject> hoveredObject;
  std::shared_ptr<GameObject> localChampion;

  // Exposed methods
  Vector2 WorldToScreen(const Vector3& pos) {
    return ms->renderer->WorldToScreen(pos);
  }

  Vector2 WorldToMinimap(const Vector3& pos) {
    return ms->renderer->WorldToMinimap(pos, ms->minimapPos, ms->minimapSize);
  }

  float DistanceToMinimap(float dist) {
    return ms->renderer->DistanceToMinimap(dist, ms->minimapSize);
  }

  bool IsScreenPointOnScreen(const Vector2& point, float offsetX = 0.f,
                             float offsetY = 0.f) {
    return ms->renderer->IsScreenPointOnScreen(point, offsetX, offsetY);
  }

  bool IsWorldPointOnScreen(const Vector3& point, float offsetX = 0.f,
                            float offsetY = 0.f) {
    return ms->renderer->IsWorldPointOnScreen(point, offsetX, offsetY);
  }

  void DrawCircle(const Vector2& center, float radius, int numPoints,
                  float thickness, const ImVec4& color) {
    overlay->AddCircle(ImVec2(center.x, center.y), radius, ImColor(color),
                       numPoints, thickness);
  }

  void DrawCircleFilled(const Vector2& center, float radius, int numPoints,
                        const ImVec4& color) {
    overlay->AddCircleFilled(ImVec2(center.x, center.y), radius, ImColor(color),
                             numPoints);
  }

  void DrawTxt(const Vector2& pos, const char* text, const ImVec4& color) {
    overlay->AddText(ImVec2(pos.x, pos.y), ImColor(color), text);
  }

  void DrawRect(const Vector4& box, const ImVec4& color, float rounding = 0,
                float thickness = 1.0) {
    overlay->AddRect(ImVec2(box.x, box.y), ImVec2(box.z, box.w), ImColor(color),
                     rounding, 15, thickness);
  }

  void DrawRectFilled(const Vector4& box, const ImVec4& color,
                      float rounding = 0) {
    overlay->AddRectFilled(ImVec2(box.x, box.y), ImVec2(box.z, box.w),
                           ImColor(color), rounding);
  }

  void DrawRectWorld(const Vector3& p1, const Vector3& p2, const Vector3& p3,
                     const Vector3& p4, float thickness, const ImVec4& color) {
    static Vector2 points[4];
    points[0] = ms->renderer->WorldToScreen(p1);
    points[1] = ms->renderer->WorldToScreen(p2);
    points[2] = ms->renderer->WorldToScreen(p3);
    points[3] = ms->renderer->WorldToScreen(p4);

    overlay->AddPolyline((ImVec2*)points, 4, ImColor(color), true, thickness);
  }

  void DrawTriangleWorld(const Vector3& p1, const Vector3& p2,
                         const Vector3& p3, float thickness,
                         const ImVec4& color) {
    overlay->AddTriangle((ImVec2&)ms->renderer->WorldToScreen(p1),
                         (ImVec2&)ms->renderer->WorldToScreen(p2),
                         (ImVec2&)ms->renderer->WorldToScreen(p3),
                         ImColor(color), thickness);
  }

  void DrawTriangleWorldFilled(const Vector3& p1, const Vector3& p2,
                               const Vector3& p3, const ImVec4& color) {
    overlay->AddTriangleFilled((ImVec2&)ms->renderer->WorldToScreen(p1),
                               (ImVec2&)ms->renderer->WorldToScreen(p2),
                               (ImVec2&)ms->renderer->WorldToScreen(p3),
                               ImColor(color));
  }

  void DrawCircleWorld(const Vector3& center, float radius, int numPoints,
                       float thickness, const ImVec4& color) {
    ms->renderer->DrawCircleAt(overlay, center, radius, false, numPoints,
                               ImColor(color), thickness);
  }

  void DrawCircleWorldFilled(const Vector3& center, float radius, int numPoints,
                             const ImVec4& color) {
    ms->renderer->DrawCircleAt(overlay, center, radius, true, numPoints,
                               ImColor(color));
  }

  void DrawLine(const Vector2& start, const Vector2& end, float thickness,
                const ImVec4& color) {
    overlay->AddLine((const ImVec2&)start, (const ImVec2&)end, ImColor(color),
                     thickness);
  }

  void DrawImage(const char* img, const Vector2& start, const Vector2& end,
                 const ImVec4& color) {
    static ImVec2 zero = ImVec2(0.f, 0.f);
    static ImVec2 one = ImVec2(1.f, 1.f);

    auto it = GameData::Images.find(std::string(img));
    if (it == GameData::Images.end()) return;
    overlay->AddImage(it->second->resourceView, (ImVec2&)start, (ImVec2&)end,
                      zero, one, ImColor(color));
  }

  void DrawImageRounded(const char* img, const Vector2& start,
                        const Vector2& end, const ImVec4& color,
                        float rounding) {
    static ImVec2 zero = ImVec2(0.f, 0.f);
    static ImVec2 one = ImVec2(1.f, 1.f);

    auto it = GameData::Images.find(std::string(img));
    if (it == GameData::Images.end()) return;
    overlay->AddImageRounded(it->second->resourceView, (ImVec2&)start,
                             (ImVec2&)end, zero, one, ImColor(color), rounding);
  }

  void DrawButton(const Vector2& p, const char* text, ImVec4& colorButton,
                  ImVec4& colorText, float rounding = 0) {
    int txtSize = strlen(text);
    overlay->AddRectFilled(ImVec2(p.x, p.y),
                           ImVec2(p.x + txtSize * 7.2f + 5, p.y + 17),
                           ImColor(colorButton), rounding);
    overlay->AddText(ImVec2(p.x + 5, p.y + 2), ImColor(colorText), text);
  }

  Vector2 HpBarPos(GameObject& obj) {
    Vector3 pos = obj.position.clone();
    pos.y += obj.GetHpBarHeight();

    Vector2 w2s = ms->renderer->WorldToScreen(pos);
    w2s.y -= (ms->renderer->height * 0.00083333335f * obj.GetHpBarHeight());

    return w2s;
  }

  void PressKey(int key) { Input::PressKey((HKey)key); }

  bool WasKeyPressed(int key) { return Input::WasKeyPressed((HKey)key); }

  void PressLeftClick() { Input::PressLeftClick(); }

  void PressRightClick() { Input::PressRightClick(); }

  void ClickAt(bool leftClick, const Vector2& pos) {
    Input::ClickAt(leftClick, pos.x, pos.y);
  }

  bool IsKeyDown(int key) { return Input::IsKeyDown((HKey)key); }

  Vector2 GetCursor() { return Input::GetCursorPosition(); }

  SpellInfo* GetSpellInfo(const char* spellName) {
    std::string name(spellName);
    return GameData::GetSpellInfoByName(name);
  }

  Vector2 LinearCollision(const Vector2& p1, const Vector2& d1,
                          const Vector2& p2, const Vector2& d2, float radius) {
    static float Ax, Bx, Cx, Ay, By, Cy;
    static float a, b, c, delta;
    static float sqrt_d, t1, t2;

    Ax = pow((d1.x - d2.x), 2.f);
    Bx = p1.x * d1.x - p1.x * d2.x - p2.x * d1.x + p2.x * d2.x;
    Cx = pow((p1.x - p2.x), 2.f);

    Ay = pow((d1.y - d2.y), 2.f);
    By = p1.y * d1.y - p1.y * d2.y - p2.y * d1.y + p2.y * d2.y;
    Cy = pow((p1.y - p2.y), 2.f);

    a = Ax + Ay;
    b = 2.f * (Bx + By);
    c = Cx + Cy - pow(radius, 2.f);
    delta = b * b - 4.f * a * c;

    if (a == 0.f || delta < 0.f) return Vector2(-1.f, -1.f);

    sqrt_d = sqrt(delta);
    t1 = (-b + sqrt_d) / (2.f * a);
    t2 = (-b - sqrt_d) / (2.f * a);

    return Vector2(t1, t2);
  }

  void MoveCursor(const Vector2& pos) { Input::MoveCursorTo(pos.x, pos.y); }

  float Distance(GameObject* first, GameObject* second) {
    int key = (first->objectIndex > second->objectIndex)
                  ? (first->objectIndex << 16) | second->objectIndex
                  : (second->objectIndex << 16) | first->objectIndex;

    auto it = distanceCache.find(key);
    if (it != distanceCache.end()) return it->second;

    float dist = first->position.distance(second->position);
    distanceCache[key] = dist;

    return dist;
  }

  GameObject* GetObjectByIndex(short index) {
    auto it = ms->indexToNetId.find(index);
    if (it == ms->indexToNetId.end()) return nullptr;

    auto it2 = ms->objectMap.find(it->second);
    if (it2 == ms->objectMap.end()) return nullptr;

    return it2->second.get();
  }

  GameObject* GetObjectByNetId(int net_id) {
    auto it = ms->objectMap.find(net_id);
    return (it != ms->objectMap.end()) ? it->second.get() : nullptr;
  }

  static Game ConstructFromMemSnapshot(MemSnapshot& snapshot) {
    Game game;

    game.ms = &snapshot;
    game.gameTime = snapshot.gameTime;
    game.hoveredObject = snapshot.hoveredObject;
    game.localChampion = snapshot.player;
    game.map = snapshot.map;

    for (std::shared_ptr<GameObject> champion : snapshot.champions) {
      game.champs.push_back(*champion);
    }
    for (std::shared_ptr<GameObject> minion : snapshot.minions) {
      game.minions.push_back(*minion);
    }
    for (std::shared_ptr<GameObject> turret : snapshot.turrets) {
      game.turrets.push_back(*turret);
    }
    for (std::shared_ptr<GameObject> jungle : snapshot.jungle) {
      game.jungle.push_back(*jungle);
    }
    for (std::shared_ptr<GameObject> missile : snapshot.missiles) {
      game.minions.push_back(*missile);
    }
    for (std::shared_ptr<GameObject> other : snapshot.others) {
      game.others.push_back(*other);
    }
    return game;
  }
};