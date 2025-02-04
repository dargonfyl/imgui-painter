#include "Interface.hpp"

#include <string>
#include <GLFW/glfw3.h>
#include <tinyfiledialogs.h>
#include <iostream>

#include "Image_IO.hpp"

#define LAYER_SAMELINE_EPSILON 0.001f


namespace Im_Painter {
	namespace {
		// Color editting internal state

		ImGuiColorEditFlags colour_selector_flags = ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoInputs;

		ImVec4 color_primary;
		ImVec4 color_secondary;
		float opacity = 1.0f;

		void colour_selector(Brush &brush) {
			ImGui::ColorPicker4("Color", (float*)&color_primary, colour_selector_flags, nullptr);
			ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f);
			brush.set_color(glm::vec4(color_primary.x, color_primary.y, color_primary.z, opacity));
		}

		std::vector<ImVec4> swatch_vector(16, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));


		/**
		 * Draws and handles logic for paint swatches.
		 * 
		 * A note: ImGui keys for menu items don't always work. Thus, we need to read the key press status from GLFW keys.
		 */
		void swatches(Brush &brush) {
			for (int i = 0; i < swatch_vector.size(); i++) {
				std::string swatch_id = "swatch##" + std::to_string(i);

				int alpha_prev = swatch_vector[i].w == 0.0f ? ImGuiColorEditFlags_AlphaPreview : 0;
				bool disabled = swatch_vector[i].w == 0.0f;
				if (ImGui::ColorButton(swatch_id.c_str(), swatch_vector[i], ImGuiColorEditFlags_NoTooltip | alpha_prev, ImVec2(20, 20)) && !disabled) {
					ImVec4 col = swatch_vector[i];
					brush.set_color(glm::vec4(col.x, col.y, col.z, opacity));
					color_primary = col;
				}
				if (ImGui::BeginPopupContextItem()) {
					if (ImGui::MenuItem("Set", "S") || ImGui::IsKeyPressed(GLFW_KEY_S, false)) {
						glm::vec4 bc = brush.get_color();
						ImVec4 im_brush_col = ImVec4(bc.x, bc.y, bc.z, 1.0f);
						swatch_vector[i] = im_brush_col;
					}

					if (ImGui::MenuItem("Unset", "X", nullptr, swatch_vector[i].w != 0.0f) || ImGui::IsKeyPressed(GLFW_KEY_X, false)) {
						swatch_vector[i] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
					}

					ImGui::Separator();
					if (ImGui::MenuItem("Cancel", "C") || ImGui::IsKeyPressed(GLFW_KEY_C, false)) {
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

				bool visible = canvas.get_layer_visibility(index);
				if (ImGui::MenuItem("Visible", "", &visible)) {
					canvas.toggle_layer_visibility(index);
				}

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


	void UI::handle_keyboard(int key) {

	}


	void UI::show_layers_window(Canvas &canvas, bool &show) {
		ImGui::Begin("Layers", &show);

		// TODO: layer UI drag & drop
		if (ImGui::Button("New layer")) {
			canvas.new_layer();
		}

		ImVec2 size = ImGui::GetWindowSize();
		unsigned int switched = canvas.get_active_layer_index();

		for (unsigned int i = 0; i < canvas.get_num_layers(); i++) {
			std::string imgui_layer_id = "##" + std::to_string(i);

			// std::string name = "layer" + imgui_layer_id;
			std::string name = imgui_layer_id;
			if (ImGui::Selectable(name.c_str(), i == canvas.get_active_layer_index(), 0, ImVec2(size.x, 30))) {
				switched = i;
			}
			std::string options = "options" + imgui_layer_id;
			layer_options(canvas, i);

			ImGui::SameLine(LAYER_SAMELINE_EPSILON);

			ImGui::Image((void*)(intptr_t)canvas.layer_texture_id(i), ImVec2(40, 30));

			ImGui::SameLine();
			ImGui::Text("Layer %d", i);
		}

		// HACK: this is a weird fix to ensure that the index of the active layer is not out of bounds.
		if (canvas.get_active_layer_index() != switched && switched < canvas.get_num_layers()) {
			canvas.switch_active_layer(switched);
		}

		ImGui::End();
	}


	void UI::show_colours_window(Brush &brush) {
		ImGui::Begin("Colour Picker");

		colour_selector(brush);

		swatches(brush);
		ImGui::End();
	}


	void UI::show_brush_window(Brush &brush) {
		ImGui::Begin("Brush Settings");

		// TODO
		static Brush_size_t size = 10;
		ImGui::SliderInt("Size", &size, 1, 100);

		brush.set_size(size);

		ImGui::End();
	}

	void UI::show_menu_toolbar_window(Canvas &canvas, Renderer &renderer) {
		static float H = 0.0f;
		static float S = 0.0f;
		static float V = 0.0f;
		
		if (ImGui::Button("Filter HSV")) {
			ImGui::OpenPopup("hsv_filter");
		}
		if (ImGui::BeginPopupModal("hsv_filter")) {
			ImGui::SliderFloat("Hue", &H, -1.0f, 1.0f);
			ImGui::SliderFloat("Saturation", &S, -1.0f, 1.0f);
			ImGui::SliderFloat("Value", &V, -1.0f, 1.0f);
			
			canvas.set_hsv_shift(180.0 * H, S, V);
			
			if (ImGui::Button("Confirm##hsv_filter")) {
                // Persist changes
				unsigned char *data = renderer.get_hsv_filtered_layer_buffer(canvas);
				canvas.replace_active_layer(data);

				// Reset state
				H = 0.0f;
				S = 0.0f;
				V = 0.0f;
                canvas.set_hsv_shift(H, S, V);
				ImGui::CloseCurrentPopup();
			}
            
            ImGui::SameLine();
            if (ImGui::Button("Cancel##hsv_filter")) {
                // Reset state
                H = 0.0f;
                S = 0.0f;
                V = 0.0f;
                canvas.set_hsv_shift(H, S, V);
                ImGui::CloseCurrentPopup();
            }
			ImGui::EndPopup();
		}


		if (ImGui::Button("Export")) {
			char const *filters[] = {"*.png"};
			char const *file = tinyfd_saveFileDialog("Save file", "", 1, filters, "image files");
			if (file) {
				Image_IO::write(file, canvas, renderer);				
			}
		}
		
	}
} // namespace Im_Painter
