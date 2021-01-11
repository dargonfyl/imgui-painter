#pragma once

#include <string>

#include "Texture.hpp"
#include "Canvas.hpp"
#include "Renderer.hpp"


namespace Im_Painter
{
	/**
	 * Handles image IO. Includes reading and writing.
	 */
	namespace Image_IO {
		// TODO: image io errors
		// TODO: image formats


		/**
		 * Reads an image from a path, and returns a new canvas
		 */
		Canvas canvas_from_image(std::string path);


		/**
		 * Adds an image to a canvas.
		 */
		void layer_from_image(std::string path, Canvas &canvas);


		/**
		 * Writes an image to a path.
		 */
		void write(std::string path, Canvas &canvas, Renderer &renderer);
	};
} // namespace Im_Painter
