/* 	========================================
	[리스트 관련 헤더]
	  클래스 선언과 
	  클래스 내 간단한 메소드만 여기서 정의
	======================================== */

#include <iostream>

extern int bits; // 비트를 지정해주는 변수는 main.cpp에서 지정됩니다.

class booltype {
	// Bool Eq 저장용
	public:
		char* boolvalue; // 값 ex) 0000, 0010
		int one; // 1 갯수
		bool connect;
		booltype* next; // 다음요소
		booltype(char* txt) {
			// 기본 값 초기화
			one = 0; connect = false; next = NULL;
			// bool 값 저장을 위해 동적할당
			boolvalue = new char[bits+1];
			// 값을 복사하고 마지막에 NULL 추가
			for(int i = 0; i < bits; ++i){
				if (txt[i] == '1') {++one;}
				boolvalue[i] = txt[i];
			} boolvalue[bits] = '\0';
		}
		~booltype(){
			// 동적할당 해제
			delete[] boolvalue;
		}
};

class dontcare : public booltype{
	// don't care를 저장하기 위한 클래스
	public:
		dontcare(char* txt):booltype(txt) {}
};

class minterm : public booltype{
	// minterm을 저장하기 위한 클래스
	public:
		minterm* mtnext; // minterm 리스트의 다음 
		minterm(char* txt) :booltype(txt) { mtnext = NULL; }
};

class boolList; class ImplicantList; class PIList;

class Lists{
	// 링크드 리스트 관련 클래스 (상속용)
	protected:
		booltype* head;
		minterm* minhead;
		int index;
	public:
		Lists(int newindex) { index = newindex; head = NULL; minhead = NULL; }
		~Lists(){
			// booltype의 동적할당 해제
			booltype* currEq = head, *temp;
			while(currEq != NULL){
				temp = currEq;
				currEq = currEq->next;
				temp->~booltype();
			}
		}
		bool findBoolty(char* txt); // 해당 bool Eq가 존재하는지 찾기
		booltype* gethead(){return head;}
		minterm* getMinhead() { return minhead; }
		ImplicantList* newLists(); // 후에 연결되는 리스트 만들기
		booltype* getOneStart(int idx); // one을 idx로 하는 최초 booltype 구하기
		void getUnconnect(PIList* PI); // unconnect되는 모든 요소를 PI 리스트에 추가
};

class boolList : public Lists{
	// txt에서 받아들인 Bool Eq를 링크드 리스트로 처리하는 부분
	public:
		boolList():Lists(0){}
		~boolList(){}

		void insertBoolEq(char* intxt); // 초기 Bool Eq를 추가하는 함수
};

class ImplicantList : public Lists{
	// implicant를 구하기 위해 처리되는 부분을 링크드 리스트를 이용
	private:
		Lists* prevList; // connect 확인용 이전 리스트
	public:
		ImplicantList(int index, Lists* prList):Lists(index) {prevList = prList;}
		~ImplicantList(){}
		void insertImpliEq(char* intxt, int baridx); // implicant를 추가하는 함수
};

class PIList : public Lists{
	// Prime Implicant List
	public:
		PIList():Lists(0) {}
		~PIList() {}
		void insertPI(char* intxt); // Prime Implicant 추가
		booltype* gethead(); // head 출력
		void getEPI(PIList* EPIs, boolList* minterms, int *count); // Essential Prime Implicant 구하기
};