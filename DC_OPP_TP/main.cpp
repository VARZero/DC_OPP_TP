#include <fstream>
#include <iostream>
using namespace std;

class booltype {
	// ��ũ�� ����Ʈ
public: 
	bool md; // minterm, don't care ����
	char* boolvalue; // �� ex) 0000, 0010
	int one; // 1 ����
	int sort_one; // ������ ���� ����
	bool start; // 1�� ������ ���е� �������� Ȯ��
	bool end; // 1�� ������ ���е� ������ Ȯ��
	booltype* next; // �������
	booltype();
	
	~booltype();
};

void Q_M_process() {
	// ����-��Ŭ����Ű �˰��� ó��

	// 1�� ���� ���� �� �и�, ������� ���� (Ŀ���� ��ũ�� ����Ʈ)
	// 1�� ������ �и��� �κг��� �� �� ����. 
	// ���ĵ� ��ҵ��� ���ϰ� �ִ� ��ҵ鿡 ������ �Ǵ��� Ȯ���ϰ�
	// ���� �ʴ� ��ҵ��� ���� ��ũ�� ����Ʈ�� ����
	// (�ݺ� - ������� �ʴ� ��Ҹ� �����Ҷ� ����)
	   // => Prime implicant ����

	// minterm ��� �ϳ��� PIs ��Ҹ� �ݺ��Ͽ� ����Ǵ� �κ��� Ȯ�� 
	// (�ݺ� - ����� �񱳱���)
	   // => Essential Prime implicant ����

	// ��ȯ - Essential Prime implicant (char*)
}

void Optimize() {
	// ����ȭ

	// TWO-LEVEL LOGIC CIRCUIT
}

void BoolEqu() {
	// BOOL EQ�� ��Ÿ���� (������) -> Ʈ�������� ���� �ľ�
}

int main() {
	ifstream bool_input;
	bool_input.open("input_minterm.txt");
	/* ���� �Է� ó�� */
	// ��Ʈ �Է�
	// don't care / minterm ����

	// ����-��Ŭ����Ű �˰��� ó�� �Լ��� �ѱ��
	
	return 0;
}