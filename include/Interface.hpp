#pragma once

#include <imgui.h>


#include "Canvas.hpp"


namespace Im_Painter
{
	/**
	 * UI functions. You must have imgui setup already done, otherwise none of this will work.
	 * 
	 * Note that this also includes global hotkeys, which you should be aware of.
	 */
	namespace UI
	{
		/**
		 * Keyboard input handler for global operations (i.e. CTRL + S, CTRL + Z, etc.)
		 */
		void handle_keyboard(int key);
        
        /**
         * Control panel for the majority of functions
         */
        void show_menu_toolbar_window(Canvas &canvas);

		/**
		 * Show layers window, which contains the following functions:
		 * 	- New layer
		 * 	- Individual layer functions
		 * 		- Visibility toggle
		 * 		- Delete
		 * 	
		 */
		void show_layers_window(Canvas &canvas, bool &show);


		/**
		 * Draws colours window.
		 */
		void show_colours_window(Brush &brush);


		/**
		 * Shows brush window.
		 */
		void show_brush_window(Brush &brush);

	} // namespace UI
} // namespace Im_Painter
