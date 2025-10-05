#pragma once

#include <ftxui/component/component.hpp> // User input, Radiobox
#include <vector>

#include "AppModel.h"

using namespace ftxui;


/**
 * @brief The Cryptography window.
 */
class CryptoWindow : public ComponentBase
{
	private:
		// State variables
		AppModel &model_;

		// For the radiobox inFileContents_
		Component radioboxComp_;
		int selectedRadioBtn_ = 0; // For the radioboxComp_

		// Algorithm hash input
		Component hashInput_Comp_;
		std::string hashInput_ = "";

		// New file name for the user to create.
		Component newFileNameInput_Comp_;
		std::string newFileNameInput_ = "";

		std::string err_msg = "";
		std::string informUserHowToDecrypt = "";

		// Action button
		Component actionButton_;

		std::function<void()> resetApp_;

		/**
         * @brief Set the up algo radio array (algo_radio_), with algorithms names from (algosDict)
         * 
         * @return std::vector<std::string> 
         */
		std::vector<std::string> setup_algo_radio();

		/**
         * @brief Perform the cryptography
         * 
         */
		void performCryptography();


		/**
        * @brief # TODO
        *
        * @param window
        */
		void onSaveBtn();

		/**
        * @brief Callback function when user selects a file from the file browser window.
        *
        * @param path Full path to that file.
        */
		void onFileSelected(
				const std::string &path);

		/**
        * @brief Reset state variables back to default.
        * 
        */
		void resetCrypto()
		{
			selectedRadioBtn_ = 0;
			hashInput_ = "";
			newFileNameInput_ = "";
			err_msg = "";
			informUserHowToDecrypt = "";
		}


	public:
		CryptoWindow(AppModel &model, std::function<void()> resetApp);
};
