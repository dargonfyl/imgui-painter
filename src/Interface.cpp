#include "Interface.hpp"

#include <string>

#define LAYER_SAMELINE_EPSILON 0.001f


namespace Im_Painter {
	namespace {
		static ImGuiColorEditFlags colour_selector_flags = ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs;

		ImVec4 color_primary;
		ImVec4 color_secondary;
		float opacity = 1.0f;

		void colour_selector(Brush &brush) {
			ImGui::ColorPicker4("Color", (float*)&color_primary, colour_selector_flags, nullptr);
			ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f);
			brush.set_color(glm::vec4(color_primary.x, color_primary.y, color_primary.z, opacity));
		}

		std::vector<ImVec4> swatch(16, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		void swatches(Brush &brush) {
			for (int i = 0; i < swatch.size(); i++) {
				std::string swatch_id = "swatch##" + std::to_string(i);

				int alpha_prev = swatch[i].w == 0.0f ? ImGuiColorEditFlags_AlphaPreview : 0;
				bool disabled = swatch[i].w == 0.0f;
				if (ImGui::ColorButton(swatch_id.c_str(), swatch[i], ImGuiColorEditFlags_NoTooltip | alpha_prev, ImVec2(20, 20)) && !disabled) {
					ImVec4 col = swatch[i];
					brush.set_color(glm::vec4(col.x, col.y, col.z, opacity));
					color_primary = col;
				}
				if (ImGui::BeginPopupContextItem()) {
					if (ImGui::MenuItem("Set", "S")) { // TODO: press s to set color
						glm::vec4 bc = brush.get_color();
						ImVec4 im_brush_col = ImVec4(bc.x, bc.y, bc.z, 1.0f);
						swatch[i] = im_brush_col;
					}

					if (ImGui::MenuItem("Unset", "", nullptr, swatch[i].w != 0.0f)) {
						swatch[i] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
					}

					ImGui::Separator();
					if (ImGui::MenuItem("Cancel")) {
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}
				if (((i + 1) % 8)) {
					ImGui::SameLine();
				}
			}
		}


		void layer_options(Canvas &canvas, layer_index_t index) {
			if (ImGui::BeginPopupContextItem()) {
				ImGui::Text("Layer things");

				// bool visible;  // TODO
				ImGui::MenuItem("Not implemented", "", true);
				if (ImGui::MenuItem("Delete", "", false, canvas.get_num_layers() != 1)) {
					canvas.delete_layer(index);
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Cancel")) {
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
	}


	void UI::show_layers_window(Canvas &canvas, bool &show) {
		ImGui::Begin("Layers", &show);

		// TODO: layer UI
		if (ImGui::Button("New layer")) {
			canvas.new_layer();
		}

		static int selected = -1;
		ImVec2 size = ImGui::GetWindowSize();

		for (unsigned int i = 0; i < canvas.get_num_layers(); i++) {
			std::string imgui_layer_id = "##" + std::to_string(i);

			// std::string name = "layer" + imgui_layer_id;
			std::string name = imgui_layer_id;
			if (ImGui::Selectable(name.c_str(), selected == i, 0, ImVec2(size.x, 30))) {
				selected = i;
			}
			std::string options = "options" + imgui_layer_id;
			layer_options(canvas, i);

			ImGui::SameLine(LAYER_SAMELINE_EPSILON);

			ImGui::Image((void*)(intptr_t)canvas.layer_texture_id(i), ImVec2(40, 30));

			ImGui::SameLine();
			ImGui::Text("Layer %d", i);
		}
		if (selected >= 0)
			canvas.switch_active_layer(static_cast<unsigned int>(selected));

		ImGui::End();
	}


	void UI::colours(Brush &brush) {
		ImGui::Begin("Colour Picker");

		colour_selector(brush);

		swatches(brush);
		ImGui::End();
	}

} // namespace Im_Painter
