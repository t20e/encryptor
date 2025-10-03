#pragma once

#include <ftxui/component/component.hpp> // User input, Radiobox, components


/**
 * @brief A File Preview component to render the selected file and the encrypted or decrypt version of it to it's right.
 * 
 */
class FilePreview : public ftxui::ComponentBase
{
	public:
		FilePreview();

	private:
		std::string in_content_;
		std::string out_content_;
		std::vector<unsigned char> inFileContents_;
		std::vector<unsigned char> outFileContents_;

		float scroll_x_ = 0.1f;
		float scroll_y_ = 0.1f;

		void setInFileContents(std::vector<unsigned char> inFileContents)
		{
			this->inFileContents_ = inFileContents;
		}

		void setOutFileContents(std::vector<unsigned char> outFileContents)
		{
			this->outFileContents_ = outFileContents;
		}
};
