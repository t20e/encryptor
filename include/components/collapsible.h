
#include <ftxui/component/component.hpp>

/**
 * @brief Create a Collapsible component that displays details about the cryptography algorithm.
 * 
 * @param algoName_ptr Pointer to the algo name.
 * @param description_ptr Pointer to the description of the algorithm.
 * @return ftxui::Component 
 */
ftxui::Component createCollapsible(
	std::shared_ptr<std::string> algoName_ptr,
	std::shared_ptr<std::string> description_ptr);