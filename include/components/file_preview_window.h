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
		float scroll_x_ = 0.1f;
		float scroll_y_ = 0.1f;
		AppModel &model_;

		/**
         * @brief Convert the selected file and its output (encrypted or decrypted) vector buffers into strings to render them in FTXUI.
         * 
         * @param buffer 
         * @return std::string 
         */
		ftxui::Element convertBufferToString(const std::vector<unsigned char> &buffer);
};
