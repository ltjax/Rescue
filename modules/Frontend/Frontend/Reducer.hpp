#pragma once
#include "Model.hpp"
#include "rtti_reducer.hpp"

std::shared_ptr<rtti_reducer<Model>> createReducer();
