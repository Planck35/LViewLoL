#include "DebugView.h"


const char* DebugView::GetName() {
	return "Debug";
}


void DrawSpell(Spell spell) {
	if (ImGui::TreeNode(spell.GetTypeStr())) {
		
		ImGui::Button(spell.name.c_str());
		ImGui::LabelText("Address Slot", "%#010x", spell.addressSlot);
		ImGui::DragFloat("Ready At", &spell.readyAt);
		ImGui::DragInt("Level", &spell.level);
		ImGui::DragFloat("Damage", &spell.damage);
		ImGui::TreePop();
	}
}

void DrawMatrix(float* matrix, int rows, int cols) {
	for (int i = 0; i < rows; ++i) {
		ImGui::BeginGroup();
		for (int j = 0; j < cols; ++j) {
			ImGui::Button(std::to_string(matrix[i*rows + j]).c_str());
			ImGui::SameLine();
		}
		ImGui::EndGroup();
	}
}

void DrawGameObject(GameObject* obj, bool openItem = false) {


	if (obj == nullptr) {
		ImGui::TextColored((ImVec4)Colors::Red, "nullptr");
		return;
	}

	std::string nodeName(obj->name);
	nodeName = nodeName.append("_");
	nodeName = nodeName.append(std::to_string((unsigned int)obj->objectIndex));

	if(openItem)
		ImGui::SetNextItemOpen(true);
	if (ImGui::TreeNode(nodeName.c_str())) {

		int team = obj->team;
		int type = (int)obj->type;
		ImGui::LabelText("Address", "%#010x", obj->address);
		ImGui::DragInt("Type", &type);
		ImGui::DragInt("Team", &team);
		ImGui::DragFloat("Health", &obj->health);
		ImGui::DragFloat("Target Radius", &obj->targetRadius);
		ImGui::DragFloat("Gameplay Radius", &obj->gameplayRadius);
		ImGui::DragFloat("Attack Range", &obj->baseAttackRange);
		ImGui::DragFloat("Expires In", &obj->expiresIn);
		ImGui::DragFloat("Last Visible At", &obj->lastVisibleAt);
		ImGui::Checkbox("Is Alive", &obj->isAlive);
		ImGui::Checkbox("Is Visible", &obj->isVisible);
		ImGui::LabelText("Position", "X:%.2f Y:%.2f Z:%.2f", obj->position.x, obj->position.y, obj->position.z);

		ImGui::TreePop();
	}
}

void DrawGameObjects(const char* objectType, std::vector<GameObject*> gameObjects) {

	if (ImGui::TreeNode(objectType)) {
		int count = gameObjects.size();
		ImGui::DragInt("Count", &count);
		for (size_t i = 0; i < gameObjects.size(); ++i) {
			DrawGameObject(gameObjects[i], false);
			ImGui::Separator();
		}
		ImGui::TreePop();
	}

}

void DebugView::DrawPanel(const MemSnapshot& snapshot, const MiscToolbox& toolbox) {

	ImGui::Begin("Debug");

	ImGui::LabelText("GameTime", "%.2f", snapshot.gameTime);
	
	if (ImGui::TreeNode("Hovered Object")) {
		DrawGameObject(snapshot.hoveredObject, true);

		ImGui::TreePop();
	}

	// Draw renderer
	if (ImGui::TreeNode("Renderer")) {

		ImGui::DragInt("Width", &snapshot.renderer->width);
		ImGui::DragInt("Height", &snapshot.renderer->height);

		ImGui::Text("View Matrix");
		DrawMatrix(snapshot.renderer->viewMatrix, 4, 4);
		ImGui::Text("Projection Matrix");
		DrawMatrix(snapshot.renderer->projMatrix, 4, 4);
		ImGui::TreePop();
	}

	// Draw champs
	if (ImGui::TreeNode("Champions")) {
		for (auto it = snapshot.champions.begin(); it != snapshot.champions.end(); ++it) {
			Champion* champ = *it;
			if (ImGui::TreeNode(champ->name.c_str())) {
				int team = champ->team;
				ImGui::LabelText("Address", "%#010x", champ->address);
				ImGui::DragInt("Team", &team);

				ImGui::DragFloat("Current Health", &champ->health);
				ImGui::DragFloat("Base Atk", &champ->baseAttack);
				ImGui::DragFloat("Bonus Atk", &champ->bonusAttack);
				ImGui::DragFloat("Armour", &champ->armour);
				ImGui::DragFloat("Magic resist", &champ->magicResist);

				ImGui::Checkbox("Is Visible", &champ->isVisible);
				ImGui::LabelText("Position", "X:%.2f Y:%.2f Z:%.2f", champ->position.x, champ->position.y, champ->position.z);

				DrawSpell(champ->Q);
				DrawSpell(champ->W);
				DrawSpell(champ->E);
				DrawSpell(champ->R);
				DrawSpell(champ->D);
				DrawSpell(champ->F);

				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	
	DrawGameObjects("Minions", snapshot.minions);
	DrawGameObjects("Jungle", snapshot.jungle);
	DrawGameObjects("Turrets", snapshot.turrets);
	DrawGameObjects("Others", snapshot.others);

	ImGui::End();
}

void DebugView::DrawWorldSpaceOverlay(const MemSnapshot& snapshot, const MiscToolbox& toolbox) {

	if (Input::WasKeyPressed(showHoveredObjectKeySelector->GetSelectedKey()))
		showHoveredObject ^= true;
	if (showHoveredObject && snapshot.hoveredObject != nullptr) {

		Vector2 cursorPos = Input::GetCursorPosition();
		ImGui::SetNextWindowPos(ImVec2(cursorPos.x, cursorPos.y));

		ImGui::Begin("##DebugHovered", nullptr,
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoInputs |
			ImGuiWindowFlags_AlwaysAutoResize
		);

		DrawGameObject(snapshot.hoveredObject, true);
		ImGui::End();
	}
}

void DebugView::DrawSettings(const MemSnapshot& snapshot, const MiscToolbox& toolbox) {
	ImGui::Checkbox("Show info on hovered obj##debugShowHoveredObject", &showHoveredObject);
	showHoveredObjectKeySelector->DrawImGuiWidget();
}

void DebugView::OnSaveSettings(ConfigSet& configs) {
	BaseView::OnSaveSettings(configs);
	configs.Set<bool>("showHoveredObject", showHoveredObject);
	configs.Set<int>("showHoveredObjectKey", showHoveredObjectKeySelector->GetSelectedKey());
}

void DebugView::OnLoadSettings(ConfigSet& configs) {
	BaseView::OnLoadSettings(configs);
	showHoveredObject = configs.Get<bool>("showHoveredObject", false);
	showHoveredObjectKeySelector = new KeySelector("Key Show info on hovered object", (HKey)configs.Get<int>("showHoveredObjectKey", HKey::NO_KEY));
}
