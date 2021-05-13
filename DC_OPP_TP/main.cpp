#include <fstream>
#include "boolList.hpp"

using namespace std;

int bits; // 비트

ofstream bool_output; // 출력 텍스트 파일

char** Q_M_process(boolList* firstList, int *EPI_length, PIList* PIs) { // 콰인-매클러스키 알고리즘 처리
	
	// Prime Implicant를 구하기 위해 처리하는 클래스로 넘기기
	int newList = 0; // 
	ImplicantList** nextList = new ImplicantList* [bits];
	nextList[0] = firstList->newLists();
	
	// 더이상 모든 bool들이 연결되지 않는 경우가 될때까지 반복
	while(nextList[newList] != NULL){
		nextList[newList+1] = nextList[newList]->newLists();
		++newList;
	}
	
	// 연결되지 않는 bool들을 PIs에 저장
	firstList->getUnconnect(PIs);
	for (int i = 0; i < newList; ++i){
		nextList[i]->getUnconnect(PIs);
	}
	
	// Prime implicant 출력
	booltype* currPI = PIs->gethead();
	while (currPI != NULL) {
		cout << currPI->boolvalue << endl;
		currPI = currPI->next;
	}
	cout << "========" << endl;
	
	// Essantial prime implicant 구하기
	int count = 0;
	PIList EPI;
	PIs->getEPI(&EPI, firstList, &count);

	// 링크드 리스트로 정리된 EPI를 이차원 배열로 변경
	booltype* EPIp = EPI.gethead();
		// 이차원 배열 동적할당
	char **EPIda;
	EPIda = new char*[count];
	for (int i = 0; i < count; ++i) {
		EPIda[i] = new char[bits];
	}
		// 이차원 배열 복사
	int j = 0;
	while (EPIp != NULL) {
		cout << EPIp->boolvalue << endl;
		bool_output << EPIp->boolvalue << endl;
		for (int k = 0; k < bits; ++k){
			EPIda[j][k] = EPIp->boolvalue[k];
		}
		EPIp = EPIp->next; ++j;
	}

	// 추후 PI를 만족하는 minimum set 구하는 단계를 추가예젱
	/* 곂치는_PI를_EPI와_함께_반환함; */


	// 반환 - 이차원 배열로 출력
	*EPI_length = count; // EPI의 갯수 반환
	return EPIda;
}

void Optimize() {
	// 최적화
	
	// 결합법칙

	// TWO-LEVEL LOGIC CIRCUIT

	// T11, T12

	// 내부 외부요소 전부 드모르간으로 처리
}

int tran_conut(char** bools, int row) {
	// 트랜지스터 갯수 파악
	int trans_count = 0;
	// NAND 부분 갯수
	for (int i = 0; i < row; ++i)
	{
		int countbool = 0;
		for(int j=0;j<bits;++j){
			if (bools[i][j] != '-') countbool++;
		}
		trans_count += countbool * 2;
	}
	
	//NOR 갯수
	if (row != 1)
		trans_count += row * 2;
	
	// 0갯수
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < bits; ++j) {
			if (bools[i][j] == '0') trans_count += 2;
		}
	}

	return trans_count;
}

int main() {
	// 파일 입출력 지정 및, 읽는 파일에서 한 줄 가져오는 변수 전언
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

	// 비트 처리용 클래스 선언 및 파일에서 가져오기
	boolList boolNew;
	while(!bool_input.eof()){
		bool_input.getline(fileline, bits+3);
		boolNew.insertBoolEq(fileline);
	}
	
	// 콰인-매클러스키 알고리즘 처리 함수로 넘기기
	int length = 0; // EPI갯수
	PIList allPIs; // PI를 반환하는 링크드 리스트
	char** EsPrIm = Q_M_process(&boolNew, &length, &allPIs);

	// 트랜지스터 갯수 확인
	int trans = tran_conut(EsPrIm, length);
	bool_output << "\nCost (# of transistors): " << trans;

	// 파일 입출력 스트림 종료
	bool_input.close();
	bool_output.close();

	// 동적할당 해제
	for (int i = 0; i < length; ++i) {
		delete[] EsPrIm[i]; // Essantial Prime 할당 제거
	}
	delete EsPrIm;

	return 0;
}