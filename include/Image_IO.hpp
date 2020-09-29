#pragma once

#include <string>

#include "Texture.hpp" 


namespace Im_Painter
{
	/**
	 * Handles image IO. Includes reading and writing.
	 */
	namespace Image_IO {
		// TODO: image io errors
		// TODO: image formats


		/**
		 * Reads an image from a path.
		 */
		Texture read(std::string path, bool alpha=false);


		/**
		 * Reads an image from a path.
		 */
		Texture read(const char *path, bool alpha=false);


		/**
		 * Not implemented lol.
		 */
		void write(std::string path);
	};
} // namespace Im_Painter
