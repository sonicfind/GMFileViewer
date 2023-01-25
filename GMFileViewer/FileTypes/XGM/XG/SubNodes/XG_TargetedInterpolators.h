#pragma once
#include "XG_InterpolatorNodes.h"

template <typename T>
class XG_TargetedInterpolator : public XG_InterpolatorNode<GMArray<T>, InterpolatorType::TARGETED>
{
public:
	using XG_InterpolatorNode<GMArray<T>, InterpolatorType::TARGETED>::XG_InterpolatorNode;
private:
	void loadKeys(FilePointer& file) override
	{
		if (this->m_keys.reserve(file))
			for (auto& element : this->m_keys)
				element.reserve_and_fill(file);
	}

	void saveKeys(FileWriter& file) const override
	{
		this->m_keys.write_size(file);
		for (const auto& key : this->m_keys)
			key.write_full(file);
	}
};






