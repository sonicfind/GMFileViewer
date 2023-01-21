#pragma once
#include "XG_InterpolatorNodes.h"

template <typename T>
class XG_TargetedInterpolator : public XG_InterpolatorNode<GMArray<T>, InterpolatorType::TARGETED>
{
private:
	void loadKeys(FilePointer& file) override
	{
		if (this->m_keys.reserve(file))
			for (auto& element : this->m_keys)
				element.reserve_and_fill(file);
	}
};






