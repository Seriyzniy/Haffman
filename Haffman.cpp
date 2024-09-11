#include <iostream> 
#include <string>
#include <vector>
#include <bitset>
using namespace std;

class Haffman {
public:
	//
	// Структура для хранения символа и частоты его появления
	struct Repeat_symbol {
		char symbol[30];
		uint16_t count_repeat = 1;
	};
	vector<Repeat_symbol> Alphabet;
	//
	// Структура для хранения пар (симол : битовый код)
	struct Symbol_code {
		char code[15];
		char symbol;
	};
	vector<Symbol_code> dict_codes;
	//
	// Переменные для СЖАТОЙ и декодируемой строки
	int count_word = 0;
	uint16_t escape_uint = 0;
	vector<uint16_t> coder_msg;
	string decoder_msg;
	//
	// Структура листа дерева
	struct node {
		char symbol[30] = "0";
		int count_repeat = 0;
		node* left = nullptr;
		node* right = nullptr;
		node* par = nullptr;
	}escape_node;
	vector<node*> list_nodes;

public:
	//
	// Функции для создания алфавита
	int Find_index_to_insert(vector<Repeat_symbol> vectr, int size_p, int value);
	int Check_in_Alphabet(char symb);
	void Create_Alphabet(string msg);
	void Out_Alphabet();
	//
	// Построение дерева хаффмана
	int Check_in_list_nodes(string node);
	void Create_node(string par, int p_repeat, string left, int l_repeat, string right, int r_repeat);
	int GetMinRepeat(Repeat_symbol* p, int size_p);
	void Create_Hafman_Tree();
	void Out_Haffman_Tree();
	//
	// Алгоритм Хаффмана
	void Create_Haffman_codes();
	void Out_Haffman_codes();
	//
	// Кодирование строки
	void Write_in_coder_str(uint16_t code, int coder_bits);
	char* Get_Haffman_code(char symb);
	void Coder_msg(string msg);
	void Out_Coder_msg();
	//
	// Декодирование строки
	char Get_symb_in_dict(string code);
	bool Check_bitcode_in_dict(string bitcode);
	void Decoder_msg();
	void Out_Decoder_msg();
};
//
// Функции для создания алфавита
int Haffman::Find_index_to_insert(vector<Repeat_symbol> vectr, int size_p, int value) {
	int index = 0;
	if (size_p == 0) {
		return 0;
	}
	for (int i = 0; i < size_p; i++) {
		if (value < vectr[i].count_repeat) {
			index++;
		}
	}
	return index;
}
int Haffman::Check_in_Alphabet(char symb) {
	string str_symb;
	str_symb.push_back(symb);
	for (int i = 0; i < Alphabet.size(); i++) {
		if (str_symb == Alphabet[i].symbol) {
			return i;
		}
	}
	return -1;
}
void Haffman::Create_Alphabet(string msg) {
	Repeat_symbol tmp;
	int index_repeat;
	int index_insert;

	//Считывание первого символа
	tmp.symbol[0] = msg[0];
	tmp.symbol[1] = '\0';
	Alphabet.push_back(tmp);
	//Чтение оставшихся символов
	for (int i = 1; i < msg.size(); i++) {
		tmp.symbol[0] = msg[i];
		tmp.symbol[1] = '\0';
		index_repeat = Check_in_Alphabet(msg[i]);
		if (index_repeat != -1) {
			Alphabet[index_repeat].count_repeat++;
			//Сортировка вставкой
			index_insert = Find_index_to_insert(Alphabet, Alphabet.size(), Alphabet[index_repeat].count_repeat);
			Alphabet.insert(Alphabet.begin() + index_insert, Alphabet[index_repeat]);
			Alphabet.erase(Alphabet.begin() + index_repeat + 1);
			continue;
		}
		Alphabet.push_back(tmp);
	}
}
void Haffman::Out_Alphabet() {
	for (int i = 0; i < Alphabet.size(); i++) {
		cout << Alphabet[i].symbol << " : " << Alphabet[i].count_repeat << endl;
	}
}
//
// Построение дерева хаффмана
int Haffman::Check_in_list_nodes(string node) {
	string str_symb;
	if (list_nodes.size() == 0) {
		return -1;
	}
	for (int i = 0; i < list_nodes.size(); i++) {
		str_symb = list_nodes[i]->symbol;
		if (node == str_symb) {
			return i;
		}
	}
	return -1;
}
int Haffman::GetMinRepeat(Repeat_symbol* p, int size_p) {
	int min = p[0].count_repeat;
	int index = 0;
	for (int i = 0; i < size_p; i++) {
		if (p[i].count_repeat <= min) {
			min = p[i].count_repeat;
			index = i;
		}
	}
	return index;
}
void Haffman::Create_node(string par, int p_repeat, string left, int l_repeat, string right, int r_repeat)
{
	int index_right_in_list = 0;
	int index_left_in_list = 0;
	//Создание связующего узла
	node* p = new node;
	par.copy(p->symbol, sizeof(p->symbol));
	p->symbol[par.size()] = '\0';
	p->count_repeat = p_repeat;
	//
	//Если правый узел уже есть в списке вершин, а левого нет
	index_right_in_list = Check_in_list_nodes(right);
	if (index_right_in_list != -1) {
		p->right = list_nodes[index_right_in_list];
		p->right->par = p;

		index_left_in_list = Check_in_list_nodes(left);
		if (index_left_in_list == -1) {
			node* l = new node;
			left.copy(l->symbol, sizeof(l->symbol));
			l->symbol[left.size()] = '\0';
			l->count_repeat = l_repeat;
			l->par = p;

			p->left = l;
			list_nodes.erase(list_nodes.begin() + index_right_in_list);
			list_nodes.push_back(p);
			return;
		}
	}
	//Если левый узел уже есть в списке вершин, а правого нет
	index_left_in_list = Check_in_list_nodes(left);
	if (index_left_in_list != -1) {
		p->left = list_nodes[index_left_in_list];
		p->left->par = p;

		index_right_in_list = Check_in_list_nodes(right);
		if (index_right_in_list == -1) {
			node* r = new node;
			right.copy(r->symbol, sizeof(r->symbol));
			r->symbol[right.size()] = '\0';
			r->count_repeat = r_repeat;
			r->par = p;

			p->right = r;
			list_nodes.erase(list_nodes.begin() + index_left_in_list);
			list_nodes.push_back(p);
			return;
		}
	}
	//Если ни левого ни правого узла в спике нет
	else {
		node* l = new node;
		left.copy(l->symbol, sizeof(l->symbol));
		l->symbol[left.size()] = '\0';
		l->count_repeat = l_repeat;
		l->par = p;

		node* r = new node;
		right.copy(r->symbol, sizeof(r->symbol));
		r->symbol[right.size()] = '\0';
		r->count_repeat = r_repeat;
		r->par = p;

		p->left = l;
		p->right = r;
	}
	list_nodes.push_back(p);
	if (index_right_in_list != -1) {
		list_nodes.erase(list_nodes.begin());
	}
	if (index_left_in_list != -1) {
		list_nodes.erase(list_nodes.begin());
	}
}
void Haffman::Create_Hafman_Tree() {
	vector<Repeat_symbol> list = Alphabet;
	Repeat_symbol* p = &list[0];

	Repeat_symbol first_min;
	Repeat_symbol sum_symbol;
	string join_symbol;
	int index_min;
	while (list.size() > 1) {
		//Поиск первого минимального элемента
		index_min = GetMinRepeat(p, list.size());
		first_min = list[index_min];
		//Поиск второго минимального
		list.erase(list.begin() + index_min);
		index_min = GetMinRepeat(p, list.size());
		//Сложение двух минимальных элементов
		sum_symbol.count_repeat = first_min.count_repeat + list[index_min].count_repeat;
		//Символ нового символа = первый символ + второй символ
		join_symbol += first_min.symbol;
		join_symbol += list[index_min].symbol;
		join_symbol.copy(sum_symbol.symbol, sizeof(sum_symbol.symbol));
		sum_symbol.symbol[join_symbol.size()] = '\0';
		//cout << first_min.symbol << " + " << list[index_min].symbol << " = " << sum_min.symbol << " (" << sum_min.count_repeat <<")" << endl;

		Create_node(sum_symbol.symbol, sum_symbol.count_repeat,
			list[index_min].symbol, list[index_min].count_repeat,
			first_min.symbol, first_min.count_repeat);

		list.insert(list.begin() + index_min, sum_symbol);
		list.erase(list.begin() + index_min + 1);
		join_symbol.clear();
	}
}
void Haffman::Out_Haffman_Tree() {
	int tabs = 0;
	node* p = list_nodes[0];
	node* head = list_nodes[0];
	node* last = nullptr;
	cout << "\n" << "(" << p->count_repeat << ")";
	while (1) {
		if (p->left == nullptr && p->right == nullptr) {
			break;
		}
		//Вставка пробелов
		tabs += 3;
		cout << "\n" << char(179);
		for (int i = 0; i < tabs - 2; i++) {
			cout << (tabs == 4 ? char(179) : ' ');
		}
		cout << char(192) << char(196);
		//Переход к первому листу
		p = (p->left ? p->left : p->right);
		//Переход к одноуровнему листу
		if (p->left == nullptr && p->right == nullptr && p->par != last) {
			cout << p->symbol << "(" << p->count_repeat << ")";
			//Если есть брат по уровню - переход на него
			if (p->par->right && p != p->par->right) {
				cout << "\n" << char(179);
				for (int i = 0; i < tabs - 2; i++) {
					cout << " ";
				}
				cout << char(192) << char(196);
				p = p->par->right;
				if (!p->left && !p->right) {
					cout << p->symbol << "(" << p->count_repeat << ")";
				}
				else {
					cout << "(" << p->count_repeat << ")";
					continue;
				}
			}
			last = p->par;//Отмечаем родителя как пройденную вершину
		}
		//
		//Переход выше по пройденым вершинам
		if (p->left == nullptr && p->right == nullptr && p->par == last) {
			while (p->par == last) {
				tabs -= 3;
				//Перемещаемся на пройденную вершину
				p = p->par;
				if (p == head) {
					cout << "\n\n";
					return;
				}
				//Переход на соседа по уровню, если такой есть
				if (p->par->right && p->par->right != last) {
					cout << "\n" << char(179);
					for (int i = 0; i < tabs; i++) {
						cout << " ";
					}
					p = p->par->right;
					if (!p->left && !p->right) {
						cout << p->symbol << "(" << p->count_repeat << ")";
						last = p->par;
						continue;
					}
					else {
						cout << "(" << p->count_repeat << ")";
						continue;
					}
					break;
				}
				last = p->par;//Родительскую от пройденной помечаем пройденой
			}
			continue;
		}
		cout << "(" << p->count_repeat << ")";
	}
}
//
// Алгоритм Хаффмана
void Haffman::Create_Haffman_codes() {
	node* p = list_nodes[0];
	node* passed = nullptr;

	Symbol_code s_c;
	string code;
	//Вправо - 1
	//Влево - 0
	while (1) {
		if (p->left == nullptr && p->right == nullptr) {
			break;
		}
		if (p->right) {
			p = p->right;
			code.append("1");
		}
		else if (p->left) {
			p = p->left;
			code.append("0");
		}
		//Переход к листу
		if (!p->left && !p->right && p->par != passed) {
			//Запись символа и кода в таблицу
			code.copy(s_c.code, sizeof(s_c.code));
			s_c.code[code.size()] = '\0';
			//strcpy_s(s_c.symbol, p->symbol);
			s_c.symbol = p->symbol[0];

			dict_codes.push_back(s_c);
			//Если последний код 0 - мы левые, 
			//переходим вправо
			if (code[code.size()] == '0') {
				p = p->par->right;
				code.erase(code.end() - 1);
				code.append("1");
			}
			//Если последний код 1 - мы правые, 
			//переходим влево
			else {
				p = p->par->left;
				code.erase(code.end() - 1);
				code.append("0");
			}
			passed = p->par;
		}
		//Если при переходе попали в лист
		if (!p->left && !p->right && p->par == passed) {
			//Запись символа и кода в таблицу
			code.copy(s_c.code, sizeof(s_c.code));
			s_c.code[code.size()] = '\0';
			//strcpy_s(s_c.symbol, p->symbol);
			s_c.symbol = p->symbol[0];

			dict_codes.push_back(s_c);
			//Переход вверх по вершинам
			while (p->par == passed) {
				p = p->par;
				code.erase(code.end() - 1);
				if (p == list_nodes[0])
					return;
				if (p->par->left && p->par->left != passed) {
					p = p->par->left;
					code.erase(code.end() - 1);
					code.append("0");
					if (!p->left && !p->right) {
						code.copy(s_c.code, sizeof(s_c.code));
						s_c.code[code.size()] = '\0';
						//strcpy_s(s_c.symbol, p->symbol);
						s_c.symbol = p->symbol[0];
						dict_codes.push_back(s_c);
						passed = p->par;
						continue;
					}
					break;
				}
				passed = p->par;
			}
		}
	}
}
void Haffman::Out_Haffman_codes() {
	for (int i = 0; i < dict_codes.size(); i++) {
		cout << dict_codes[i].symbol << " : " << dict_codes[i].code << endl;
	}
	cout << endl;
}
//
// Кодирование строки
char* Haffman::Get_Haffman_code(char symb) {
	for (int i = 0; i < dict_codes.size(); i++) {
		if (symb == dict_codes[i].symbol) {
			return dict_codes[i].code;
		}
	}
	return nullptr;
}
void Haffman::Write_in_coder_str(uint16_t code, int coder_bits) {
	//Смещение для старших и младших разрядов числа
	int offset_high_bits = 0;
	int offset_low_bits = 0;
	//Размер ячейки для записи
	int block = sizeof(uint16_t) * 8;
	//Индекс последнего занятого бита
	static int last_buzy_bit = 0;
	//Размер свободного блока
	int free_block = 0;

	//Расчет смещения
	if (last_buzy_bit == 0 || last_buzy_bit == block) {
		offset_high_bits = block - coder_bits;
		last_buzy_bit = block - offset_high_bits;
	}
	else {
		free_block = block - last_buzy_bit;
		if (free_block >= coder_bits) {
			offset_high_bits = free_block - coder_bits;
			last_buzy_bit = block - offset_high_bits;
		}
		else {
			offset_high_bits = coder_bits - free_block;
			offset_low_bits = block - offset_high_bits;
			last_buzy_bit = block - offset_low_bits;

		}
	}
	if (free_block < coder_bits) {
		coder_msg.push_back(escape_uint);
	}
	//Запись кода в массив
	if (offset_low_bits == 0) {
		coder_msg[count_word] = (code << offset_high_bits) | coder_msg[count_word];
		if (last_buzy_bit == block)
			count_word++;
	}
	else {
		coder_msg[count_word] = (code >> offset_high_bits) | coder_msg[count_word];
		count_word++;
		coder_msg[count_word] = (code << offset_low_bits) | coder_msg[count_word];
	}
}
void Haffman::Coder_msg(string msg) {
	char symb;
	string code_str;
	uint16_t code_uint;
	for (int i = 0; i < msg.size(); i++) {
		symb = msg[i];
		code_str = Get_Haffman_code(symb);
		code_uint = stoi(code_str, 0, 2);
		Write_in_coder_str(code_uint, code_str.size());
	}
}
void Haffman::Out_Coder_msg() {
	for (int i = 0; i < coder_msg.size(); i++) {
		cout << coder_msg[i] << "\t" << bitset<16>(coder_msg[i]) << endl;
	}
	cout << "Size compress message = " << sizeof(uint16_t) * 8 * coder_msg.size() << " bites" << endl;
}
//
// Декодирование строки
char Haffman::Get_symb_in_dict(string bitcode) {
	for (int i = 0; i < dict_codes.size(); i++) {
		if (bitcode == dict_codes[i].code) {
			return dict_codes[i].symbol;
		}
	}
	return 0;
}
bool Haffman::Check_bitcode_in_dict(string bitcode) {
	string code_str;
	for (int i = 0; i < dict_codes.size(); i++) {
		code_str = dict_codes[i].code;
		if (bitcode == code_str) {
			return 1;
		}
	}
	return 0;
}
void Haffman::Decoder_msg() {
	uint16_t bits = coder_msg[0];
	string bits_str;
	char decod_symb;
	//Смещение для старших и младших разрядов числа
	int offset_high_bits = 0;
	int offset_low_bits = 0;
	//Размер ячейки для записи
	int block = sizeof(uint16_t) * 8;
	//Индекс последнего занятого бита
	static int last_buzy_bit = 0;
	//Размер свободного блока
	int free_block = 0;
	for (int i = 1, j = 0; i <= sizeof(uint16_t) * 8; i++) {
		//Расчет смещения
		offset_high_bits = block - i;
		last_buzy_bit = block - offset_high_bits;
		// Читает последние нули за символы
		bits >>= offset_high_bits;
		bits_str.append(to_string(bits));
		if (Get_symb_in_dict(bits_str) == '#') {// Get_symb_in_dict(stoi(bits_str, 0, 2)
			break;
		}
		bits = coder_msg[j];
		bits <<= last_buzy_bit;
		bits >>= last_buzy_bit;
		if (Check_bitcode_in_dict(bits_str)) {
			decod_symb = Get_symb_in_dict(bits_str);
			decoder_msg.push_back(decod_symb);
			bits_str.clear();
		}
		if (i == sizeof(uint16_t) * 8 && j + 1 <= coder_msg.size()) {
			i = 0;
			j++;
			bits = coder_msg[j];
		}
	}
}
void Haffman::Out_Decoder_msg() {
	for (int i = 0; i < decoder_msg.size(); i++) {
		cout << decoder_msg[i];
	}
	cout << endl;
}
int main()
{
	Haffman t;
	//						!!!В конце сообщения ставить #!!!
	string msg = "Hello world #";
	cout << "Alphabet\n";
	t.Create_Alphabet(msg);//  /wed/we/wee/web/wet/week/hello#
	t.Out_Alphabet();

	cout << "\nHaffman tree";
	t.Create_Hafman_Tree();
	t.Out_Haffman_Tree();

	cout << "\nHaffman Codes\n";
	t.Create_Haffman_codes();
	t.Out_Haffman_codes();

	cout << "\nCoder msg\n";
	t.Coder_msg(msg);
	t.Out_Coder_msg();

	cout << "\nDecoder msg\n";
	t.Decoder_msg();
	t.Out_Decoder_msg();

	cout << "\nPlain msg\n" << msg << " size = " << sizeof(char) * 8 * msg.size() << " bites" << endl;
}
