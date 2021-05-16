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
	// 이차원 배열 각행의 마지막은 &, | 구분
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
	/* 겹치는_PI를_EPI와_함께_반환함; */
	

	// 반환 - 이차원 배열로 출력
	*EPI_length = count; // EPI의 갯수 반환
	return EPIda;
}

int Optimize(char** bools, int row) {
	// 최적화, 트랜지스터 갯수 확인
	int trans_count = 0; // 트랜지스터 갯수 저장

	/* 결합법칙 (존폐 결정 중..)
	char* checkOneRow = new char [row]; int indexOR = 0; // 1부터 시작해서 확인
	for (int i = 0; i < bits; ++i){ // 한 열에서 결합 가능한 요소를 찾음
		int oneEx = -1, zeroEx = -1;
		for (int j = 0; j < row; ++j){
			if (bools[j][i] == '-') {continue;} // - 무시
			else if (bools[j][i] == '0') {
				if (zeroEx != -1) {
					bools[j][i] = '~';
				} // 이후 0을 찾았을때
				else {zeroEx = j;} // 처음 0을 찾을때
			}
			else if (bools[j][i] == '1') {
				if (oneEx != -1) {bools[j][i] = '/';} // 이후 1을 찾았을 때
				else {oneEx = j;} // 처음 1을 찾을때
			}
		}
		if (zeroEx != -1) {
			bools[zeroEx][i] = '/';
			inputNotG++; // not 게이트 추가
		}
		if (oneEx != -1) {bools[oneEx][i] = '/';}
	}*/

	// TWO-LEVEL LOGIC CIRCUIT

	// T11
	
	// T12
	// 최상위 (output에서 가장 가까운) 게이트의 처리
	// 내부 외부요소 전부 드모르간으로 처리
	// 트랜지스터 갯수 파악
	
	// 최상위 (Output에서 가장 가까운, NAND) 게이트 부분 갯수
	trans_count += row * 2;
	cout << trans_count << endl;


	// 중앙 게이트의 트랜지스터 갯수 및 input에 Not게이트가 존재시 트랜지스터 추가
	for (int i = 0; i < row; ++i){
		int NotG_i = 0, inputs = 0;
		for (int j = 0; j < bits; ++j){
			if (bools[i][j] == '0'){NotG_i++;}
			if (bools[i][j] != '-'){inputs++;}
		}
		if ((NotG_i/2) > (inputs/2)){
			// not 게이트가 input 갯수의 절반보다 많을때는 OR로 처리
			trans_count += inputs*2 + 2;
			// input은 반전됨 (0->1 / 1->0)
			trans_count += (inputs - NotG_i) * 2;
		}
		else{
			// not 게이트가 input 갯수의 절반보다 적을때는 NAND로 처리
			trans_count += inputs*2;
			// input은 그대로
			trans_count += NotG_i * 2;
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

	// 최적화 및 트랜지스터 갯수 확인
	int trans = Optimize(EsPrIm, length);
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