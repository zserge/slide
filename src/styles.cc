#include "styles.h"

namespace slide{
	namespace Style{
		const std::string &to_str(const Style &s) {
			static const std::array<const std::string, 4> arr{"Normal", "Strong",
															  "Header", "Monospace"};
			return arr[s];
		}
	}
}


