#pragma once
#include "Model.hpp"

struct ADD_ACTION
{
};

struct LOAD_FROM
{
    std::string filename;
};

struct FILE_NEW
{
};

Action newDocument();
Action addAction();
Action loadDocumentFrom(std::string const& filename);
Action saveDocumentTo(std::string const& filename);
