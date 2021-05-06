/* 	========================================
	[리스트 관련 헤더]
	  클래스 선언과 
	  클래스 내 간단한 메소드만 여기서 정의
	======================================== */

#include <iostream>

extern int bits;

class booltype {
	// Bool Eq 저장용
	public:
		char* boolvalue; // 값 ex) 0000, 0010
		int one; // 1 갯수
		bool connect;
		booltype* next; // 다음요소
		booltype(char* txt) {
			// 기본 값 초기회
			one = 0; connect = true;
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
		minterm(char* txt):booltype(txt) {}
};

class Lists{
	// 링크드 리스트 관련 클래스 (상속용)
	protected:
		booltype* head;
		int index;
	public:
		Lists(int newindex){index = newindex; head = NULL;}
		~Lists(){
			// booltype의 동적할당 해제
			booltype* currEq = head, *temp;
			while(currEq != NULL){
				temp = currEq;
				currEq = currEq->next;
				temp->~booltype();
			}
		}
		booltype* gethead(){return head;}
		Lists* newLists(); // 후에 연결되는 리스트 만들기
};

class boolList : public Lists{
	// txt에서 받아들인 Bool Eq를 링크드 리스트로 처리하는 부분
	public:
		boolList():Lists(0){}
		~boolList(){
			// booltype의 동적할당 해제
			booltype* currEq = head, *temp;
			while(currEq != NULL){
				temp = currEq;
				currEq = currEq->next;
				temp->~booltype();
			}
		}

		void insertBoolEq(char* intxt); // 초기 Bool Eq를 추가하는 함수
};

class ImplicantList : public Lists{
	// implicant를 구하기 위해 처리되는 부분을 링크드 리스트를 이용
	private:
		Lists* prevList; // connect 확인용 이전 리스트
	public:
		ImplicantList(int index, Lists* prList):Lists(index) {prevList = prList;}
		~ImplicantList();
		void insertImpliEq(char* intxt); // implicant를 추가하는 함수
		void checkPrevConnect(); // 이전 리스트에서 Prime Implicant를 찾기위해 연결되지 않는것을 찾는것
};