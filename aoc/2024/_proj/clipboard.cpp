
#ifdef _WIN32
#include <Windows.h>
#include "aoc.h"

extern bool is_example;

bool checkPotentialResult(const std::string& text)
{
    if (is_example)
        return false;
    if (text.empty())
        return false;
    if (text == "0")
        return false;
    return true;
}

void textToClipboard(const std::string& text)
{
    if (!checkPotentialResult(text))
        return;

    // Open the clipboard
    if (!OpenClipboard(nullptr)) {
        return;
    }

    // Empty the clipboard
    EmptyClipboard();

    // Allocate memory for the text
    HGLOBAL hData = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
    if (hData == nullptr) {
        CloseClipboard();
        return;
    }

    // Lock the handle to copy the text
    char* pszText = static_cast<char*>(GlobalLock(hData));
    if (pszText == nullptr) {
        CloseClipboard();
        return;
    }
    strcpy(pszText, text.c_str());
    GlobalUnlock(hData);

    // Set the clipboard data
    SetClipboardData(CF_TEXT, hData);

    // Close the clipboard
    CloseClipboard();
}


#else

void textToClipboard(const std::string& text) {}

#endif


