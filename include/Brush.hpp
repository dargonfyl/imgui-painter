#pragma once

#include <glm/glm.hpp>
#include <vector>


namespace Im_Painter
{
	typedef int Brush_size_t;
	
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


		void set_size(Brush_size_t size);


		/**
		 * Sets color using floats in the range [0, 1].
		 * 
		 * @param color  vec4 of floats in the range [0, 1]
		 */
		void set_color(glm::vec4 color);


		glm::vec4 get_color();


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


		void use(std::vector<unsigned char> &buffer, int x, int y, int width, int height);
	};
} // namespace Im_Painter
