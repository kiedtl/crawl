/**
 * @file
 * @brief Declarations for the god menu.
 */

#pragma once

#include "god-type.h"
#include "menu.h"

class GodMenuEntry : public MenuEntry
{
public:
    GodMenuEntry(god_type god, bool long_name = false);

    virtual string get_text() const override;

public:
    god_type god;

private:
    string colour_text;
};
