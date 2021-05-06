#include <fstream>
#include "boolList.hpp"
using namespace std;

int bits; // 비트

ofstream bool_output; // 출력 텍스트 파일

void Q_M_process(boolList* firstList) { // 콰인-매클러스키 알고리즘 처리

	// 1의 개수 정렬 및 분리, 순서대로 정렬 (커스텀 링크드 리스트)
	// 1의 개수로 분리된 부분끼리 비교 및 정렬. 
	// 정렬된 요소들이 비교하고 있는 요소들에 연결이 되는지 확인하고
	// 되지 않는 요소들은 따로 링크드 리스트에 저장
	// (반복 - 연결되지 않는 요소만 존재할때 까지)
	   // => Prime implicant 저장
	
	int newLists = 0;
	ImplicantList* nextList[bits];
	nextList[0] = firstList->newLists();
	
	while(nextList[newLists] != NULL){
		nextList[newLists+1] = nextList[newLists]->newLists();
		++newLists;
	}

	PIList PIs;
	
	firstList->getUnconnect(&PIs);
	for (int i = 0; i < newLists; ++i){
		nextList[i]->getUnconnect(&PIs);
	}

	// 파일에 저장
	booltype* PIbool = PIs.gethead();

	while(PIbool != NULL){
		cout << PIbool->boolvalue << endl;
		bool_output.write(PIbool->boolvalue, bits);
		PIbool = PIbool->next;
	}

	// minterm 요소 하나로 PIs 요소를 반복하여 연결되는 부분을 확인 
	// (반복 - 모든요소 비교까지)
	   // => Essential Prime implicant 저장

	// 반환 - Essential Prime implicant (char*)
}

void Optimize() {
	// 최적화
	
	// TWO-LEVEL LOGIC CIRCUIT
}

void BoolEqu() {
	// BOOL EQ로 나타내고 (내부적) -> 트랜지스터 갯수 파악
}

int main() {
	char fileline[256];
	ifstream bool_input;
	bool_input.open("input_minterm.txt");
	bool_output.open("result.txt");

	/* 파일 입력 처리 */
	if (!bool_input.is_open()){
		cout << "File Open Error!" << endl;
		return 0;
	}

	// 비트 입력
	bool_input.getline(fileline, 256);
	bits = atoi(fileline);

	boolList boolNew;
	
	while(!bool_input.eof()){
		bool_input.getline(fileline, bits+3);
		boolNew.insertBoolEq(fileline);
	}
	
	// 콰인-매클러스키 알고리즘 처리 함수로 넘기기
	Q_M_process(&boolNew);

	bool_input.close();
	bool_output.close();
}