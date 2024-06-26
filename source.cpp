import <random>;
import <iostream>;
import <bitset>;
import <deque>;
std::default_random_engine engine((std::random_device())());
auto generateRow(std::deque<int8_t >& lastrows) {
	static std::uniform_int_distribution<> getRandomBit(0, 1);
	const auto lastlowpadded = (((getRandomBit(engine) << 8) | lastrows.back()) << 1) | getRandomBit(engine);
	lastrows.emplace_back(0);
	for (auto i = 7, lasttwo = 0b10; 0 <= i; --i) {
		bool newBit;
		const std::bitset<5> key = (lasttwo << 3) | ((lastlowpadded >> i) & 0b111);
		if (!key[2] && (!(key[1] || key[3]) || !(key[4]) && (key[3]) || !(key[0]) && (key[1])))newBit = 1;
		else if ((key[2] && ((key >> 1) != 0b0011) && (key << 1) != 0b11000) || key == 0b11001)newBit = 0;
		else newBit = getRandomBit(engine);
		lastrows.back() = (lastrows.back() << 1) | newBit;
		lasttwo = ((lasttwo << 1) | newBit) & 0b11;
	}
	if (12 < lastrows.size())lastrows.pop_front();
	[&lastrows] {
		for (const auto& row : lastrows) {
			if (!(row & 0xf0) || row & 0x80)return;
		}
		lastrows.back() = 0;
	}();
	auto count = 0, sum = 0;
	for (auto iter = lastrows.rbegin(); iter != lastrows.rend() && count < 7; ++iter, ++count) {
		if (const auto clip = (*iter & 0xf); clip)sum += clip & 1;
		else return;
	}
	count = 0;
	if (8 < lastrows.size())
		count = *(lastrows.rbegin() + 8);
	if (sum == (count & 1) * 7) {
		lastrows.back() &= 0xf0;
	}
}
int main() {
	std::deque<int8_t> a{ 0 };
	while (true) {
		generateRow(a);
		auto bit = std::bitset<8>(a.back());
		for (auto i = 0; i < bit.size(); ++i) {
			std::cout << (bit[i] ? "■" : "□");
		}
		std::cout << std::endl;
	}
	return EXIT_SUCCESS;
}
