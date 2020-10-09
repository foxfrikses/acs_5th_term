#include "menu.h"
#include "tasks.h"
#include <iostream>

enum class Menu::eTask : uint32_t {
    exit = 0u,
    _1_2 = 1u,
    _1_3 = 2u,
    _2_1 = 3u,
    _2_2 = 4u,
    _2_3 = 5u,
    none = 6u
};

enum class Menu::eMenu : uint32_t {
    exit    = 0u,
    tasks   = 1u,
    results = 2u,
    none    = 3u
};

uint32_t Menu::runMenu(const char *menuName, const char *menuText, uint32_t numMenuElem) {
    system("clear");
    std::cout << "\t" << menuName << ":\n" << menuText << "\n\n>> ";
    std::string input;
    std::cin >> input;
    uint32_t result{numMenuElem};
    try {
        result = std::stoi(input);
    } catch (...) {
        return numMenuElem;
    }
    if ( result < numMenuElem )
        return result;
    else
        return numMenuElem;
}

Menu::eMenu Menu::runMainMenu() {
    constexpr char menuName[] = "Main menu";
    constexpr char menuText[] = "1. Exec tasks\n"
                                "2. Show results\n"
                                "0. Exit";
    return eMenu(runMenu( menuName, menuText, uint32_t(eMenu::none)));
}

Menu::eTask Menu::runTaskListMenu( const char *menuName ) {
    constexpr char menuText[] = "1. Task 1.2\n"
                                "2. Task 1.3\n"
                                "3. Task 2.1\n"
                                "4. Task 2.2\n"
                                "5. Task 2.3\n"
                                "0. Exit";
    return eTask( runMenu( menuName,
                           menuText,
                           uint32_t(eTask::none)));
}

Menu::eTask Menu::runTasksMenu() {
    constexpr char menuName[] = "Tasks";
    return runTaskListMenu( menuName );
}

Menu::eTask Menu::runResultsMenu() {
    constexpr char menuName[] = "Results";
    return runTaskListMenu( menuName );
}

int Menu::run() {
    while( true ) {
        switch ( runMainMenu() ) {
        case eMenu::tasks:
            switch ( runTasksMenu() ) {
            case eTask::_1_2:
                task_1_2();
                break;
            case eTask::_1_3:
                task_1_3();
                break;
            case eTask::_2_1:
                task_2_1();
                break;
            case eTask::_2_2:
                task_2_2();
                break;
            case eTask::_2_3:
                task_2_3();
                break;
            case eTask::exit:
            case eTask::none:
                break;
            }
            break;
        case eMenu::results:
            switch ( runResultsMenu() ) {
            case eTask::_1_2:
                result_1_2();
                break;
            case eTask::_1_3:
                result_1_3();
                break;
            case eTask::_2_1:
                result_2_1();
                break;
            case eTask::_2_2:
                result_2_2();
                break;
            case eTask::_2_3:
                result_2_3();
                break;
            case eTask::exit:
            case eTask::none:
                break;
            }
            break;
        case eMenu::none:
            break;
        case eMenu::exit:
            return 0;
        }
    }
}
