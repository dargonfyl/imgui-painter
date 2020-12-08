#pragma once

#include <imgui.h>


#include "Canvas.hpp"


namespace Im_Painter
{
	/**
	 * UI functions. You must have imgui setup already done, otherwise none of this will work.
	 */
	namespace UI
	{
		/**
		 * 
		 */
		void show_layers_window(Canvas &canvas, bool &show);
	} // namespace UI
} // namespace Im_Painter
