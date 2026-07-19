#pragma once

#include <string>

enum class MessageType { INFO, SUCCESS, ERROR_MSG, WARNING };

// Show a message dialog with an OK button
void showMessage(const std::string& title, const std::string& message,
                 MessageType type = MessageType::INFO);

// Show a yes/no confirmation dialog, returns true if confirmed
bool showConfirmDialog(const std::string& message);

// Returns entered text, or an empty string when cancelled.
std::string showTextInputForm(const std::string& title,
                              const std::string& label,
                              const std::string& placeholder = "");

struct PasswordChangeData {
    std::string currentPassword;
    std::string newPassword;
    std::string confirmPassword;
    bool confirmed = false;
};

PasswordChangeData showChangePasswordForm(const std::string& title);
