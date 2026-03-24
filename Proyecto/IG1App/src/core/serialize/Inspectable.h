#pragma once

namespace cme {
	class Inspectable
	{
	public:
		virtual ~Inspectable() = default;

		virtual void drawOnInspector() = 0;
	};
}
