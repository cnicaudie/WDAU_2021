#include <stdafx.h>
#include "UIViewModel.h"

UIViewModel* UIViewModel::m_UIViewModel = nullptr;

UIViewModel* UIViewModel::GetInstance()
{
    if (m_UIViewModel == nullptr)
    {
        m_UIViewModel = new UIViewModel();
    }

    return m_UIViewModel;
}

UIViewModel::UIViewModel()
{
    // TODO
}

UIViewModel::~UIViewModel()
{
    delete m_UIViewModel;
}