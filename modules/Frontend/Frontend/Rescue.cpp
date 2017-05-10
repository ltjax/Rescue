#include "Rescue.hpp"
#include "ui_Rescue.h"

Rescue::Rescue()
: mUi(std::make_unique<Ui::MainWindow>())
{
	mUi->setupUi(this);
}

Rescue::~Rescue() = default;
