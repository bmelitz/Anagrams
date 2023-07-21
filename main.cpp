#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <algorithm>

using std::ifstream;
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::getline;
using std::vector;
using std::array;
using std::unordered_map;
using std::sort;
using std::pair;

namespace std {
	template <>
	struct hash<array<int, 26>> {
		unsigned long operator() (const array<int, 26>& arr) const {
			unsigned long seed = 0;
			for (const auto& val : arr) {
				seed ^= std::hash<int>{}(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			}
			return seed;
		}
	};
}

int main() {
	ifstream input_file("dictionary.txt");
	if (!input_file.is_open()) {
		cerr << "Unable to open dictionary file" << endl;
		return 1;
	}

	unordered_map<array<int, 26>, vector<string>> anagrams;
	string word;

	while (getline(input_file, word)) {
		array<int, 26> occurances = {};

		for (const char& c : word) {
			int ascii_num = (int) c;
			if (ascii_num < 97 || ascii_num > 122) {
				cerr << "Invalid ascii character in word: " << word << endl;
				return 1;
			}
			occurances[ascii_num - 97] += 1;
		}

		if (anagrams.find(occurances) != anagrams.end()) {
			anagrams[occurances].push_back(word);
		} else {
			anagrams[occurances] = {word};
		}
	}
	input_file.close();

	vector<pair<array<int, 26>, vector<string>>> anagram_pairs;
	anagram_pairs.reserve(anagrams.size());
	for (const auto& entry : anagrams) {
		anagram_pairs.emplace_back(entry);
	}

	sort(anagram_pairs.begin(), anagram_pairs.end(),
		[] (const pair<array<int, 26>, vector<string>>& elem1,
			const pair<array<int, 26>, vector<string>>& elem2) {
			return elem1.second.size() > elem2.second.size();
		}
	);

	for (const auto& entry : anagram_pairs) {
		int num_anagrams = entry.second.size();
		if (num_anagrams > 1) {
			for (int i = 0; i < num_anagrams; ++i) {
				if (i == num_anagrams - 1) {
					cout << entry.second[i] << endl;
				} else {
					cout << entry.second[i] << ", ";
				}
			}
		}
	}
}