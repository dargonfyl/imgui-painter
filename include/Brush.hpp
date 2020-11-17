#pragma once

#include <glm/glm.hpp>


namespace Im_Painter
{
	typedef unsigned int Brush_size_t;
	
	/**
	 * Brush class. Holds brush parameters, colors, and other data.
	 */
	class Brush {
		private:
		Brush_size_t size;  // TODO: WHAT DO WE DO WITH THIS THING???? SHOULD THIS BE LEFT TO THE BRUSH OR THE CANVAS????
		unsigned char R;
		unsigned char G;
		unsigned char B;
		unsigned char A;


		public:
		Brush();


		Brush(Brush_size_t size);


		~Brush();


		/**
		 * Sets color using floats in the range [0, 1].
		 * 
		 * @param color  vec4 of floats in the range [0, 1]
		 */
		void set_color(glm::vec4 color);


		/**
		 * Sets color using unsigned chars;
		 * 
		 * @param R
		 * @param G
		 * @param B
		 * @param A
		 */
		void set_color(unsigned char R, unsigned char G, unsigned char B, unsigned char A);


		/**
		 * Colours a single pixel.
		 */
		void use(unsigned char *pixel);
	};
} // namespace Im_Painter
