#include "Interface.hpp"

#include <string>

#define LAYER_SAMELINE_EPSILON 0.001f


namespace Im_Painter {
	void UI::show_layers_window(Canvas &canvas, bool &show) {
		ImGui::Begin("Layers", &show);

		// TODO: layer UI
		if (ImGui::Button("New layer")) {
			canvas.new_layer();
		}

		static int selected = -1;
		ImVec2 size = ImGui::GetWindowSize();

		for (unsigned int i = 0; i < canvas.get_num_layers(); i++) {
			std::string name = "##" + std::to_string(i);
			if (ImGui::Selectable(name.c_str(), selected == i, 0, ImVec2(size.x, 30))) {
				selected = i;
			}
			ImGui::SameLine(LAYER_SAMELINE_EPSILON);

			ImGui::Image((void*)(intptr_t)canvas.layer_texture_id(i), ImVec2(40, 30));

			ImGui::SameLine();
			ImGui::Text("Layer %d", i);
		}
		if (selected >= 0)
			canvas.switch_active_layer(static_cast<unsigned int>(selected));

		ImGui::End();
	}
} // namespace Im_Painter
