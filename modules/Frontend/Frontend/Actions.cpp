#include "Actions.hpp"

Action addAction()
{
    return ADD_ACTION{};
}

Action loadDocumentFrom(std::string const& filename)
{
    return LOAD_FROM{ filename };
}
