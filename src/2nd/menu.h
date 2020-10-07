#ifndef MENU_H
#define MENU_H

#include <stdint.h>

class Menu{
    enum class eTask : uint32_t;
    enum class eMenu : uint32_t;
    uint32_t runMenu(const char *menuName, const char *menuText, uint32_t numMenuElem);
    eMenu runMainMenu();
    eTask runTaskListMenu( const char *menuName );
    eTask runTasksMenu();
    eTask runResultsMenu();
public:
    int run();
};
#endif // MENU_H
