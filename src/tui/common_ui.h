#pragma once

#include <string>

enum class MessageType { INFO, SUCCESS, ERROR_MSG, WARNING };

// Show a message dialog with an OK button
void showMessage(const std::string& title, const std::string& message,
                 MessageType type = MessageType::INFO);

// Show a yes/no confirmation dialog, returns true if confirmed
bool showConfirmDialog(const std::string& message);
