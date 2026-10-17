#include <iostream>
#include <fstream>

int alp(unsigned char rom) {
	switch (rom) {
	case 'I': return 1;
	case 'V': return 5;
	case 'X': return 10;
	case 'L': return 50;
	case 'C': return 100;
	case 'D': return 500;
	case 'M': return 1000;
	default: return 0;
	}
}

int to_dec(unsigned char* list) {
	int n = 0;
	int len = 0;

	while (list[len] != '\0' && len < 15) {
		++len;
	}

	for (int st = 0; st < len;) {
		if (st < len - 1 && alp(list[st]) < alp(list[st + 1])) {
			n += alp(list[st + 1]) - alp(list[st]);
			st += 2;
		}
		else {
			n += alp(list[st]);
			++st;
		}
	}
	return n;
}

char* to_rom(int number) {

	int v[13] = { 1000,900,500,400,100,90,50,40,10,9,5,4,1 };
	const char* title[13] = { "M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I" };

	char* comp = new char[15];
	int index = 0;

	while (number > 0) {
		for (int jar = 0; jar < 13; ++jar) {
			while (number >= v[jar]) {
				const char* simbol = title[jar];
				while (*simbol != '\0') {
					comp[index] = *simbol;
					++index;
					++simbol;
				}
				number -= v[jar];
			}
		}
	}
	comp[index] = '\0';
	return comp;
}

void del_list(unsigned char list[], int size) {
	for (int flak = 0; flak < size; ++flak) {
		list[flak] = 0;
	}
}

int main(int argc, char* argv[]) {
	std::ifstream infile(argv[1], std::ios::binary);
	std::ofstream outfile(argv[2], std::ios::binary);

	if (!infile || !outfile) {
		return 0;
	}

	unsigned char* operand = new unsigned char[20]();

	unsigned char ch;
	int i = 0;
	int j = 0;

	bool flag = false;
	bool end = false;
	int* stack = new int[1000]();
	while (!end) {

		int c = infile.get();
		bool end = (c == EOF);

		if (!end) {
			ch = static_cast<unsigned char>(c);

			if (ch >= 'A' && ch <= 'Z') {
				operand[j] = ch;
				flag = true;
				++j;
			}

			else if (ch == ' ' && flag) {

				operand[j] = '\0';
				stack[i] = to_dec(operand);

				del_list(operand, j);
				flag = false;
				j = 0;
				++i;
			}

			else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
				if (ch == '+') {
					stack[i - 2] += stack[i - 1];
				}
				else if (ch == '-') {
					stack[i - 2] -= stack[i - 1];
				}
				else if (ch == '*') {
					stack[i - 2] *= stack[i - 1];
				}
				else if (ch == '/') {
					stack[i - 2] /= stack[i - 1];
				}
				stack[i - 1] = 0;
				flag = false;
				--i;
			}
		}

		if (ch == '\n' || end) {
			if (i > 0) {
				if (stack[i - 1] > 0 && stack[i - 1] < 4000) {
					char* res = to_rom(stack[i - 1]);
					outfile << " = ";
					for (int index = 0; res[index] != '\0'; ++index) {
						outfile << res[index];
					}
					delete[] res;
				}
				else if (stack[i - 1] >= 4000) {
					outfile << " = ERR";
				}
			}
			i = 0;
		}
		if (!end) {
			outfile.put(static_cast<char>(ch));
		}
		else {
			break;
		}
	}

	infile.close();
	outfile.close();

	delete[] operand;
	delete[] stack;
	return 0;
}
