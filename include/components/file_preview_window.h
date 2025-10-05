#pragma once

#include <ftxui/component/component.hpp> // User input, Radiobox, components
#include <vector>

#include "AppModel.h"


/**
 * @brief A File Preview component to render the selected file and the encrypted or decrypt version of it to it's right.
 * 
 */
class FilePreview : public ftxui::ComponentBase
{
	public:
		FilePreview(AppModel &model);

	private:
		float scroll_x_ = 0.01f;
		float scroll_y_ = 0.01f;
		AppModel &model_;

		/**
         * @brief Convert a file's vector buffer byte data into FTXUI elements, to display them in the file preview window.
         * 
         * @param buffer 
         * @return std::string 
         */
		ftxui::Element convertBufferToElements(const std::vector<unsigned char> &buffer);
};
